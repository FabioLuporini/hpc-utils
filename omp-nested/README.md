### Example compilation

```
CC=icc CFLAGS=-qopenmp make clean nested
```

### Example runs

With nested parallelism:
```
OMP_PLACES=threads OMP_PROC_BIND=spread,close KMP_HOT_TEAMS_MODE=1 KMP_HOT_TEAMS_MAX_LEVEL=2 KMP_BLOCKTIME=infinite ./nested 64 2
```

Without nested parallelism:
```
OMP_PLACES=threads OMP_PROC_BIND=spread ./nested 128 1
```

Check out the runtimes of these two runs!
