#!/usr/bin/env python3
"""
SortVector sequential - EntradaAleatoria (Python)
Genera un vector aleatorio del tamaño solicitado (lee size de params.txt si existe, o usa 10 por defecto),
lo imprime, lo ordena y mide tiempo CPU (similar a clock() en C).
"""
import random
import time
import sys

DEFAULT_SIZE = 10

def read_size_from_params(path='params.txt'):
    try:
        with open(path, 'r') as f:
            s = f.read().strip().split()
            if s:
                return int(s[0])
    except Exception:
        return None
    return None

def main():
    size = read_size_from_params() or DEFAULT_SIZE
    print("--------- SortVector&Sequential (EntradaAleatoria - Python) --------")

    # Generar vector aleatorio
    vec = [random.randint(0, 100) for _ in range(size)]
    print("\nEl vector generado es: [", ' '.join(map(str, vec)), "]")

    # Medir tiempo CPU (similar a clock())
    start = time.process_time()
    vec.sort()
    elapsed = time.process_time() - start

    print("\nEl vector ordenado es: [", ' '.join(map(str, vec)), "]")
    print(f"\nTiempo total de ejecución (CPU): {elapsed:.6f} segundos")
    print("---------- Program End ----------")

if __name__ == '__main__':
    main()
