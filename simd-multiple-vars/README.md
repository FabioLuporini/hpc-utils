### Example compilation

```
CC=icc CFLAGS=-qopenmp make all
CC=gcc CFLAGS="-O3 -march=native --fast-math -ftree-vectorizer-verbose=7" make clean simd-multiple-vars
```

With ``gcc``, to emit verbose assembly, add: ``-S -g -fverbose-asm``
