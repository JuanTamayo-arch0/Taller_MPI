#!/usr/bin/env python3
"""
SortVector MPI (Python using mpi4py)
Rank 0 reads `params.txt` (size followed by `size` integers), then distributes chunks to processes,
each process sorts its chunk, and rank 0 gathers and merges them.
This implementation uses comm.scatter with list-of-chunks to avoid requiring numpy.
"""
from mpi4py import MPI
import time
import sys
import heapq

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size_procs = comm.Get_size()

def read_params(path='params.txt'):
    try:
        with open(path, 'r') as f:
            data = f.read().strip().split()
    except FileNotFoundError:
        print(f"params.txt not found. Place a file with size and elements here.")
        comm.Abort(1)
    if len(data) < 1:
        print("params.txt is empty or malformed.")
        comm.Abort(1)
    n = int(data[0])
    if len(data) < 1 + n:
        print(f"params.txt does not contain enough elements (expected {n}).")
        comm.Abort(1)
    values = [int(x) for x in data[1:1+n]]
    return n, values

if rank == 0:
    print("--------- SortVector&MPI (Python - params.txt) ---------")
    n, values = read_params()
    # create chunks for scatter
    base = n // size_procs
    rem = n % size_procs
    chunks = []
    offset = 0
    for i in range(size_procs):
        cnt = base + (1 if i < rem else 0)
        chunks.append(values[offset:offset+cnt])
        offset += cnt
else:
    n = None
    chunks = None

# Broadcast n to all
n = comm.bcast(n, root=0)
# Scatter chunks (list-of-lists)
local_chunk = comm.scatter(chunks, root=0)
print(f"Rank {rank} received {len(local_chunk)} elements")

comm.Barrier()
start = time.time()
local_sorted = sorted(local_chunk)
# Gather sorted chunks at root
gathered = comm.gather(local_sorted, root=0)
end = time.time()

if rank == 0:
    # Merge all sorted chunks
    merged = list(heapq.merge(*gathered))
    print("Final sorted vector:", merged)
    print(f"Elapsed time (local sort+comm): {end - start:.6f} seconds")

