#!/usr/bin/env python3
"""
MPI RemoveDuplicates (Python stub)
Rank 0 reads params.txt and scatters chunks; each rank removes duplicates locally and gathers results.
Note: This simple port preserves the first occurrence within each chunk only; for global deduplication a merge step is required.
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
    n = int(data[0])
    vals = [int(x) for x in data[1:1+n]]
    base = n // size
    rem = n % size
    chunks = []
    offset = 0
    for i in range(size):
        cnt = base + (1 if i < rem else 0)
        chunks.append(vals[offset:offset+cnt])
        offset += cnt
else:
    chunks = None

local = comm.scatter(chunks, root=0)
seen = set()
out = []
for x in local:
    if x not in seen:
        seen.add(x)
        out.append(x)

gathered = comm.gather(out, root=0)
if rank == 0:
    # Simple concatenation; note this may still contain duplicates across chunks
    result = []
    for part in gathered:
        for x in part:
            if x not in result:
                result.append(x)
    print('Result (approx global unique preserving first occurrences):', result)
