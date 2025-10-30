#!/usr/bin/env python3
"""
SumN MPI - EntradaCommandLine (Python + mpi4py)
Usage: mpirun -np P python sum_n_mpi.py <n>
Rank 0 parses n from argv and broadcasts to others.
"""
from mpi4py import MPI
import sys

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

if rank == 0:
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <n>")
        comm.Abort(1)
    n = int(sys.argv[1])
else:
    n = None

n = comm.bcast(n, root=0)

base = n // size
rem = n % size
start_i = rank * base + (rank if rank < rem else rem) + 1
end_i = start_i + base - 1 + (1 if rank < rem else 0)

local_sum = sum(range(start_i, end_i+1)) if start_i <= end_i else 0

comm.Barrier()
start_time = MPI.Wtime()

total = comm.reduce(local_sum, op=MPI.SUM, root=0)
end_time = MPI.Wtime()

if rank == 0:
    print('--------- SumN&MPI (CommandLine - Python) --------')
    print(f'Sum of first {n} naturals = {total}')
    print(f'Tiempo total de ejecución paralelo (MPI_Wtime): {end_time - start_time:.6f} segundos')
