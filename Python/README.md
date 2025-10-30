Python ports of the EntradaTXT C programs.

- Install requirements (in a virtualenv) before running MPI ports:

  pip install -r Python/requirements.txt

- Each folder contains a `params.txt` used to provide inputs in the same format as the C `EntradaTXT`:
  - Vectors: size followed by elements
  - Matrices: n m followed by n*m elements (row-major)
  - SumMatrix: n m followed by A (n*m) then B (n*m)

- Examples:
  - Sequential SortVector:
    python3 Python/EntradaTXT/1_SortVector/Sequential/sort_vector.py

  - MPI SortVector (2 procs):
    mpirun -np 2 python3 Python/EntradaTXT/1_SortVector/MPI/sort_vector_mpi.py

Notes:
- MPI ports use mpi4py and scatter/gather of Python lists to avoid numpy dependency.
- These are initial ports; some MPI implementations use list scattering which may copy memory; for large data numpy+Scatterv would be more efficient.
