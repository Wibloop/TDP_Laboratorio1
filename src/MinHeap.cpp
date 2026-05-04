#include "MinHeap.h"

// Implementacion del MinHeap binario para el Open Set de A*

// Metodo: Constructor
// Descripcion: Reserva memoria para el arreglo de punteros e inicializa size=0
// Parametros:
//      - int initialCapacity: Capacidad inicial del arreglo (default 64)
// Retorna: void
MinHeap::MinHeap(int initialCapacity) {
    capacity = initialCapacity;
    size = 0;
    data = new GameState*[capacity];
}

// Metodo: Destructor
// Descripcion: Libera el arreglo de punteros. NO libera los GameState apuntados.
// Parametros: Ninguno
// Retorna: void
MinHeap::~MinHeap() {
    delete[] data;
}

// Metodo: resize
// Descripcion: Duplica la capacidad del arreglo cuando se llena.
//   Crea un nuevo arreglo del doble de tamaño, copia los punteros y libera el viejo.
// Parametros: Ninguno
// Retorna: void
void MinHeap::resize() {
    capacity *= 2;
    GameState** newData = new GameState*[capacity];
    for (int i = 0; i < size; i++) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
}

// Metodo: siftUp
// Descripcion: Restaura la propiedad del heap subiendo un elemento.
//   Despues de insertar al final, si el hijo tiene menor f que su padre,
//   se intercambian. Se repite hasta llegar a la raiz o encontrar un padre menor.
//   Complejidad: O(log N) porque la altura del arbol es log2(N).
// Parametros:
//      - int index: Indice del elemento a subir
// Retorna: void
void MinHeap::siftUp(int index) {
    // Mientras no sea la raiz (indice 0)
    while (index > 0) {
        int parentIndex = (index - 1) / 2;
        // Si el padre tiene mayor f, intercambiar
        if (data[parentIndex]->getF() > data[index]->getF()) {
            GameState* temp = data[parentIndex];
            data[parentIndex] = data[index];
            data[index] = temp;
            index = parentIndex; // Subir al nivel del padre
        } else {
            break; // La propiedad del heap se cumple
        }
    }
}

// Metodo: siftDown
// Descripcion: Restaura la propiedad del heap bajando un elemento.
//   Despues de extraer la raiz y poner el ultimo en su lugar, si el padre
//   tiene mayor f que alguno de sus hijos, se intercambia con el menor hijo.
//   Se repite hasta llegar a una hoja o encontrar hijos mayores.
//   Complejidad: O(log N).
// Parametros:
//      - int index: Indice del elemento a bajar
// Retorna: void
void MinHeap::siftDown(int index) {
    while (true) {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        int smallest = index;

        // Comparar con hijo izquierdo
        if (leftChild < size && data[leftChild]->getF() < data[smallest]->getF()) {
            smallest = leftChild;
        }
        // Comparar con hijo derecho
        if (rightChild < size && data[rightChild]->getF() < data[smallest]->getF()) {
            smallest = rightChild;
        }
        // Si el menor no es el actual, intercambiar y continuar
        if (smallest != index) {
            GameState* temp = data[smallest];
            data[smallest] = data[index];
            data[index] = temp;
            index = smallest;
        } else {
            break; // La propiedad del heap se cumple
        }
    }
}

// Metodo: push
// Descripcion: Inserta un GameState en el heap.
//   Se agrega al final del arreglo y se sube (siftUp) hasta su posicion correcta.
//   Si el arreglo esta lleno, se redimensiona primero.
//   Complejidad: O(log N) amortizado.
// Parametros:
//      - GameState* state: Puntero al estado a insertar
// Retorna: void
void MinHeap::push(GameState* state) {
    if (size >= capacity) {
        resize();
    }
    data[size] = state;
    siftUp(size);
    size++;
}

// Metodo: pop
// Descripcion: Extrae y retorna el GameState con menor f (la raiz).
//   Se reemplaza la raiz con el ultimo elemento y se baja (siftDown).
//   Complejidad: O(log N).
// Parametros: Ninguno
// Retorna: GameState* puntero al estado con menor f, nullptr si esta vacio
GameState* MinHeap::pop() {
    if (size == 0) {
        return nullptr;
    }
    GameState* minState = data[0];
    size--;
    data[0] = data[size]; // Mover el ultimo a la raiz
    siftDown(0);          // Restaurar propiedad del heap
    return minState;
}

// Metodo: peek
// Descripcion: Retorna el estado con menor f sin extraerlo. O(1).
// Parametros: Ninguno
// Retorna: GameState* raiz del heap, nullptr si vacio
GameState* MinHeap::peek() const {
    if (size == 0) return nullptr;
    return data[0];
}

// Metodo: isEmpty
// Descripcion: Retorna true si el heap no tiene elementos
// Parametros: Ninguno
// Retorna: bool
bool MinHeap::isEmpty() const {
    return size == 0;
}

// Metodo: getSize
// Descripcion: Retorna la cantidad de elementos en el heap
// Parametros: Ninguno
// Retorna: int
int MinHeap::getSize() const {
    return size;
}
