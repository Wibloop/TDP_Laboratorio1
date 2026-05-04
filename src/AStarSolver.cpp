#include "AStarSolver.h"
#include <iostream>

// Implementacion del AStarSolver — Motor del algoritmo A*

// Metodo: Constructor
// Descripcion: Inicializa el solver con capacidades por defecto
AStarSolver::AStarSolver() : openSet(256), closedSet(4096) {
  stateCapacity = 10000;
  stateCount = 0;
  allStates = new GameState *[stateCapacity];
  solutionFound = false;
  solutionState = nullptr;
}

// Metodo: Destructor
// Descripcion: Libera TODA la memoria de los estados creados durante la
// busqueda.
//   Esto es importante porque los estados se crean con new y deben liberarse.
AStarSolver::~AStarSolver() {
  for (int i = 0; i < stateCount; i++) {
    delete allStates[i];
  }
  delete[] allStates;
}

// Metodo: trackState
// Descripcion: Registra un puntero a GameState en el arreglo allStates para
//   poder liberarlo al destruir el solver. Si el arreglo esta lleno, lo
//   duplica.
void AStarSolver::trackState(GameState *state) {
  if (stateCount >= stateCapacity) {
    stateCapacity *= 2;
    GameState **newArr = new GameState *[stateCapacity];
    for (int i = 0; i < stateCount; i++) {
      newArr[i] = allStates[i];
    }
    delete[] allStates;
    allStates = newArr;
  }
  allStates[stateCount++] = state;
}

// Metodo: generateSuccessors
// Descripcion: Genera todos los estados hijos posibles del estado actual.
//
//   Para cada bloque NO evacuado:
//     Para cada direccion (U, D, L, R):
//       Intenta mover 1 celda, luego 2, luego 3... hasta que el movimiento
//       sea invalido (colision con pared u otro bloque).
//
//   Cada movimiento valido genera un nuevo GameState:
//   1. Se crea una copia profunda del estado actual
//   2. Se aplica el movimiento en la copia
//   3. Se verifica evacuacion (checkAndEvacuate)
//   4. Se calcula la heuristica
//   5. Si no esta en el ClosedSet, se agrega al OpenSet
//
//   El formato de movimiento es: "DA,3" (Direccion + ID_bloque->Color + , +
//   celdas) donde D=direccion, A=ID del bloque transformado a color, 3=cantidad
//   de celdas movidas.
void AStarSolver::generateSuccessors(GameState *current) {
  const Board &curBoard = current->getBoard();
  int numBlocks = curBoard.getNumBlocks();

  // Direcciones: dx, dy, caracter descriptor
  int dx[] = {0, 0, -1, 1};
  int dy[] = {-1, 1, 0, 0};
  // Up, Down, Left, Right
  char dirChar[] = {'U', 'D', 'L', 'R'};

  for (int bi = 0; bi < numBlocks; bi++) {
    // Saltar bloques evacuados
    if (current->isBlockEvacuated(bi))
      continue;

    // Intentar evacuar explícitamente
    char evacDir;
    int evacDist;
    if (current->canEvacuate(bi, evacDir, evacDist)) {
      GameState *child = new GameState(*current);
      child->setEvacuated(bi, true);
      child->getBoard().removeBlock(bi);
      // Se calcula el costo + 1 para dar el movimineto final de entrar a la
      // puerta
      child->setG(current->getG() + 1);
      child->setParent(current);

      char desc[16];
      desc[0] = evacDir;
      char blockChar = current->getBoard().getBlock(bi).getColor();
      if (blockChar >= 'a' && blockChar <= 'z')
        blockChar -= 32;
      int pos = 1;
      desc[pos++] = blockChar;
      desc[pos++] = ',';
      if (evacDist >= 10)
        desc[pos++] = '0' + (evacDist / 10);
      desc[pos++] = '0' + (evacDist % 10);
      desc[pos] = '\0';
      child->setMoveDesc(desc);

      child->computeHeuristic();

      if (!closedSet.contains(child)) {
        trackState(child);
        openSet.push(child);
      }
      continue; // Si el bloque puede evacuar, la mejor accion es hacerlo
    }

    // Intentar las 4 direcciones
    for (int d = 0; d < 4; d++) {
      // Intentar mover 1, 2, 3... celdas en esta direccion
      for (int dist = 1; dist < 14; dist++) {
        int totalDx = dx[d] * dist;
        int totalDy = dy[d] * dist;

        // Crear copia del estado actual
        GameState *child = new GameState(*current);

        // Intentar mover el bloque en la copia
        bool moved = child->getBoard().moveBlock(bi, totalDx, totalDy);

        if (!moved) {
          // Movimiento invalido -> no hay mas movimientos en esta direccion
          delete child;
          break;
        }

        // Configurar el estado hijo
        child->setG(current->getG() + 1);
        child->setParent(current);

        // Construir descripcion del movimiento: "DA,3"
        char desc[16];
        desc[0] = dirChar[d];
        // Convertir el color del bloque a mayuscula
        char blockChar = curBoard.getBlock(bi).getColor();
        if (blockChar >= 'a' && blockChar <= 'z') {
          blockChar -= 32;
        }
        int pos = 1;
        desc[pos++] = blockChar;
        desc[pos++] = ',';
        // Escribir la distancia
        if (dist >= 10) {
          desc[pos++] = '0' + (dist / 10);
        }
        desc[pos++] = '0' + (dist % 10);
        desc[pos] = '\0';
        child->setMoveDesc(desc);

        // Calcular heuristica
        child->computeHeuristic();

        // Verificar si ya fue visitado (ClosedSet)
        if (closedSet.contains(child)) {
          delete child;
          continue; // Ya visitado, descartar
        }

        // Agregar al OpenSet y registrar para liberacion posterior
        trackState(child);
        openSet.push(child);
      }
    }
  }
}

// Metodo: solve
// Descripcion: Ejecuta el algoritmo A* completo.
//
//   Pseudocodigo A*:
//   1. Insertar estado_inicial en OpenSet
//   2. Mientras OpenSet no este vacio:
//      a. Extraer el estado con menor f del OpenSet
//      b. Si es estado meta -> EXITO, retornar true
//      c. Insertar en ClosedSet
//      d. Generar todos los sucesores
//   3. Si OpenSet se vacia -> NO HAY SOLUCION
//
// Parametros:
//      - GameState* initialState: Estado inicial del juego
// Retorna: bool true si se encontro solucion
bool AStarSolver::solve(GameState *initialState) {
  // Calcular heuristica del estado inicial
  initialState->computeHeuristic();

  // Verificar si ya esta resuelto
  if (initialState->isSolved()) {
    solutionFound = true;
    solutionState = initialState;
    return true;
  }

  // Insertar en el OpenSet
  openSet.push(initialState);
  trackState(initialState);

  int iterations = 0;
  // Limite de seguridad para evitar un consumo excesivo de CPU y prevenir que
  // el computador se congele. Debido a la naturaleza del algoritmo A* y al
  // tamaño del espacio de estados, existe el riesgo de una explosion
  // combinatoria donde se pruebe un numero absurdo de combinaciones sin llegar
  // a una solucion. Limitar la ejecucion a un maximo de 100000 iteraciones
  // garantiza que el programa termine en un tiempo razonable, siendo esta
  // cantidad suficiente para resolver cualquier mapa valido del juego.
  int maxIterations = 1000000;

  while (!openSet.isEmpty() && iterations < maxIterations) {
    iterations++;

    // Extraer el estado con menor f
    GameState *current = openSet.pop();

    // Verificar si es estado meta
    if (current->isSolved()) {
      solutionFound = true;
      solutionState = current;
      std::cout << "Solucion encontrada en " << iterations << " iteraciones"
                << std::endl;
      std::cout << "Estados explorados: " << closedSet.getSize() << std::endl;
      std::cout << "Pasos de la solucion: " << current->getG() << std::endl;
      return true;
    }

    // Verificar limite de pasos
    if (current->getG() >= current->getBoard().getStepLimit()) {
      continue; // No generar mas hijos si excede el limite
    }

    // Insertar en ClosedSet
    if (!closedSet.insert(current)) {
      continue; // Ya estaba visitado (puede pasar con duplicados en el heap)
    }

    // Generar sucesores
    generateSuccessors(current);
  }

  std::cout << "No se encontro solucion despues de " << iterations
            << " iteraciones" << std::endl;
  return false;
}

// Metodo: getSolutionPath
// Descripcion: Reconstruye el camino solucion siguiendo los punteros parent.
//   Almacena las descripciones de movimientos en orden inverso y luego los
//   invierte.
// Parametros:
//      - char moves[][16]: Arreglo donde almacenar las descripciones
//      - int maxMoves: Tamaño maximo del arreglo
// Retorna: int cantidad de movimientos en la solucion
int AStarSolver::getSolutionPath(char moves[][16], int maxMoves) {
  if (!solutionFound || solutionState == nullptr)
    return 0;

  // Primero, contar cuantos pasos hay
  int count = 0;
  GameState *current = solutionState;
  while (current->getParent() != nullptr) {
    count++;
    current = current->getParent();
  }

  if (count > maxMoves)
    count = maxMoves;

  // Ahora recorrer de nuevo llenando el arreglo en orden inverso
  current = solutionState;
  for (int i = count - 1; i >= 0; i--) {
    const char *desc = current->getMoveDesc();
    int j = 0;
    while (desc[j] != '\0' && j < 15) {
      moves[i][j] = desc[j];
      j++;
    }
    moves[i][j] = '\0';
    current = current->getParent();
  }

  return count;
}

// Metodo: getSolutionState
// Descripcion: Retorna el estado meta si se encontro solucion
GameState *AStarSolver::getSolutionState() const { return solutionState; }

// Metodo: printSolution
// Descripcion: Imprime la secuencia de movimientos y opcionalmente los
// tableros.
//   Recorre el camino solucion y muestra cada paso.
void AStarSolver::printSolution() {
  if (!solutionFound) {
    std::cout << "No hay solucion para imprimir" << std::endl;
    return;
  }

  // Contar pasos
  int count = 0;
  GameState *current = solutionState;
  while (current->getParent() != nullptr) {
    count++;
    current = current->getParent();
  }

  // Almacenar punteros a estados en orden (del inicial al final)
  // Usamos un arreglo temporal
  GameState **path = new GameState *[count + 1];
  current = solutionState;
  for (int i = count; i >= 0; i--) {
    path[i] = current;
    current = current->getParent();
  }

  // Imprimir secuencia de movimientos
  std::cout << "\nSECUENCIA DE MOVIMIENTOS\n" << std::endl;
  std::cout << "Movimientos: ";
  for (int i = 1; i <= count; i++) {
    std::cout << path[i]->getMoveDesc();
    if (i < count)
      std::cout << " ";
  }
  std::cout << std::endl;

  // Imprimir tablero paso a paso
  std::cout << "\nTABLERO PASO A PASO:\n" << std::endl;
  for (int i = 0; i <= count; i++) {
    if (i == 0) {
      std::cout << "Estado Inicial\n" << std::endl;
    } else {
      std::cout << "\nPaso " << i << ": " << path[i]->getMoveDesc() << "\n"
                << std::endl;
    }
    path[i]->printBoard();
  }

  delete[] path;
}
