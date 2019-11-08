ifeq ($(CC),)
CC := gcc
endif

ifeq ($(CFLAGS),)
CFLAGS := -fopenmp
endif

all: mixed_numthreads

mixed_numthreads: mixed_numthreads.c
	$(CC) $(CFLAGS) -std=c99 -o mixed_numthreads mixed_numthreads.c

clean:
	rm -f mixed_numthreads
