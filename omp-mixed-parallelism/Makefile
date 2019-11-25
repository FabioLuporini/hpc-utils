ifeq ($(CC),)
CC := gcc
endif

ifeq ($(CFLAGS),)
CFLAGS := -fopenmp
endif

all: mixed_parallelism

mixed_parallelism: mixed_parallelism.c
	$(CC) $(CFLAGS) -std=c99 -o mixed_parallelism mixed_parallelism.c

clean:
	rm -f mixed_parallelism
