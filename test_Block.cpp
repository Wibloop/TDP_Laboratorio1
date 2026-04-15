#include "Block.h"
#include <iostream>

// El uso de namespace std está permitido para aumentar la legibilidad [5]
using namespace std;

int main(){

    // Mensaje de inicio de test :p
    cout << "--- Iniciando Test de Block ---" << endl;

    // Inicializamos el Block con parametros iniciales
    Block miBlock(0, 'n', 3, 4, 0, 0);

    cout << "La posicion inicial del bloque es: "
    << "(" << miBlock.getX() << ", " << miBlock.getY() << ")"
    << endl;

    // Movemos e imprimimos nueva ubicacion
    cout << "Movemos 2 a la derecha y 5 hacia arriba el Block" << endl;
    miBlock.move(2, 5);
    cout << "La nueva posicion del bloque es: "
    << "(" << miBlock.getX() << ", " << miBlock.getY() << ")"
    << endl;
    

}