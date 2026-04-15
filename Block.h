// Generamos dos archivos distintos, los .h y los .cpp
// Esto con el fin de separar la declaracion de la implementacion
// Separar el QUE del COMO se hace o ejecuta una accion

// El archivo .h lo usamos para la declaracion de la clase, atributos privados y metodos publicos

#ifndef Block_h // Siempre va el ifndef junto al endif final para "cerrar" la lectura obligatoria del codigo
#define Block_h

/**
 * Block class
 *
 */

class Block{

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
        
        // Constructor
        Block(int id, char color, int width, int height, int x, int y);

        // Destructor
        ~Block();

        // Getters
        int getID();
        int getX();
        int getY();
        int getWidth();
        int getHeight();
        char getColor();

        // Setters
        void setX(int newX);
        void setY(int newY);

        // Metodos generales
        void move(int newX, int newY);

};

#endif