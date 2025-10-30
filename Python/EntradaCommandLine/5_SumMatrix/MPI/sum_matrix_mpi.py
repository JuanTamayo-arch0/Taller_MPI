#!/usr/bin/env python3
"""
SumMatrix MPI - EntradaCommandLine (Python, numpy + mpi4py)
Usage: mpirun -np P python sum_matrix_mpi.py <n> <m> <a00>... <b00>...
Root parses argv, scatters rows of A and B using Scatterv, computes local sums and gathers result with Gatherv.
"""
from mpi4py import MPI
import sys
try:
    import numpy as np
    import time
except Exception:
    if MPI.COMM_WORLD.Get_rank() == 0:
        print('numpy is required. Install with: pip install numpy')
    MPI.COMM_WORLD.Abort(1)

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

if rank == 0:
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <n> <m> <a00>... <b00>...")
        comm.Abort(1)
    n = int(sys.argv[1]); m = int(sys.argv[2])
    expected = n * m
    if len(sys.argv) < 3 + expected * 2:
        print(f"Expected {expected*2} matrix elements after n and m (A then B)")
        comm.Abort(1)
    vals = [int(x) for x in sys.argv[3:3+expected*2]]
    A_flat = np.array(vals[:expected], dtype='i')
    B_flat = np.array(vals[expected:], dtype='i')
else:
    n = None; m = None; A_flat = None; B_flat = None

n = comm.bcast(n, root=0)
m = comm.bcast(m, root=0)

rows_per = n // size
rem = n % size
my_rows = rows_per + (1 if rank < rem else 0)
local_buf_A = np.empty(my_rows * m, dtype='i')
local_buf_B = np.empty(my_rows * m, dtype='i')

if rank == 0:
    sendcounts = np.array([(rows_per + (1 if i < rem else 0)) * m for i in range(size)], dtype='i')
    displs = np.array([int(sum(sendcounts[:i])) for i in range(size)], dtype='i')
else:
    sendcounts = None; displs = None

comm.Scatterv([A_flat, sendcounts, displs, MPI.INT] if rank == 0 else [None, None, None, MPI.INT], local_buf_A, root=0)
comm.Scatterv([B_flat, sendcounts, displs, MPI.INT] if rank == 0 else [None, None, None, MPI.INT], local_buf_B, root=0)

comm.Barrier()
start = MPI.Wtime()
local_sum = local_buf_A + local_buf_B
# gather the sums back
if rank == 0:
    result_flat = np.empty(n*m, dtype='i')
else:
    result_flat = None
comm.Gatherv(local_sum, [result_flat, sendcounts, displs, MPI.INT], root=0)
end = MPI.Wtime()

if rank == 0:
    print('Suma de matrices (fila-major):')
    print(' '.join(str(int(x)) for x in result_flat))
    print(f"Tiempo (MPI_Wtime): {end - start:.6f} segundos")
