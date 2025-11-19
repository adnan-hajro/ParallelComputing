default:	DebugPrintout
all:	DebugPrintout

%.o: %.c
	mpicc -c $^

DebugPrintout: DebugPrintout.o timer.o
	mpicc $^ -o DebugPrintout

run:
	mpirun -n 2 ./DebugPrintout

clean:
	rm -f DebugPrintout DebugPrintout.o timer.o

distclean:
	rm -f Makefile