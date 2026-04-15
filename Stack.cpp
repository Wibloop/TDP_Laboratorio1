#include <iostream>
#include "Stack.h"

// El archivo cpp contiene la logica de los metodos definidos en el ehader (.h) en este caso Stack.h
// En los archivos cpp debemos comentar a modo de abstraccion en lenguaje natural lo siguiente:
// Nombre del metodo
// Descripcion detallada de lo que hace
// Los parametros que recibe la funcion
// El retorno (que tipo de dato devuelve)

/**
 * Metodo: Constructor -> Stack (Nombre)
 * Descripcion: Inicializa la pila dinamica reervando memoria para el arreglo
 * Parametros:
 *      - initialCapacity: Capacidad maxima inicial de la pila (por defecto 10)
 * Retorna:
 *      - void
 */

Stack::Stack(int initialCapacity){ // Los :: es el operador de resolucion de ambito
    // A::B -> Esta es la implementacion de la funcion B que le pertenece a la clase A
    capacity = initialCapacity;
    // new Tipo[tamano]
    data = new int[capacity]; // Equivalente a malloc en C
    topIndex = -1;            // -1 indica que la pila esta vacia
};

/*
 * Method: Destructor -> ~Stack
 * Description: Libera la memoria dinámica reservada para el arreglo al destruir el objeto
 * Parameters:
 *  - Ninguno
 * Return:
 *  - void
 */

Stack::~Stack(){
    // Esta es la implementaicon de la funcion ~Stack que le pertenece a la clase Stack
    delete[] data; // Equivalente a free en C, los [] indican que es un arreglo
};

/*
 * Method: resize
 * Description: Duplica la capacidad del arreglo dinámico cuando se llena
 * Parameters:
 *  - Ninguno
 * Return:
 *  - void
 */

void Stack::resize(){
    capacity *= 2;
    int* newData = new int[capacity];

    for(int i = 0; i <= topIndex; i++){
        newData[i] = data[i];
    }

    delete[] data;  // Liberamos la memoria del arreglo vacio
    data = newData; // Apuntamos al nuevo arreglo
};


/*
 * Method: push
 * Description: Apila un nuevo elemento en la cima. Es una operación O(1) amortizada.
 * Parameters:
 *  - value: El número entero a insertar.
 * Return:
 *  - void
 */
void Stack::push(int value) {
    if(topIndex == capacity - 1){
        resize();
    }
    data[++topIndex] = value;
}


/*
 * Method: pop
 * Description: Desapila y retorna el elemento en la cima. Es una operación O(1).
 * Parameters:
 *  - Ninguno.
 * Return:
 *  - int: El valor extraído de la cima. Retorna -1 si está vacía (manejo de error básico).
 */
int Stack::pop(){
    if(isEmpty()){
        std::cerr << "Error: Pila vacia" << std::endl;
        return -1;
    }
    return data[topIndex--];
}


/*
 * Method: peek
 * Description: Retorna el elemento en la cima sin desapilarlo.
 * Parameters:
 *  - Ninguno.
 * Return:
 *  - int: El valor en la cima.
 */
int Stack::peek(){
    if(isEmpty()){
        return -1;
    }
    return data[topIndex];
}


/*
 * Method: isEmpty
 * Description: Verifica si la pila no contiene elementos.
 * Parameters:
 *  - Ninguno.
 * Return:
 *  - bool: True si está vacía, False en caso contrario.
 */
bool Stack::isEmpty(){ // Por definicion en el constructor (Stack) tenemos que si topIndex es -1 entonces esta vacia
    return topIndex == -1;
}