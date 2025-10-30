#!/usr/bin/env python3
"""
SumN MPI (Python)
Rank 0 reads n from params.txt, broadcasts it, each rank sums its subrange and reduce to total.
"""
from mpi4py import MPI
import sys

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

if rank == 0:
    try:
        with open('params.txt','r') as f:
            n = int(f.read().strip().split()[0])
    except Exception:
        print('params.txt missing or malformed (expected integer n)')
        comm.Abort(1)
else:
    n = None

n = comm.bcast(n, root=0)

base = n // size
rem = n % size
start = rank * base + (rank if rank < rem else rem) + 1
end = start + base - 1 + (1 if rank < rem else 0)

local_sum = sum(range(start, end+1)) if start <= end else 0

total = comm.reduce(local_sum, op=MPI.SUM, root=0)
if rank == 0:
    print(f"Sum of first {n} naturals = {total}")
