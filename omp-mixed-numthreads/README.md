### Example compilation

```
CC=icc CFLAGS=-qopenmp make clean mixed_numthreads
```

### Example runs

Assume you're running on a platform with 64 physical cores and hyperthreading enabled.

First, try both loops with same `num_threads`

```
OMP_PLACES=threads OMP_PROC_BIND=spread ./mixed_numthreads 32 32
OMP_PLACES=threads OMP_PROC_BIND=spread ./mixed_numthreads 64 64
```

Then, try with different `num_threads`

```
OMP_PLACES=threads OMP_PROC_BIND=spread ./mixed_numthreads 64 32
```

With the intel suite, you can make it better:

```
OMP_PLACES=threads OMP_PROC_BIND=spread KMP_HOT_TEAMS_MODE=1 ./mixed_numthreads 64 32
```

It may still run awfully slow. Why? Because of thread migration due to executing two
loops with different `num_threads` while having set `OMP_PROC_BIND=spread`,
which may cause lots of L2 cache misses.

Then you may try one of the following:

```
OMP_PLACES=cores OMP_PROC_BIND=close KMP_HOT_TEAMS_MODE=1 ./mixed_numthreads 4 2
OMP_PLACES=0,16,32,48 OMP_PROC_BIND=close KMP_HOT_TEAMS_MODE=1 ./mixed_numthreads 4 2
OMP_PLACES=1,17,33,49 OMP_PROC_BIND=close KMP_HOT_TEAMS_MODE=1 ./mixed_numthreads 4 2
```

They should all perform the same, but potentially still a bit slower than:

```
OMP_PLACES=threads OMP_PROC_BIND=spread ./mixed_numthreads 64 64
```
