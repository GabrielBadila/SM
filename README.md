# Selection Sort (Romanian language)



	Structuri multiprocesor (C1)

	Comenzi de compilare si de rulare pentru fiecare versiune (Mac OS)

	Badila Gabriel Alin
	342 C1


	============================================================================

	Varianta Seriala
        compilare:  gcc-7 -o serial serial.c
        rulare:     ./serial <in_file> ( <out_file> )

    Varianta OpenMP
        compilare:  g++-7 -o openmp openmp.cpp -fopenmp
        rulare:     ./openmp <in_file> ( <out_file> )

    Varianta MPI
        compilare:  mpicc -o mpi mpi.c
        rulare:     mpirun --oversubscribe -np <num_procs> ./mpi <in_file> ( <out_file> )

    Varianta Pthreads
        compilare:  gcc-7 -o pthreads pthreads.c -lpthread
        rulare:     ./pthreads <in_file> ( <out_file> )

    Varianta Hybrid 1 (OpenMP + MPI)
        compilare:  mpic++ -o hybrid1 hybrid1.cpp -fopenmp
        rulare:     mpirun --oversubscribe -np <num_procs> ./mpi <in_file> ( <out_file> )

	============================================================================
