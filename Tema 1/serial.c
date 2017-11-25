#include <stdio.h>
#include <stdlib.h>

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
    int n, i;
    int *data = NULL;
    FILE * file = NULL;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <in_file> ( <out_file> )\n", argv[0]);
        exit(1);
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
