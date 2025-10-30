#!/usr/bin/env python3
"""
SparseMatrix sequential (Python stub)
Reads params.txt: n m followed by n*m elements (0/1). Counts zeros and reports sparse/not-sparse.
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
    return n,m,vals

if __name__ == '__main__':
    n,m,vals = read_matrix()
    zeros = vals.count(0)
    print(f"Zeros: {zeros} of {n*m} elements")
    if zeros > (n*m)//2:
        print('Matrix is sparse')
    else:
        print('Matrix is not sparse')
