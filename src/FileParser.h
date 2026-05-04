#ifndef FileParser_h
#define FileParser_h

#include "GameState.h"

/**
 * FileParser class
 * Responsable de leer y parsear archivos de configuracion de niveles
 * del juego Color Block Jam. Cada archivo contiene secciones:
 * [META], [BLOCK], [WALL], [EXIT], [GATE].
 *
 * El parser lee linea por linea, detecta la seccion actual y
 * extrae los datos necesarios para construir un GameState inicial.
 */

class FileParser {

    private:
        char levelName[64];     // Nombre del nivel
        int width;              // Ancho del tablero
        int height;             // Alto del tablero
        int stepLimit;          // Limite de pasos
        Board* board;           // Tablero construido
        Exit* exits;            // Arreglo de salidas
        int numExits;           // Cantidad de salidas
        int exitCapacity;       // Capacidad del arreglo de salidas
        Gate* gates;            // Arreglo de compuertas
        int numGates;           // Cantidad de compuertas
        int gateCapacity;       // Capacidad del arreglo de compuertas

        // Metodos auxiliares de parseo

        // parseMeta: Extrae nombre, ancho, alto y step_limit de la seccion [META]
        void parseMeta(const char* line);

        // parseBlock: Parsea una linea de bloque y lo agrega al tablero
        void parseBlock(const char* line);

        // parseWallLine: Procesa una fila del mapa de paredes
        void parseWallLine(const char* line, int row);

        // parseExit: Parsea una linea de salida y la agrega al arreglo
        void parseExit(const char* line);

        // parseGate: Parsea una linea de compuerta y la agrega al arreglo
        void parseGate(const char* line);

        // Utilidades de parseo de strings sin STL
        int parseInt(const char* str, int& pos);
        char parseChar(const char* str, int& pos);
        void skipSpaces(const char* str, int& pos);
        bool startsWith(const char* str, const char* prefix);

    public:
        // Constructor
        FileParser();

        // Destructor
        ~FileParser();

        // parse: Lee el archivo completo y construye el estado inicial
        GameState* parse(const char* filename);

};

#endif /* FileParser_h */
