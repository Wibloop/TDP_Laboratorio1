#include "../src/AStarSolver.h"
#include "../src/FileParser.h"
#include <iostream>

using namespace std;

int main() {
    cout << "--- Iniciando Test de AStarSolver ---" << endl;
    
    FileParser parser;
    GameState* state = parser.parse("test/facil4.txt");
    
    if (state != nullptr) {
        AStarSolver solver;
        cout << "Resolviendo nivel facil4.txt..." << endl;
        
        bool solved = solver.solve(state);
        
        if (solved) {
            cout << "Solucion encontrada!" << endl;
            solver.printSolution();
        } else {
            cout << "No se encontro solucion o se supero el limite de pasos." << endl;
        }
        
        // No borramos 'state' aquí si solver lo maneja, 
        // pero de acuerdo a la clase solver.trackState se encarga de todo lo que procesa.
        // Aún así, solver libera todo en el destructor.
        // Por seguridad no llamamos a delete state a menos que sepamos que no fue trackeado.
    } else {
        cout << "Error al cargar facil4.txt" << endl;
    }
    
    return 0;
}
