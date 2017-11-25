#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <mpi.h>

/* Function to interchange two variables */
static inline /* this improves performance */
void swap(int *v, int i, int j) {
    int aux = v[i];
    v[i] = v[j];
    v[j] = aux;
}

/* Function to sort an array */
void selectionSort(int *v, int n) {
    // One by one move boundary of unsorted subarray
    for (int i = n - 1; i > 0; --i) {
        // Find the maximum element in unsorted array
        int max = i;
        for (int j = i - 1; j >= 0; --j) {
            if (v[j] > v[max]) {
                max = j;
            }
        }

        // Swap the found maximum element with the first element
        swap(v, i, max);
    }
}

/* Function to merge two sorted arrays v1, v2 of lengths n1, n2, respectively */
int * merge(int *v1, int n1, int *v2, int n2) {
    int * result = (int *) malloc((n1 + n2) * sizeof(int));
    int i = 0, j = 0, k;
    for (k = 0; k < n1 + n2; k++) {
        if (i >= n1) {
            result[k] = v2[j];
            j++;
        } else if (j >= n2) {
            result[k] = v1[i];
            i++;
        } else if (v1[i] < v2[j]) { // indices in bounds as i < n1 && j < n2
            result[k] = v1[i];
            i++;
        } else { // v2[j] <= v1[i]
            result[k] = v2[j];
            j++;
        }
    }
    return result;
}

/* Function to print an array */
void printArray(int *v, int n) {
    int i;
    for (i=0; i < n; ++i) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

/* Driver program to test above functions */
int main(int argc, char *argv[]) {
    int n, rank, nProcesses;
    int *data = NULL, *chunk, *other;
    int c, s, o, step, i;

    FILE * file = NULL;
    MPI_Status status;

    if (argc < 2) {
        fprintf(stderr, "Usage: mpirun --oversubscribe -np <num_procs> %s <in_file> ( <out_file> )\n", argv[0]);
        exit(1);
    }

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	printf("Hello from %i/%i\n", rank, nProcesses);

    if(rank == 0) { // This code is run by a single process
        // read size of data
        file = fopen(argv[1], "r");
        fscanf(file, "%d", &n);
        // compute chunk size
        c = n / nProcesses;
        if (n % nProcesses) {
            c++;
        }
        // read data from file
        data = (int *)malloc(nProcesses * c * sizeof(int));
        for (i = 0; i < n; i++) {
            fscanf(file, "%d", &(data[i]));
        }
        fclose(file);
        // pad data with 0 -- doesn't matter
        for (i = n; i < nProcesses * c; i++) {
            data[i] = 0;
        }
	}

    // broadcast size
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // compute chunk size
    c = n / nProcesses;
    if (n % nProcesses) {
        c++;
    }

    // scatter data
    chunk = (int *) malloc(c * sizeof(int));
    MPI_Scatter(data, c, MPI_INT, chunk, c, MPI_INT, 0, MPI_COMM_WORLD);
    free(data);
    data = NULL;

    // compute size of own chunk and sort it
    s = (n >= c * (rank + 1)) ? c : n - c * rank;
    selectionSort(chunk, s);

    // up to log_2 p merge steps
    for (step = 1; step < nProcesses; step = 2 * step) {
        if (rank % (2 * step) != 0) {
            // rank is no multiple of 2 * step: send chunk to rank-step and exit loop
            MPI_Send(chunk, s, MPI_INT, rank - step, 0, MPI_COMM_WORLD);
            break;
        }
        // rank is multiple of 2 * step: merge in chunk from rank + step (if it exists)
        if (rank + step < nProcesses) {
            // compute size of chunk to be received
            o = (n >= c * (rank + 2 * step)) ? c * step : n - c * (rank + step);
            // receive other chunk
            other = (int *)malloc(o * sizeof(int));
            MPI_Recv(other, o, MPI_INT, rank + step, 0, MPI_COMM_WORLD, &status);
            // merge and free memory
            data = merge(chunk, s, other, o); // pe aici e ceva in neregula
            free(chunk);
            free(other);
            chunk = data;
            s = s + o;
        }
    }

    // print sorted data
    if(rank == 0) {
        printf("Sorted array: \n");
        printArray(chunk, s);
    }

    // write sorted data to out file
    /*
    if(rank == 0) {
        file = fopen(argv[2], "w");
        for (i = 0; i < s; i++) {
            fprintf(file, "%d\n", chunk[i]);
        }
        fclose(file);
    }
    */

    MPI_Finalize();
    return 0;
}
