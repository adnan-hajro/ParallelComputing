CFLAGS=-g -O3 -fstrict-aliasing \
 -ftree-vectorize -march=native -mtune=native \
 -fopt-info-vec-optimized -mprefer-vector-width=256

stream_triad: stream_triad.o timer.o

clean:
	rm -rf ./stream_triad ./stream_triad.o ./timer.o