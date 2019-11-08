### Example compilation

```
CC=icc CFLAGS=-qopenmp make clean mixed_numthreads
```

### Example runs

First, try both loops with same `num_threads`

```
OMP_PLACES=threads OMP_PROC_BIND=spread ./mixed_numthreads 32 32
OMP_PLACES=threads OMP_PROC_BIND=spread ./mixed_numthreads 64 64
```

Then, try with different `num_threads`

```
OMP_PLACES=threads OMP_PROC_BIND=spread ./mixed_numthreads 64 32
```

Check out the runtimes of these runs!
