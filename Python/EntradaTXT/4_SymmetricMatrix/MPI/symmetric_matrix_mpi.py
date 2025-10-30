#!/usr/bin/env python3
"""
SymmetricMatrix MPI (Python stub)
Rank 0 reads square matrix from params.txt, computes transpose, broadcasts both matrices and each rank checks a subset of rows.
"""
from mpi4py import MPI
import sys

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

if rank == 0:
    try:
        with open('params.txt','r') as f:
            data = f.read().strip().split()
    except FileNotFoundError:
        print('params.txt not found')
        comm.Abort(1)
    n = int(data[0]); m = int(data[1])
    if n != m:
        print('Matrix is not square')
        comm.Abort(1)
    vals = [int(x) for x in data[2:2+n*m]]
    matrix = vals
    trans = [0]*(n*m)
    for i in range(n):
        for j in range(n):
            trans[j*n + i] = matrix[i*n + j]
else:
    n = None; m = None; matrix = None; trans = None

n = comm.bcast(n, root=0)
# broadcast flat arrays
matrix = comm.bcast(matrix, root=0)
trans = comm.bcast(trans, root=0)

rows_per = n // size
rem = n % size
start = rank * rows_per + (rank if rank < rem else rem)
end = start + rows_per + (1 if rank < rem else 0)

local_ok = True
for i in range(start, end):
    for j in range(n):
        if matrix[i*n + j] != trans[i*n + j]:
            local_ok = False
            break

global_ok = comm.reduce(1 if local_ok else 0, op=MPI.MIN, root=0)
if rank == 0:
    if global_ok == 1:
        print('Matrix is symmetric')
    else:
        print('Matrix is not symmetric')
