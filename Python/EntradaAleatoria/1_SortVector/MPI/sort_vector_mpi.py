#!/usr/bin/env python3
"""
SortVector MPI - EntradaAleatoria (Python + mpi4py)
Rank 0 generates a random vector (size from params.txt if present, otherwise default),
distributes chunks with scatter, each rank sorts its chunk, then gather and merge at root.
Timing uses MPI.Wtime() for wall-clock parallel timing.
"""
from mpi4py import MPI
import random
import time

DEFAULT_SIZE = 10
comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size_procs = comm.Get_size()

def read_size_from_params(path='params.txt'):
    try:
        with open(path, 'r') as f:
            s = f.read().strip().split()
            if s:
                return int(s[0])
    except Exception:
        return None
    return None

if rank == 0:
    n = read_size_from_params() or DEFAULT_SIZE
    print("--------- SortVector&MPI (EntradaAleatoria - Python) --------")
    vec = [random.randint(0, 100) for _ in range(n)]
    print("\nEl vector generado por root es: [", ' '.join(map(str, vec)), "]")
    # create chunks
    base = n // size_procs
    rem = n % size_procs
    chunks = []
    offset = 0
    for i in range(size_procs):
        cnt = base + (1 if i < rem else 0)
        chunks.append(vec[offset:offset+cnt])
        offset += cnt
else:
    n = None
    chunks = None

# broadcast n
n = comm.bcast(n if rank == 0 else None, root=0)
local = comm.scatter(chunks, root=0)
print(f"Rank {rank} received {len(local)} elements")

comm.Barrier()
start = MPI.Wtime()
local.sort()
gathered = comm.gather(local, root=0)
end = MPI.Wtime()

if rank == 0:
    # merge sorted lists
    from heapq import merge
    merged = list(merge(*gathered))
    print("\nVector final ordenado: [", ' '.join(map(str, merged)), "]")
    print(f"Tiempo total de ejecución (MPI_Wtime): {end - start:.6f} segundos")
    print("---------- Program End ----------")
