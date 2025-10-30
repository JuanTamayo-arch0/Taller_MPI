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

Comprueba que están disponibles:

```sh
gcc --version
mpicc --version
mpirun --version
```

## Cómo compilar

Las instrucciones siguientes asumen que estás en la raíz del repositorio (la carpeta que contiene este `README.md`). Ajusta rutas si trabajas desde otra carpeta.

Compilación secuencial (ejemplo para el ejercicio 1 - SortVector):

```sh
cd 1_SortVector/Sequential
gcc -O2 -Wall -o SortVector SortVector.c
# El binario resultante será: 1_SortVector/Sequential/SortVector
```

Compilación con MPI (ejemplo para el ejercicio 1 - SortVector):

```sh
cd 1_SortVector/MPI
mpicc -O2 -Wall -o SortVector SortVector.c
# El binario resultante será: 1_SortVector/MPI/SortVector
```

Compilar varios ejercicios en un bucle (secuencial):

```sh
for d in 1_SortVector 2_RemoveDuplicates 3_SparseMatrix 4_SymmetricMatrix 5_SumMatrix 6_SumN; do
	src="$d/Sequential/$(basename $d).c"
	out="$d/Sequential/$(basename $d)"
	if [ -f "$src" ]; then
		gcc -O2 -Wall -o "$out" "$src"
		echo "Compilado $out"
	fi
done
```

Compilar varios ejercicios en un bucle (MPI):

```sh
for d in 1_SortVector 2_RemoveDuplicates 3_SparseMatrix 4_SymmetricMatrix 5_SumMatrix 6_SumN; do
	src="$d/MPI/$(basename $d).c"
	out="$d/MPI/$(basename $d)"
	if [ -f "$src" ]; then
		mpicc -O2 -Wall -o "$out" "$src"
		echo "Compilado $out"
	fi
done
```

## Cómo ejecutar

Ejecución secuencial (sin MPI):

```sh
./1_SortVector/Sequential/SortVector [args-opcionales]
# p.ej. ./1_SortVector/Sequential/SortVector 1000
```

Ejecución con MPI (paralela):

```sh
mpirun -np 4 ./1_SortVector/MPI/SortVector [args-opcionales]
# o alternativamente
mpiexec -n 4 ./1_SortVector/MPI/SortVector [args-opcionales]
```

- Cambia `-np 4` o `-n 4` al número de procesos que quieras usar.
- Si el programa espera parámetros (tamaño del vector, archivo de entrada, etc.) pásalos después del ejecutable.

Ejemplo práctico:

```sh
# Compilar
mpicc -O2 -Wall -o 1_SortVector/MPI/SortVector 1_SortVector/MPI/SortVector.c
# Ejecutar con 8 procesos
mpirun -np 8 ./1_SortVector/MPI/SortVector 100000
```

## Notas y recomendaciones

- Si al ejecutar con `mpirun` ves errores tipo "mpirun: command not found", asegúrate de tener OpenMPI o MPICH instalados y que el `PATH` incluye sus binarios.
- Muchos programas imprimen mensajes por pantalla desde distintos procesos; para una salida ordenada puede ser útil redirigir la salida de cada proceso a ficheros separados o usar barreras/ordenación dentro del código.
- Para medir tiempos y comparar versiones, usa `time` o instrumentos más avanzados (por ejemplo `perf`, `mpiP`, o herramientas de profiling de MPI).

## ¿Qué sigue?

- Puedes compilar y ejecutar cada ejercicio, probar diferentes números de procesos y comparar tiempos frente a la versión secuencial.
- Si quieres, puedo añadir un `Makefile` o scripts `build_all.sh` y `run_example.sh` para automatizar compilación y ejecución; dime si lo prefieres y cómo quieres los ejemplos por defecto (nº procesos, tamaños, etc.).

---

Autor: Material del taller de Arquitectura de Nube / MPI
