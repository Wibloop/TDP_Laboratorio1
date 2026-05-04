#include <iostream>
#include "../src/Stack.h"

// El uso de namespace std está permitido para aumentar la legibilidad [5]
using namespace std;

int main() {
    cout << "--- Iniciando Test de Stack ---" << endl;
    
    // Instanciamos la pila con una capacidad inicial pequena para forzar el resize
    Stack miPila(2);

    cout << "Apilando valores: 10, 20, 30" << endl;
    miPila.push(10);
    miPila.push(20);
    miPila.push(30); // Aquí el arreglo dinámico hace resize() automáticamente

    cout << "El valor en la cima (peek) es: " << miPila.peek() << endl;

    cout << "Desapilando (pop): " << miPila.pop() << endl;
    cout << "Desapilando (pop): " << miPila.pop() << endl;

    if (!miPila.isEmpty()) {
        cout << "La pila aun tiene elementos. Cima actual: " << miPila.peek() << endl;
    }

    cout << "Desapilando (pop): " << miPila.pop() << endl;
    
    // Probamos el manejo de error sacando de una pila vacía
    cout << "Intentando desapilar una pila vacia" << endl;
    miPila.pop();

    cout << "--- Fin del Test ---" << endl;
    
    return 0;
}