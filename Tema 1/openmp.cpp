/* A C++ program like a C program */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <omp.h>

// Structure for enabling reduction on the index of elements
struct Compare {
    int val = INT_MIN;
    int index;
};

// Custom reduction for finding the index of the max element
#pragma omp declare reduction(maximum : struct Compare : omp_out = omp_in.val > omp_out.val ? omp_in : omp_out)

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
        struct Compare max;
        // max.val = INT_MIN;
        max.val = v[i];
        max.index = i;
        // Use pragma omp paralle for reduction to parallelize work
        #pragma omp parallel for reduction(maximum:max)
        for (int j = i - 1; j >= 0; --j) {
            if (v[j] > max.val) {
                max.val = v[j];
                max.index = j;
            }
        }

        // Swap the found maximum element with the first element
        swap(v, i, max.index);
    }
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
    int n, i, tid;
    int *data = NULL;
    FILE * file = NULL;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <in_file> ( <out_file> )\n", argv[0]);
        exit(1);
    }

    omp_set_num_threads(4);

    #pragma omp parallel
	{
		tid = omp_get_thread_num();
		if (tid == 0) {
			printf("Number of threads: %d\n", omp_get_num_threads());
		}
	}

    // read size of data
    file = fopen(argv[1], "r");
    fscanf(file, "%d", &n);

    // read data from file
    data = (int *)malloc(n * sizeof(int));
    for (i = 0; i < n; i++) {
        fscanf(file, "%d", &(data[i]));
    }
    fclose(file);

    selectionSort(data, n);
    printf("Sorted array: \n");
    printArray(data, n);

    // write sorted data to out file
    /*
    file = fopen(argv[2], "w");
    for (i = 0; i < n; i++) {
        fprintf(file, "%d\n", data[i]);
    }
    fclose(file);
    */

    return 0;
}
