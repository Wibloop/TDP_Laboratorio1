#include <iostream>
#include "Block.h"

// Generamos dos archivos distintos, los .h y los .cpp
// Esto con el fin de separar la declaracion de la implementacion
// Separar el QUE del COMO se hace o ejecuta una accion
// El archivo .cpp lo usamos para la implementacion de la clase

// Nombre del metodo
// Descripcion detallada de lo que hace
// Los parametros que recibe la funcion
// El retorno (que tipo de dato devuelve)

// Metodo: Constructor -> Block
// Descripcion: Inicializa el objeto Block
// Parametros:
//      - int id: ID del Block creado
//      - char color: Color del Block creado
//      - int width: Ancho del Block creado
//      - int height: Largo del Block creado
//      - int x: Posicion esquina inicial del Block (X,Y)
//      - int y: Posicion esquina inicial del Block (X,Y)
// Retorna:
//      - void

Block::Block(int id, char color, int width, int height, int x, int y){
    
    // puntero this-> : Asigna el parametro local id(parametro) al atributo id(definido en abtributos private) de este objeto
    this->id = id;
    this->color = color;
    this->width = width;
    this->height = height;
    this->x = x;
    this->y = y;

    // Reserva memoria para la matriz aplanada de geometria usando sus dimensiones
    this->geometry = new bool[width * height]; // Arreglo de booleanos de dimensiones (ancho * largo)

    // Inicializamos TODO el arreglo en true(cada posicion en 1)
    int totalPositions = width * height;
    for(int i = 0; i < totalPositions; i++){
        this->geometry[i] = 1;
    }
}

// Metodo: Destructor -> ~Block
// Descripcion: Libera memoria de los atributos
// Parametros: Ninguno
// Retorna:
//      - void

Block::~Block(){
    delete[] geometry;
}

// Metodo: getID
// Descripcion: Entrega el ID del Block
// Parametros: Ninguno
// Retorna:
//     - int id

int Block::getID(){
    return id;
}

// Metodo: getColor
// Descripcion: Entrega el color del Block
// Parametros: Ninguno
// Retorna:
//    - char color;

char Block::getColor(){
    return color;
}

// Metodo: getX
// Descripcion: Entrega la posicion en X del Block
// Parametros: Ninguno
// Retorna:
//    - int X;

int Block::getX(){
    return x;
}

// Metodo: getY
// Descripcion: Entrega la posicion en Y del Block
// Parametros: Ninguno
// Retorna:
//    - int Y;

int Block::getY(){
    return y;
}


// Metodo: getWidth
// Descripcion: Entrega el ancho del Block
// Parametros: Ninguno
// Retorna:
//    - int width;

int Block::getWidth(){
    return width;
}

// Metodo: getHeight
// Descripcion: Entrega el largo del Block
// Parametros: Ninguno
// Retorna:
//    - int height;

int Block::getHeight(){
    return height;
}

// Metodo: setX
// Descripcion: Cambia la posicion x del Block
// Parametros:
//    - int newX: El nuevo valor de X
// Retorna:
//    - void

void Block::setX(int newX){
    x = newX;
}

// Metodo: setY
// Descripcion: Cambia la posicion y del Block
// Parametros:
//    - int newY: El nuevo valor de Y
// Retorna:
//    - void

void Block::setY(int newY){
    y = newY;
}

// Metodo: move
// Descripcion: Mueve el Block a la posicion entregada por parametro
// Parametros:
//     - int newX: Posicion nueva en X
//     - int neyY: Posicion nueva en Y
// Retorna:
//     - void

void Block::move(int newX, int newY){
    x = newX;
    y = newY;
}
