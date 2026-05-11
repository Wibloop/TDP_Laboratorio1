#include "AStarSolver.h"
#include <iostream>

// aca implementamos el solver a* que es el corazon del algoritmo

// iniciamos el constructor reservando memoria de una vez para el arbol y los hashsets
AStarSolver::AStarSolver() : openSet(256), closedSet(4096) {
  stateCapacity = 10000;
  stateCount = 0;
  allStates = new GameState *[stateCapacity];
  solutionFound = false;
  solutionState = nullptr;
}

// destruimos todos los estados que creamos durante la busqueda en el arbol
// esto es super util para prevenir fugas de memoria por los news que se hacen en generateSuccessors
AStarSolver::~AStarSolver() {
  for (int i = 0; i < stateCount; i++) {
    delete allStates[i];
  }
  delete[] allStates;
}

// guardamos el puntero al gamestate en el arreglo global para poder borrarlo despues
// si se nos acaba el espacio en el array simplemente lo duplicamos con memoria dinamica
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

// aca generamos los estados hijos desde el actual mirando todas las ramas posibles
// checamos cada bloque que aun no sale y tratamos de moverlo a todos lados
// si el mov es valido entonces creamos un estado nuevo clonamos el tablero y se lo aplicamos
// luego vemos si de pura casualidad el bloque ya llego a su salida
// al final le sacamos la heuristica al nuevo tablero y lo metemos al openset
// tambien guardamos la accion en un string tipo DA,3 donde D es direccion A es el bloque y 3 es cuanto se movio
void AStarSolver::generateSuccessors(GameState *current) {
  const Board &curBoard = current->getBoard();
  int numBlocks = curBoard.getNumBlocks();

  // arreglos para las direcciones dx dy y las letras que las representan
  int dx[] = {0, 0, -1, 1};
  int dy[] = {-1, 1, 0, 0};
  // arriba abajo izquierda derecha
  char dirChar[] = {'U', 'D', 'L', 'R'};

  for (int bi = 0; bi < numBlocks; bi++) {
    // ignoramos los bloques que ya se evacuaron
    if (current->isBlockEvacuated(bi))
      continue;

    // probamos sacar el bloque directo si de casualidad ya esta alineado con su agujero
    char evacDir;
    int evacDist;
    if (current->canEvacuate(bi, evacDir, evacDist)) {
      GameState *child = new GameState(*current);
      child->setEvacuated(bi, true);
      child->getBoard().removeBlock(bi);
      // le sumamos 1 al g para contar el paso extra de salir del mapa
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
    }

    // logica para lidiar con las compuertas
    char jumpDir;
    int jumpDx, jumpDy;
    if (current->canJumpGate(bi, jumpDir, jumpDx, jumpDy)) {
      GameState *child = new GameState(*current);
      bool moved = child->getBoard().moveBlock(bi, jumpDx, jumpDy);
      if (moved) {
        child->setG(current->getG() + 1);
        child->setParent(current);

        char desc[16];
        desc[0] = jumpDir;
        char blockChar = current->getBoard().getBlock(bi).getColor();
        if (blockChar >= 'a' && blockChar <= 'z')
          blockChar -= 32;
        int pos = 1;
        desc[pos++] = blockChar;
        desc[pos++] = ',';
        int dist = (jumpDx != 0) ? jumpDx : jumpDy;
        if (dist < 0)
          dist = -dist;

        if (dist >= 10)
          desc[pos++] = '0' + (dist / 10);
        desc[pos++] = '0' + (dist % 10);
        desc[pos] = '\0';

        child->setMoveDesc(desc);
        child->computeHeuristic();

        if (!closedSet.contains(child)) {
          trackState(child);
          openSet.push(child);
        } else {
          delete child;
        }
      } else {
        delete child;
      }
    }

    // intentamos movernos hacia los cuatro lados
    for (int d = 0; d < 4; d++) {
      // probamos estirando el mov desde 1 hasta 13 casillas de golpe
      for (int dist = 1; dist < 14; dist++) {
        int totalDx = dx[d] * dist;
        int totalDy = dy[d] * dist;

        // armamos copia del estado actual
        GameState *child = new GameState(*current);

        // a ver si el bloque se deja mover en esta copia
        bool moved = child->getBoard().moveBlock(bi, totalDx, totalDy);

        if (!moved) {
          // si choco o algo cortamos y ya no intentamos empujarlo mas lejos
          delete child;
          break;
        }

        // preparamos el hijo recien salido del horno
        child->setG(current->getG() + 1);
        child->setParent(current);

        // construimos como se llama este paso onda DA,3
        char desc[16];
        desc[0] = dirChar[d];
        // pasamos el color a mayuscula para que se vea mas chulo
        char blockChar = curBoard.getBlock(bi).getColor();
        if (blockChar >= 'a' && blockChar <= 'z') {
          blockChar -= 32;
        }
        int pos = 1;
        desc[pos++] = blockChar;
        desc[pos++] = ',';
        // aca guardamos que tanta distancia avanzo
        if (dist >= 10) {
          desc[pos++] = '0' + (dist / 10);
        }
        desc[pos++] = '0' + (dist % 10);
        desc[pos] = '\0';
        child->setMoveDesc(desc);

        // sacamos su heuristica a ver que tan bueno pinta
        child->computeHeuristic();

        // nos fijamos si este tablero ya lo habiamos pisado
        if (closedSet.contains(child)) {
          delete child;
          continue; // si ya fuimos ni nos molestamos en repetir
        }

        // lo tiramos al openset para revisarlo mas tarde y lo guardamos para no dejar basura
        trackState(child);
        openSet.push(child);
      }
    }
  }
}

// aca corre todo el algoritmo a* desde cero
// metemos el primer estado y nos ponemos a loopear hasta encontrar la meta o quedarnos secos
// siempre sacamos el estado que tenga menos costo f para ir por el camino mas corto
// si lo que revisamos ya soluciono el mapa entonces paramos la maquina
// sino lo marcamos como visto en el closedset y sacamos todos sus hijitos posibles
bool AStarSolver::solve(GameState *initialState) {
  // calculamos la heuristica del punto de partida
  initialState->computeHeuristic();

  // checkeamos si de milagro ya nos dieron el mapa ganado
  if (initialState->isSolved()) {
    solutionFound = true;
    solutionState = initialState;
    return true;
  }

  // lo metemos al openset para empezar la diversion
  openSet.push(initialState);
  trackState(initialState);

  int iterations = 0;
  // le ponemos un tope a la cantidad de vueltas para que no se nos queme el proce
  // si los mapas son muy trambolicos el solver podria pasarse de rosca explorando
  // por pura seguridad le cortamos el chorro a las 10 millones de iteraciones
  int maxIterations = 10000000;

  while (!openSet.isEmpty() && iterations < maxIterations) {
    iterations++;

    // sacamos el que tenga menor f
    // f son los costos enteros del nodo onda costo real mas heuristico
    // g es cuanto nos costo llegar desde el inicio hasta ahora
    // h es cuanto estimamos que nos falta para ganar
    GameState *current = openSet.pop();

    // si este estado cumple pues ganamos el juego
    if (current->isSolved()) {
      solutionFound = true;
      solutionState = current;
      std::cout << "Solucion encontrada en " << iterations << " iteraciones"
                << std::endl;
      std::cout << "Estados explorados: " << closedSet.getSize() << std::endl;
      std::cout << "Pasos de la solucion: " << current->getG() << std::endl;
      return true;
    }

    // si ya gastamos el maximo de pasos permitidos por el nivel pos ni modo
    if (current->getG() >= current->getBoard().getStepLimit()) {
      continue; // skipeamos y no sacamos mas ramificaciones
    }

    // lo mandamos a los visitados
    if (!closedSet.insert(current)) {
      continue; // si ya estaba visto seguimos de largo a veces pasa por duplicados en el heap
    }

    // sacamos todos los movimientos siguientes
    generateSuccessors(current);
  }

  std::cout << "No se encontro solucion despues de " << iterations
            << " iteraciones" << std::endl;
  return false;
}

// aca reconstruimos el caminito que nos llevo a ganar usando los punteros de papas
// vamos guardando los pasos al reves y al final le damos la vuelta para que quede bien
// recibe el arreglo donde meter todo y cuantos caben como maximo
// retorna cuantos movimientos fueron en total
int AStarSolver::getSolutionPath(char moves[][16], int maxMoves) {
  if (!solutionFound || solutionState == nullptr)
    return 0;

  // primero damos una pasada a ver cuantos pasos dimos
  int count = 0;
  GameState *current = solutionState;
  while (current->getParent() != nullptr) {
    count++;
    current = current->getParent();
  }

  if (count > maxMoves)
    count = maxMoves;

  // le damos otra pasada pero esta vez metiendo los datos de atras pa lante
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

// nos tira el estado en el que finalmente ganamos la partida
GameState *AStarSolver::getSolutionState() const { return solutionState; }

// este pinta toda la seguidilla de movimientos por la consola 
// se pasea por todo el camino ganador y tira cada frame del tablero
void AStarSolver::printSolution() {
  if (!solutionFound) {
    std::cout << "No hay solucion para imprimir" << std::endl;
    return;
  }

  // contamos cuantos pasos dimos
  int count = 0;
  GameState *current = solutionState;
  while (current->getParent() != nullptr) {
    count++;
    current = current->getParent();
  }

  // nos armamos un arreglo temporal para guardar todos los nodos del camino al derecho
  // asi los podemos mostrar de forma bonita
  GameState **path = new GameState *[count + 1];
  current = solutionState;
  for (int i = count; i >= 0; i--) {
    path[i] = current;
    current = current->getParent();
  }

  // tiramos la serie de string movs primero
  std::cout << "\nSECUENCIA DE MOVIMIENTOS\n" << std::endl;
  std::cout << "Movimientos: ";
  for (int i = 1; i <= count; i++) {
    std::cout << path[i]->getMoveDesc();
    if (i < count)
      std::cout << " ";
  }
  std::cout << std::endl;

  // y por ultimo mostramos el mapa pintado frame por frame
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
