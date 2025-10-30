#!/usr/bin/env python3
"""
SortVector sequential (Python)
Reads `params.txt` from the same folder: first integer is size, then `size` integers follow.
Sorts the vector and prints timings and results (small sizes only recommended for printing).
"""
import time
import sys

def read_params(path='params.txt'):
    try:
        with open(path, 'r') as f:
            data = f.read().strip().split()
    except FileNotFoundError:
        print(f"params.txt not found in {path}. Place a file with size and elements here.")
        sys.exit(1)
    if len(data) < 1:
        print("params.txt is empty or malformed.")
        sys.exit(1)
    size = int(data[0])
    if len(data) < 1 + size:
        print(f"params.txt does not contain enough elements (expected {size}).")
        sys.exit(1)
    values = [int(x) for x in data[1:1+size]]
    return size, values


def main():
    size, values = read_params()
    print("--------- SortVector&Sequential (Python - params.txt) ---------")
    print("Vector read:", values)
    start = time.time()
    sorted_values = sorted(values)
    end = time.time()
    print("Sorted vector:", sorted_values)
    print(f"Elapsed time: {end - start:.6f} seconds")

if __name__ == '__main__':
    main()
