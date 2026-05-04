#include "../src/Board.h"
#include "../src/Exit.h"
#include "../src/HashSet.h"
#include <iostream>

using namespace std;

int main() {
  cout << "Iniciando Test de HashSet" << endl;

  // Crear tablero base
  Board board(5, 5, 50);
  Block b1(1, 'a', 2, 2, 1, 1);
  board.addBlock(b1);

  Exit exits[1];
  exits[0] = Exit('a', 0, 0, 'H', 2, 2, 0);

  HashSet set(4); // Capacidad pequeña para forzar rehash

  // Estado 1
  GameState *s1 = new GameState(board, exits, 1, nullptr, 0);

  cout << "Hash de s1: " << s1->computeHash() << endl;

  // Insertar s1
  bool inserted = set.insert(s1);
  cout << "Insertar s1: " << (inserted ? "OK" : "ya existia") << endl;
  cout << "Tamaño: " << set.getSize() << endl;

  // Intentar insertar duplicado
  GameState *s1_dup = new GameState(*s1);
  inserted = set.insert(s1_dup);
  cout << "Insertar duplicado de s1: "
       << (inserted ? "insertado (ERROR)" : "rechazado (OK)") << endl;

  // Verificar contains
  cout << "Contains s1: " << (set.contains(s1) ? "si" : "no") << endl;

  // Crear estado diferente (mover bloque)
  GameState *s2 = new GameState(board, exits, 1, nullptr, 0);
  s2->getBoard().moveBlock(0, 1, 0);
  cout << "Hash de s2 (bloque movido): " << s2->computeHash() << endl;

  inserted = set.insert(s2);
  cout << "Insertar s2 (diferente): " << (inserted ? "OK" : "rechazado (ERROR)")
       << endl;
  cout << "Contains s2: " << (set.contains(s2) ? "si" : "no") << endl;
  cout << "Tamaño: " << set.getSize() << endl;

  // Insertar varios mas para forzar rehash
  for (int i = 0; i < 10; i++) {
    GameState *si = new GameState(board, exits, 1, nullptr, 0);
    si->setG(i +
             10); // Diferente g no afecta el hash (solo posicion y evacuacion)
    // Mover el bloque a diferentes posiciones
    if (i < 5) {
      si->getBoard().moveBlock(0, 0, 1); // Diferentes posiciones
    }
    set.insert(si);
    // Nota: No se liberan estos estados (leak intencional en test)
  }
  cout << "Tamaño despues de multiples inserciones: " << set.getSize() << endl;

  // Liberar estados principales
  delete s1;
  delete s1_dup;
  delete s2;

  cout << "\nFin del Test de HashSet" << endl;
  return 0;
}
