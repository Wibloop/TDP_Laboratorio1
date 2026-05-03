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
CXXFLAGS = -Wall -g -std=c++11

# --- Objetos comunes (clases base) ---
OBJ_BLOCK = Block.o
OBJ_BOARD = Board.o
OBJ_STACK = Stack.o
OBJ_EXIT = Exit.o
OBJ_GATE = Gate.o
OBJ_GAMESTATE = GameState.o
OBJ_MINHEAP = MinHeap.o
OBJ_HASHSET = HashSet.o
OBJ_FILEPARSER = FileParser.o
OBJ_ASTARSOLVER = AStarSolver.o

# Todos los objetos que el programa principal necesita
OBJ_MAIN = $(OBJ_BLOCK) $(OBJ_BOARD) $(OBJ_EXIT) $(OBJ_GATE) $(OBJ_GAMESTATE) \
           $(OBJ_MINHEAP) $(OBJ_HASHSET) $(OBJ_FILEPARSER) $(OBJ_ASTARSOLVER)

# --- Regla principal: compilar todo ---
all: main tests

# --- Tests agrupados ---
tests: test_Stack test_Block test_MinHeap test_HashSet test_GameState

# ============================================
# Reglas de Enlazado (Generacion de Ejecutables)
# ============================================

# Programa principal
main: main.o $(OBJ_MAIN)
	$(CXX) $(CXXFLAGS) -o main main.o $(OBJ_MAIN)

# Tests individuales
test_Stack: test_Stack.o $(OBJ_STACK)
	$(CXX) $(CXXFLAGS) -o test_Stack test_Stack.o $(OBJ_STACK)

test_Block: test_Block.o $(OBJ_BLOCK)
	$(CXX) $(CXXFLAGS) -o test_Block test_Block.o $(OBJ_BLOCK)

test_MinHeap: test_MinHeap.o $(OBJ_MINHEAP) $(OBJ_GAMESTATE) $(OBJ_BOARD) $(OBJ_BLOCK) $(OBJ_EXIT) $(OBJ_GATE)
	$(CXX) $(CXXFLAGS) -o test_MinHeap test_MinHeap.o $(OBJ_MINHEAP) $(OBJ_GAMESTATE) $(OBJ_BOARD) $(OBJ_BLOCK) $(OBJ_EXIT) $(OBJ_GATE)

test_HashSet: test_HashSet.o $(OBJ_HASHSET) $(OBJ_GAMESTATE) $(OBJ_BOARD) $(OBJ_BLOCK) $(OBJ_EXIT) $(OBJ_GATE)
	$(CXX) $(CXXFLAGS) -o test_HashSet test_HashSet.o $(OBJ_HASHSET) $(OBJ_GAMESTATE) $(OBJ_BOARD) $(OBJ_BLOCK) $(OBJ_EXIT) $(OBJ_GATE)

test_GameState: test_GameState.o $(OBJ_GAMESTATE) $(OBJ_BOARD) $(OBJ_BLOCK) $(OBJ_EXIT) $(OBJ_GATE)
	$(CXX) $(CXXFLAGS) -o test_GameState test_GameState.o $(OBJ_GAMESTATE) $(OBJ_BOARD) $(OBJ_BLOCK) $(OBJ_EXIT) $(OBJ_GATE)

# ============================================
# Reglas de Compilacion de Objetos (.o)
# ============================================

# Programa principal
main.o: main.cpp FileParser.h AStarSolver.h
	$(CXX) $(CXXFLAGS) -c main.cpp

# Clases base
Block.o: Block.cpp Block.h
	$(CXX) $(CXXFLAGS) -c Block.cpp

Board.o: Board.cpp Board.h Block.h
	$(CXX) $(CXXFLAGS) -c Board.cpp

Stack.o: Stack.cpp Stack.h
	$(CXX) $(CXXFLAGS) -c Stack.cpp

Exit.o: Exit.cpp Exit.h
	$(CXX) $(CXXFLAGS) -c Exit.cpp

Gate.o: Gate.cpp Gate.h
	$(CXX) $(CXXFLAGS) -c Gate.cpp

# Clases nuevas
GameState.o: GameState.cpp GameState.h Board.h Block.h Exit.h Gate.h
	$(CXX) $(CXXFLAGS) -c GameState.cpp

MinHeap.o: MinHeap.cpp MinHeap.h GameState.h
	$(CXX) $(CXXFLAGS) -c MinHeap.cpp

HashSet.o: HashSet.cpp HashSet.h GameState.h
	$(CXX) $(CXXFLAGS) -c HashSet.cpp

FileParser.o: FileParser.cpp FileParser.h GameState.h
	$(CXX) $(CXXFLAGS) -c FileParser.cpp

AStarSolver.o: AStarSolver.cpp AStarSolver.h GameState.h MinHeap.h HashSet.h
	$(CXX) $(CXXFLAGS) -c AStarSolver.cpp

# Objetos de tests
test_Stack.o: test_Stack.cpp Stack.h
	$(CXX) $(CXXFLAGS) -c test_Stack.cpp

test_Block.o: test_Block.cpp Block.h
	$(CXX) $(CXXFLAGS) -c test_Block.cpp

test_MinHeap.o: test_MinHeap.cpp MinHeap.h GameState.h
	$(CXX) $(CXXFLAGS) -c test_MinHeap.cpp

test_HashSet.o: test_HashSet.cpp HashSet.h GameState.h
	$(CXX) $(CXXFLAGS) -c test_HashSet.cpp

test_GameState.o: test_GameState.cpp GameState.h
	$(CXX) $(CXXFLAGS) -c test_GameState.cpp

# ============================================
# Regla de Limpieza
# ============================================
clean:
	rm -f *.o main test_Stack test_Block test_MinHeap test_HashSet test_GameState

# ============================================
# Phony targets (no son archivos reales)
# ============================================
.PHONY: all tests clean