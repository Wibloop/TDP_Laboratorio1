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

// Metodo: Constructor por defecto -> Block
// Descripcion: Crea un Block sin inicializar (valores por defecto).
//              Necesario para poder crear arreglos dinamicos de Block.
// Parametros: Ninguno
// Retorna: void
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

// Metodo: Constructor -> Block
// Descripcion: Inicializa el objeto Block con los valores proporcionados
//              y reserva memoria para la geometria (todo en true por defecto).
// Parametros:
//      - int id: ID del Block creado
//      - char color: Color del Block creado
//      - int width: Ancho del Block creado
//      - int height: Largo del Block creado
//      - int x: Posicion esquina inicial del Block (X,Y)
//      - int y: Posicion esquina inicial del Block (X,Y)
// Retorna:
//      - void

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

// Metodo: Constructor de copia -> Block
// Descripcion: Crea una copia profunda (deep copy) de otro Block.
//              Es fundamental para el A* porque cada estado del juego
//              necesita su propia copia independiente de los bloques.
//              Sin deep copy, multiples estados compartiran la misma memoria
//              y modificar un estado corrompera los demas.
// Parametros:
//      - const Block& other: Referencia constante al Block a copiar
// Retorna: void
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

// Metodo: Operador de asignacion -> operator=
// Descripcion: Asigna los valores de otro Block a este, liberando la memoria
//              previa y haciendo deep copy de la geometria.
//              Patron "copy and assign" seguro contra auto-asignacion.
// Parametros:
//      - const Block& other: El Block fuente
// Retorna:
//      - Block&: Referencia a este Block (permite encadenamiento a = b = c)
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

// Metodo: Destructor -> ~Block
// Descripcion: Libera memoria de los atributos
// Parametros: Ninguno
// Retorna:
//      - void

Block::~Block() {
    delete[] geometry;
}

// Metodo: getID
// Descripcion: Entrega el ID del Block
// Parametros: Ninguno
// Retorna:
//     - int id

int Block::getID() const {
    return id;
}

// Metodo: getColor
// Descripcion: Entrega el color del Block
// Parametros: Ninguno
// Retorna:
//    - char color;

char Block::getColor() const {
    return color;
}

// Metodo: getX
// Descripcion: Entrega la posicion en X del Block
// Parametros: Ninguno
// Retorna:
//    - int X;

int Block::getX() const {
    return x;
}

// Metodo: getY
// Descripcion: Entrega la posicion en Y del Block
// Parametros: Ninguno
// Retorna:
//    - int Y;

int Block::getY() const {
    return y;
}


// Metodo: getWidth
// Descripcion: Entrega el ancho del Block
// Parametros: Ninguno
// Retorna:
//    - int width;

int Block::getWidth() const {
    return width;
}

// Metodo: getHeight
// Descripcion: Entrega el largo del Block
// Parametros: Ninguno
// Retorna:
//    - int height;

int Block::getHeight() const {
    return height;
}

// Metodo: getGeometry
// Descripcion: Retorna el puntero al arreglo de geometria del bloque.
//              Permite a otras clases (como GameState) consultar
//              que celdas del bounding box estan realmente ocupadas.
// Parametros: Ninguno
// Retorna:
//    - bool*: Puntero al arreglo de geometria (lectura)

bool* Block::getGeometry() const {
    return geometry;
}

// Metodo: setX
// Descripcion: Cambia la posicion x del Block
// Parametros:
//    - int newX: El nuevo valor de X
// Retorna:
//    - void

void Block::setX(int newX) {
    x = newX;
}

// Metodo: setY
// Descripcion: Cambia la posicion y del Block
// Parametros:
//    - int newY: El nuevo valor de Y
// Retorna:
//    - void

void Block::setY(int newY) {
    y = newY;
}

// Metodo: setGeometry
// Descripcion: Reemplaza la geometria actual por una nueva.
//              Libera la geometria anterior y asigna el nuevo puntero.
//              IMPORTANTE: El puntero pasado debe haber sido creado con new[],
//              pues Block se encargara de liberarlo en su destructor.
// Parametros:
//    - bool* geom: Puntero al nuevo arreglo de geometria
// Retorna:
//    - void

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

// Metodo: occupies
// Descripcion: Verifica si este bloque ocupa la celda global (gx, gy).
//              Primero chequea si (gx, gy) esta dentro del bounding box del bloque,
//              luego consulta la geometria para ver si esa celda especifica esta llena.
//              Esto es crucial para la deteccion de colisiones durante los movimientos.
// Parametros:
//    - int gx: Coordenada X global en el tablero
//    - int gy: Coordenada Y global en el tablero
// Retorna:
//    - bool: true si el bloque ocupa esa celda, false en caso contrario

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

// Metodo: operator==
// Descripcion: Compara dos bloques para determinar si representan el mismo
//              estado. Dos bloques son iguales si tienen el mismo ID y la misma
//              posicion. Se usa para comparar estados del juego en A*.
// Parametros:
//    - const Block& other: El otro bloque a comparar
// Retorna:
//    - bool: true si son iguales, false si no

bool Block::operator==(const Block& other) const {
    return id == other.id && x == other.x && y == other.y;
}
