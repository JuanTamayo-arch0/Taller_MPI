# Taller_MPI

Este repositorio contiene los ejercicios del taller de Arquitectura de Nube usando programación paralela con MPI y sus implementaciones secuenciales. El objetivo del taller es comparar la versión secuencial y la versión paralela (basada en MPI) de algoritmos clásicos, entender la comunicación entre procesos y medir mejoras de rendimiento.

## Estructura del repositorio

Cada ejercicio está en una carpeta numerada y dentro de ella hay dos subcarpetas:

- `MPI/` : implementación que utiliza MPI (ficheros fuente en C y binarios si ya están compilados).
- `Sequential/` : implementación secuencial (C puro).

Ejercicios incluidos:

- `1_SortVector/` — Ordenación de un vector (versión secuencial y paralela).
- `2_RemoveDuplicates/` — Eliminación de duplicados en un vector.
- `3_SparseMatrix/` — Operaciones con matrices dispersas (sparse matrices).
- `4_SymmetricMatrix/` — Operaciones con matrices simétricas.
- `5_SumMatrix/` — Suma de matrices (por bloques/paralelismo entre procesos).
- `6_SumN/` — Suma de los primeros N números (ejemplo sencillo de reducción).

> Nota: cada ejercicio tiene su propio ejecutable en las carpetas `MPI/` y `Sequential/` con nombres como `SortVector`, `RemoveDuplicates`, `SparseMatrix`, etc.

## Requisitos

- Un compilador de C (por ejemplo `gcc`).
- Para las versiones MPI: una implementación de MPI instalada (OpenMPI o MPICH). Las herramientas principales son `mpicc` (compilador wrapper) y `mpirun`/`mpiexec` para ejecutar.

En Debian/Ubuntu se pueden instalar con:

```sh
sudo apt update
sudo apt install -y build-essential openmpi-bin libopenmpi-dev
```
# Taller_MPI

Este repositorio contiene los ejercicios del taller de Arquitectura de Nube con implementaciones en C (secuencial y MPI) y en Python (secuencial y MPI usando mpi4py). El objetivo es comparar implementaciones secuenciales y paralelas, entender la comunicación entre procesos y practicar particionado/transferencia de datos con MPI.

## Estructura del repositorio (resumen)

Raíz:

- `C/` : copia de las implementaciones en C organizadas por modo de entrada.
- `Python/` : puertos a Python de los ejercicios (secuencial + MPI con `mpi4py` y `numpy` cuando aplica).
- `EntradaAleatoria/`, `EntradaCommandLine/`, `EntradaTXT/` : dentro de cada lenguaje (C o Python) hay tres variantes de entrada:
	- `EntradaAleatoria/` : el programa genera datos aleatorios internamente.
	- `EntradaCommandLine/` : todos los datos se pasan por `argv` (útil para pruebas rápidas o evaluación automática).
	- `EntradaTXT/` : el proceso root lee un `params.txt` y reparte los datos mediante MPI.

Cada ejercicio sigue la numeración original:

- `1_SortVector/`
- `2_RemoveDuplicates/`
- `3_SparseMatrix/`
- `4_SymmetricMatrix/`
- `5_SumMatrix/`
- `6_SumN/`

Dentro de cada ejercicio hay subcarpetas `Sequential/` y `MPI/` para las versiones correspondientes.

## Requisitos

- Compilador de C: `gcc`.
- Implementación MPI: OpenMPI o MPICH (`mpicc`, `mpirun`/`mpiexec`).
- Python 3: `python3`.
- Paquetes Python recomendados para la rama Python:
	- `mpi4py`
	- `numpy` (recomendado para operaciones con matrices y buffers eficientes)

Instalación rápida en Debian/Ubuntu:

```sh
sudo apt update
sudo apt install -y build-essential openmpi-bin libopenmpi-dev python3-venv python3-pip
python3 -m pip install --user mpi4py numpy
```

También puedes crear un entorno virtual e instalar dependencias desde `Python/requirements.txt`:

```sh
python3 -m venv .venv
source .venv/bin/activate
pip install -r Python/requirements.txt
```

## Compilar las implementaciones en C

Las instrucciones siguientes asumen que estás en la raíz del repositorio.

Compilar un ejercicio secuencial (ejemplo ejercicio 1):

```sh
cd C/EntradaCommandLine/1_SortVector/Sequential
gcc -O2 -Wall -o SortVector SortVector.c
# binario: C/EntradaCommandLine/1_SortVector/Sequential/SortVector
```

Compilar la versión MPI (ejemplo ejercicio 1):

```sh
cd C/EntradaCommandLine/1_SortVector/MPI
mpicc -O2 -Wall -o SortVector SortVector.c
# binario: C/EntradaCommandLine/1_SortVector/MPI/SortVector
```

Compilar varios ejercicios en bucle (C/EntradaCommandLine como ejemplo):

```sh
for d in 1_SortVector 2_RemoveDuplicates 3_SparseMatrix 4_SymmetricMatrix 5_SumMatrix 6_SumN; do
	src="C/EntradaCommandLine/$d/Sequential/$(basename $d).c"
	out="C/EntradaCommandLine/$d/Sequential/$(basename $d)"
	if [ -f "$src" ]; then
		gcc -O2 -Wall -o "$out" "$src"
		echo "Compilado $out"
	fi
done

for d in 1_SortVector 2_RemoveDuplicates 3_SparseMatrix 4_SymmetricMatrix 5_SumMatrix 6_SumN; do
	src="C/EntradaCommandLine/$d/MPI/$(basename $d).c"
	out="C/EntradaCommandLine/$d/MPI/$(basename $d)"
	if [ -f "$src" ]; then
		mpicc -O2 -Wall -o "$out" "$src"
		echo "Compilado $out"
	fi
done
```

## Ejecutar los programas

Ejecutar binario C secuencial:

```sh
./C/EntradaCommandLine/1_SortVector/Sequential/SortVector [args...]
```

Ejecutar binario C con MPI:

```sh
mpirun -np 4 ./C/EntradaCommandLine/1_SortVector/MPI/SortVector [args...]
```

Ejecutar los puertos Python (secuencial):

```sh
python3 Python/EntradaCommandLine/1_SortVector/Sequential/sort_vector.py <n> <v0> <v1> ...
# Ejemplo: python3 Python/EntradaAleatoria/1_SortVector/Sequential/sort_vector.py  # genera aleatorio
```

Ejecutar los puertos Python con MPI (usar mpi4py):

```sh
mpirun -np 4 python3 Python/EntradaCommandLine/1_SortVector/MPI/sort_vector_mpi.py <n> <v0> <v1> ...
```

Notas prácticas:
- Para los programas Python que usan `numpy` y `mpi4py`, `mpirun` ejecuta el intérprete que debe tener esos paquetes instalados en su entorno.
- Si usas entornos virtuales, asegúrate de activar el entorno en todos los nodos (o usar rutas absolutas al intérprete dentro del entorno virtual).

## Formato de `params.txt` (cuando aplica)

Se usa en `EntradaTXT/` y su formato es sencillo (valores separados por espacios o saltos de línea). Ejemplos:

- Vector: `N v0 v1 ... v{N-1}`
- Matriz: `n m a00 a01 ... a{n-1}{m-1}` (orden fila-major)
- SumMatrix: `n m A_elements... B_elements...` (primero A fila-major, luego B fila-major)

Los programas `EntradaTXT` suelen hacer que el proceso root lea `params.txt` y luego distribuya los datos con `Bcast`, `Scatterv`, etc.

## Pruebas rápidas (smoke tests)

- Ejecuta las versiones secuenciales y compara con la versión MPI con pocos procesos (por ejemplo 2):

```sh
# Secuencial
python3 Python/EntradaAleatoria/4_SymmetricMatrix/Sequential/symmetric_matrix.py

# MPI (2 procesos)
mpirun -np 2 python3 Python/EntradaAleatoria/4_SymmetricMatrix/MPI/symmetric_matrix_mpi.py
```

Si faltan `numpy` o `mpi4py`, instala con `pip install -r Python/requirements.txt` o individualmente.

## Sugerencias y próximos pasos

- Puedo añadir un `Makefile` o `scripts/` para automatizar la compilación y pruebas. Dime qué ejemplos por defecto prefieres (nº procesos, tamaños, si usar `EntradaAleatoria` o `EntradaCommandLine`).
- Puedo añadir `params.txt` de ejemplo en cada `EntradaTXT/` si quieres tener casos listos.

---
Autor: Material del taller de Arquitectura de Nube / MPI (adaptado: versiones C y Python con mpi4py)
