# Lucas Silva / TDP_Laboratorio1

Solucionador eficiente de Color Block Jam en C++ nativo mediante el algoritmo de búsqueda A*(A-Star) con heurística. Desarrollado desde cero sin librerías STL, implementando una Cola de Prioridad (MinHeap) propia para lograr complejidad O(log N). Arquitectura modular orientada a objetos con Makefile :)

## Instrucciones de Uso

### 1. Compilar y Limpiar el Proyecto
El proyecto incluye un `makefile` estructurado que facilita la compilación.
- **Compilar el programa principal (`main`)**:
  ```bash
  make main
  ```
- **Compilar todos los tests unitarios**:
  ```bash
  make tests
  ```
- **Compilar TODO (main y tests)**:
  ```bash
  make all
  ```
- **Limpiar ejecutables y archivos objeto (`.o`)**:
  ```bash
  make clean
  ```

### 2. Probar los Archivos de Test
Se han implementado pruebas unitarias para cada componente del juego. Una vez compilados usando `make tests`, puedes ejecutarlos individualmente desde la raíz del proyecto. Por ejemplo:
```bash
./test_Block
./test_Board
./test_AStarSolver
```

### 3. Ejecutar el Programa Principal y Cargar Niveles
Una vez compilado el programa principal (`make main`), puedes ejecutarlo:
```bash
./main
```
Dentro del menú principal, cuando selecciones la **Opción 1 (Cargar archivo de configuración)**, el programa te pedirá la ruta del archivo del nivel. Dado que los archivos de texto están guardados dentro de la carpeta `test/`, **debes escribir la ruta incluyendo esta carpeta**.

Ejemplos de rutas correctas para cargar en el programa:
- `test/facil1.txt`
- `test/facil2.txt`
- `test/facil3.txt`
- `test/facil4.txt`
- `test/dificil.txt`
