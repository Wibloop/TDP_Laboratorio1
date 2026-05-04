#include "../src/Board.h"
#include "../src/Exit.h"
#include "../src/MinHeap.h"
#include <iostream>

using namespace std;

int main() {
  cout << "Iniciando Test de MinHeap" << endl;

  // Crear un tablero y salidas de prueba para construir GameStates
  Board board(5, 5, 50);
  Exit exits[1];
  exits[0] = Exit('a', 0, 0, 'H', 2, 2, 0);

  // Crear varios GameStates con distintos valores de f = g + h
  MinHeap heap(4); // Capacidad inicial pequeña para probar resize

  // Estado 1: g=5, h=10 -> f=15
  GameState *s1 = new GameState(board, exits, 1, nullptr, 0);
  s1->setG(5);
  s1->setH(10);
  s1->setMoveDesc("R1,2");

  // Estado 2: g=2, h=3 -> f=5
  GameState *s2 = new GameState(board, exits, 1, nullptr, 0);
  s2->setG(2);
  s2->setH(3);
  s2->setMoveDesc("U2,1");

  // Estado 3: g=1, h=1 -> f=2 (menor)
  GameState *s3 = new GameState(board, exits, 1, nullptr, 0);
  s3->setG(1);
  s3->setH(1);
  s3->setMoveDesc("L3,1");

  // Estado 4: g=8, h=2 -> f=10
  GameState *s4 = new GameState(board, exits, 1, nullptr, 0);
  s4->setG(8);
  s4->setH(2);
  s4->setMoveDesc("D4,3");

  // Estado 5: g=0, h=0 -> f=0 (deberia ser el primero al extraer)
  GameState *s5 = new GameState(board, exits, 1, nullptr, 0);
  s5->setG(0);
  s5->setH(0);
  s5->setMoveDesc("INI");

  cout << "Insertando 5 estados con f={15, 5, 2, 10, 0}" << endl;
  heap.push(s1);
  heap.push(s2);
  heap.push(s3);
  heap.push(s4);
  heap.push(s5);

  cout << "Tamaño del heap: " << heap.getSize() << endl;
  cout << "isEmpty: " << (heap.isEmpty() ? "si" : "no") << endl;

  // Extraer en orden: deberia salir f=0, f=2, f=5, f=10, f=15
  cout << "\nExtrayendo en orden de prioridad:" << endl;
  while (!heap.isEmpty()) {
    GameState *s = heap.pop();
    cout << "  f=" << s->getF() << " (g=" << s->getG() << ", h=" << s->getH()
         << ") mov=" << s->getMoveDesc() << endl;
  }

  cout << "isEmpty despues de vaciar: " << (heap.isEmpty() ? "si" : "no")
       << endl;

  // Liberar memoria
  delete s1;
  delete s2;
  delete s3;
  delete s4;
  delete s5;

  cout << "\nFin del Test de MinHeap" << endl;
  return 0;
}
