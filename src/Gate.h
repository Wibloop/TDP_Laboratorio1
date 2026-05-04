#ifndef Gate_h
#define Gate_h

/**
 * Gate class
 * Representa una compuerta en el tablero del juego Color Block Jam.
 * Una compuerta es una pared especial que puede ser atravesada por un bloque
 * cuyo color coincide con el color actual de la compuerta.
 *
 * El color de la compuerta cicla entre CI (Color Inicial) y CF (Color Final)
 * cada STEP pasos de tiempo. El valor ASCII del color se incrementa en 1
 * hasta llegar a CF, tras lo cual vuelve a CI.
 * Si STEP == 0, el color es estatico en CI.
 *
 * La compuerta tiene una posicion (x, y), una orientacion (H o V),
 * un largo fijo (length) y ocupa celdas en el grid marcadas como pared+compuerta.
 */

class Gate {

    private:
        int x;              // Posicion X de la compuerta en el grid
        int y;              // Posicion Y de la compuerta en el grid
        char orientation;   // 'H' = horizontal, 'V' = vertical
        int length;         // Largo de la compuerta (LI en el archivo)
        char colorInitial;  // Color inicial (CI)
        char colorFinal;    // Color final (CF)
        int step;           // Cada cuantos pasos cambia el color (0 = estatico)

    public:
        // Constructor por defecto
        Gate();

        // Constructor parametrizado
        Gate(int x, int y, char orientation, int length,
             char colorInitial, char colorFinal, int step);

        // Constructor de copia
        Gate(const Gate& other);

        // Operador de asignacion
        Gate& operator=(const Gate& other);

        // Destructor
        ~Gate();

        // Getters
        int getX() const;
        int getY() const;
        char getOrientation() const;
        int getLength() const;
        char getColorInitial() const;
        char getColorFinal() const;
        int getStep() const;

        // Metodo: getColorAtStep
        // Descripcion: Calcula el color actual de la compuerta en el paso dado.
        //   Si step == 0, retorna colorInitial (estatico).
        //   Si no, cicla el valor ASCII entre CI y CF cada 'step' pasos.
        char getColorAtStep(int currentStep) const;
};

#endif /* Gate_h */
