#include "FileParser.h"
#include "Gate.h"
#include <cstring>
#include <fstream>
#include <iostream>

// Implementacion del FileParser

// Metodo: Constructor
// Descripcion: Inicializa atributos por defecto
FileParser::FileParser() {
  levelName[0] = '\0';
  width = 0;
  height = 0;
  stepLimit = 50;
  board = nullptr;
  exits = nullptr;
  numExits = 0;
  exitCapacity = 10;
  gates = nullptr;
  numGates = 0;
  gateCapacity = 10;
}

// Metodo: Destructor
// Descripcion: No libera board ni exits porque la propiedad se transfiere
//   al GameState retornado por parse().
FileParser::~FileParser() {
  // La memoria se transfiere al GameState; no liberar aqui
}

// Metodo: skipSpaces
// Descripcion: Avanza el indice pos saltando espacios y tabs
void FileParser::skipSpaces(const char *str, int &pos) {
  while (str[pos] == ' ' || str[pos] == '\t') {
    pos++;
  }
}

// Metodo: parseInt
// Descripcion: Lee un entero desde la posicion actual del string.
//   Maneja signo negativo y avanza pos hasta despues del ultimo digito.
int FileParser::parseInt(const char *str, int &pos) {
  skipSpaces(str, pos);
  int sign = 1;
  if (str[pos] == '-') {
    sign = -1;
    pos++;
  }
  int result = 0;
  while (str[pos] >= '0' && str[pos] <= '9') {
    result = result * 10 + (str[pos] - '0');
    pos++;
  }
  return result * sign;
}

// Metodo: parseChar
// Descripcion: Lee un caracter desde la posicion actual
char FileParser::parseChar(const char *str, int &pos) {
  skipSpaces(str, pos);
  return str[pos++];
}

// Metodo: startsWith
// Descripcion: Retorna true si str comienza con prefix
bool FileParser::startsWith(const char *str, const char *prefix) {
  int i = 0;
  while (prefix[i] != '\0') {
    if (str[i] != prefix[i])
      return false;
    i++;
  }
  return true;
}

// Metodo: parseMeta
// Descripcion: Extrae atributos de lineas tipo "KEY = VALUE" en seccion [META]
void FileParser::parseMeta(const char *line) {
  if (startsWith(line, "NAME")) {
    // Buscar '=' y copiar el valor
    int i = 0;
    while (line[i] != '=' && line[i] != '\0')
      i++;
    if (line[i] == '=') {
      i++;
      while (line[i] == ' ')
        i++;
      int j = 0;
      while (line[i] != '\0' && line[i] != '\n' && line[i] != '\r' && j < 63) {
        levelName[j++] = line[i++];
      }
      levelName[j] = '\0';
    }
  } else if (startsWith(line, "WIDTH")) {
    int i = 0;
    while (line[i] != '=' && line[i] != '\0')
      i++;
    if (line[i] == '=') {
      i++;
      width = parseInt(line, i);
    }
  } else if (startsWith(line, "HEIGHT")) {
    int i = 0;
    while (line[i] != '=' && line[i] != '\0')
      i++;
    if (line[i] == '=') {
      i++;
      height = parseInt(line, i);
    }
  } else if (startsWith(line, "STEP_LIMIT")) {
    int i = 0;
    while (line[i] != '=' && line[i] != '\0')
      i++;
    if (line[i] == '=') {
      i++;
      stepLimit = parseInt(line, i);
    }
  }
}

// Metodo: parseBlock
// Descripcion: Parsea una linea de bloque con formato:
//   ID COLOR=x WIDTH=w HEIGHT=h INIT_X=x INIT_Y=y GEOMETRY=0 1 0 1 ...
//   Crea un Block y lo agrega al board.
void FileParser::parseBlock(const char *line) {
  int pos = 0;
  // Leer ID
  int id = parseInt(line, pos);
  skipSpaces(line, pos);

  // Leer COLOR=x
  char color = ' ';
  int bwidth = 0, bheight = 0, initX = 0, initY = 0;

  // Parsear pares KEY=VALUE
  while (line[pos] != '\0' && line[pos] != '\n' && line[pos] != '\r') {
    skipSpaces(line, pos);
    if (line[pos] == '\0' || line[pos] == '\n' || line[pos] == '\r')
      break;

    if (startsWith(line + pos, "COLOR=")) {
      pos += 6;
      color = line[pos++];
    } else if (startsWith(line + pos, "WIDTH=")) {
      pos += 6;
      bwidth = parseInt(line, pos);
    } else if (startsWith(line + pos, "HEIGHT=")) {
      pos += 7;
      bheight = parseInt(line, pos);
    } else if (startsWith(line + pos, "INIT_X=")) {
      pos += 7;
      initY = parseInt(line, pos); // X en el archivo son las filas (Y interno)
    } else if (startsWith(line + pos, "INIT_Y=")) {
      pos += 7;
      initX =
          parseInt(line, pos); // Y en el archivo son las columnas (X interno)
    } else if (startsWith(line + pos, "GEOMETRY=")) {
      pos += 9;
      // Crear el bloque primero
      Block b(id, color, bwidth, bheight, initX, initY);
      // Leer la geometria: arreglo de 0s y 1s separados por espacio
      bool *geom = new bool[bwidth * bheight];
      for (int i = 0; i < bwidth * bheight; i++) {
        skipSpaces(line, pos);
        if (line[pos] == '1') {
          geom[i] = true;
        } else {
          geom[i] = false;
        }
        pos++;
      }
      b.setGeometry(geom);
      board->addBlock(b);
      return;
    } else {
      pos++; // Saltar caracter desconocido
    }
  }

  // Si no se encontro GEOMETRY, crear bloque con geometria default (todo true)
  Block b(id, color, bwidth, bheight, initX, initY);
  board->addBlock(b);
}

// Metodo: parseWallLine
// Descripcion: Procesa una fila del mapa de paredes. Cada '#' se agrega como
// pared.
void FileParser::parseWallLine(const char *line, int row) {
  for (int col = 0; line[col] != '\0' && line[col] != '\n' &&
                    line[col] != '\r' && col < width;
       col++) {
    if (line[col] == '#') {
      board->addWall(col, row);
    }
  }
}

// Metodo: parseExit
// Descripcion: Parsea una linea de salida con formato:
//   COLOR=x X=n Y=n ORIENTATION=H LI=n LF=n STEP=n
void FileParser::parseExit(const char *line) {
  int pos = 0;
  char color = ' ';
  int x = 0, y = 0, li = 0, lf = 0, step = 0;
  char orientation = 'H';

  while (line[pos] != '\0' && line[pos] != '\n' && line[pos] != '\r') {
    skipSpaces(line, pos);
    if (line[pos] == '\0' || line[pos] == '\n' || line[pos] == '\r')
      break;

    if (startsWith(line + pos, "COLOR=")) {
      pos += 6;
      color = line[pos++];
    } else if (startsWith(line + pos, "ORIENTATION=")) {
      pos += 12;
      char fileOri = line[pos++];
      // Si el agujero es horizontal (H), el movimiento es vertical (V interno)
      orientation = (fileOri == 'H') ? 'V' : 'H';
    } else if (startsWith(line + pos, "LI=")) {
      pos += 3;
      li = parseInt(line, pos);
    } else if (startsWith(line + pos, "LF=")) {
      pos += 3;
      lf = parseInt(line, pos);
    } else if (startsWith(line + pos, "STEP=")) {
      pos += 5;
      step = parseInt(line, pos);
    } else if (startsWith(line + pos, "X=")) {
      pos += 2;
      y = parseInt(line, pos); // X en archivo = Fila (Y interno)
    } else if (startsWith(line + pos, "Y=")) {
      pos += 2;
      x = parseInt(line, pos); // Y en archivo = Columna (X interno)
    } else {
      pos++;
    }
  }

  // Redimensionar si es necesario
  if (numExits >= exitCapacity) {
    exitCapacity *= 2;
    Exit *newExits = new Exit[exitCapacity];
    for (int i = 0; i < numExits; i++)
      newExits[i] = exits[i];
    delete[] exits;
    exits = newExits;
  }

  exits[numExits] = Exit(color, x, y, orientation, li, lf, step);
  numExits++;
}

// Metodo: parseGate
// Descripcion: Parsea una linea de compuerta con formato:
//   X=n Y=n ORIENTATION=H LI=n CI=c CF=c STEP=n
//   Crea un Gate y lo agrega al arreglo de compuertas.
void FileParser::parseGate(const char *line) {
  int pos = 0;
  int x = 0, y = 0, li = 0, step = 0;
  char orientation = 'H';
  char ci = ' ', cf = ' ';

  while (line[pos] != '\0' && line[pos] != '\n' && line[pos] != '\r') {
    skipSpaces(line, pos);
    if (line[pos] == '\0' || line[pos] == '\n' || line[pos] == '\r')
      break;

    if (startsWith(line + pos, "ORIENTATION=")) {
      pos += 12;
      char fileOri = line[pos++];
      orientation = (fileOri == 'H') ? 'V' : 'H';
    } else if (startsWith(line + pos, "LI=")) {
      pos += 3;
      li = parseInt(line, pos);
    } else if (startsWith(line + pos, "CI=")) {
      pos += 3;
      ci = line[pos++];
    } else if (startsWith(line + pos, "CF=")) {
      pos += 3;
      cf = line[pos++];
    } else if (startsWith(line + pos, "STEP=")) {
      pos += 5;
      step = parseInt(line, pos);
    } else if (startsWith(line + pos, "X=")) {
      pos += 2;
      y = parseInt(line, pos); // X en archivo = Fila (Y interno)
    } else if (startsWith(line + pos, "Y=")) {
      pos += 2;
      x = parseInt(line, pos); // Y en archivo = Columna (X interno)
    } else {
      pos++;
    }
  }

  // Redimensionar si es necesario
  if (numGates >= gateCapacity) {
    gateCapacity *= 2;
    Gate *newGates = new Gate[gateCapacity];
    for (int i = 0; i < numGates; i++)
      newGates[i] = gates[i];
    delete[] gates;
    gates = newGates;
  }

  gates[numGates] = Gate(x, y, orientation, li, ci, cf, step);
  numGates++;
}

// Metodo: parse
// Descripcion: Lee el archivo de nivel completo y construye el GameState
// inicial.
//   Recorre linea por linea, detecta secciones por marcadores [META], [BLOCK],
//   etc. y delega a los metodos especializados de parseo. Retorna un GameState*
//   con el tablero, bloques y salidas configurados.
// Parametros:
//      - const char* filename: Ruta al archivo de configuracion
// Retorna: GameState* estado inicial del nivel, nullptr si error
GameState *FileParser::parse(const char *filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: No se pudo abrir el archivo: " << filename
              << std::endl;
    return nullptr;
  }

  // Primera pasada: leer META para obtener dimensiones
  char line[512];
  int section = 0; // 0=none, 1=META, 2=BLOCK, 3=WALL, 4=EXIT, 5=GATE
  int wallRow = 0;

  // Leer META primero
  while (file.getline(line, 512)) {
    // Eliminar \r si existe (archivos Windows)
    int len = 0;
    while (line[len] != '\0')
      len++;
    if (len > 0 && line[len - 1] == '\r')
      line[len - 1] = '\0';

    if (startsWith(line, "[META]")) {
      section = 1;
      continue;
    }
    if (startsWith(line, "[BLOCK]")) {
      section = 2;
      break;
    }
    if (section == 1 && line[0] != '\0')
      parseMeta(line);
  }

  // Crear el tablero con las dimensiones leidas
  board = new Board(width, height, stepLimit);
  exits = new Exit[exitCapacity];
  gates = new Gate[gateCapacity];

  // Continuar leyendo el resto del archivo
  while (file.getline(line, 512)) {
    int len = 0;
    while (line[len] != '\0')
      len++;
    if (len > 0 && line[len - 1] == '\r')
      line[len - 1] = '\0';

    if (startsWith(line, "[BLOCK]")) {
      section = 2;
      continue;
    }
    if (startsWith(line, "[WALL]")) {
      section = 3;
      wallRow = 0;
      continue;
    }
    if (startsWith(line, "[EXIT]")) {
      section = 4;
      continue;
    }
    if (startsWith(line, "[GATE]")) {
      section = 5;
      continue;
    }

    if (line[0] == '\0')
      continue; // Linea vacia

    switch (section) {
    case 2:
      parseBlock(line);
      break;
    case 3:
      parseWallLine(line, wallRow++);
      break;
    case 4:
      parseExit(line);
      break;
    case 5:
      parseGate(line);
      break;
    }
  }

  file.close();

  // Estampar compuertas como paredes en el grid
  for (int i = 0; i < numGates; i++) {
    int gx = gates[i].getX();
    int gy = gates[i].getY();
    char gori = gates[i].getOrientation();
    int glen = gates[i].getLength();
    for (int j = 0; j < glen; j++) {
      if (gori == 'H') {
        board->addWall(gx + j, gy);
      } else {
        board->addWall(gx, gy + j);
      }
    }
  }
  // Construir y retornar el GameState
  GameState *state = new GameState(*board, exits, numExits, gates, numGates);

  // Limpiar memoria temporal (el GameState tiene sus propias copias)
  delete board;
  delete[] exits;
  delete[] gates;
  board = nullptr;
  exits = nullptr;
  gates = nullptr;

  return state;
}
