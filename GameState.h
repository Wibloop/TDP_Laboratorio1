#ifndef GameState_h
#define GameState_h

#include "Board.h"
#include "Exit.h"
#include "Gate.h"

/**
 * GameState class
 * Representa un nodo completo en el arbol de busqueda A*.
 * Encapsula el estado del tablero (Board), las salidas (Exit[]),
 * las compuertas (Gate[]), los costos g (costo real) y h (heuristica),
 * y un puntero al estado padre para poder reconstruir el camino solucion.
 *
 * Tambien almacena la descripcion del movimiento que genero este estado
 * (ej: "R1,3" = mover bloque 1 tres celdas a la derecha).
 *
 * Es la unidad fundamental que el MinHeap y HashSet manejan.
 */

class GameState {

private:
  Board board;  // Copia del tablero en este estado
  Exit *exits;  // Arreglo de salidas del nivel
  int numExits; // Cantidad de salidas
  Gate *gates;  // Arreglo de compuertas del nivel
  int numGates; // Cantidad de compuertas
  int g;        // Costo real: numero de movimientos desde el estado inicial
  int h;        // Heuristica: estimacion de movimientos restantes
  GameState *parent; // Puntero al estado padre (para reconstruir camino)
  char moveDesc[16]; // Descripcion del movimiento (ej: "R1,3\0")
  bool *evacuated;   // Arreglo que indica si cada bloque fue evacuado

public:
  // Constructor
  GameState(const Board &board, Exit *exits, int numExits, Gate *gates,
            int numGates);

  // Constructor de copia
  GameState(const GameState &other);

  // Operador de asignacion
  GameState &operator=(const GameState &other);

  // Destructor
  ~GameState();

  // Getters
  Board &getBoard();
  const Board &getBoard() const;
  int getG() const;
  int getH() const;
  int getF() const; // f = g + h
  GameState *getParent() const;
  const char *getMoveDesc() const;
  Exit *getExits() const;
  int getNumExits() const;
  Gate *getGates() const;
  int getNumGates() const;
  bool isBlockEvacuated(int index) const;

  // Setters
  void setG(int g);
  void setH(int h);
  void setParent(GameState *parent);
  void setMoveDesc(const char *desc);
  void setEvacuated(int index, bool value);

  // Metodos principales

  // isSolved: Retorna true si todos los bloques han sido evacuados
  bool isSolved() const;

  // computeHash: Calcula un hash unico del estado actual
  //              basado en las posiciones de los bloques y su estado de
  //              evacuacion
  unsigned long computeHash() const;

  // equals: Compara si dos estados son equivalentes
  //         (misma configuracion de bloques en el tablero)
  bool equals(const GameState &other) const;

  // computeHeuristic: Calcula la heuristica admisible h.
  //                   Suma la distancia Manhattan de cada bloque no evacuado
  //                   a su salida mas cercana del mismo color.
  //                   Usa getSizeAtStep(g) para considerar fluctuacion
  //                   temporal.
  void computeHeuristic();

  // canEvacuate: Verifica si un bloque especifico puede ser evacuado
  //              (esta adyacente a su salida y alineado con ella).
  //              Si lo esta, retorna true y asigna la direccion y distancia en outDir y outDist.
  bool canEvacuate(int blockIndex, char& outDir, int& outDist) const;

  // printBoard: Imprime el tablero superponiendo las letras de las salidas
  void printBoard() const;
};

#endif /* GameState_h */
