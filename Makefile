build:
	mpicc -fopenmp main.c functions.c -o histogram -lm

clean:
	rm -f *.o ./histogram

run:
	mpirun -np 2 ./histogram <input.txt 10000


