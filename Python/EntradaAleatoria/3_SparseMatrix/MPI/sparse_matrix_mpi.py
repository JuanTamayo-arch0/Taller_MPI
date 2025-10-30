#!/usr/bin/env python3
"""
SparseMatrix MPI - EntradaAleatoria (Python, numpy + mpi4py)
Rank 0 reads n m from params.txt or uses defaults, generates a random 0/1 matrix, distributes rows with Scatterv,
each rank counts zeros locally and reduce to total. Timing uses MPI.Wtime().
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

DEFAULT_N = 10
DEFAULT_M = 10

if rank == 0:
    # try read params.txt
    n = None; m = None
    try:
        with open('params.txt','r') as f:
            toks = f.read().strip().split()
            if len(toks) >= 2:
                n = int(toks[0]); m = int(toks[1])
    except Exception:
        pass
    if n is None or m is None:
        n,m = DEFAULT_N, DEFAULT_M
    print('--------- SparseMatrix&MPI (EntradaAleatoria - Python) --------')
    mat = np.random.randint(0,2,size=(n,m), dtype='i')
    print('\nLa matriz generada por root es:')
    for i in range(n): print(' '.join(str(int(x)) for x in mat[i]))
    flat = mat.ravel()
else:
    n = None; m = None; flat = None

# broadcast dims
n = comm.bcast(n, root=0)
m = comm.bcast(m, root=0)

rows_per = n // size
rem = n % size
my_rows = rows_per + (1 if rank < rem else 0)

# prepare receive buffer
local_buf = np.empty(my_rows * m, dtype='i')

if rank == 0:
    sendcounts = np.array([(rows_per + (1 if i < rem else 0)) * m for i in range(size)], dtype='i')
    displs = np.array([sum(sendcounts[:i]) for i in range(size)], dtype='i')
else:
    sendcounts = None
    displs = None

comm.Scatterv([flat, sendcounts, displs, MPI.INT] if rank == 0 else [None, None, None, MPI.INT], local_buf, root=0)

comm.Barrier()
start = MPI.Wtime()
local_zeros = int((local_buf == 0).sum())
# reduce
total_zeros = comm.reduce(local_zeros, op=MPI.SUM, root=0)
end = MPI.Wtime()

if rank == 0:
    total_elements = n * m
    print(f"\nCantidad total de ceros: {total_zeros} de {total_elements} elementos")
    if total_zeros > total_elements/2:
        print('La matriz es dispersa (sparse)')
    else:
        print('La matriz no es dispersa')
    print(f"Tiempo total de ejecución (MPI_Wtime): {end - start:.6f} segundos")
    print('---------- Program End ----------')
