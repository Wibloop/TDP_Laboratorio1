#include "GameState.h"
#include <iostream>
#include <vector>

// Implementacion de GameState. Nodo del arbol de busqueda A*

// aca inicializamos un estado nuevo copiando todo lo del anterior 
// clonamos salidas compuertas y el tablero para que no se pisen entre simulaciones
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

// constructor de copia puro deep copy de todo el estado y la memoria dinamica
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

// sobrecarga para asignar estados sin que nos quede basura en la memoria
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

// borramos la memoria dinamica que pedimos para este nodo
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

// copiamos el string del movimiento que hicimos onda DA,3 al buffer
// le clavamos limite de 15 chars por las dudas
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

// aca miramos si todos los bloques que de verdad podian salir ya lo hicieron
// si hay algun bloque que por geometria o color nunca iba a poder salir lo tomamos como obstaculo y fue
// para saber si cabe vemos si es H el hoyo entonces se mueve vertical y vemos el ancho
// si es V se mueve horizontal y revisamos su alto
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
        if (ori == 'H' && bw <= maxSize) {
          hasCompatibleExit = true;
          break;
        }
        if (ori == 'V' && bh <= maxSize) {
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

// helpers que usamos por aca nomas
static int gcd_temporal(int a, int b) {
  while (b != 0) {
    int t = b;
    b = a % b;
    a = t;
  }
  return a;
}

static int getLevelPeriod(Exit *exits, int numExits, Gate *gates,
                          int numGates) {
  int currentLcm = 1;
  for (int e = 0; e < numExits; e++) {
    int step = exits[e].getStep();
    if (step > 0) {
      int range = exits[e].getLineEnd() - exits[e].getLineStart();
      if (range < 0)
        range = -range;
      if (range > 0) {
        int period = step * 2 * range;
        currentLcm = (currentLcm / gcd_temporal(currentLcm, period)) * period;
      }
    }
  }
  for (int i = 0; i < numGates; i++) {
    int step = gates[i].getStep();
    if (step > 0) {
      int range = gates[i].getColorFinal() - gates[i].getColorInitial();
      if (range < 0)
        range = -range;
      range += 1;
      if (range > 0) {
        int period = step * range;
        currentLcm = (currentLcm / gcd_temporal(currentLcm, period)) * period;
      }
    }
  }
  return currentLcm;
}

// armamos un hash loco fnv1a usando donde estan los bloques y si salieron o no
// la idea es que tire pocos choques para que la hashtable funcione de 10
unsigned long GameState::computeHash() const {
  unsigned long hash = 2166136261UL;
  unsigned long prime = 16777619UL;
  int period = getLevelPeriod(exits, numExits, gates, numGates);
  unsigned long tByte = (unsigned long)(g % period);
  hash = (hash ^ tByte) * prime;
  int n = board.getNumBlocks();
  for (int i = 0; i < n; i++) {
    const Block &b = board.getBlock(i);
    unsigned char xByte = evacuated[i] ? 0 : (unsigned char)(b.getX() + 50);
    hash = (hash ^ xByte) * prime;
    unsigned char yByte = evacuated[i] ? 0 : (unsigned char)(b.getY() + 50);
    hash = (hash ^ yByte) * prime;
    unsigned char evByte = evacuated[i] ? 1 : 0;
    hash = (hash ^ evByte) * prime;
  }
  return hash;
}

// checkea si dos estados son igualitos onda mismos bloques en mismos lugares y evacuados
bool GameState::equals(const GameState &other) const {
  int period = getLevelPeriod(exits, numExits, gates, numGates);
  if ((this->g % period) != (other.g % period))
    return false;
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

// aca sacamos la heuristica que es la suma de las distancias de los bloques a su salida mas cercana
// usamos orientacion para saber si nos movemos por filas o columnas dependiendo de como esta el hoyo
void GameState::computeHeuristic() {
  int totalH = 0;
  int n = board.getNumBlocks();
  for (int i = 0; i < n; i++) {
    if (evacuated[i])
      continue;
    const Block &b = board.getBlock(i);
    char blockColor = b.getColor();

    int bx, by, bw, bh;
    int orig_bw = b.getWidth();
    int orig_bh = b.getHeight();
    bool *geom = b.getGeometry();
    int minCol = orig_bw, maxCol = -1;
    int minRow = orig_bh, maxRow = -1;
    for (int r = 0; r < orig_bh; r++) {
      for (int c = 0; c < orig_bw; c++) {
        if (geom == nullptr || geom[r * orig_bw + c]) {
          if (c < minCol)
            minCol = c;
          if (c > maxCol)
            maxCol = c;
          if (r < minRow)
            minRow = r;
          if (r > maxRow)
            maxRow = r;
        }
      }
    }
    if (maxCol == -1) {
      bx = b.getX();
      by = b.getY();
      bw = orig_bw;
      bh = orig_bh;
    } else {
      bx = b.getX() + minCol;
      by = b.getY() + minRow;
      bw = maxCol - minCol + 1;
      bh = maxRow - minRow + 1;
    }

    int minDist = 99999;
    bool hasValidExit = false;

    for (int e = 0; e < numExits; e++) {
      if (exits[e].getColor() == blockColor) {
        char ori = exits[e].getOrientation();
        int li = exits[e].getLineStart();
        int lf = exits[e].getLineEnd();
        int maxSize = (li > lf) ? li : lf;

        // H -> mov. vertical, bloque necesita WIDTH <= tamaño
        // V -> mov. horizontal, bloque necesita HEIGHT <= tamaño
        bool fits = false;
        if (ori == 'H' && bw <= maxSize)
          fits = true;
        if (ori == 'V' && bh <= maxSize)
          fits = true;

        if (fits) {
          hasValidExit = true;
          // Distancia en X (columnas)
          int dCol = 0;
          if (ori == 'V') {
            int eX = exits[e].getX();
            int eY = exits[e].getY();
            int realTargetX1 = eX;
            int realTargetX2 = eX;
            if (bx <= eX) {
              for (int y = eY; y < eY + maxSize && y < board.getHeight(); y++) {
                for (int x = eX; x >= 0; x--) {
                  if (board.getCell(x, y) == 1) {
                    if (x < realTargetX2)
                      realTargetX2 = x;
                    break;
                  }
                }
              }
              realTargetX2 -= bw;
              dCol = (bx < realTargetX2) ? realTargetX2 - bx : 0;
            } else {
              for (int y = eY; y < eY + maxSize && y < board.getHeight(); y++) {
                for (int x = eX; x < board.getWidth(); x++) {
                  if (board.getCell(x, y) == 1) {
                    if (x > realTargetX1)
                      realTargetX1 = x;
                    break;
                  }
                }
              }
              realTargetX1 += 1;
              dCol = (bx > realTargetX1) ? bx - realTargetX1 : 0;
            }
          } else {
            int validMinX = exits[e].getX();
            int validMaxX = exits[e].getX() + maxSize - bw;
            if (bx < validMinX)
              dCol = validMinX - bx;
            else if (bx > validMaxX)
              dCol = bx - validMaxX;
          }

          // Distancia en Y (filas)
          int dRow = 0;
          if (ori == 'H') {
            int eX = exits[e].getX();
            int eY = exits[e].getY();
            int realTargetY1 = eY;
            int realTargetY2 = eY;
            if (by <= eY) {
              for (int x = eX; x < eX + maxSize && x < board.getWidth(); x++) {
                for (int y = eY; y >= 0; y--) {
                  if (board.getCell(x, y) == 1) {
                    if (y < realTargetY2)
                      realTargetY2 = y;
                    break;
                  }
                }
              }
              realTargetY2 -= bh;
              dRow = (by < realTargetY2) ? realTargetY2 - by : 0;
            } else {
              for (int x = eX; x < eX + maxSize && x < board.getWidth(); x++) {
                for (int y = eY; y < board.getHeight(); y++) {
                  if (board.getCell(x, y) == 1) {
                    if (y > realTargetY1)
                      realTargetY1 = y;
                    break;
                  }
                }
              }
              realTargetY1 += 1;
              dRow = (by > realTargetY1) ? by - realTargetY1 : 0;
            }
          } else {
            int validMinY = exits[e].getY();
            int validMaxY = exits[e].getY() + maxSize - bh;
            if (by < validMinY)
              dRow = validMinY - by;
            else if (by > validMaxY)
              dRow = by - validMaxY;
          }

          // calculamos la distancia final juntando lo que nos movimos en x y en y
          // le sumamos uno extra para que la heuristica pese un pelin mas y el a* vuele mas rapido
          int dist = (dCol + dRow) + 1;
          if (dist < minDist)
            minDist = dist;
        }
      }
    }

    if (!hasValidExit) {
      // si este bloque no tiene por donde salir lo dejamos como obstaculo y no nos penaliza
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
  bool *geom = b.getGeometry();

  for (int e = 0; e < numExits; e++) {
    if (exits[e].getColor() != blockColor)
      continue;

    int eCol = exits[e].getX();
    int eRow = exits[e].getY();
    char ori = exits[e].getOrientation();
    int cs = exits[e].getSizeAtStep(g);

    int dx = 0, dy = 0;
    char oDir = ' ';
    if (ori == 'V') {
      if (bx <= eCol) {
        dx = 1;
        oDir = 'R';
      } else {
        dx = -1;
        oDir = 'L';
      }
    } else {
      if (by <= eRow) {
        dy = 1;
        oDir = 'D';
      } else {
        dy = -1;
        oDir = 'U';
      }
    }

    bool canStep = true;
    bool touchesHole = false;

    for (int r = 0; r < bh; r++) {
      for (int c = 0; c < bw; c++) {
        if (geom == nullptr || geom[r * bw + c]) {
          int gX = bx + c + dx;
          int gY = by + r + dy;

          if (gX < 0 || gX >= board.getWidth() || gY < 0 ||
              gY >= board.getHeight()) {
            // Completamente fuera del tablero
            canStep = false;
          } else {
            int cellVal = board.getCell(gX, gY);
            if (cellVal == 1) {
              // Choca con pared, verificar si es exactamente el agujero
              if (ori == 'V' && gY >= eRow && gY < eRow + cs) {
                touchesHole = true;
              } else if (ori == 'H' && gX >= eCol && gX < eCol + cs) {
                touchesHole = true;
              } else {
                canStep = false;
              }
            } else if (cellVal != 0 && cellVal != b.getID() + 10) {
              // Choca con otro bloque
              canStep = false;
            }
          }
        }
      }
    }

    if (canStep && touchesHole) {
      outDir = oDir;
      outDist = 1;
      return true;
    }
  }
  return false;
}

bool GameState::canJumpGate(int blockIndex, char &outDir, int &outDx,
                            int &outDy) const {
  if (evacuated[blockIndex] || numGates == 0 || gates == nullptr)
    return false;

  const Block &b = board.getBlock(blockIndex);
  char blockColor = b.getColor();

  int orig_bx = b.getX();
  int orig_by = b.getY();
  int orig_bw = b.getWidth();
  int orig_bh = b.getHeight();
  bool *geom = b.getGeometry();
  int minCol = orig_bw, maxCol = -1;
  int minRow = orig_bh, maxRow = -1;
  for (int r = 0; r < orig_bh; r++) {
    for (int c = 0; c < orig_bw; c++) {
      if (geom == nullptr || geom[r * orig_bw + c]) {
        if (c < minCol)
          minCol = c;
        if (c > maxCol)
          maxCol = c;
        if (r < minRow)
          minRow = r;
        if (r > maxRow)
          maxRow = r;
      }
    }
  }
  int bx, by, bw, bh;
  if (maxCol == -1) {
    bx = orig_bx;
    by = orig_by;
    bw = orig_bw;
    bh = orig_bh;
  } else {
    bx = orig_bx + minCol;
    by = orig_by + minRow;
    bw = maxCol - minCol + 1;
    bh = maxRow - minRow + 1;
  }

  for (int gi = 0; gi < numGates; gi++) {
    const Gate &gate = gates[gi];
    if (blockColor != gate.getColorAtStep(g))
      continue;

    char ori = gate.getOrientation();
    int gLen = gate.getLength();
    int gx = gate.getX();
    int gy = gate.getY();

    outDx = 0;
    outDy = 0;
    outDir = ' ';

    if (ori == 'H') {
      if (bw <= gLen && bx >= gx && bx + bw - 1 <= gx + gLen - 1) {
        if (by + bh == gy) {
          outDy = bh + 1;
          outDir = 'D';
        } else if (by == gy + 1) {
          outDy = -bh - 1;
          outDir = 'U';
        }
      }
    } else if (ori == 'V') {
      if (bh <= gLen && by >= gy && by + bh - 1 <= gy + gLen - 1) {
        if (bx + bw == gx) {
          outDx = bw + 1;
          outDir = 'R';
        } else if (bx == gx + 1) {
          outDx = -bw - 1;
          outDir = 'L';
        }
      }
    }

    if (outDir != ' ') {
      return true; // Se encontró una compuerta válida para saltar
    }
  }

  return false;
}

// dibuja el mapita igual que el del board pero le manda las salidas encimadas a la pared
// y de paso quita los puntitos vacios de afuera para que no se vea sucio
void GameState::printBoard() const {
  int w = board.getWidth();
  int h = board.getHeight();

  // primero pillamos donde empiezan y terminan las paredes reales
  std::vector<int> firstWallRow(h, w), lastWallRow(h, -1);
  std::vector<int> firstWallCol(w, h), lastWallCol(w, -1);
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      if (board.getCell(x, y) == 1) {
        if (x < firstWallRow[y])
          firstWallRow[y] = x;
        if (x > lastWallRow[y])
          lastWallRow[y] = x;
        if (y < firstWallCol[x])
          firstWallCol[x] = y;
        if (y > lastWallCol[x])
          lastWallCol[x] = y;
      }
    }
  }

  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      char exitColor = ' ';
      for (int e = 0; e < numExits; e++) {
        int ex = exits[e].getX();
        int ey = exits[e].getY();
        char ori = exits[e].getOrientation();
        int cs = exits[e].getSizeAtStep(g);

        if (ori == 'V') {
          if (y >= ey && y < ey + cs && x == ex) {
            exitColor = exits[e].getColor();
            break;
          }
        } else if (ori == 'H') {
          if (x >= ex && x < ex + cs && y == ey) {
            exitColor = exits[e].getColor();
            break;
          }
        }
      }

      char gateColor = ' ';
      for (int gi = 0; gi < numGates; gi++) {
        int gx = gates[gi].getX();
        int gy = gates[gi].getY();
        char gori = gates[gi].getOrientation();
        int glen = gates[gi].getLength();

        if (gori == 'H') {
          if (y == gy && x >= gx && x < gx + glen) {
            gateColor = gates[gi].getColorAtStep(g);
            break;
          }
        } else if (gori == 'V') {
          if (x == gx && y >= gy && y < gy + glen) {
            gateColor = gates[gi].getColorAtStep(g);
            break;
          }
        }
      }

      if (exitColor != ' ') {
        std::cout << exitColor;
      } else if (gateColor != ' ') {
        if (gateColor >= 'A' && gateColor <= 'Z') {
          std::cout << (char)(gateColor + 32);
        } else {
          std::cout << gateColor;
        }
      } else {
        int val = board.getCell(x, y);
        if (val == 1) {
          std::cout << '#';
        } else if (val == 0) {
          if (x < firstWallRow[y] || x > lastWallRow[y]) {
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
