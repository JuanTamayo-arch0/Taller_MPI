#!/usr/bin/env python3
"""
RemoveDuplicates MPI - EntradaCommandLine (Python + mpi4py)
Usage: mpirun -np P python remove_duplicates_mpi.py <size> <v0> <v1> ... <v(size-1)>
Rank 0 parses argv and scatters chunks; local dedup and final gather+global dedup at root.
"""
from mpi4py import MPI
import sys

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size_procs = comm.Get_size()

if rank == 0:
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <size> <v0> <v1> ... <v(size-1)>")
        comm.Abort(1)
    n = int(sys.argv[1])
    if len(sys.argv) < 2 + n:
        print(f"Usage: {sys.argv[0]} <size> <v0> <v1> ... <v(size-1)>")
        comm.Abort(1)
    vals = [int(x) for x in sys.argv[2:2+n]]
    print('--------- RemoveDuplicates&MPI (CommandLine - Python) --------')
    print('\nEl vector proporcionado:', vals)
    base = n // size_procs
    rem = n % size_procs
    chunks = []
    offset = 0
    for i in range(size_procs):
        cnt = base + (1 if i < rem else 0)
        chunks.append(vals[offset:offset+cnt])
        offset += cnt
else:
    n = None
    chunks = None

n = comm.bcast(n if rank == 0 else None, root=0)
local = comm.scatter(chunks, root=0)
print(f"Rank {rank} received {len(local)} elements")

# local dedup
seen = set()
local_out = []
for x in local:
    if x not in seen:
        local_out.append(x)
        seen.add(x)

gathered = comm.gather(local_out, root=0)

if rank == 0:
    final = []
    seen_total = set()
    for part in gathered:
        for x in part:
            if x not in seen_total:
                final.append(x)
                seen_total.add(x)
    print('\nEl vector sin duplicados (global) es:', final)
    print('---------- Program End ----------')
