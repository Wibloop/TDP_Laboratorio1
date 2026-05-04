#ifndef MinHeap_h
#define MinHeap_h

#include "GameState.h"

/**
 * MinHeap class
 * Implementa un Min-Heap binario (monticulo minimo) para servir como
 * el Open Set del algoritmo A*.
 *
 * Propiedades:
 * - Los elementos se ordenan por el valor f = g + h de cada GameState.
 * - El elemento con menor f siempre esta en la raiz (indice 0).
 * - Insercion (push): O(log N) — se inserta al final y se hace sift-up.
 * - Extraccion del minimo (pop): O(log N) — se reemplaza la raiz con el ultimo
 *   elemento y se hace sift-down.
 * - Peek: O(1) — retorna la raiz sin removerla.
 *
 * Estructura interna:
 * - Arreglo dinamico de punteros a GameState (GameState**).
 * - Para el nodo en indice i:
 *     - Padre: (i - 1) / 2
 *     - Hijo izquierdo: 2*i + 1
 *     - Hijo derecho: 2*i + 2
 *
 * NOTA: El heap NO es dueno de los GameState*. No los libera al destruirse.
 *       La responsabilidad de liberar la memoria es del AStarSolver.
 */

class MinHeap {

    private:
        GameState** data;   // Arreglo dinamico de punteros a GameState
        int capacity;       // Capacidad maxima actual del arreglo
        int size;           // Cantidad actual de elementos en el heap

        // siftUp: Sube un elemento hasta restaurar la propiedad del heap.
        //         Se llama despues de insertar un nuevo elemento al final.
        void siftUp(int index);

        // siftDown: Baja un elemento hasta restaurar la propiedad del heap.
        //           Se llama despues de extraer la raiz y reemplazarla con el ultimo.
        void siftDown(int index);

        // resize: Duplica la capacidad del arreglo cuando se llena.
        void resize();

    public:
        // Constructor
        MinHeap(int initialCapacity = 64);

        // Destructor
        ~MinHeap();

        // push: Inserta un GameState en el heap. O(log N).
        void push(GameState* state);

        // pop: Extrae y retorna el GameState con menor f. O(log N).
        GameState* pop();

        // peek: Retorna el GameState con menor f sin extraerlo. O(1).
        GameState* peek() const;

        // isEmpty: Retorna true si el heap no tiene elementos.
        bool isEmpty() const;

        // getSize: Retorna la cantidad de elementos en el heap.
        int getSize() const;

};

#endif /* MinHeap_h */
