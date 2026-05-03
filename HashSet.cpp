#include "HashSet.h"

// Implementacion del HashSet con encadenamiento para el Closed Set de A*

// Metodo: Constructor
// Descripcion: Crea la tabla hash con la capacidad inicial dada.
//   Inicializa todos los buckets en nullptr (vacios).
// Parametros:
//      - int initialCapacity: Numero inicial de buckets (default 1024)
// Retorna: void
HashSet::HashSet(int initialCapacity) {
    capacity = initialCapacity;
    size = 0;
    buckets = new HashNode*[capacity];
    // Inicializar todos los buckets vacios
    for (int i = 0; i < capacity; i++) {
        buckets[i] = nullptr;
    }
}

// Metodo: Destructor
// Descripcion: Libera todos los nodos de las listas enlazadas y el arreglo de buckets.
//   NO libera los GameState apuntados (responsabilidad del AStarSolver).
// Parametros: Ninguno
// Retorna: void
HashSet::~HashSet() {
    for (int i = 0; i < capacity; i++) {
        HashNode* current = buckets[i];
        while (current != nullptr) {
            HashNode* next = current->next;
            delete current;
            current = next;
        }
    }
    delete[] buckets;
}

// Metodo: getBucketIndex
// Descripcion: Mapea un hash al rango [0, capacity) usando modulo.
//   Es fundamental que sea rapido porque se llama en cada insert/contains.
// Parametros:
//      - unsigned long hash: El hash a mapear
// Retorna: int indice del bucket
int HashSet::getBucketIndex(unsigned long hash) const {
    return (int)(hash % (unsigned long)capacity);
}

// Metodo: rehash
// Descripcion: Duplica la cantidad de buckets y redistribuye todos los nodos.
//   Se invoca cuando el factor de carga (size/capacity) supera 0.75.
//   Esto mantiene las cadenas cortas y las operaciones en O(1) amortizado.
//   Proceso:
//   1. Crear nuevo arreglo de buckets con el doble de capacidad
//   2. Recorrer todos los nodos del arreglo viejo
//   3. Re-calcular el bucket de cada nodo con la nueva capacidad
//   4. Insertar al inicio de la lista del nuevo bucket
//   5. Liberar el arreglo viejo (no los nodos, ya fueron movidos)
// Parametros: Ninguno
// Retorna: void
void HashSet::rehash() {
    int oldCapacity = capacity;
    HashNode** oldBuckets = buckets;

    capacity *= 2;
    buckets = new HashNode*[capacity];
    for (int i = 0; i < capacity; i++) {
        buckets[i] = nullptr;
    }

    // Redistribuir todos los nodos existentes
    for (int i = 0; i < oldCapacity; i++) {
        HashNode* current = oldBuckets[i];
        while (current != nullptr) {
            HashNode* next = current->next;
            // Recalcular bucket con nueva capacidad
            int newIndex = getBucketIndex(current->hash);
            // Insertar al inicio de la lista del nuevo bucket
            current->next = buckets[newIndex];
            buckets[newIndex] = current;
            current = next;
        }
    }

    delete[] oldBuckets;
}

// Metodo: insert
// Descripcion: Inserta un estado en el set si no existe uno equivalente.
//   1. Calcula el hash del estado
//   2. Busca en el bucket correspondiente si ya existe (por hash + equals)
//   3. Si no existe, crea un nuevo nodo y lo inserta al inicio de la cadena
//   4. Si el factor de carga supera 0.75, hace rehash
// Parametros:
//      - GameState* state: Estado a insertar
// Retorna: bool true si se inserto, false si ya existia
bool HashSet::insert(GameState* state) {
    unsigned long hash = state->computeHash();
    int index = getBucketIndex(hash);

    // Buscar si ya existe
    HashNode* current = buckets[index];
    while (current != nullptr) {
        // Primero comparar por hash (rapido) y luego por equals (exacto)
        if (current->hash == hash && current->state->equals(*state)) {
            return false; // Ya existe
        }
        current = current->next;
    }

    // No existe, crear nuevo nodo e insertar al inicio de la cadena
    HashNode* newNode = new HashNode();
    newNode->hash = hash;
    newNode->state = state;
    newNode->next = buckets[index];
    buckets[index] = newNode;
    size++;

    // Verificar factor de carga: si size > 0.75 * capacity, rehash
    // Usamos multiplicacion entera: size * 4 > capacity * 3
    if (size * 4 > capacity * 3) {
        rehash();
    }

    return true;
}

// Metodo: contains
// Descripcion: Verifica si existe un estado equivalente en el set.
//   Busca por hash primero (O(1) para acceder al bucket) y luego
//   compara con equals para resolver posibles colisiones de hash.
// Parametros:
//      - GameState* state: Estado a buscar
// Retorna: bool true si existe, false si no
bool HashSet::contains(GameState* state) const {
    unsigned long hash = state->computeHash();
    int index = getBucketIndex(hash);

    HashNode* current = buckets[index];
    while (current != nullptr) {
        if (current->hash == hash && current->state->equals(*state)) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Metodo: getSize
// Descripcion: Retorna la cantidad de estados almacenados
// Parametros: Ninguno
// Retorna: int
int HashSet::getSize() const {
    return size;
}
