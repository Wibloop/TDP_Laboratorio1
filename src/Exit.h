#ifndef Exit_h
#define Exit_h

/**
 * Exit class
 * Representa una salida (portal) en el tablero del juego Color Block Jam.
 * Cada salida esta asociada a un color de bloque especifico.
 * Tiene una posicion (x, y), una orientacion (H = horizontal, V = vertical),
 * un largo inicial (lineStart/LI) y un largo final (lineEnd/LF).
 *
 * El tamaño de la salida fluctua entre LI y LF cada STEP pasos de tiempo.
 * Si STEP == 0 o LI == LF, el tamaño es estatico.
 * Las celdas se "apagan" (se vuelven pared) desde la coordenada mayor
 * hacia la menor.
 *
 * Regla de ajuste dimensional:
 *   - ORIENTATION=H: el bloque debe tener WIDTH <= tamaño actual de la salida
 *   - ORIENTATION=V: el bloque debe tener HEIGHT <= tamaño actual de la salida
 */

class Exit {

    private:
        char color;         // Color del bloque que puede salir por aqui
        int x;              // Posicion X de la salida en el grid
        int y;              // Posicion Y de la salida en el grid
        char orientation;   // 'H' = horizontal, 'V' = vertical
        int lineStart;      // Largo inicial del portal (LI en el archivo)
        int lineEnd;        // Largo final del portal (LF en el archivo)
        int step;           // Cada cuantos pasos fluctua el tamaño (0 = estatico)

    public:
        // Constructor por defecto
        Exit();

        // Constructor parametrizado
        Exit(char color, int x, int y, char orientation, int lineStart, int lineEnd, int step);

        // Constructor de copia
        Exit(const Exit& other);

        // Operador de asignacion
        Exit& operator=(const Exit& other);

        // Destructor
        ~Exit();

        // Getters
        char getColor() const;
        int getX() const;
        int getY() const;
        char getOrientation() const;
        int getLineStart() const;
        int getLineEnd() const;
        int getStep() const;

        // Metodo: getSizeAtStep
        // Descripcion: Calcula el tamaño actual de la salida en el paso dado.
        //   Si step == 0 o LI == LF, retorna LI (estatico).
        //   El tamaño fluctua linealmente entre LI y LF cada STEP pasos.
        //   Ciclo triangular: LI -> LF -> LI -> LF -> ...
        int getSizeAtStep(int currentStep) const;

};

#endif /* Exit_h */
