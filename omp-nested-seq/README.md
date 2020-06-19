### Example compilation

```
CC=icc CFLAGS=-qopenmp make clean nested-seq
```

### Example runs

Without nested parallelism:
```
OMP_PLACES=threads OMP_PROC_BIND=spread ./nested-seq 128 1
```

With nested parallelism (multiple `num_threads` combinations):
```
OMP_PLACES=threads OMP_PROC_BIND=spread,spread KMP_HOT_TEAMS_MODE=1 KMP_HOT_TEAMS_MAX_LEVEL=2 ./nested-seq 64 2
OMP_PLACES=threads OMP_PROC_BIND=spread,spread KMP_HOT_TEAMS_MODE=1 KMP_HOT_TEAMS_MAX_LEVEL=2 ./nested-seq 32 4
OMP_PLACES=threads OMP_PROC_BIND=spread,spread KMP_HOT_TEAMS_MODE=1 KMP_HOT_TEAMS_MAX_LEVEL=2 ./nested-seq 16 8
OMP_PLACES=threads OMP_PROC_BIND=spread,spread KMP_HOT_TEAMS_MODE=1 KMP_HOT_TEAMS_MAX_LEVEL=2 ./nested-seq 8 16
OMP_PLACES=threads OMP_PROC_BIND=spread,spread KMP_HOT_TEAMS_MODE=1 KMP_HOT_TEAMS_MAX_LEVEL=2 ./nested-seq 4 32
OMP_PLACES=threads OMP_PROC_BIND=spread,spread KMP_HOT_TEAMS_MODE=1 KMP_HOT_TEAMS_MAX_LEVEL=2 ./nested-seq 2 64
OMP_PLACES=threads OMP_PROC_BIND=spread,spread KMP_HOT_TEAMS_MODE=1 KMP_HOT_TEAMS_MAX_LEVEL=2 ./nested-seq 1 128
```

Check out the runtimes of these runs!

Is nested parallelism actually happening?
