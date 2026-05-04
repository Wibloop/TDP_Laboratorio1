#ifndef Board_h
#define Board_h
#include "Block.h" // Necesitamos que el Board conozca lo que es un Block


// Generamos dos archivos distintos, los .h y los .cpp
// Esto con el fin de separar la declaracion de la implementacion
// Separar el QUE del COMO se hace o ejecuta una accion

// El archivo .h lo usamos para la declaracion de la clase, atributos privados y metodos publicos


/**
 * Board Class
 * La clase Board actuara como el "estado" para nuestro algoritmo A*
 * Contiene el grid (mapa) del tablero, los bloques sobre el y
 * metodos para manipular, validar movimientos e imprimir el estado.
 * 
 * El grid usa codificacion:
 *   0 = celda vacia
 *   1 = pared (#)
 *   Valores > 1 = ID de bloque + 10 (para distinguir de pared/vacio)
 */

class Board {

    private:
        // Necesitamos definir de forma privada CUALES seran los atributos(variables) del Board

        // Dimensiones
        int width;
        int height;
        // Limite de pasos
        int stepLimit;   // El sistema tiene una cantidad de pasos limite 
        int currentStep; // Pasos actuales que lleva el sistema en uso
        // Matriz principal (Mapa)
        // Matriz de tamaño (width * height) para ver si una celda es: vacio, pared, salida o compuerta
        int* grid; 
        // Necesitamos registrar la cantidad de Blocks que se encuentran en el tablero
        Block* blocks; // Arreglo dinamico llamado Blocks que almacena objetos de tipo Block
        int numBlocks; // Cantidad de Blocks en el tablero
        int blockCapacity; // Capacidad maxima del arreglo de bloques

        // Necesitamos validar cada movimiento del Block antes de efectuar el mismo
        bool isValidMove(int blockIndex, int dx, int dy) const;

    public:
        // Necesitamos definir de forma publica CUALES seran los metodos(funciones) del Board

        // Constructor
        Board(int width, int height, int steplimit);

        // Constructor de copia (deep copy)
        Board(const Board& other);

        // Operador de asignacion
        Board& operator=(const Board& other);

        // Destructor
        ~Board();

        // Getters
        int getWidth() const;
        int getHeight() const;
        int getStepLimit() const;
        int getCurrentStep() const;
        int getNumBlocks() const;
        int* getGrid() const;
        int getCell(int x, int y) const;
        Block& getBlock(int index);
        const Block& getBlock(int index) const;

        // Setters
        void setCell(int x, int y, int value);
        void setCurrentStep(int step);

        // Metodos generales
        
        // addBlock: Agrega un bloque al Board y lo estampa en el grid
        void addBlock(const Block& b);
        // addWall: Agrega un muro al Board
        void addWall(int x, int y);
        // moveBlock: Intenta mover el bloque dado desplazamiento (dx, dy)
        bool moveBlock(int blockIndex, int dx, int dy);
        // removeBlock: Remueve un bloque del grid (no del arreglo, solo marca posicion como removido)
        void removeBlock(int blockIndex);
        // rebuildGrid: Reconstruye el grid desde cero usando la posicion actual de los bloques
        void rebuildGrid();
        // printBoard: Necesitamos imprimir el estado actual del Board para el menu
        void printBoard() const;

};

#endif