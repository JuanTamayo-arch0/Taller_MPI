#!/usr/bin/env python3
"""
RemoveDuplicates sequential (Python stub)
Reads `params.txt`: size followed by elements. Returns vector with duplicates removed (preserving first occurrences).
"""
import sys

def read_params(path='params.txt'):
    try:
        with open(path,'r') as f:
            data = f.read().strip().split()
    except FileNotFoundError:
        print('params.txt not found')
        sys.exit(1)
    if len(data) < 1:
        print('params.txt empty')
        sys.exit(1)
    n = int(data[0])
    if len(data) < 1 + n:
        print('params.txt does not contain enough elements')
        sys.exit(1)
    return n, [int(x) for x in data[1:1+n]]

if __name__ == '__main__':
    n, v = read_params()
    print('Input:', v)
    seen = set()
    out = []
    for x in v:
        if x not in seen:
            out.append(x)
            seen.add(x)
    print('Output (duplicates removed):', out)
