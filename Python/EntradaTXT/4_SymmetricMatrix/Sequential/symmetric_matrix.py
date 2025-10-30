#!/usr/bin/env python3
"""
SymmetricMatrix sequential (Python stub)
Reads params.txt: n m then n*m elements (row-major). If n==m computes transpose and compares.
"""
import sys

def read_matrix(path='params.txt'):
    try:
        with open(path,'r') as f:
            data = f.read().strip().split()
    except FileNotFoundError:
        print('params.txt not found')
        sys.exit(1)
    if len(data) < 2:
        print('params.txt malformed')
        sys.exit(1)
    n = int(data[0]); m = int(data[1])
    if len(data) < 2 + n*m:
        print('params.txt does not contain enough elements')
        sys.exit(1)
    vals = [int(x) for x in data[2:2+n*m]]
    matrix = [vals[i*m:(i+1)*m] for i in range(n)]
    return n,m,matrix

if __name__ == '__main__':
    n,m,mat = read_matrix()
    print('Matrix:')
    for row in mat:
        print(row)
    if n != m:
        print('Matrix is not square -> cannot be symmetric')
        sys.exit(0)
    trans = [[mat[j][i] for j in range(n)] for i in range(n)]
    print('Transpose:')
    for row in trans:
        print(row)
    if mat == trans:
        print('Matrix is symmetric')
    else:
        print('Matrix is not symmetric')
