#include "AStarSolver.h"
#include "FileParser.h"
#include <ctime>
#include <iostream>

// main.cpp — Menu principal del juego Color Block Jam Solver
// Este archivo contiene la unica funcion main permitida fuera de las clases

using namespace std;

int main() {
  cout << "Color Block Jam - Solver con A*" << endl;
  int opcion = 0;
  GameState *initialState = nullptr;
  bool levelLoaded = false;
  char filename[256];

  while (opcion != 4) {
    cout << "\nMenu Principal" << endl;
    cout << "1. Cargar archivo de configuracion" << endl;
    cout << "2. Resolver el problema" << endl;
    cout << "3. Mostrar tablero paso a paso" << endl;
    cout << "4. Salir" << endl;
    cout << "Seleccione una opcion: ";
    cin >> opcion;

    switch (opcion) {
    case 1: {
      // Cargar archivo de nivel
      cout << "Ingrese la ruta del archivo: ";
      cin >> filename;

      // Liberar estado anterior si existe
      if (initialState != nullptr) {
        delete initialState;
        initialState = nullptr;
        levelLoaded = false;
      }

      FileParser parser;
      initialState = parser.parse(filename);

      if (initialState != nullptr) {
        levelLoaded = true;
        cout << "\nNivel cargado exitosamente" << endl;
        cout << "Tablero inicial:" << endl;
        initialState->printBoard();
        cout << "Bloques: " << initialState->getBoard().getNumBlocks() << endl;
        cout << "Salidas: " << initialState->getNumExits() << endl;
      } else {
        cout << "Error al cargar el nivel" << endl;
      }
      break;
    }

    case 2: {
      // Resolver con A*
      if (!levelLoaded || initialState == nullptr) {
        cout << "Debe cargar un nivel primero (opcion 1)" << endl;
        break;
      }

      cout << "\nResolviendo con A*" << endl;

      // Crear copia del estado inicial para el solver
      // (el solver modifica los estados internamente)
      GameState *stateCopy = new GameState(*initialState);

      // Medir tiempo con clock()
      clock_t start = clock();

      AStarSolver solver;
      bool found = solver.solve(stateCopy);

      clock_t end = clock();
      double elapsed = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;

      if (found) {
        // Obtener secuencia de movimientos
        char moves[200][16];
        int numMoves = solver.getSolutionPath(moves, 200);

        cout << "\nSolucion encontrada" << endl;
        cout << "Tiempo: " << elapsed << " ms" << endl;
        cout << "Movimientos (" << numMoves << "): ";
        for (int i = 0; i < numMoves; i++) {
          cout << moves[i];
          if (i < numMoves - 1)
            cout << " ";
        }
        cout << endl;
      } else {
        cout << "\nNo se encontro solucion" << endl;
        cout << "Tiempo: " << elapsed << " ms" << endl;
      }
      break;
    }

    case 3: {
      // Mostrar tablero paso a paso
      if (!levelLoaded || initialState == nullptr) {
        cout << "Debe cargar un nivel primero (opcion 1)" << endl;
        break;
      }

      cout << "\nResolviendo y mostrando paso a paso" << endl;

      GameState *stateCopy = new GameState(*initialState);

      AStarSolver solver;
      bool found = solver.solve(stateCopy);

      if (found) {
        solver.printSolution();
      } else {
        cout << "No se encontro solucion para mostrar" << endl;
      }
      break;
    }

    case 4:
      cout << "Saliendo del programa" << endl;
      break;

    default:
      cout << "Opcion invalida. Intente de nuevo" << endl;
      break;
    }
  }

  // Liberar memoria del estado inicial cargado
  if (initialState != nullptr) {
    delete initialState;
  }

  return 0;
}
