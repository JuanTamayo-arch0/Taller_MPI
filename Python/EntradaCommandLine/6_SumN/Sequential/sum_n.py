#!/usr/bin/env python3
"""
SumN sequential - EntradaCommandLine (Python)
Usage: python sum_n.py <n>
"""
import sys
import time

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <n>")
        sys.exit(1)
    n = int(sys.argv[1])
    print('--------- SumN&Sequential (CommandLine - Python) --------')
    start = time.process_time()
    s = sum(range(1, n+1))
    end = time.process_time()
    print(f'Sum of first {n} naturals = {s}')
    print(f'Tiempo total de ejecución (CPU): {end - start:.6f} segundos')
