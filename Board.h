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
 * 
 */

class Board{

    private:
        // Necesitamos definir de forma privada CUALES seran los atributos(variables) del Board

        // Dimensiones
        int width;
        int height;
        // Limite de pasos
        int stepLimit;   // El sistema tiene una cantidad de pasos limite 
        int currentStep = 0; // Pasos actuales que lleva el sistema en uso
        // Matriz principal (Mapa)
        // Matriz de tamaño (width * height) para ver si una celda es: vacio, pared, salida o compuerta
        int* grid; 
        // Necesitamos registrar la cantidad de Blocks que se encuentran en el tablero
        Block* blocks; // Arreglo dinamico llamado Blocks que almacena objetos de tipo Block
        int numBlocks; // Cantidad de Blocks en el tablero

        // Necesitamos validar cada movimiento del Block antes de efectuar el mismo
        bool isValidMove(Block b, int x, int y);

    public:
        // Necesitamos definir de forma publica CUALES seran los metodos(funciones) del Board

        // Constructor
        Board(int width, int height, int steplimit);

        // Destructor
        ~Board();

        // Metodos generales
        
        // addBlock: Agrega un bloque al Board
        void addBlock(Block b);
        // addWall: Agrega un muro al Board
        void addWall(int x, int y);
        // moveBlock: Mueve el Block a la posicion especificada y suma un paso a currentStep
        void moveBlock(Block b, int x, int y);
        // isSolved: Metodo que retorna true si TODOS los Blocks salieron del Board, caso contrario retorna false
        bool isSolved(int numBlocks);
        // printBoard: Necesitamos imprimir el estado actual del Board para el menu
        void printBoard(int* grid);


};

#endif