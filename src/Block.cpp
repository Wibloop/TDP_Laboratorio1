#include <iostream>
#include "Block.h"

// Generamos la separacion logica de implementacion en este fichero cpp

// Constructor default que instancia un bloque en estado seguro nulo
Block::Block() {
    // Se inicializan todos los campos con valores nulos/por defecto
    // para evitar accesos a memoria basura si se usa antes de asignar.
    id = 0;
    color = ' ';
    width = 0;
    height = 0;
    x = 0;
    y = 0;
    geometry = nullptr; // nullptr indica que no hay geometria asignada aun
}

// Constructor principal que fija identificador y color ademas de localizarlo en el mapa
// Reserva memoria en el heap para la matriz aplanada de colisiones del bloque llenandola toda en 1

Block::Block(int id, char color, int width, int height, int x, int y) {
    
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
    for (int i = 0; i < totalPositions; i++) {
        this->geometry[i] = 1;
    }
}

// Constructor de copia para poder instanciar un bloque identico con su propia matriz
// Protege el heap para evitar que los estados compartan los punteros de la malla logica
Block::Block(const Block& other) {
    // Copiamos todos los atributos primitivos directamente
    id = other.id;
    color = other.color;
    width = other.width;
    height = other.height;
    x = other.x;
    y = other.y;

    // Deep copy de geometry: reservamos nueva memoria y copiamos celda a celda.
    // Si copiaramos solo el puntero (shallow copy), ambos bloques apuntarian
    // a la misma memoria y al destruir uno se invalidaria el otro.
    if (other.geometry != nullptr && width > 0 && height > 0) {
        int total = width * height;
        geometry = new bool[total];
        for (int i = 0; i < total; i++) {
            geometry[i] = other.geometry[i];
        }
    } else {
        geometry = nullptr;
    }
}

// Asigna atributos liberando la memoria de la matriz anterior y clonando la del objetivo
// Protegido contra casos donde un bloque se asigne a si mismo
Block& Block::operator=(const Block& other) {
    // Proteccion contra auto-asignacion: si a = a, no hacer nada
    if (this == &other) {
        return *this;
    }

    // Liberar la geometria actual antes de sobreescribir
    delete[] geometry;

    // Copiar todos los atributos primitivos
    id = other.id;
    color = other.color;
    width = other.width;
    height = other.height;
    x = other.x;
    y = other.y;

    // Deep copy de geometry
    if (other.geometry != nullptr && width > 0 && height > 0) {
        int total = width * height;
        geometry = new bool[total];
        for (int i = 0; i < total; i++) {
            geometry[i] = other.geometry[i];
        }
    } else {
        geometry = nullptr;
    }

    return *this;
}

// Destructor encargado de limpiar la geometria alojada en memoria dinamica

Block::~Block() {
    delete[] geometry;
}

// Retorna el identificador unico del bloque

int Block::getID() const {
    return id;
}

// Retorna el color asignado a este bloque en formato char

char Block::getColor() const {
    return color;
}

// Devuelve la posicion X logica del bloque

int Block::getX() const {
    return x;
}

// Devuelve la posicion Y logica del bloque

int Block::getY() const {
    return y;
}


// Devuelve el ancho fisico del bloque

int Block::getWidth() const {
    return width;
}

// Devuelve el largo fisico del bloque

int Block::getHeight() const {
    return height;
}

// Retorna el puntero directo al booleano de la geometria del bloque

bool* Block::getGeometry() const {
    return geometry;
}

// Cambia la coordenada logica x del bloque

void Block::setX(int newX) {
    x = newX;
}

// Cambia la coordenada logica y del bloque

void Block::setY(int newY) {
    y = newY;
}

// Intercambia la matriz logica interna de geometria liberando la memoria previa
// Asume propiedad total del puntero recibido por lo que este debe crearse con new

void Block::setGeometry(bool* geom) {
    // Liberamos la geometria previa si existia
    delete[] geometry;
    // Asignamos la nueva geometria (transferencia de ownership)
    geometry = geom;
}

// Metodo: move
// Descripcion: Mueve el Block a la posicion entregada por parametro
// Parametros:
//     - int newX: Posicion nueva en X
//     - int neyY: Posicion nueva en Y
// Retorna:
//     - void

void Block::move(int newX, int newY) {
    x = newX;
    y = newY;
}

// Verifica si el bloque esta ocupando la celda logica global
// Primero revisa que este dentro de la caja para luego evaluar la matriz de geometria

bool Block::occupies(int gx, int gy) const {
    // Convertimos la coordenada global a local restando la posicion del bloque
    int localX = gx - x;
    int localY = gy - y;

    // Verificamos que la coordenada local esta dentro del bounding box
    if (localX < 0 || localX >= width || localY < 0 || localY >= height) {
        return false;
    }

    // Si no hay geometria, no ocupa nada
    if (geometry == nullptr) {
        return false;
    }

    // La geometria es una matriz aplanada: row = localY, col = localX
    // Indice = localY * width + localX
    return geometry[localY * width + localX];
}

// Compara este bloque con otro basandose en el id de forma y las coordenadas fisicas actuales
// Fundamental para identificar estados duplicados en AStar

bool Block::operator==(const Block& other) const {
    return id == other.id && x == other.x && y == other.y;
}
