ifeq ($(CC),)
CC := gcc
endif

ifeq ($(CFLAGS),)
CFLAGS := -fopenmp
endif

all: nested

nested: nested.c
	$(CC) $(CFLAGS) -std=c99 -o nested nested.c

clean:
	rm -f nested
