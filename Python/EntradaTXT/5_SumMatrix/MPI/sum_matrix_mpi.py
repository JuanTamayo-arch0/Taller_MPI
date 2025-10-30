#!/usr/bin/env python3
"""
SumMatrix MPI (Python stub)
Rank 0 reads n m and matrices A and B, scatters row-blocks, each rank sums its block, then gather.
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
    vals = [int(x) for x in data[2:2+2*n*m]]
    Aflat = vals[:n*m]
    Bflat = vals[n*m:]
else:
    n = None; m = None; Aflat = None; Bflat = None

n = comm.bcast(n, root=0)
m = comm.bcast(m, root=0)

rows_per = n // size
rem = n % size
my_rows = rows_per + (1 if rank < rem else 0)

# prepare chunks of A and B on root
if rank == 0:
    Achunks = []
    Bchunks = []
    offset = 0
    for i in range(size):
        rows = rows_per + (1 if i < rem else 0)
        Achunks.append(Aflat[offset:offset+rows*m])
        Bchunks.append(Bflat[offset:offset+rows*m])
        offset += rows*m
else:
    Achunks = None; Bchunks = None

localA = comm.scatter(Achunks, root=0)
localB = comm.scatter(Bchunks, root=0)
localC = [localA[i] + localB[i] for i in range(len(localA))]

gathered = comm.gather(localC, root=0)
if rank == 0:
    # flatten and print
    Cflat = []
    for part in gathered:
        Cflat.extend(part)
    print('C = A + B:')
    for i in range(n):
        row = Cflat[i*m:(i+1)*m]
        print(' '.join(str(x) for x in row))
