#!/usr/bin/env python3
"""
SymmetricMatrix MPI - EntradaCommandLine (Python, numpy + mpi4py)
Usage: mpirun -np P python symmetric_matrix_mpi.py <n> <m> <a00> <a01> ...
Root parses argv and broadcasts full matrix; each rank checks a subset of rows against the transpose and a global AND is reduced to root.
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
        print(f"Usage: {sys.argv[0]} <n> <m> <a00> <a01> ...")
        comm.Abort(1)
    n = int(sys.argv[1]); m = int(sys.argv[2])
    expected = n * m
    if len(sys.argv) < 3 + expected:
        print(f"Expected {expected} matrix elements after n and m")
        comm.Abort(1)
    vals = [int(x) for x in sys.argv[3:3+expected]]
    A_flat = np.array(vals, dtype='i')
else:
    n = None; m = None; A_flat = None

n = comm.bcast(n, root=0)
m = comm.bcast(m, root=0)

# Broadcast full matrix (flat) to all ranks
if rank != 0:
    A_flat = np.empty(n*m, dtype='i')
comm.Bcast(A_flat, root=0)
A = A_flat.reshape((n, m))

start = MPI.Wtime()
if n != m:
    is_sym = False
else:
    # compute rows to check
    rows_per = n // size
    rem = n % size
    start_row = rank * rows_per + min(rank, rem)
    my_rows = rows_per + (1 if rank < rem else 0)
    end_row = start_row + my_rows
    # each rank verifies its rows
    local_ok = True
    for i in range(start_row, end_row):
        # compare A[i, j] with A[j, i] for all j
        if not np.array_equal(A[i, :], A[:, i]):
            local_ok = False
            break
    # reduce with logical AND
    is_sym = comm.allreduce(local_ok, op=MPI.LAND)
end = MPI.Wtime()

if rank == 0:
    if is_sym:
        print('La matriz es simétrica')
    else:
        print('La matriz NO es simétrica')
    print(f"Tiempo (MPI_Wtime): {end - start:.6f} segundos")
