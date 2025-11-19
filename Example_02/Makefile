default:	SendRecv1 SendRecv2 SendRecv3 SendRecv4
all:	SendRecv1 SendRecv2 SendRecv3 SendRecv4

SendRecv1: SendRecv1.c
	mpicc SendRecv1.c  -o SendRecv1

SendRecv2: SendRecv2.c
	mpicc SendRecv2.c  -o SendRecv2

SendRecv3: SendRecv3.c
	mpicc SendRecv3.c  -o SendRecv3

SendRecv4: SendRecv4.c
	mpicc SendRecv4.c  -o SendRecv4

run-1:
	mpirun -n 2 ./SendRecv1

run-2:
	mpirun -n 2 ./SendRecv2

run-3:
	mpirun -n 2 ./SendRecv3

run-4:
	mpirun -n 2 ./SendRecv4


clean:
	rm -f SendRecv1 SendRecv1.o SendRecv2 SendRecv2.o
	rm -f SendRecv3 SendRecv3.o SendRecv4 SendRecv4.o
