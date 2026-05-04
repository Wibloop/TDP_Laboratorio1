#ifndef AStarSolver_h
#define AStarSolver_h

#include "GameState.h"
#include "MinHeap.h"
#include "HashSet.h"

/**
 * AStarSolver class
 * Orquesta el algoritmo A* para resolver el juego Color Block Jam.
 *
 * El algoritmo A* explora estados del juego en orden de prioridad f = g + h:
 * - g: costo real (numero de movimientos desde el estado inicial)
 * - h: heuristica (estimacion de movimientos restantes)
 *
 * Usa un MinHeap como Open Set (estados por explorar, priorizados por f)
 * y un HashSet como Closed Set (estados ya visitados, para evitar duplicados).
 *
 * Para cada estado, genera sucesores intentando mover cada bloque en las
 * 4 direcciones posibles (U, D, L, R) por cada cantidad de celdas posible.
 * Cada sucesor que no este en el Closed Set se agrega al Open Set.
 *
 * El algoritmo termina cuando:
 * 1. Se encuentra un estado resuelto (todos los bloques evacuados) -> exito
 * 2. El Open Set se vacia -> no hay solucion
 * 3. Se excede el limite de pasos
 *
 * El camino solucion se reconstruye siguiendo los punteros parent
 * desde el estado meta hasta el estado inicial.
 */

class AStarSolver {

    private:
        MinHeap openSet;            // Heap de estados por explorar
        HashSet closedSet;          // Set de estados ya visitados
        GameState** allStates;      // Arreglo de todos los estados creados (para liberar memoria)
        int stateCount;             // Cantidad de estados creados
        int stateCapacity;          // Capacidad del arreglo allStates
        bool solutionFound;         // Flag de solucion encontrada
        GameState* solutionState;   // Estado meta (si se encontro)

        // trackState: Registra un estado para poder liberar su memoria al final
        void trackState(GameState* state);

        // generateSuccessors: Genera todos los estados hijos de un estado dado.
        //   Para cada bloque no evacuado, intenta moverlo en 4 direcciones
        //   tantas celdas como sea posible. Cada movimiento valido genera un sucesor.
        void generateSuccessors(GameState* current);

    public:
        // Constructor
        AStarSolver();

        // Destructor: libera TODA la memoria de estados creados
        ~AStarSolver();

        // solve: Ejecuta el algoritmo A* desde el estado inicial dado.
        //   Retorna true si se encontro solucion, false si no.
        bool solve(GameState* initialState);

        // getSolutionPath: Reconstruye el camino solucion como arreglo de descripciones.
        //   Retorna la cantidad de pasos. Llena el arreglo moves[] con las descripciones.
        int getSolutionPath(char moves[][16], int maxMoves);

        // getSolutionState: Retorna el estado meta
        GameState* getSolutionState() const;

        // printSolution: Imprime la secuencia de movimientos y tableros intermedios
        void printSolution();

};

#endif /* AStarSolver_h */
