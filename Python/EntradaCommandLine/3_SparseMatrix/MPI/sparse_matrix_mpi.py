#!/usr/bin/env python3
"""
SparseMatrix MPI - EntradaCommandLine (Python, numpy + mpi4py)
Usage: mpirun -np P python sparse_matrix_mpi.py <n> <m> <a00> <a01> ...
Root parses argv and scatters rows via Scatterv.
"""
from mpi4py import MPI
import sys
try:
    import numpy as np
except ImportError:
    if MPI.COMM_WORLD.Get_rank() == 0:
        print('numpy is required. Install with: pip install numpy')
    MPI.COMM_WORLD.Abort(1)

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

if rank == 0:
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <n> <m> <a00> <a01> ...")
        comm.Abort(1)
    n = int(sys.argv[1]); m = int(sys.argv[2])
    expected = n*m
    if len(sys.argv) < 3 + expected:
        print(f"Expected {expected} matrix elements after n and m")
        comm.Abort(1)
    vals = [int(x) for x in sys.argv[3:3+expected]]
    flat = np.array(vals, dtype='i')
else:
    n = None; m = None; flat = None

n = comm.bcast(n, root=0)
m = comm.bcast(m, root=0)

rows_per = n // size
rem = n % size
my_rows = rows_per + (1 if rank < rem else 0)
local_buf = np.empty(my_rows * m, dtype='i')

if rank == 0:
    sendcounts = np.array([(rows_per + (1 if i < rem else 0)) * m for i in range(size)], dtype='i')
    displs = np.array([int(sum(sendcounts[:i])) for i in range(size)], dtype='i')
else:
    sendcounts = None; displs = None

comm.Scatterv([flat, sendcounts, displs, MPI.INT] if rank == 0 else [None, None, None, MPI.INT], local_buf, root=0)

comm.Barrier()
start = MPI.Wtime()
local_zeros = int((local_buf == 0).sum())
total_zeros = comm.reduce(local_zeros, op=MPI.SUM, root=0)
end = MPI.Wtime()

if rank == 0:
    total_elements = n*m
    print(f"\nCantidad total de ceros: {total_zeros} de {total_elements} elementos")
    if total_zeros > total_elements/2:
        print('La matriz es dispersa (sparse)')
    else:
        print('La matriz no es dispersa')
    print(f"Tiempo total de ejecución (MPI_Wtime): {end - start:.6f} segundos")
    print('---------- Program End ----------')
