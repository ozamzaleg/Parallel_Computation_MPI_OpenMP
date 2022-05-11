**Name:** Oz Amzaleg , **ID:** 313554958

**Name:** Tal Saar , **ID:** 209151380

To run this program on terminal:

1) Enter this line : **sudo apt-get install libopenmpi-dev**

2) **make** - mpicc -fopenmp main.c functions.c -o histogram -lm

3) **make run** - mpirun -np 2 ./histogram <input.txt 10000  
