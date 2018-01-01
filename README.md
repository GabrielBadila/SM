# Selection Sort (Romanian language)



	Structuri multiprocesor (C1)

	Comenzi de compilare si de rulare pentru fiecare versiune (Mac OS)

	Badila Gabriel Alin
	342 C1


	=========================================================================================

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
        compilare:  gcc-7 -o pthreads pthreads.c -lpthread -lm
        rulare:     ./pthreads <in_file> ( <out_file> )

    Varianta Hybrid 1 (OpenMP + MPI)
        compilare:  mpic++ -o hybrid1 hybrid1.cpp -fopenmp
        rulare:     mpirun --oversubscribe -np <num_procs> ./hybrid1 <in_file> ( <out_file> )

	Rulare checker de verificare a fisierelor de output
		compilare:	gcc-7 -o orderChecker orderChecker.c
		rulare:		./orderChecker <in_file>

	=========================================================================================

	Tema 1
		- varianta seriala
		- varianta paralelizata folosind OpenMP
		- varianta paralelizata folosind MPI

	Tema 2
		- varianta paralelizata folosind Pthreads
		- varianta paralelizata folosind hibridul OpenMP + MPI

	Tema 3
		- diagramele cu rezultate pentru toate variantele
		- prezentarea rezultatelor

	=========================================================================================
