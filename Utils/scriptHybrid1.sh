#!bin/bash
module load compilers/gnu-5.4.0
module load libraries/openmpi-2.0.1-gcc-5.4.0
time mpirun -np 1 ./mpi superBigTestFile.txt
