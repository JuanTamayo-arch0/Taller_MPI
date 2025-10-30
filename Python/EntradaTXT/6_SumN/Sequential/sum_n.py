#!/usr/bin/env python3
"""
SumN sequential (Python)
Reads `params.txt` containing a single integer n and sums 1..n.
"""
import time
import sys

try:
    with open('params.txt','r') as f:
        n = int(f.read().strip().split()[0])
except Exception:
    print('params.txt missing or malformed (expected integer n)')
    sys.exit(1)

print('--------- SumN&Sequential (Python - params.txt) ---------')
start = time.time()
s = sum(range(1, n+1))
end = time.time()
print(f'Sum of first {n} naturals = {s}')
print(f'Elapsed time: {end - start:.6f} seconds')
