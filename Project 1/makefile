CFLAGS = -g -Wall -Wstrict-prototypes
PROGS = make-list print-list serial-add-list parallel-add-list
LDFLAGS = -lm
CC = gcc
MCC = mpicc

all: $(PROGS)
make-list: make-list.o
print-list: print-list.o
serial-add-list: serial-add-list.o
parallel-add-list: parallel-add-list.o MyMPI.o
	$(MCC) $(LDFLAGS) -o $@ $+
clean:
	rm -f $(PROGS) *.o core*
