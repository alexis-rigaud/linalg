# Makefile
# (c) Matthew Drury, 2017
# (c) Alexis Rigaud, 2024

all: clean linalg

clean:
	rm -fr *.o
	rm -fr linalg.dSYM
	rm -fr linalg

linalg:
	gcc -std=c99 -Wall -g -O3 -o linalg -Wall main.c vector.c matrix.c errors.c util.c tests.c linsolve.c eigen.c linreg.c rand.c

mem:
	clang -fsanitize=address,leak,undefined -std=c99 -Wall -g -O3 -o linalg -Wall main.c vector.c matrix.c errors.c util.c tests.c linsolve.c eigen.c linreg.c rand.c
	ASAN_OPTIONS=detect_leaks=1 ./linalg
