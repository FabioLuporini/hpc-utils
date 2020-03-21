ifeq ($(CC),)
CC := gcc
endif

ifeq ($(CFLAGS),)
CFLAGS := -fopenmp
endif

all: clean simd-multiple-vars simd-single-var

simd-multiple-vars: simd-multiple-vars.c
	$(CC) $(CFLAGS) -std=c99 -o simd-multiple-vars simd-multiple-vars.c

simd-single-var: simd-single-var.c
	$(CC) $(CFLAGS) -std=c99 -o simd-single-var simd-single-var.c

clean:
	rm -f simd-multiple-vars simd-single-var
