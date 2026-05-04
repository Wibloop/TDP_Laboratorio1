#ifndef HashSet_h
#define HashSet_h

#include "GameState.h"

/**
 * HashSet class
 * Implementa una tabla hash con encadenamiento (chaining) para servir como
 * el Closed Set del algoritmo A*.
 *
 * Proposito:
 * - Almacenar estados ya visitados para evitar explorar duplicados.
 * - La clave es el hash del estado (computeHash de GameState).
 * - Ante colisiones, se usa una lista enlazada en cada bucket.
 *
 * Complejidad:
 * - Insercion: O(1) amortizado (O(N) en peor caso por rehash).
 * - Busqueda (contains): O(1) amortizado.
 * - El factor de carga se mantiene bajo 0.75 mediante rehashing automatico.
 *
 * Estructura interna:
 * - Arreglo de buckets (HashNode**), donde cada bucket es una lista enlazada.
 * - HashNode contiene: hash, puntero al GameState, puntero al siguiente nodo.
 *
 * NOTA: El HashSet NO es dueno de los GameState*. No los libera.
 */

class HashSet {

    private:
        // Nodo interno para la lista enlazada en cada bucket
        struct HashNode {
            unsigned long hash;     // Hash pre-calculado del estado
            GameState* state;       // Puntero al estado almacenado
            HashNode* next;         // Siguiente nodo en la cadena (nullptr si es el ultimo)
        };

        HashNode** buckets;         // Arreglo de punteros a listas enlazadas
        int capacity;               // Numero de buckets
        int size;                   // Cantidad total de elementos insertados

        // rehash: Duplica la cantidad de buckets y redistribuye todos los nodos.
        //         Se llama cuando el factor de carga supera 0.75.
        //         Complejidad: O(N) donde N es la cantidad de elementos.
        void rehash();

        // getBucketIndex: Calcula el indice del bucket para un hash dado.
        //                 Usa modulo para mapear el hash al rango [0, capacity).
        int getBucketIndex(unsigned long hash) const;

    public:
        // Constructor
        HashSet(int initialCapacity = 1024);

        // Destructor
        ~HashSet();

        // insert: Inserta un estado en el set. No inserta duplicados.
        //         Retorna true si se inserto, false si ya existia.
        bool insert(GameState* state);

        // contains: Verifica si un estado equivalente ya existe en el set.
        //           Compara primero por hash (rapido) y luego por equals (exacto).
        bool contains(GameState* state) const;

        // getSize: Retorna la cantidad de elementos en el set.
        int getSize() const;

};

#endif /* HashSet_h */
