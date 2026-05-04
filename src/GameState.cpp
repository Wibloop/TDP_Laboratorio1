#include "GameState.h"
#include <iostream>

// Implementacion de GameState. Nodo del arbol de busqueda A*

// Metodo: Constructor -> GameState
// Descripcion: Crea un estado de juego. Inicializa g=0, h=0, padre=nullptr.
// Parametros:
//      - const Board& board: Estado del tablero
//      - Exit* exits: Arreglo de salidas (se copia)
//      - int numExits: Cantidad de salidas
//      - Gate* gates: Arreglo de compuertas (se copia)
//      - int numGates: Cantidad de compuertas
// Retorna: void
GameState::GameState(const Board &board, Exit *exits, int numExits, Gate *gates,
                     int numGates)
    : board(board) {
  this->numExits = numExits;
  this->numGates = numGates;
  this->g = 0;
  this->h = 0;
  this->parent = nullptr;
  this->moveDesc[0] = '\0';
  this->exits = new Exit[numExits];
  for (int i = 0; i < numExits; i++) {
    this->exits[i] = exits[i];
  }
  this->gates = new Gate[numGates > 0 ? numGates : 1];
  for (int i = 0; i < numGates; i++) {
    this->gates[i] = gates[i];
  }
  int numBlocks = board.getNumBlocks();
  this->evacuated = new bool[numBlocks];
  for (int i = 0; i < numBlocks; i++) {
    this->evacuated[i] = false;
  }
}

// Metodo: Constructor de copia
// Descripcion: Deep copy completo del estado
GameState::GameState(const GameState &other) : board(other.board) {
  numExits = other.numExits;
  numGates = other.numGates;
  g = other.g;
  h = other.h;
  parent = other.parent;
  for (int i = 0; i < 16; i++)
    moveDesc[i] = other.moveDesc[i];
  exits = new Exit[numExits];
  for (int i = 0; i < numExits; i++)
    exits[i] = other.exits[i];
  gates = new Gate[numGates > 0 ? numGates : 1];
  for (int i = 0; i < numGates; i++)
    gates[i] = other.gates[i];
  int numBlocks = board.getNumBlocks();
  evacuated = new bool[numBlocks];
  for (int i = 0; i < numBlocks; i++)
    evacuated[i] = other.evacuated[i];
}

// Metodo: Operador de asignacion
GameState &GameState::operator=(const GameState &other) {
  if (this == &other)
    return *this;
  delete[] exits;
  delete[] gates;
  delete[] evacuated;
  board = other.board;
  numExits = other.numExits;
  numGates = other.numGates;
  g = other.g;
  h = other.h;
  parent = other.parent;
  for (int i = 0; i < 16; i++)
    moveDesc[i] = other.moveDesc[i];
  exits = new Exit[numExits];
  for (int i = 0; i < numExits; i++)
    exits[i] = other.exits[i];
  gates = new Gate[numGates > 0 ? numGates : 1];
  for (int i = 0; i < numGates; i++)
    gates[i] = other.gates[i];
  int numBlocks = board.getNumBlocks();
  evacuated = new bool[numBlocks];
  for (int i = 0; i < numBlocks; i++)
    evacuated[i] = other.evacuated[i];
  return *this;
}

// Metodo: Destructor
GameState::~GameState() {
  delete[] exits;
  delete[] gates;
  delete[] evacuated;
}

// Getters
Board &GameState::getBoard() { return board; }
const Board &GameState::getBoard() const { return board; }
int GameState::getG() const { return g; }
int GameState::getH() const { return h; }
int GameState::getF() const { return g + h; }
GameState *GameState::getParent() const { return parent; }
const char *GameState::getMoveDesc() const { return moveDesc; }
Exit *GameState::getExits() const { return exits; }
int GameState::getNumExits() const { return numExits; }
Gate *GameState::getGates() const { return gates; }
int GameState::getNumGates() const { return numGates; }

bool GameState::isBlockEvacuated(int index) const {
  if (index < 0 || index >= board.getNumBlocks())
    return false;
  return evacuated[index];
}

// Setters
void GameState::setG(int g) { this->g = g; }
void GameState::setH(int h) { this->h = h; }
void GameState::setParent(GameState *parent) { this->parent = parent; }

// Metodo: setMoveDesc
// Descripcion: Copia cadena de movimiento al buffer interno (max 15 chars)
void GameState::setMoveDesc(const char *desc) {
  int i = 0;
  while (desc[i] != '\0' && i < 15) {
    moveDesc[i] = desc[i];
    i++;
  }
  moveDesc[i] = '\0';
}

void GameState::setEvacuated(int index, bool value) {
  if (index >= 0 && index < board.getNumBlocks())
    evacuated[index] = value;
}

// Metodo: isSolved
// Descripcion: Retorna true si todos los bloques que TIENEN al menos una
//   salida compatible (color + dimension) fueron evacuados.
//   Bloques sin salida compatible actuan como obstaculos y se ignoran.
bool GameState::isSolved() const {
  int n = board.getNumBlocks();
  for (int i = 0; i < n; i++) {
    if (evacuated[i])
      continue;
    // Verificar si este bloque tiene alguna salida compatible
    const Block &b = board.getBlock(i);
    char color = b.getColor();
    int bw = b.getWidth();
    int bh = b.getHeight();
    bool hasCompatibleExit = false;
    for (int e = 0; e < numExits; e++) {
      if (exits[e].getColor() == color) {
        char ori = exits[e].getOrientation();
        int li = exits[e].getLineStart();
        int lf = exits[e].getLineEnd();
        int maxSize = (li > lf) ? li : lf;
        if (ori == 'H' && bh <= maxSize) {
          hasCompatibleExit = true;
          break;
        }
        if (ori == 'V' && bw <= maxSize) {
          hasCompatibleExit = true;
          break;
        }
      }
    }
    if (hasCompatibleExit)
      return false; // Bloque con salida valida no evacuado
  }
  return true;
}

// Metodo: computeHash
// Descripcion: Hash FNV-1a basado en posiciones de bloques y estado de
// evacuacion.
//   FNV-1a: hash = (hash XOR byte) * prime. Buena distribucion para tablas
//   hash.
unsigned long GameState::computeHash() const {
  unsigned long hash = 2166136261UL;
  unsigned long prime = 16777619UL;
  int n = board.getNumBlocks();
  for (int i = 0; i < n; i++) {
    const Block &b = board.getBlock(i);
    unsigned char xByte = (unsigned char)(b.getX() + 50);
    hash = (hash ^ xByte) * prime;
    unsigned char yByte = (unsigned char)(b.getY() + 50);
    hash = (hash ^ yByte) * prime;
    unsigned char evByte = evacuated[i] ? 1 : 0;
    hash = (hash ^ evByte) * prime;
  }
  return hash;
}

// Metodo: equals
// Descripcion: Dos estados son iguales si misma posicion de bloques y
// evacuacion
bool GameState::equals(const GameState &other) const {
  int n = board.getNumBlocks();
  if (n != other.board.getNumBlocks())
    return false;
  for (int i = 0; i < n; i++) {
    if (evacuated[i] != other.evacuated[i])
      return false;
    if (!evacuated[i]) {
      const Block &a = board.getBlock(i);
      const Block &b = other.board.getBlock(i);
      if (a.getX() != b.getX() || a.getY() != b.getY())
        return false;
    }
  }
  return true;
}

// Metodo: computeHeuristic
// Descripcion: Heuristica admisible = suma de distancias Manhattan de cada
// bloque
//   no evacuado a su salida mas cercana del mismo color.
//
//   Sistema de coordenadas (igual para EXIT y BLOCK):
//     X = Columna (eje horizontal), Y = Fila (eje vertical). (0,0) = esquina
//     sup-izq.
//
//   ORIENTATION describe la DIRECCION DE MOVIMIENTO del bloque para salir:
//     - H (mov. horizontal): Salida en paredes laterales (X=0 o X=W-1).
//       Agujero vertical [Y, Y+LI-1]. Bloque necesita HEIGHT <= LI.
//     - V (mov. vertical): Salida en paredes sup/inf (Y=0 o Y=H-1).
//       Agujero horizontal [X, X+LI-1]. Bloque necesita WIDTH <= LI.
void GameState::computeHeuristic() {
  int totalH = 0;
  int n = board.getNumBlocks();
  for (int i = 0; i < n; i++) {
    if (evacuated[i])
      continue;
    const Block &b = board.getBlock(i);
    char blockColor = b.getColor();
    int bw = b.getWidth();
    int bh = b.getHeight();
    int bx = b.getX(); // columna
    int by = b.getY(); // fila
    int minDist = 99999;
    bool hasValidExit = false;

    for (int e = 0; e < numExits; e++) {
      if (exits[e].getColor() == blockColor) {
        char ori = exits[e].getOrientation();
        int li = exits[e].getLineStart();
        int lf = exits[e].getLineEnd();
        int maxSize = (li > lf) ? li : lf;

        // H -> mov. horizontal, bloque necesita HEIGHT <= tamaño
        // V -> mov. vertical, bloque necesita WIDTH <= tamaño
        bool fits = false;
        if (ori == 'H' && bh <= maxSize)
          fits = true;
        if (ori == 'V' && bw <= maxSize)
          fits = true;

        if (fits) {
          hasValidExit = true;
          // Distancia en X (columnas)
          int dCol = 0;
          if (ori == 'H') {
            // Salida en pared lateral. El bloque debe llegar a bx == 1 o bx+bw
            // == boardW-1
            int targetX1 = 1;
            int targetX2 = board.getWidth() - 1 - bw;
            int dist1 = (bx > targetX1) ? bx - targetX1 : targetX1 - bx;
            int dist2 = (bx > targetX2) ? bx - targetX2 : targetX2 - bx;
            dCol = (exits[e].getX() == 0) ? dist1 : dist2;
          } else {
            // Salida en pared sup/inf. El bloque debe alinearse con el agujero
            // [eCol, eCol + cs - 1] El bloque necesita bx >= eCol y bx + bw <=
            // eCol + cs
            int validMinX = exits[e].getX();
            int validMaxX = exits[e].getX() + maxSize - bw;
            if (bx < validMinX)
              dCol = validMinX - bx;
            else if (bx > validMaxX)
              dCol = bx - validMaxX;
          }

          // Distancia en Y (filas)
          int dRow = 0;
          if (ori == 'V') {
            // Salida en pared sup/inf. El bloque debe llegar a by == 1 o by+bh
            // == boardH-1
            int targetY1 = 1;
            int targetY2 = board.getHeight() - 1 - bh;
            int dist1 = (by > targetY1) ? by - targetY1 : targetY1 - by;
            int dist2 = (by > targetY2) ? by - targetY2 : targetY2 - by;
            dRow = (exits[e].getY() == 0) ? dist1 : dist2;
          } else {
            // Salida en pared lateral. El bloque debe alinearse con el agujero
            // [eRow, eRow + cs - 1] El bloque necesita by >= eRow y by + bh <=
            // eRow + cs
            int validMinY = exits[e].getY();
            int validMaxY = exits[e].getY() + maxSize - bh;
            if (by < validMinY)
              dRow = validMinY - by;
            else if (by > validMaxY)
              dRow = by - validMaxY;
          }

          // La distancia total es la suma de desplazamientos necesarios.
          // Se divide entre 3 para hacerla admisible (aprox) ya que en 1
          // movimiento puede avanzar varias celdas, manteniendo un buen
          // gradiente hacia la meta.
          // Se suma 1 porque ahora la evacuación en sí requiere un paso extra.
          int dist = (dCol + dRow) / 3 + 1;
          if (dist < minDist)
            minDist = dist;
        }
      }
    }

    if (!hasValidExit) {
      // Bloque sin salida compatible: es un obstaculo, no penalizar
      continue;
    } else {
      totalH += minDist;
    }
  }
  this->h = totalH;
}

bool GameState::canEvacuate(int blockIndex, char &outDir, int &outDist) const {
  if (evacuated[blockIndex])
    return false;
  const Block &b = board.getBlock(blockIndex);
  char blockColor = b.getColor();
  int bx = b.getX();
  int by = b.getY();
  int bw = b.getWidth();
  int bh = b.getHeight();
  int boardW = board.getWidth();
  int boardH = board.getHeight();

  for (int e = 0; e < numExits; e++) {
    if (exits[e].getColor() != blockColor)
      continue;
    int eCol = exits[e].getX();
    int eRow = exits[e].getY();
    char ori = exits[e].getOrientation();
    int cs = exits[e].getSizeAtStep(g);

    if (ori == 'H') {
      if (eCol == 0 && bx == 1) {
        if (bh <= cs && by >= eRow && by + bh <= eRow + cs) {
          outDir = 'L';
          outDist = 1;
          return true;
        }
      } else if (eCol == boardW - 1 && bx + bw == boardW - 1) {
        if (bh <= cs && by >= eRow && by + bh <= eRow + cs) {
          outDir = 'R';
          outDist = 1;
          return true;
        }
      }
    } else if (ori == 'V') {
      if (eRow == 0 && by == 1) {
        if (bw <= cs && bx >= eCol && bx + bw <= eCol + cs) {
          outDir = 'U';
          outDist = 1;
          return true;
        }
      } else if (eRow == boardH - 1 && by + bh == boardH - 1) {
        if (bw <= cs && bx >= eCol && bx + bw <= eCol + cs) {
          outDir = 'D';
          outDist = 1;
          return true;
        }
      }
    }
  }
  return false;
}

// Metodo: printBoard
// Descripcion: Imprime el tablero de la misma forma que Board::printBoard,
// pero superpone las letras de las salidas (en minuscula) sobre los muros.
// Ademas, no imprime '.' fuera de los limites de los muros de cada fila.
void GameState::printBoard() const {
  int w = board.getWidth();
  int h = board.getHeight();
  for (int y = 0; y < h; y++) {
    // Encontrar los limites de las paredes en esta fila
    int firstWall = w;
    int lastWall = -1;
    for (int x = 0; x < w; x++) {
      int val = board.getCell(x, y);
      bool isExit = false;
      for (int e = 0; e < numExits; e++) {
        int ex = exits[e].getX();
        int ey = exits[e].getY();
        char ori = exits[e].getOrientation();
        int cs = exits[e].getSizeAtStep(g);
        if (ori == 'V' && y == ey && x >= ex && x < ex + cs)
          isExit = true;
        if (ori == 'H' && x == ex && y >= ey && y < ey + cs)
          isExit = true;
      }
      if (val == 1 || isExit) {
        if (x < firstWall)
          firstWall = x;
        if (x > lastWall)
          lastWall = x;
      }
    }

    for (int x = 0; x < w; x++) {
      char exitColor = ' ';
      for (int e = 0; e < numExits; e++) {
        int ex = exits[e].getX();
        int ey = exits[e].getY();
        char ori = exits[e].getOrientation();
        int cs = exits[e].getSizeAtStep(g);

        if (ori == 'V') { // Movimiento vertical -> agujero horizontal
          if (y == ey && x >= ex && x < ex + cs) {
            exitColor = exits[e].getColor();
            break;
          }
        } else if (ori == 'H') { // Movimiento horizontal -> agujero vertical
          if (x == ex && y >= ey && y < ey + cs) {
            exitColor = exits[e].getColor();
            break;
          }
        }
      }

      if (exitColor != ' ') {
        std::cout << exitColor;
      } else {
        int val = board.getCell(x, y);
        if (val == 1) {
          std::cout << '#';
        } else if (val == 0) {
          // Si esta fuera de los muros, imprimir espacio en lugar de '.'
          if (x < firstWall || x > lastWall) {
            std::cout << ' ';
          } else {
            std::cout << '.';
          }
        } else {
          int blockId = val - 10;
          bool found = false;
          for (int i = 0; i < board.getNumBlocks(); i++) {
            if (board.getBlock(i).getID() == blockId) {
              char c = board.getBlock(i).getColor();
              if (c >= 'a' && c <= 'z') {
                std::cout << (char)(c - 32);
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
    }
    std::cout << std::endl;
  }
}
