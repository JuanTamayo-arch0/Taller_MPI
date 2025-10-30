#!/usr/bin/env python3
"""
SumMatrix sequential (Python stub)
Reads params.txt: n m followed by n*m elements for A then n*m for B. Prints A, B and A+B.
"""
import sys

def read_two_matrices(path='params.txt'):
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
    if len(data) < 2 + 2*n*m:
        print('params.txt does not contain enough elements for A and B')
        sys.exit(1)
    vals = [int(x) for x in data[2:2+2*n*m]]
    A = [vals[i*m:(i+1)*m] for i in range(n)]
    Bstart = 2 + n*m
    Bflat = [int(x) for x in data[2+n*m:2+2*n*m]]
    B = [Bflat[i*m:(i+1)*m] for i in range(n)]
    return n,m,A,B

if __name__ == '__main__':
    n,m,A,B = read_two_matrices()
    print('A:')
    for r in A: print(r)
    print('B:')
    for r in B: print(r)
    C = [[A[i][j] + B[i][j] for j in range(m)] for i in range(n)]
    print('A+B:')
    for r in C: print(r)
