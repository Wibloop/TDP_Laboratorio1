// Generamos dos archivos distintos, los .h y los .cpp
// Esto con el fin de separar la declaracion de la implementacion
// Separar el QUE del COMO se hace o ejecuta una accion

// El archivo .h lo usamos para la declaracion de la clase, atributos privados y metodos publicos

#ifndef Block_h // Siempre va el ifndef junto al endif final para "cerrar" la lectura obligatoria del codigo
#define Block_h

/**
 * Block class
 * Representa una pieza del juego Color Block Jam.
 * Cada bloque tiene un ID unico, un color, dimensiones (width x height),
 * una posicion (x, y) en el tablero y una geometria (matriz booleana aplanada)
 * que indica cuales celdas dentro de su bounding box estan ocupadas.
 */

class Block {

    private:
        // Las variables (atributos) son privados
        
        int id;
        char color;
        int width;
        int height;
        int x;
        int y;
        bool* geometry; // Puntero a un arreglo dinamico de valores booleanos

    public:
        // Los metodos (funciones) son publicos
        
        // Constructor por defecto (necesario para arreglos dinamicos)
        Block();

        // Constructor parametrizado
        Block(int id, char color, int width, int height, int x, int y);

        // Constructor de copia (deep copy de geometry)
        Block(const Block& other);

        // Operador de asignacion (deep copy)
        Block& operator=(const Block& other);

        // Destructor
        ~Block();

        // Getters
        int getID() const;
        int getX() const;
        int getY() const;
        int getWidth() const;
        int getHeight() const;
        char getColor() const;
        bool* getGeometry() const;

        // Setters
        void setX(int newX);
        void setY(int newY);
        void setGeometry(bool* geom);

        // Metodos generales
        void move(int newX, int newY);

        // occupies: Retorna true si la celda global (gx, gy) esta ocupada por este bloque
        bool occupies(int gx, int gy) const;

        // operator==: Compara dos bloques por posicion e ID
        bool operator==(const Block& other) const;

};

#endif