#include "FileParser.h"
#include "Gate.h"
#include <cstring>
#include <fstream>
#include <iostream>

// Implementacion del FileParser

// Prepara las estructuras internas para la decodificacion de los datos del fichero
FileParser::FileParser() {
  levelName[0] = '\0'; // Inicializamos el nombre vacio
  width = 0;           // Ancho por defecto
  height = 0;          // Alto por defecto
  stepLimit = 50;      // Limite de pasos por defecto
  board = nullptr;     // Puntero nulo por seguridad
  exits = nullptr;     // Puntero nulo por seguridad
  numExits = 0;        // Contador a cero
  exitCapacity = 10;   // Capacidad inicial del arreglo de salidas
  gates = nullptr;     // Puntero nulo por seguridad
  numGates = 0;        // Contador a cero
  gateCapacity = 10;   // Capacidad inicial del arreglo de compuertas
}

// Las limpiezas de arreglos dinamicos como el tablero y las salidas se transfieren al estado
// Por tanto el parser jamas las elimina aca
FileParser::~FileParser() {
  // La memoria se transfiere al GameState; no liberar aqui
}

// Avanza silenciosamente el cursor del buffer de lectura mientras se tope con espacios o saltos de tabulador
void FileParser::skipSpaces(const char *str, int &pos) {
  while (str[pos] == ' ' || str[pos] == '\t') {
    pos++; // Avanzar la posicion si hay espacio o tabulacion
  }
}

// Procesa y devuelve un numero entero leyendo caracter a caracter
// Se salta los espacios en blanco y aplica el signo si detecta un guion
int FileParser::parseInt(const char *str, int &pos) {
  skipSpaces(str, pos); // Ignorar espacios antes del numero
  int sign = 1; // Por defecto es positivo
  
  // Revisamos si es un numero negativo
  if (str[pos] == '-') {
    sign = -1;
    pos++;
  }
  
  int result = 0;
  // Mientras sea un caracter numerico (0-9), lo acumulamos
  while (str[pos] >= '0' && str[pos] <= '9') {
    result = result * 10 + (str[pos] - '0');
    pos++;
  }
  
  return result * sign; // Retornamos el numero aplicando su signo
}

// Devuelve el primer caracter no blanco que encuentra a partir del indice proporcionado
char FileParser::parseChar(const char *str, int &pos) {
  skipSpaces(str, pos); // Ignorar espacios
  return str[pos++]; // Retornar el caracter y luego avanzar la posicion
}

// Verifica si la cadena principal arranca con un prefijo especifico
bool FileParser::startsWith(const char *str, const char *prefix) {
  int i = 0;
  while (prefix[i] != '\0') {
    if (str[i] != prefix[i])
      return false;
    i++;
  }
  return true;
}

// Identifica la clave dentro de las configuraciones meta y le asigna su valor correspondiente
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

// Lee la informacion de la linea correspondiente a un bloque y la parsea
// Extrae el ID el color y sus coordenadas fisicas
// Recuerda que el documento asume que X es la fila y Y es la columna
void FileParser::parseBlock(const char *line) {
  int pos = 0;
  
  // Leer el ID del bloque (es el primer numero en la linea)
  int id = parseInt(line, pos);
  skipSpaces(line, pos);

  // Variables para guardar las propiedades del bloque
  char color = ' ';
  int bwidth = 0, bheight = 0;
  int initX = 0; // Guardara la columna (Y del archivo)
  int initY = 0; // Guardara la fila (X del archivo)

  // Bucle para parsear todos los pares KEY=VALUE hasta fin de linea
  while (line[pos] != '\0' && line[pos] != '\n' && line[pos] != '\r') {
    skipSpaces(line, pos);
    if (line[pos] == '\0' || line[pos] == '\n' || line[pos] == '\r')
      break; // Salir si llegamos al final

    if (startsWith(line + pos, "COLOR=")) {
      pos += 6; // Avanzar despues de "COLOR="
      color = line[pos++];
    } else if (startsWith(line + pos, "WIDTH=")) {
      pos += 6;
      bwidth = parseInt(line, pos);
    } else if (startsWith(line + pos, "HEIGHT=")) {
      pos += 7;
      bheight = parseInt(line, pos);
    } else if (startsWith(line + pos, "INIT_X=")) {
      pos += 7;
      // El archivo usa INIT_X para las FILAS.
      // Nuestro tablero usa y=Fila. Asignamos a initY.
      initY = parseInt(line, pos); 
    } else if (startsWith(line + pos, "INIT_Y=")) {
      pos += 7;
      // El archivo usa INIT_Y para las COLUMNAS.
      // Nuestro tablero usa x=Columna. Asignamos a initX.
      initX = parseInt(line, pos); 
    } else if (startsWith(line + pos, "GEOMETRY=")) {
      pos += 9;
      // Cuando encontramos GEOMETRY, ya tenemos lo necesario para instanciar el bloque
      Block b(id, color, bwidth, bheight, initX, initY);
      
      // Creamos el arreglo que guardara la forma del bloque
      bool *geom = new bool[bwidth * bheight];
      for (int i = 0; i < bwidth * bheight; i++) {
        skipSpaces(line, pos);
        // Cada '1' es un espacio ocupado, '0' es vacio
        if (line[pos] == '1') {
          geom[i] = true;
        } else {
          geom[i] = false;
        }
        pos++;
      }
      
      // Le pasamos la geometria al bloque y lo guardamos
      b.setGeometry(geom);
      board->addBlock(b);
      return; // Fin del parseo para esta linea
    } else {
      pos++; // Saltar caracteres desconocidos para evitar bucles infinitos
    }
  }

  // En caso de que no haya GEOMETRY, se asume un bloque lleno (default)
  Block b(id, color, bwidth, bheight, initX, initY);
  board->addBlock(b);
}

// Metodo: parseWallLine
// Mapea los caracteres de muralla detectados en el texto y los inserta en la matriz del tablero
void FileParser::parseWallLine(const char *line, int row) {
  for (int col = 0; line[col] != '\0' && line[col] != '\n' &&
                    line[col] != '\r' && col < width;
       col++) {
    if (line[col] == '#') {
      board->addWall(col, row);
    }
  }
}

// Extrae todos los parametros de una linea de salida
// El texto especifica color orientacion pasos de la puerta y sus dimensiones
// Es vital notar que la X pasa a ser la fila del tablero y la Y su columna
void FileParser::parseExit(const char *line) {
  int pos = 0;
  char color = ' ';
  int x = 0; // Columna (asignado por Y=)
  int y = 0; // Fila (asignado por X=)
  int li = 0, lf = 0, step = 0;
  char orientation = 'H'; // Orientacion por defecto

  // Bucle de parseo de propiedades de la salida
  while (line[pos] != '\0' && line[pos] != '\n' && line[pos] != '\r') {
    skipSpaces(line, pos);
    if (line[pos] == '\0' || line[pos] == '\n' || line[pos] == '\r')
      break; // Detenerse al final de linea

    if (startsWith(line + pos, "COLOR=")) {
      pos += 6;
      color = line[pos++]; // Extraer el color especifico
    } else if (startsWith(line + pos, "ORIENTATION=")) {
      pos += 12;
      orientation = line[pos++]; // 'H' o 'V'
    } else if (startsWith(line + pos, "LI=")) {
      pos += 3;
      li = parseInt(line, pos); // Largo inicial de la puerta
    } else if (startsWith(line + pos, "LF=")) {
      pos += 3;
      lf = parseInt(line, pos); // Largo final de la puerta
    } else if (startsWith(line + pos, "STEP=")) {
      pos += 5;
      step = parseInt(line, pos); // Pasos para que se abra
    } else if (startsWith(line + pos, "X=")) {
      pos += 2;
      // En los .txt, X indica la FILA (y)
      y = parseInt(line, pos);
    } else if (startsWith(line + pos, "Y=")) {
      pos += 2;
      // En los .txt, Y indica la COLUMNA (x)
      x = parseInt(line, pos);
    } else {
      pos++; // Saltar caracteres basura
    }
  }

  // Verificamos si tenemos capacidad suficiente, si no, duplicamos
  if (numExits >= exitCapacity) {
    exitCapacity *= 2;
    Exit *newExits = new Exit[exitCapacity];
    for (int i = 0; i < numExits; i++)
      newExits[i] = exits[i];
    delete[] exits; // Liberar la memoria antigua
    exits = newExits; // Apuntar al nuevo arreglo expandido
  }

  // Agregamos la nueva salida al arreglo
  exits[numExits] = Exit(color, x, y, orientation, li, lf, step);
  numExits++;
}

// Procesa una linea de compuerta ignorando el color general pero rescatando color inicial y final
// Al igual que los bloques convierte la coordenada X en la fila y la Y en la columna
void FileParser::parseGate(const char *line) {
  int pos = 0;
  int x = 0; // Columna (Y del archivo)
  int y = 0; // Fila (X del archivo)
  int li = 0, step = 0;
  char orientation = 'H';
  char ci = ' ', cf = ' ';

  // Bucle de lectura de las propiedades
  while (line[pos] != '\0' && line[pos] != '\n' && line[pos] != '\r') {
    skipSpaces(line, pos);
    if (line[pos] == '\0' || line[pos] == '\n' || line[pos] == '\r')
      break; // Salimos al llegar al final

    if (startsWith(line + pos, "COLOR=")) {
      // Ignoramos COLOR= ya que la compuerta se rige por CI (color inicial) 
      // y CF (color final). Saltamos el tag y su valor.
      pos += 6;
      pos++; // Saltar la letra del color
    } else if (startsWith(line + pos, "ORIENTATION=")) {
      pos += 12;
      orientation = line[pos++];
    } else if (startsWith(line + pos, "LI=")) {
      pos += 3;
      li = parseInt(line, pos); // Largo inicial de la compuerta
    } else if (startsWith(line + pos, "CI=")) {
      pos += 3;
      ci = line[pos++]; // Color inicial permitido
    } else if (startsWith(line + pos, "CF=")) {
      pos += 3;
      cf = line[pos++]; // Color final permitido luego de que pase el CI
    } else if (startsWith(line + pos, "STEP=")) {
      pos += 5;
      step = parseInt(line, pos); // Pasos para activacion
    } else if (startsWith(line + pos, "X=")) {
      pos += 2;
      // En los .txt, X indica la FILA (y)
      y = parseInt(line, pos);
    } else if (startsWith(line + pos, "Y=")) {
      pos += 2;
      // En los .txt, Y indica la COLUMNA (x)
      x = parseInt(line, pos);
    } else {
      pos++; // Evitar atascarse en caracteres no reconocidos
    }
  }

  // Redimensionar el arreglo si llegamos a su limite
  if (numGates >= gateCapacity) {
    gateCapacity *= 2;
    Gate *newGates = new Gate[gateCapacity];
    for (int i = 0; i < numGates; i++)
      newGates[i] = gates[i];
    delete[] gates; // Limpiamos el arreglo viejo
    gates = newGates;
  }

  // Instanciamos la compuerta y la añadimos a nuestro arreglo
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
