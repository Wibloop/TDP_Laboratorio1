#include "../src/GameState.h"
#include <iostream>

using namespace std;

int main() {
  cout << "Iniciando Test de GameState" << endl;

  // Crear tablero 6x6
  Board board(6, 6, 50);

  // Agregar paredes en los bordes
  for (int x = 0; x < 6; x++) {
    board.addWall(x, 0); // Fila superior
    board.addWall(x, 5); // Fila inferior
  }
  for (int y = 0; y < 6; y++) {
    board.addWall(0, y); // Columna izquierda
    board.addWall(5, y); // Columna derecha
  }

  // Agregar un bloque 2x2 en posicion (1,1) con color 'a'
  Block b1(1, 'a', 2, 2, 1, 1);
  board.addBlock(b1);

  // Agregar un bloque 1x2 en posicion (3,3) con color 'b'
  Block b2(2, 'b', 1, 2, 3, 3);
  board.addBlock(b2);

  cout << "Tablero inicial:" << endl;
  board.printBoard();

  // Crear salidas
  Exit exits[2];
  exits[0] = Exit('a', 5, 1, 'H', 2, 2, 0); // Salida derecha para color 'a'
  exits[1] = Exit('b', 0, 3, 'H', 2, 2, 0); // Salida izquierda para color 'b'

  // Crear GameState
  GameState state(board, exits, 2, nullptr, 0);

  // Test heuristica
  state.computeHeuristic();
  cout << "Heuristica h = " << state.getH() << endl;
  cout << "f = g + h = " << state.getF() << endl;

  // Test hash
  unsigned long hash1 = state.computeHash();
  cout << "Hash del estado: " << hash1 << endl;

  // Crear copia y verificar igualdad
  GameState copy(state);
  cout << "Copy equals original: " << (state.equals(copy) ? "si" : "no")
       << endl;

  // Mover un bloque en la copia y verificar desigualdad
  copy.getBoard().moveBlock(0, 1, 0); // Mover bloque 1 a la derecha
  cout << "Copy (movido) equals original: "
       << (state.equals(copy) ? "si" : "no") << endl;

  unsigned long hash2 = copy.computeHash();
  cout << "Hash original: " << hash1 << ", Hash copia movida: " << hash2
       << endl;
  cout << "Hashes diferentes: " << (hash1 != hash2 ? "si" : "no") << endl;

  // Test isSolved (no deberia estar resuelto)
  cout << "isSolved: " << (state.isSolved() ? "si" : "no") << endl;

  // Test evacuacion
  cout << "Bloque 0 evacuado: " << (state.isBlockEvacuated(0) ? "si" : "no")
       << endl;
  state.setEvacuated(0, true);
  state.setEvacuated(1, true);
  cout << "isSolved (ambos evacuados): " << (state.isSolved() ? "si" : "no")
       << endl;

  // Test moveDesc
  state.setMoveDesc("R1,3");
  cout << "MoveDesc: " << state.getMoveDesc() << endl;

  cout << "\nFin del Test de GameState" << endl;
  return 0;
}
