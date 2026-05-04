# ============================================
# Makefile - Color Block Jam Solver con A*
# ============================================
# Compilacion separada de cada test y del programa principal
# Uso:
#   make            -> Compila todo (programa principal + tests)
#   make main       -> Solo el programa principal
#   make tests      -> Solo los tests
#   make test_Stack -> Solo test_Stack
#   make clean      -> Limpia objetos y ejecutables

# --- Variables de compilacion ---
CXX = g++
CXXFLAGS = -Wall -g -std=c++11 -Isrc

# --- Objetos comunes (clases base) ---
OBJ_BLOCK = src/Block.o
OBJ_BOARD = src/Board.o
OBJ_STACK = src/Stack.o
OBJ_EXIT = src/Exit.o
OBJ_GATE = src/Gate.o
OBJ_GAMESTATE = src/GameState.o
OBJ_MINHEAP = src/MinHeap.o
OBJ_HASHSET = src/HashSet.o
OBJ_FILEPARSER = src/FileParser.o
OBJ_ASTARSOLVER = src/AStarSolver.o

# Todos los objetos que el programa principal necesita
OBJ_MAIN = $(OBJ_BLOCK) $(OBJ_BOARD) $(OBJ_EXIT) $(OBJ_GATE) $(OBJ_GAMESTATE) \
           $(OBJ_MINHEAP) $(OBJ_HASHSET) $(OBJ_FILEPARSER) $(OBJ_ASTARSOLVER)

# --- Regla principal: compilar todo ---
all: main tests

# --- Tests agrupados ---
tests: test_Stack test_Block test_MinHeap test_HashSet test_GameState test_Exit test_Gate test_Board test_FileParser test_AStarSolver

# ============================================
# Reglas de Enlazado (Generacion de Ejecutables)
# ============================================

# Programa principal
main: src/main.o $(OBJ_MAIN)
	$(CXX) $(CXXFLAGS) -o main src/main.o $(OBJ_MAIN)

# Tests individuales
test_Stack: test/test_Stack.o $(OBJ_STACK)
	$(CXX) $(CXXFLAGS) -o test_Stack test/test_Stack.o $(OBJ_STACK)

test_Block: test/test_Block.o $(OBJ_BLOCK)
	$(CXX) $(CXXFLAGS) -o test_Block test/test_Block.o $(OBJ_BLOCK)

test_MinHeap: test/test_MinHeap.o $(OBJ_MINHEAP) $(OBJ_GAMESTATE) $(OBJ_BOARD) $(OBJ_BLOCK) $(OBJ_EXIT) $(OBJ_GATE)
	$(CXX) $(CXXFLAGS) -o test_MinHeap test/test_MinHeap.o $(OBJ_MINHEAP) $(OBJ_GAMESTATE) $(OBJ_BOARD) $(OBJ_BLOCK) $(OBJ_EXIT) $(OBJ_GATE)

test_HashSet: test/test_HashSet.o $(OBJ_HASHSET) $(OBJ_GAMESTATE) $(OBJ_BOARD) $(OBJ_BLOCK) $(OBJ_EXIT) $(OBJ_GATE)
	$(CXX) $(CXXFLAGS) -o test_HashSet test/test_HashSet.o $(OBJ_HASHSET) $(OBJ_GAMESTATE) $(OBJ_BOARD) $(OBJ_BLOCK) $(OBJ_EXIT) $(OBJ_GATE)

test_GameState: test/test_GameState.o $(OBJ_GAMESTATE) $(OBJ_BOARD) $(OBJ_BLOCK) $(OBJ_EXIT) $(OBJ_GATE)
	$(CXX) $(CXXFLAGS) -o test_GameState test/test_GameState.o $(OBJ_GAMESTATE) $(OBJ_BOARD) $(OBJ_BLOCK) $(OBJ_EXIT) $(OBJ_GATE)

test_Exit: test/test_Exit.o $(OBJ_EXIT)
	$(CXX) $(CXXFLAGS) -o test_Exit test/test_Exit.o $(OBJ_EXIT)

test_Gate: test/test_Gate.o $(OBJ_GATE)
	$(CXX) $(CXXFLAGS) -o test_Gate test/test_Gate.o $(OBJ_GATE)

test_Board: test/test_Board.o $(OBJ_BOARD) $(OBJ_BLOCK)
	$(CXX) $(CXXFLAGS) -o test_Board test/test_Board.o $(OBJ_BOARD) $(OBJ_BLOCK)

test_FileParser: test/test_FileParser.o $(OBJ_FILEPARSER) $(OBJ_GAMESTATE) $(OBJ_BOARD) $(OBJ_BLOCK) $(OBJ_EXIT) $(OBJ_GATE)
	$(CXX) $(CXXFLAGS) -o test_FileParser test/test_FileParser.o $(OBJ_FILEPARSER) $(OBJ_GAMESTATE) $(OBJ_BOARD) $(OBJ_BLOCK) $(OBJ_EXIT) $(OBJ_GATE)

test_AStarSolver: test/test_AStarSolver.o $(OBJ_ASTARSOLVER) $(OBJ_FILEPARSER) $(OBJ_HASHSET) $(OBJ_MINHEAP) $(OBJ_GAMESTATE) $(OBJ_BOARD) $(OBJ_BLOCK) $(OBJ_EXIT) $(OBJ_GATE)
	$(CXX) $(CXXFLAGS) -o test_AStarSolver test/test_AStarSolver.o $(OBJ_ASTARSOLVER) $(OBJ_FILEPARSER) $(OBJ_HASHSET) $(OBJ_MINHEAP) $(OBJ_GAMESTATE) $(OBJ_BOARD) $(OBJ_BLOCK) $(OBJ_EXIT) $(OBJ_GATE)

# ============================================
# Reglas de Compilacion de Objetos (.o)
# ============================================

# Programa principal
src/main.o: src/main.cpp src/FileParser.h src/AStarSolver.h
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o src/main.o

# Clases base
src/Block.o: src/Block.cpp src/Block.h
	$(CXX) $(CXXFLAGS) -c src/Block.cpp -o src/Block.o

src/Board.o: src/Board.cpp src/Board.h src/Block.h
	$(CXX) $(CXXFLAGS) -c src/Board.cpp -o src/Board.o

src/Stack.o: src/Stack.cpp src/Stack.h
	$(CXX) $(CXXFLAGS) -c src/Stack.cpp -o src/Stack.o

src/Exit.o: src/Exit.cpp src/Exit.h
	$(CXX) $(CXXFLAGS) -c src/Exit.cpp -o src/Exit.o

src/Gate.o: src/Gate.cpp src/Gate.h
	$(CXX) $(CXXFLAGS) -c src/Gate.cpp -o src/Gate.o

# Clases nuevas
src/GameState.o: src/GameState.cpp src/GameState.h src/Board.h src/Block.h src/Exit.h src/Gate.h
	$(CXX) $(CXXFLAGS) -c src/GameState.cpp -o src/GameState.o

src/MinHeap.o: src/MinHeap.cpp src/MinHeap.h src/GameState.h
	$(CXX) $(CXXFLAGS) -c src/MinHeap.cpp -o src/MinHeap.o

src/HashSet.o: src/HashSet.cpp src/HashSet.h src/GameState.h
	$(CXX) $(CXXFLAGS) -c src/HashSet.cpp -o src/HashSet.o

src/FileParser.o: src/FileParser.cpp src/FileParser.h src/GameState.h
	$(CXX) $(CXXFLAGS) -c src/FileParser.cpp -o src/FileParser.o

src/AStarSolver.o: src/AStarSolver.cpp src/AStarSolver.h src/GameState.h src/MinHeap.h src/HashSet.h
	$(CXX) $(CXXFLAGS) -c src/AStarSolver.cpp -o src/AStarSolver.o

# Objetos de tests
test/test_Stack.o: test/test_Stack.cpp src/Stack.h
	$(CXX) $(CXXFLAGS) -c test/test_Stack.cpp -o test/test_Stack.o

test/test_Block.o: test/test_Block.cpp src/Block.h
	$(CXX) $(CXXFLAGS) -c test/test_Block.cpp -o test/test_Block.o

test/test_MinHeap.o: test/test_MinHeap.cpp src/MinHeap.h src/GameState.h
	$(CXX) $(CXXFLAGS) -c test/test_MinHeap.cpp -o test/test_MinHeap.o

test/test_HashSet.o: test/test_HashSet.cpp src/HashSet.h src/GameState.h
	$(CXX) $(CXXFLAGS) -c test/test_HashSet.cpp -o test/test_HashSet.o

test/test_GameState.o: test/test_GameState.cpp src/GameState.h
	$(CXX) $(CXXFLAGS) -c test/test_GameState.cpp -o test/test_GameState.o

test/test_Exit.o: test/test_Exit.cpp src/Exit.h
	$(CXX) $(CXXFLAGS) -c test/test_Exit.cpp -o test/test_Exit.o

test/test_Gate.o: test/test_Gate.cpp src/Gate.h
	$(CXX) $(CXXFLAGS) -c test/test_Gate.cpp -o test/test_Gate.o

test/test_Board.o: test/test_Board.cpp src/Board.h
	$(CXX) $(CXXFLAGS) -c test/test_Board.cpp -o test/test_Board.o

test/test_FileParser.o: test/test_FileParser.cpp src/FileParser.h
	$(CXX) $(CXXFLAGS) -c test/test_FileParser.cpp -o test/test_FileParser.o

test/test_AStarSolver.o: test/test_AStarSolver.cpp src/AStarSolver.h
	$(CXX) $(CXXFLAGS) -c test/test_AStarSolver.cpp -o test/test_AStarSolver.o

# ============================================
# Regla de Limpieza
# ============================================
clean:
	rm -f src/*.o test/*.o main test_Stack test_Block test_MinHeap test_HashSet test_GameState test_Exit test_Gate test_Board test_FileParser test_AStarSolver

# ============================================
# Phony targets (no son archivos reales)
# ============================================
.PHONY: all tests clean