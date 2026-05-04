#include "../src/Gate.h"
#include <iostream>

using namespace std;

int main() {
    cout << "--- Iniciando Test de Gate ---" << endl;
    
    // Gate(int x, int y, char orientation, int length, char colorInitial, char colorFinal, int step)
    Gate myGate(2, 2, 'V', 3, 'a', 'c', 2);
    
    cout << "Compuerta en (" << myGate.getX() << ", " << myGate.getY() << ")" << endl;
    cout << "Orientacion: " << myGate.getOrientation() << ", Largo: " << myGate.getLength() << endl;
    
    cout << "Color en paso 0: " << myGate.getColorAtStep(0) << endl;
    cout << "Color en paso 2: " << myGate.getColorAtStep(2) << endl;
    cout << "Color en paso 4: " << myGate.getColorAtStep(4) << endl;
    
    return 0;
}
