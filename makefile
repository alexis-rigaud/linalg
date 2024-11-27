# Makefile (for sanitization)
# (c) Matthew Drury, 2017
# (c) Alexis Rigaud, 2024

all: clean mem

clean:
	rm -fr *.o
	rm -fr linalg.dSYM
	rm -fr linalg

mem:
	clang -fsanitize=address,leak,undefined -std=c99 -Wall -g -O3 -o linalg main.c vector.c matrix.c errors.c util.c tests.c linsolve.c eigen.c linreg.c rand.c kernel.c -framework OpenCL
	ASAN_OPTIONS=detect_leaks=1 ./linalg
