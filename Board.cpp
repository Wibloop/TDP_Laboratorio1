#include <iostream>
#include "Board.h"

// Generamos dos archivos distintos, los .h y los .cpp
// Esto con el fin de separar la declaracion de la implementacion
// Separar el QUE del COMO se hace o ejecuta una accion
// El archivo .cpp lo usamos para la implementacion de la clase

// Nombre del metodo
// Descripcion detallada de lo que hace
// Los parametros que recibe la funcion
// El retorno (que tipo de dato devuelve)

// Metodo: Constructor -> Board
// Descripcion: Inicializa con los valores iniciales un Board
// Parametros:
//      - int width: Numero entero representando el ancho
//      - int height: Numero entero representando el largo
//      - int stepLimit: Limite de pasos permitidos en el Board
//      - int currentStep: Pasos actuales del Board, inicialmente 0
//      - int* grid: Matriz del Board como tal
//      - Block* blocks: Puntero hacia un arreglo de objetos tipo Blocks llamado blocks
//      - int numBlocks: Cantidad de bloque sdentro del Board
// Retorna:
//      - void

Board::Board(int width, int height, int stepLimit){
    this->width = width;
    this->height = height;
    this->stepLimit = stepLimit;
    this->grid = new int[width * height];

};


 