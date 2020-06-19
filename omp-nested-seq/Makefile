ifeq ($(CC),)
CC := gcc
endif

ifeq ($(CFLAGS),)
CFLAGS := -fopenmp
endif

all: nested

nested: nested-seq.c
	$(CC) $(CFLAGS) -std=c99 -o nested-seq nested-seq.c

clean:
	rm -f nested-seq
