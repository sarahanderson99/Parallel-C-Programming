CFLAGS = -g -Wall -Wstrict-prototypes
PROGS = graph.o MyMPI.o make-graph print-graph floyd-serial floyd-parallel
MCC = mpicc
CC = gcc

all: $(PROGS)

graph.o: graph.c
		$(MCC) $(CFLAGS) graph.c -c

MyMPI.o: MyMPI.c
		$(MCC) $(CFLAGS) MyMPI.c -c

make-graph: make-graph.c
		$(MCC) $(CFLAGS) make-graph.c graph.o MyMPI.o -o make-graph

print-graph: print-graph.c
		$(MCC) $(CFLAGS) print-graph.c graph.o MyMPI.o -o print-graph

floyd-serial: floyd-serial.c
	  $(MCC) $(CFLAGS) floyd-serial.c graph.o MyMPI.o -o floyd-serial

floyd-parallel: floyd-parallel.c
		$(MCC) $(CFLAGS) floyd-parallel.c graph.o MyMPI.o -o floyd-parallel -lm

clean:
		rm -f $(PROGS) *.o core*
