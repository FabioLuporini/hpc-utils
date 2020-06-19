### Example compilation

```
CC=icc CFLAGS=-qopenmp make clean mixed_parallelism
```

### Example runs

Assume you're running on a platform with 64 physical cores and hyperthreading enabled.

First, try w/o nested parallelism. This should be best performing version.

```
OMP_PLACES=threads OMP_PROC_BIND=spread,spread KMP_HOT_TEAMS_MODE=1 KMP_HOT_TEAMS_MAX_LEVEL=2  ./mixed_parallelism 128 1 128 1
```

Then, w/ nested parallelism. Several variants possible.

```
OMP_PLACES=threads OMP_PROC_BIND=spread,spread KMP_HOT_TEAMS_MODE=1 KMP_HOT_TEAMS_MAX_LEVEL=2  ./mixed_parallelism 64 2 64 2
OMP_PLACES=threads OMP_PROC_BIND=spread,spread KMP_HOT_TEAMS_MODE=1 KMP_HOT_TEAMS_MAX_LEVEL=2  ./mixed_parallelism 32 4 32 4
```

Another interesting use case is nested parallelism with mixed `num_threads`

```
OMP_PLACES=threads OMP_PROC_BIND=spread,spread KMP_HOT_TEAMS_MODE=1 KMP_HOT_TEAMS_MAX_LEVEL=2  ./mixed_parallelism 64 2 32 4
```
