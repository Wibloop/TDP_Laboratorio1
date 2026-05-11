#include "Board.h"
#include <iostream>

// Generamos los archivos header y cpp para separar la declaracion de implementacion
// El constructor inicializa el grid rellenando todo con celdas vacias de valor 0
// Tambien reserva la capacidad inicial para almacenar los bloques del puzzle

Board::Board(int width, int height, int stepLimit) {
  this->width = width;
  this->height = height;
  this->stepLimit = stepLimit;
  this->currentStep = 0;
  this->numBlocks = 0;
  this->blockCapacity = 20; // Capacidad inicial para 20 bloques

  // Reservar memoria para el grid e inicializar en 0 (vacio)
  this->grid = new int[width * height];
  int total = width * height;
  for (int i = 0; i < total; i++) {
    grid[i] = 0;
  }

  // Reservar memoria para el arreglo de bloques
  this->blocks = new Block[blockCapacity];
}

// Construye una copia profunda del tablero
// Es critico para el algoritmo AStar pues cada estado necesita su propio plano independiente
// Copia el grid entero celda por celda y duplica fisicamente cada bloque
Board::Board(const Board &other) {
  width = other.width;
  height = other.height;
  stepLimit = other.stepLimit;
  currentStep = other.currentStep;
  numBlocks = other.numBlocks;
  blockCapacity = other.blockCapacity;

  // Deep copy del grid
  int total = width * height;
  grid = new int[total];
  for (int i = 0; i < total; i++) {
    grid[i] = other.grid[i];
  }

  // Deep copy del arreglo de bloques (cada Block tiene su propio copy
  // constructor)
  blocks = new Block[blockCapacity];
  for (int i = 0; i < numBlocks; i++) {
    blocks[i] = other.blocks[i];
  }
}

// Asigna el estado de otro tablero sobreescribiendo el actual y gestionando la memoria vieja
Board &Board::operator=(const Board &other) {
  if (this == &other) {
    return *this;
  }

  // Liberar memoria actual
  delete[] grid;
  delete[] blocks;

  // Copiar atributos primitivos
  width = other.width;
  height = other.height;
  stepLimit = other.stepLimit;
  currentStep = other.currentStep;
  numBlocks = other.numBlocks;
  blockCapacity = other.blockCapacity;

  // Deep copy del grid
  int total = width * height;
  grid = new int[total];
  for (int i = 0; i < total; i++) {
    grid[i] = other.grid[i];
  }

  // Deep copy de bloques
  blocks = new Block[blockCapacity];
  for (int i = 0; i < numBlocks; i++) {
    blocks[i] = other.blocks[i];
  }

  return *this;
}

// Libera la memoria dinamica de la malla y el vector de bloques
Board::~Board() {
  delete[] grid;
  delete[] blocks;
}

// --- Getters ---

// Devuelve el ancho del tablero
int Board::getWidth() const { return width; }

// Devuelve el alto del tablero
int Board::getHeight() const { return height; }

// Devuelve el limite de pasos permitido para el nivel
// Retorna: int stepLimit
int Board::getStepLimit() const { return stepLimit; }

// Metodo: getCurrentStep
// Descripcion: Retorna los pasos actuales
// Parametros: Ninguno
// Retorna: int currentStep
int Board::getCurrentStep() const { return currentStep; }

// Metodo: getNumBlocks
// Descripcion: Retorna la cantidad de bloques en el tablero
// Parametros: Ninguno
// Retorna: int numBlocks
int Board::getNumBlocks() const { return numBlocks; }

// Metodo: getGrid
// Descripcion: Retorna el puntero al grid. Permite acceso directo al mapa
//              para hashing y comparacion de estados.
// Parametros: Ninguno
// Retorna: int* puntero al grid
int *Board::getGrid() const { return grid; }

// Metodo: getCell
// Descripcion: Retorna el valor de la celda en la posicion (x, y).
//              Usa indexacion [y * width + x] porque el grid es un arreglo
//              aplanado almacenado por filas (row-major order).
// Parametros:
//      - int x: Coordenada X (columna)
//      - int y: Coordenada Y (fila)
// Retorna: int valor de la celda (0=vacio, 1=pared, >1=bloque)
int Board::getCell(int x, int y) const {
  if (x < 0 || x >= width || y < 0 || y >= height) {
    return -1; // Fuera de limites
  }
  return grid[y * width + x];
}

// Metodo: getBlock (version no-const)
// Descripcion: Retorna referencia mutable al bloque en el indice dado
// Parametros:
//      - int index: Indice del bloque en el arreglo
// Retorna: Block& referencia al bloque
Block &Board::getBlock(int index) { return blocks[index]; }

// Metodo: getBlock (version const)
// Descripcion: Retorna referencia constante al bloque en el indice dado
// Parametros:
//      - int index: Indice del bloque en el arreglo
// Retorna: const Block& referencia constante al bloque
const Block &Board::getBlock(int index) const { return blocks[index]; }

// --- Setters ---

// Metodo: setCell
// Descripcion: Establece el valor de una celda en el grid.
// Parametros:
//      - int x: Coordenada X
//      - int y: Coordenada Y
//      - int value: Valor a asignar
// Retorna: void
void Board::setCell(int x, int y, int value) {
  if (x >= 0 && x < width && y >= 0 && y < height) {
    grid[y * width + x] = value;
  }
}

// Metodo: setCurrentStep
// Descripcion: Establece el paso actual
// Parametros:
//      - int step: Nuevo valor de paso
// Retorna: void
void Board::setCurrentStep(int step) { currentStep = step; }

// --- Metodos generales ---

// Metodo: addBlock
// Descripcion: Agrega un bloque al arreglo y lo "estampa" en el grid.
//              Cada celda que el bloque ocupa (segun su geometria) se marca
//              en el grid con el valor (ID_bloque + 10) para distinguirlo
//              de paredes (1) y celdas vacias (0).
//              Si el arreglo esta lleno, se duplica su capacidad.
// Parametros:
//      - const Block& b: El bloque a agregar
// Retorna: void
void Board::addBlock(const Block &b) {
  // Verificar si hay espacio; si no, redimensionar
  if (numBlocks >= blockCapacity) {
    blockCapacity *= 2;
    Block *newBlocks = new Block[blockCapacity];
    for (int i = 0; i < numBlocks; i++) {
      newBlocks[i] = blocks[i];
    }
    delete[] blocks;
    blocks = newBlocks;
  }

  // Agregar el bloque al arreglo
  blocks[numBlocks] = b;

  // Estampar el bloque en el grid: recorrer su bounding box
  // y marcar cada celda que la geometria indica como ocupada
  int bx = b.getX();
  int by = b.getY();
  int bw = b.getWidth();
  int bh = b.getHeight();
  bool *geom = b.getGeometry();

  for (int row = 0; row < bh; row++) {
    for (int col = 0; col < bw; col++) {
      // Si la geometria indica que esta celda esta ocupada
      if (geom == nullptr || geom[row * bw + col]) {
        int gx = bx + col;
        int gy = by + row;
        // y la celda esta dentro del tablero, marcarla
        if (gx >= 0 && gx < width && gy >= 0 && gy < height) {
          // Se usa (ID + 10) para que el valor sea unico y distinto de 0 y 1
          grid[gy * width + gx] = b.getID() + 10;
        }
      }
    }
  }

  numBlocks++;
}

// Metodo: addWall
// Descripcion: Agrega un muro en la posicion (x, y) del grid.
//              Los muros se representan con el valor 1.
// Parametros:
//      - int x: Coordenada X del muro
//      - int y: Coordenada Y del muro
// Retorna: void
void Board::addWall(int x, int y) {
  if (x >= 0 && x < width && y >= 0 && y < height) {
    grid[y * width + x] = 1;
  }
}

// Metodo: isValidMove
// Descripcion: Verifica si el bloque en el indice dado puede moverse (dx, dy)
// celdas.
//              Para cada celda que el bloque ocuparia en la nueva posicion,
//              chequea que:
//              1. Este dentro de los limites del tablero
//              2. La celda destino este vacia (0) o ya pertenezca al mismo
//              bloque Esta funcion NO modifica ningun estado, solo consulta.
// Parametros:
//      - int blockIndex: Indice del bloque en el arreglo
//      - int dx: Desplazamiento en X (positivo = derecha, negativo = izquierda)
//      - int dy: Desplazamiento en Y (positivo = abajo, negativo = arriba)
// Retorna: bool true si el movimiento es valido
bool Board::isValidMove(int blockIndex, int dx, int dy) const {
  const Block &b = blocks[blockIndex];
  int bx = b.getX();
  int by = b.getY();
  int bw = b.getWidth();
  int bh = b.getHeight();
  bool *geom = b.getGeometry();
  int blockGridVal =
      b.getID() + 10; // El valor que este bloque tiene en el grid

  for (int row = 0; row < bh; row++) {
    for (int col = 0; col < bw; col++) {
      // Solo verificar celdas que la geometria marca como ocupadas
      if (geom == nullptr || geom[row * bw + col]) {
        int newX = bx + col + dx;
        int newY = by + row + dy;

        // Si la celda destino esta fuera de bounds, verificar si la celda
        // original tambien estaba fuera (celda clippeada, ignorar)
        int origX = bx + col;
        int origY = by + row;
        bool origOOB =
            (origX < 0 || origX >= width || origY < 0 || origY >= height);

        if (newX < 0 || newX >= width || newY < 0 || newY >= height) {
          // Si la celda original tambien era OOB, esta celda nunca existio
          // en el grid, asi que no importa si la nueva posicion es OOB
          if (origOOB)
            continue;
          return false; // Celda real se saldria del tablero
        }

        // Verificar que la celda destino este vacia o sea del mismo bloque
        int cellVal = grid[newY * width + newX];
        if (cellVal != 0 && cellVal != blockGridVal) {
          return false; // Hay otro bloque o pared
        }
      }
    }
  }
  return true;
}

// Metodo: moveBlock
// Descripcion: Intenta mover el bloque en el indice dado por (dx, dy).
//              Primero valida el movimiento. Si es valido:
//              1. Borra la posicion anterior del bloque en el grid
//              2. Actualiza la posicion del bloque
//              3. Estampa la nueva posicion en el grid
//              4. Incrementa el paso actual
// Parametros:
//      - int blockIndex: Indice del bloque
//      - int dx: Desplazamiento en X
//      - int dy: Desplazamiento en Y
// Retorna: bool true si el movimiento se realizo con exito
bool Board::moveBlock(int blockIndex, int dx, int dy) {
  if (blockIndex < 0 || blockIndex >= numBlocks) {
    return false;
  }

  if (!isValidMove(blockIndex, dx, dy)) {
    return false;
  }

  Block &b = blocks[blockIndex];
  int bx = b.getX();
  int by = b.getY();
  int bw = b.getWidth();
  int bh = b.getHeight();
  bool *geom = b.getGeometry();
  int blockGridVal = b.getID() + 10;

  // Paso 1: Borrar la posicion anterior del bloque en el grid
  for (int row = 0; row < bh; row++) {
    for (int col = 0; col < bw; col++) {
      if (geom == nullptr || geom[row * bw + col]) {
        int gx = bx + col;
        int gy = by + row;
        if (gx >= 0 && gx < width && gy >= 0 && gy < height) {
          grid[gy * width + gx] = 0;
        }
      }
    }
  }

  // Paso 2: Actualizar la posicion del bloque
  b.move(bx + dx, by + dy);

  // Paso 3: Estampar la nueva posicion
  int newBx = b.getX();
  int newBy = b.getY();
  for (int row = 0; row < bh; row++) {
    for (int col = 0; col < bw; col++) {
      if (geom == nullptr || geom[row * bw + col]) {
        int gx = newBx + col;
        int gy = newBy + row;
        if (gx >= 0 && gx < width && gy >= 0 && gy < height) {
          grid[gy * width + gx] = blockGridVal;
        }
      }
    }
  }

  currentStep++;
  return true;
}

// Metodo: removeBlock
// Descripcion: Borra un bloque del grid (pone sus celdas en 0).
//              El bloque permanece en el arreglo pero se marca con posicion
//              (-1, -1) para indicar que fue evacuado/removido. Se usa cuando
//              un bloque sale por una Exit.
// Parametros:
//      - int blockIndex: Indice del bloque a remover
// Retorna: void
void Board::removeBlock(int blockIndex) {
  if (blockIndex < 0 || blockIndex >= numBlocks) {
    return;
  }

  Block &b = blocks[blockIndex];
  int bx = b.getX();
  int by = b.getY();
  int bw = b.getWidth();
  int bh = b.getHeight();
  bool *geom = b.getGeometry();

  // Borrar del grid
  for (int row = 0; row < bh; row++) {
    for (int col = 0; col < bw; col++) {
      if (geom == nullptr || geom[row * bw + col]) {
        int gx = bx + col;
        int gy = by + row;
        if (gx >= 0 && gx < width && gy >= 0 && gy < height) {
          grid[gy * width + gx] = 0;
        }
      }
    }
  }

  // Marcar como removido con posicion invalida
  b.move(-1, -1);
}

// Metodo: rebuildGrid
// Descripcion: Reconstruye el grid completo desde cero.
//              Preserva las paredes (valor 1) y re-estampa todos los bloques
//              activos (aquellos con posicion >= 0). Util despues de clonar un
//              estado y modificar posiciones de bloques directamente.
// Parametros: Ninguno
// Retorna: void
void Board::rebuildGrid() {
  // Paso 1: Limpiar todo excepto paredes
  int total = width * height;
  for (int i = 0; i < total; i++) {
    if (grid[i] != 1) { // Preservar paredes
      grid[i] = 0;
    }
  }

  // Paso 2: Re-estampar cada bloque activo
  for (int idx = 0; idx < numBlocks; idx++) {
    Block &b = blocks[idx];
    // Un bloque con x == -1 fue removido (evacuado)
    if (b.getX() < 0) {
      continue;
    }

    int bx = b.getX();
    int by = b.getY();
    int bw = b.getWidth();
    int bh = b.getHeight();
    bool *geom = b.getGeometry();

    for (int row = 0; row < bh; row++) {
      for (int col = 0; col < bw; col++) {
        if (geom == nullptr || geom[row * bw + col]) {
          int gx = bx + col;
          int gy = by + row;
          if (gx >= 0 && gx < width && gy >= 0 && gy < height) {
            grid[gy * width + gx] = b.getID() + 10;
          }
        }
      }
    }
  }
}

// Metodo: printBoard
// Descripcion: Imprime el estado actual del tablero en la consola.
//              Usa caracteres para representar cada tipo de celda:
//              '#' = pared, '.' = vacio, letras/numeros = bloques.
//              Si un bloque tiene color, muestra su color en mayuscula.
// Parametros: Ninguno
// Retorna: void
void Board::printBoard() const {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int val = grid[y * width + x];
      if (val == 1) {
        // Pared
        std::cout << '#';
      } else if (val == 0) {
        // Celda vacia
        std::cout << '.';
      } else {
        // Bloque: buscar su color
        int blockId = val - 10;
        bool found = false;
        for (int i = 0; i < numBlocks; i++) {
          if (blocks[i].getID() == blockId) {
            // Imprimir el color del bloque en mayuscula para visibilidad
            char c = blocks[i].getColor();
            if (c >= 'a' && c <= 'z') {
              std::cout << (char)(c - 32); // Convertir a mayuscula
            } else {
              std::cout << c;
            }
            found = true;
            break;
          }
        }
        if (!found) {
          std::cout << '?';
        }
      }
    }
    std::cout << std::endl;
  }
}