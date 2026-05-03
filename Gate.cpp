#include "Gate.h"

// Implementacion de la clase Gate

// Metodo: Constructor por defecto -> Gate
// Descripcion: Crea una compuerta sin inicializar. Necesario para arreglos dinamicos.
// Parametros: Ninguno
// Retorna: void
Gate::Gate() {
    x = 0;
    y = 0;
    orientation = 'H';
    length = 0;
    colorInitial = ' ';
    colorFinal = ' ';
    step = 0;
}

// Metodo: Constructor parametrizado -> Gate
// Descripcion: Inicializa una compuerta con todos sus atributos.
// Parametros:
//      - int x: Posicion X de la compuerta
//      - int y: Posicion Y de la compuerta
//      - char orientation: 'H' horizontal o 'V' vertical
//      - int length: Largo de la compuerta (LI)
//      - char colorInitial: Color inicial (CI)
//      - char colorFinal: Color final (CF)
//      - int step: Cada cuantos pasos cambia el color (0 = estatico)
// Retorna: void
Gate::Gate(int x, int y, char orientation, int length,
           char colorInitial, char colorFinal, int step) {
    this->x = x;
    this->y = y;
    this->orientation = orientation;
    this->length = length;
    this->colorInitial = colorInitial;
    this->colorFinal = colorFinal;
    this->step = step;
}

// Metodo: Constructor de copia -> Gate
// Descripcion: Copia todos los atributos. No hay memoria dinamica.
// Parametros:
//      - const Gate& other: La compuerta a copiar
// Retorna: void
Gate::Gate(const Gate& other) {
    x = other.x;
    y = other.y;
    orientation = other.orientation;
    length = other.length;
    colorInitial = other.colorInitial;
    colorFinal = other.colorFinal;
    step = other.step;
}

// Metodo: Operador de asignacion -> operator=
// Descripcion: Asigna los valores de otra compuerta a esta.
// Parametros:
//      - const Gate& other: La compuerta fuente
// Retorna:
//      - Gate&: Referencia a este objeto
Gate& Gate::operator=(const Gate& other) {
    if (this == &other) {
        return *this;
    }
    x = other.x;
    y = other.y;
    orientation = other.orientation;
    length = other.length;
    colorInitial = other.colorInitial;
    colorFinal = other.colorFinal;
    step = other.step;
    return *this;
}

// Metodo: Destructor -> ~Gate
// Descripcion: No libera memoria porque Gate no tiene atributos dinamicos.
// Parametros: Ninguno
// Retorna: void
Gate::~Gate() {
    // No hay memoria dinamica que liberar
}

// Metodo: getX
// Descripcion: Retorna la posicion X de la compuerta
// Parametros: Ninguno
// Retorna: int x
int Gate::getX() const {
    return x;
}

// Metodo: getY
// Descripcion: Retorna la posicion Y de la compuerta
// Parametros: Ninguno
// Retorna: int y
int Gate::getY() const {
    return y;
}

// Metodo: getOrientation
// Descripcion: Retorna la orientacion de la compuerta
// Parametros: Ninguno
// Retorna: char orientation ('H' o 'V')
char Gate::getOrientation() const {
    return orientation;
}

// Metodo: getLength
// Descripcion: Retorna el largo de la compuerta
// Parametros: Ninguno
// Retorna: int length
int Gate::getLength() const {
    return length;
}

// Metodo: getColorInitial
// Descripcion: Retorna el color inicial de la compuerta
// Parametros: Ninguno
// Retorna: char colorInitial
char Gate::getColorInitial() const {
    return colorInitial;
}

// Metodo: getColorFinal
// Descripcion: Retorna el color final de la compuerta
// Parametros: Ninguno
// Retorna: char colorFinal
char Gate::getColorFinal() const {
    return colorFinal;
}

// Metodo: getStep
// Descripcion: Retorna cada cuantos pasos cambia el color
// Parametros: Ninguno
// Retorna: int step
int Gate::getStep() const {
    return step;
}

// Metodo: getColorAtStep
// Descripcion: Calcula el color actual de la compuerta en el paso dado.
//   Si step == 0, retorna colorInitial (estatico).
//   El color cicla: CI, CI+1, CI+2, ..., CF, CI, CI+1, ...
//   El cambio ocurre cada 'step' pasos de tiempo.
// Parametros:
//      - int currentStep: El paso de tiempo actual (g del estado)
// Retorna: char color actual de la compuerta
char Gate::getColorAtStep(int currentStep) const {
    if (step == 0 || colorInitial == colorFinal) {
        return colorInitial;
    }
    int range = (colorFinal - colorInitial) + 1; // Cantidad de colores en el ciclo
    int cyclePos = (currentStep / step) % range;
    return (char)(colorInitial + cyclePos);
}
