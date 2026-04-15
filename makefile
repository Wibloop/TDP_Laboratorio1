# Variables de compilacion
CXX = g++
CXXFLAGS = -Wall -g -std=c++11

# Regla principal: compilar todos los tests al mismo tiempo
all: test_Stack test_Block

# --- Reglas de Enlazado (Generacion de Ejecutables) ---

test_Stack: test_Stack.o Stack.o
	$(CXX) $(CXXFLAGS) -o test_Stack test_Stack.o Stack.o

test_Block: test_Block.o Block.o
	$(CXX) $(CXXFLAGS) -o test_Block test_Block.o Block.o

# --- Reglas de Compilacion de Objetos (.o) ---

# Objetos para la Pila
test_Stack.o: test_Stack.cpp Stack.h
	$(CXX) $(CXXFLAGS) -c test_Stack.cpp

Stack.o: Stack.cpp Stack.h
	$(CXX) $(CXXFLAGS) -c Stack.cpp

# Objetos para el Bloque
test_Block.o: test_Block.cpp Block.h
	$(CXX) $(CXXFLAGS) -c test_Block.cpp

Block.o: Block.cpp Block.h
	$(CXX) $(CXXFLAGS) -c Block.cpp

# --- Regla de Limpieza ---
clean:
	rm -f *.o test_Stack test_Block