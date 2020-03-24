all: acc-routines

acc-routines: acc-routines.c Makefile
	mpicc -fast -acc -ta=nvidia acc-routines.c -o acc-routines

clean:
	rm -f acc-routines
