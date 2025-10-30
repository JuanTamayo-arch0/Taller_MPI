#!/usr/bin/env python3
"""
SortVector MPI - EntradaCommandLine (Python + mpi4py)
Usage: mpirun -np P python sort_vector_mpi.py <size> <v0> <v1> ... <v(size-1)>
Root (rank 0) parses argv and scatters chunks; processes sort locally and root merges.
"""
from mpi4py import MPI
import sys
from heapq import merge

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size_procs = comm.Get_size()

if rank == 0:
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <size> <v0> <v1> ... <v(size-1)>")
        comm.Abort(1)
    n = int(sys.argv[1])
    if len(sys.argv) < 2 + n:
        print(f"Usage: {sys.argv[0]} <size> <v0> <v1> ... <v(size-1)>")
        comm.Abort(1)
    values = [int(x) for x in sys.argv[2:2+n]]
    print("--------- SortVector&MPI (CommandLine - Python) --------")
    print('\nEl vector proporcionado:', values)
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

n = comm.bcast(n, root=0)
local = comm.scatter(chunks, root=0)
print(f"Rank {rank} received {len(local)} elements")

comm.Barrier()
start = MPI.Wtime()
local_sorted = sorted(local)
gathered = comm.gather(local_sorted, root=0)
end = MPI.Wtime()

if rank == 0:
    merged = list(merge(*gathered))
    print('\nVector final ordenado:', merged)
    print(f"Tiempo total de ejecución (MPI_Wtime): {end - start:.6f} segundos")
    print('---------- Program End ----------')
