#include "../src/FileParser.h"
#include "../src/GameState.h"
#include <iostream>

using namespace std;

int main() {
  cout << "Iniciando Test de FileParser" << endl;

  FileParser parser;
  GameState *state = parser.parse("test/facil1.txt");

  if (state != nullptr) {
    cout << "Nivel parseado correctamente." << endl;
    cout << "Limite de pasos: " << state->getBoard().getStepLimit() << endl;
    cout << "Imprimiendo estado inicial:" << endl;
    state->printBoard();
    delete state;
  } else {
    cout << "Error al parsear el nivel. Asegurate de que facil1.txt existe."
         << endl;
  }

  return 0;
}
