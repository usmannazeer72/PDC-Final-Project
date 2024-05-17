# PDC-Final-Project

## Top K Shortest Path Problem with MPI and OpenMP

This project focuses on solving the Top K Shortest Path Problem using a combination of MPI for 
distributed computing and OpenMP for shared memory parallelization within MPI processes. The 
goal is to find the Kth shortest path among all nodes of a given graph.

## Installation and Setup

To run this project on your system, you need to have the following prerequisites:
• Ubuntu OS installed
• MPI (Message Passing Interface) library
• OpenMP library

## Installing MPI and OpenMP on Ubuntu

1. MPI Installation:
   
sudo apt-get update
sudo apt-get install mpich

2. OpenMP Installation: OpenMP usually comes bundled with most compilers, including 
GCC. Ensure you have GCC installed with OpenMP support.

sudo apt-get install libomp-dev
Compiling and Executing the Code

Once you have the required libraries and setup ready, follow these steps to compile and execute 
the code:

1. Compile the Code:

mpicc -fopenmp -o K_shortest I210556_I210464_I210851.c

2. Run the Executable: Use mpiexec to run the compiled executable with the desired 
number of MPI processes. Replace 4 with the number of MPI processes you want to run.

mpiexec -np 4 ./K_shortest

## Project Contributors

• Muhammad Kashif 
• Usman Nazeer
• Bilal Saleem
