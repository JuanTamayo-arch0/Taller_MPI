#!/usr/bin/env python3
"""
SumN MPI - EntradaAleatoria (Python + mpi4py)
Rank 0 reads n from params.txt or uses default, broadcasts to all ranks, each computes partial sum and reduce.
"""
from mpi4py import MPI
import time

DEFAULT_N = 10
comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

if rank == 0:
    try:
        with open('params.txt','r') as f:
            data = f.read().strip().split()
            if data:
                n = int(data[0])
            else:
                n = DEFAULT_N
    except Exception:
        n = DEFAULT_N
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
    print('--------- SumN&MPI (EntradaAleatoria - Python) --------')
    print(f'La suma de los primeros {n} numeros naturales es: {total}')
    print(f'Tiempo total de ejecución paralelo (MPI_Wtime): {end_time - start_time:.6f} segundos')
