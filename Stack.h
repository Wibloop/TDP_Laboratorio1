// Declaracion de la clase, atributos privados y metodos publicos

#ifndef Stack_h
#define Stack_h // Si no esta definido Stack_h, definelo y lee hasta el final

// Se comenta que representa la clase (Stack en este caso) a modo de abstraccion en lenguaje natural

/**
 * Stack class:
 * Esta clase implementa una Pila Dinamica (LIFO) para almacenar enteros
 * Requerida para manejar nodos o estados sin utilizar contenedores STL
 * Utiliza un arreglo dinamico que crece automaticamente
 */

class Stack{
    // Las variables (atributos / datos) son privadas
    private:
        int* data;      // Puntero al arreglo dinamico
        int capacity;   // Capacidad maxima actual
        int topIndex;   // Indice del elemento superior de la pila

        // Metodo privado de apoyo para redimensionar el arreglo si se llena void resize();
        void resize();

    // Los metodos (funciones) son publicos
    public:
        // Constructor: se llama automaticamente al crear la pila
        Stack(int initialCapacity = 10);

        // Destructor: se llama automaticamente al destruir la pila (limpia memoria)
        ~Stack();

        // Metodos principales (Operaciones de la pila)
        void push(int value);
        int pop();
        int peek();
        bool isEmpty();

};

#endif /* Stack_h */ // Se utiliza para darle fin a los include Guards y avisar que hasta aqui es el archivo Stack_h
 
