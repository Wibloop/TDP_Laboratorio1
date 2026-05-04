#include "../src/Block.h"
#include "../src/Board.h"
#include <iostream>

using namespace std;

int main() {
  cout << "Iniciando Test de Board" << endl;

  // Board(int width, int height, int steplimit)
  Board board(10, 10, 50);

  cout << "Tablero de " << board.getWidth() << "x" << board.getHeight()
       << " creado." << endl;

  Block b1(0, 'r', 2, 2, 2, 1);
  board.addBlock(b1);
  board.addWall(0, 0);
  board.addWall(0, 1);

  cout << "Numero de bloques actuales: " << board.getNumBlocks() << endl;
  board.printBoard();

  cout << "\nMoviendo bloque id 0 en (1, 0)..." << endl;
  if (board.moveBlock(0, 1, 0)) {
    cout << "Movimiento exitoso." << endl;
  } else {
    cout << "Movimiento fallido." << endl;
  }

  board.printBoard();

  return 0;
}
