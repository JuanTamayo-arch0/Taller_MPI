#!/usr/bin/env python3
"""
SparseMatrix MPI (Python stub)
Rank 0 reads n m and n*m elements, splits rows among processes, each counts zeros locally, then reduce.
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
    vals = [int(x) for x in data[2:2+n*m]]
else:
    n = None; m = None; vals = None

n = comm.bcast(n, root=0)
m = comm.bcast(m, root=0)

rows_per = n // size
rem = n % size
my_rows = rows_per + (1 if rank < rem else 0)

# prepare chunks of rows on root
if rank == 0:
    chunks = []
    offset = 0
    for i in range(size):
        rows = rows_per + (1 if i < rem else 0)
        chunks.append(vals[offset:offset+rows*m])
        offset += rows*m
else:
    chunks = None

local = comm.scatter(chunks, root=0)
local_zeros = local.count(0)
total_zeros = comm.reduce(local_zeros, op=MPI.SUM, root=0)
if rank == 0:
    total_elements = n*m
    print(f"Total zeros: {total_zeros} of {total_elements}")
    if total_zeros > total_elements//2:
        print('Matrix is sparse')
    else:
        print('Matrix is not sparse')
