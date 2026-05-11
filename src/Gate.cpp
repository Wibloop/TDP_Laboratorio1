#include "Gate.h"

// Implementacion de la clase Gate

// constructor vacio para poder meter gates en arrays dinamicos sin que crashee
Gate::Gate() {
    x = 0;
    y = 0;
    orientation = 'H';
    length = 0;
    colorInitial = ' ';
    colorFinal = ' ';
    step = 0;
}

// constructor de verdad que le pone todos los datos de una
// recibe donde esta para donde mira cuanto mide sus colores y cada cuanto cambia
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

// copia normalita de los datos no pide memoria asi que fresh
Gate::Gate(const Gate& other) {
    x = other.x;
    y = other.y;
    orientation = other.orientation;
    length = other.length;
    colorInitial = other.colorInitial;
    colorFinal = other.colorFinal;
    step = other.step;
}

// asignamos un gate a otro copiando valor por valor
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

// destructor vacio pq no usamos news aca adentro
Gate::~Gate() {
    // todo chill sin leaks
}

// devuelve donde esta en x
int Gate::getX() const {
    return x;
}

// devuelve donde esta en y
int Gate::getY() const {
    return y;
}

// nos dice si es h o v
char Gate::getOrientation() const {
    return orientation;
}

// pasa el largo del gate
int Gate::getLength() const {
    return length;
}

// cual es el primer color
char Gate::getColorInitial() const {
    return colorInitial;
}

// cual es el ultimo color
char Gate::getColorFinal() const {
    return colorFinal;
}

// cada cuantos turnos cambia el colorcito
int Gate::getStep() const {
    return step;
}

// saca cuentas para saber de que color esta el gate en un turno exacto
// si el step es 0 siempre es el mismo color sino va ciclando
// le mandamos el step actual del juego o sea la g del estado y hace magia con el modulo
char Gate::getColorAtStep(int currentStep) const {
    if (step == 0 || colorInitial == colorFinal) {
        return colorInitial;
    }
    int range = colorFinal - colorInitial;
    int dir = 1;
    if (range < 0) {
        range = -range;
        dir = -1;
    }
    range += 1;
    int cyclePos = (currentStep / step) % range;
    return (char)(colorInitial + (cyclePos * dir));
}
