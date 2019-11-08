ifeq ($(CC),)
CC := gcc
endif

ifeq ($(CFLAGS),)
CFLAGS := -fopenmp
endif

all: affinity

affinity: affinity.c
	$(CC) $(CFLAGS) -std=c99 -o affinity affinity.c

clean:
	rm -f affinity
