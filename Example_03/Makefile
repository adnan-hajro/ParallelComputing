default:	FileRead
all:	FileRead

FileRead: FileRead.c
	mpicc FileRead.c  -o FileRead

run:
	mpirun -n 8 ./FileRead

clean:
	rm -f FileRead FileRead.o