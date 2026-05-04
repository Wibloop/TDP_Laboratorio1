#include "Exit.h"

// Implementacion de la clase Exit
// Cada metodo se documenta con nombre, descripcion, parametros y retorno

// Metodo: Constructor por defecto -> Exit
// Descripcion: Crea una salida sin inicializar. Necesario para crear arreglos
//              dinamicos de Exit (ej: Exit* exits = new Exit[n]).
// Parametros: Ninguno
// Retorna: void
Exit::Exit() {
  color = ' ';
  x = 0;
  y = 0;
  orientation = 'H';
  lineStart = 0;
  lineEnd = 0;
  step = 0;
}

// Metodo: Constructor parametrizado -> Exit
// Descripcion: Inicializa una salida con todos sus atributos.
//              La salida define por donde puede evacuar un bloque del tablero.
//              - Si orientation es 'H', la salida esta en un borde izquierdo
//              (x=0) o derecho (x=WIDTH-1).
//                Los bloques la cruzan moviendose horizontalmente.
//                lineStart y lineEnd definen el rango de filas (Y) que cubre el
//                portal.
//              - Si orientation es 'V', la salida esta en un borde superior
//              (y=0) o inferior (y=HEIGHT-1).
//                Los bloques la cruzan moviendose verticalmente.
//                lineStart y lineEnd definen el rango de columnas (X) que cubre
//                el portal.
// Parametros:
//      - char color: Color del bloque que puede usar esta salida
//      - int x: Posicion X de la salida
//      - int y: Posicion Y de la salida
//      - char orientation: 'H' horizontal o 'V' vertical
//      - int lineStart: Inicio del rango del portal (LI)
//      - int lineEnd: Fin del rango del portal (LF)
//      - int step: Paso en que se activa (0 = siempre activa)
// Retorna: void
Exit::Exit(char color, int x, int y, char orientation, int lineStart,
           int lineEnd, int step) {
  this->color = color;
  this->x = x;
  this->y = y;
  this->orientation = orientation;
  this->lineStart = lineStart;
  this->lineEnd = lineEnd;
  this->step = step;
}

// Metodo: Constructor de copia -> Exit
// Descripcion: Copia todos los atributos. No hay memoria dinamica que copiar.
// Parametros:
//      - const Exit& other: La salida a copiar
// Retorna: void
Exit::Exit(const Exit &other) {
  color = other.color;
  x = other.x;
  y = other.y;
  orientation = other.orientation;
  lineStart = other.lineStart;
  lineEnd = other.lineEnd;
  step = other.step;
}

// Metodo: Operador de asignacion -> operator=
// Descripcion: Asigna los valores de otra salida a esta.
// Parametros:
//      - const Exit& other: La salida fuente
// Retorna:
//      - Exit&: Referencia a este objeto
Exit &Exit::operator=(const Exit &other) {
  if (this == &other) {
    return *this;
  }
  color = other.color;
  x = other.x;
  y = other.y;
  orientation = other.orientation;
  lineStart = other.lineStart;
  lineEnd = other.lineEnd;
  step = other.step;
  return *this;
}

// Metodo: Destructor -> ~Exit
// Descripcion: No libera memoria porque Exit no tiene atributos dinamicos.
// Parametros: Ninguno
// Retorna: void
Exit::~Exit() {
  // No hay memoria dinamica que liberar
}

// Metodo: getColor
// Descripcion: Retorna el color de la salida
// Parametros: Ninguno
// Retorna: char color
char Exit::getColor() const { return color; }

// Metodo: getX
// Descripcion: Retorna la posicion X de la salida
// Parametros: Ninguno
// Retorna: int x
int Exit::getX() const { return x; }

// Metodo: getY
// Descripcion: Retorna la posicion Y de la salida
// Parametros: Ninguno
// Retorna: int y
int Exit::getY() const { return y; }

// Metodo: getOrientation
// Descripcion: Retorna la orientacion de la salida
// Parametros: Ninguno
// Retorna: char orientation ('H' o 'V')
char Exit::getOrientation() const { return orientation; }

// Metodo: getLineStart
// Descripcion: Retorna la linea inicial del rango del portal
// Parametros: Ninguno
// Retorna: int lineStart
int Exit::getLineStart() const { return lineStart; }

// Metodo: getLineEnd
// Descripcion: Retorna la linea final del rango del portal
// Parametros: Ninguno
// Retorna: int lineEnd
int Exit::getLineEnd() const { return lineEnd; }

// Metodo: getStep
// Descripcion: Retorna el paso en que se activa la salida
// Parametros: Ninguno
// Retorna: int step
int Exit::getStep() const { return step; }

// Metodo: getSizeAtStep
// Descripcion: Calcula el tamaño actual de la salida en el paso de tiempo dado.
//   Si step == 0 o LI == LF, el tamaño es estatico (retorna LI).
//   El tamaño fluctua con un ciclo lineal circular entre LI y LF:
//     LI -> LF -> LI -> LF -> ...
//   Cada transicion de una unidad ocurre cada STEP pasos de tiempo.
//   Las celdas se apagan desde la coordenada mayor hacia la menor.
// Parametros:
//      - int currentStep: El paso de tiempo actual (g del estado)
// Retorna: int tamaño actual de la salida
int Exit::getSizeAtStep(int currentStep) const {
  if (step == 0 || lineStart == lineEnd) {
    return lineStart;
  }
  int range = lineEnd - lineStart; // Puede ser positivo o negativo
  if (range < 0)
    range = -range;
  if (range == 0)
    return lineStart;

  // Posicion en el ciclo lineal circular
  int phase = (currentStep / step) % (2 * range);
  int offset;
  if (phase <= range) {
    offset = phase;
  } else {
    offset = 2 * range - phase;
  }

  // Aplicar en la direccion correcta
  if (lineEnd > lineStart) {
    return lineStart + offset;
  } else {
    return lineStart - offset;
  }
}
