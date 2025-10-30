#!/usr/bin/env python3
"""
RemoveDuplicates MPI - EntradaAleatoria (Python + mpi4py)
Rank 0 generates a random vector (size from params.txt or default), distributes chunks (list scatter),
each rank removes duplicates locally, then gather at root and perform final dedup preserving first occurrences.
Timing uses MPI.Wtime for parallel timed section.
"""
from mpi4py import MPI
import random
import time

DEFAULT_SIZE = 10
comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size_procs = comm.Get_size()

def read_size(path='params.txt'):
    try:
        with open(path,'r') as f:
            data = f.read().strip().split()
            if data:
                return int(data[0])
    except Exception:
        return None
    return None

if rank == 0:
    n = read_size() or DEFAULT_SIZE
    print('--------- RemoveDuplicates&MPI (EntradaAleatoria - Python) --------')
    vec = [random.randint(0, 100) for _ in range(n)]
    print('\nEl vector generado por root es: [', ' '.join(map(str, vec)), ']')
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

n = comm.bcast(n if rank == 0 else None, root=0)
local = comm.scatter(chunks, root=0)
print(f"Rank {rank} received {len(local)} elements")

comm.Barrier()
start = MPI.Wtime()
# local dedup preserving first occurrences within the chunk
seen = set()
local_out = []
for x in local:
    if x not in seen:
        local_out.append(x)
        seen.add(x)
# gather parts
gathered = comm.gather(local_out, root=0)
end = MPI.Wtime()

if rank == 0:
    # final dedup preserving first occurrences across gathered parts
    final = []
    seen_total = set()
    for part in gathered:
        for x in part:
            if x not in seen_total:
                final.append(x)
                seen_total.add(x)
    print('\nEl vector sin duplicados (global) es: [', ' '.join(map(str, final)), ']')
    print(f"Tiempo total de ejecución (MPI_Wtime): {end - start:.6f} segundos")
    print('---------- Program End ----------')
