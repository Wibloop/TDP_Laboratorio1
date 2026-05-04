#include "../src/Exit.h"
#include <iostream>

using namespace std;

int main() {
  cout << "Iniciando Test de Exit" << endl;

  // Exit(char color, int x, int y, char orientation, int lineStart, int
  // lineEnd, int step)
  Exit myExit('r', 5, 5, 'H', 2, 4, 3);

  cout << "Salida de color: " << myExit.getColor() << endl;
  cout << "Posicion: (" << myExit.getX() << ", " << myExit.getY() << ")"
       << endl;
  cout << "Orientacion: " << myExit.getOrientation() << endl;

  cout << "Tamano en paso 0: " << myExit.getSizeAtStep(0) << endl;
  cout << "Tamano en paso 3: " << myExit.getSizeAtStep(3) << endl;
  cout << "Tamano en paso 6: " << myExit.getSizeAtStep(6) << endl;

  return 0;
}
