#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define NUMTHREADS 4

struct arg_struct {
    // int rank;
    int *v;
    int len;
};

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

/* Thread function */
void *sortThread(void *arguments) {
    struct arg_struct *args = arguments;
    selectionSort(args->v, args->len);
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
    for (i = 0; i < n; ++i) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

/* Driver program to test above functions */
int main(int argc, char *argv[]) {
    int n, i, j, iret;
    int *data = NULL;
    FILE * file = NULL;

    pthread_t thread[NUMTHREADS];
    struct arg_struct args_s[NUMTHREADS];

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <in_file> ( <out_file> )\n", argv[0]);
        exit(1);
    }

    // read size of data
    file = fopen(argv[1], "r");
    fscanf(file, "%d", &n);

    // read data from file
    data = (int *) malloc(n * sizeof(int));
    for (i = 0; i < n; i++) {
        fscanf(file, "%d", &(data[i]));
    }
    fclose(file);

    // compute chunk size (for every thread)
    int c = n / NUMTHREADS;
    if (n % NUMTHREADS) {
        c++;
    }

    // share array between threads in chunks
    for (i = 0; i < NUMTHREADS; ++i) {
        // args_s[i].rank = i;
        if (i != NUMTHREADS - 1) {
            args_s[i].len = c;
            args_s[i].v = (int *) malloc(args_s[i].len * sizeof(int));
        } else {
            args_s[i].len = n - (NUMTHREADS - 1) * c;
            args_s[i].v = (int *) malloc(args_s[i].len * sizeof(int));
        }

        for (j = c * i; j < c * (i + 1); j++) {
            if (j >= n) {
                break;
            }
            args_s[i].v[j % c] = data[j];
        }
    }

    // create threads
    for (i = 0; i < NUMTHREADS; ++i) {
        iret = pthread_create(&thread[i], NULL, sortThread, (void *)&args_s[i]);
        if(iret) {
            fprintf(stderr,"Error - pthread_create() return code: %d\n",iret);
            exit(EXIT_FAILURE);
        }
    }

    // join threads
    for (i = 0; i < NUMTHREADS; ++i) {
        pthread_join( thread[i], NULL);
    }

    // calculate depth for merge operation
    int depth = (int) sqrt(NUMTHREADS);
    if (depth * depth == NUMTHREADS) {
        depth++;
    } else {
        depth += 2;
    }

    int inc = 2, k = 1;

    // up to log_2(NUMTHREADS) merge steps
    while (k < depth) {
        for (i = 0; i < NUMTHREADS; i = i + inc) {
            int step = (int) sqrt(inc);
            if (i < NUMTHREADS - k) {
                int dim = args_s[i].len + args_s[i + step].len;
                int *aux = (int *) malloc(dim * sizeof(int));
                aux = merge(args_s[i].v, args_s[i].len, args_s[i + step].v, args_s[i + step].len);
                args_s[i].v = (int *) malloc(dim * sizeof(int));
                args_s[i].len = dim;
                args_s[i].v = aux;
            }
        }
        inc *= inc;
        k++;
    }

    // print or write sorted data to out file
    if (!argv[2]) {
        printf("Sorted array: \n");
        printArray(args_s[0].v, args_s[0].len);
    } else {
        file = fopen(argv[2], "w");
        fprintf(file, "%d\n", args_s[0].len);
        for (i = 0; i < args_s[0].len; i++) {
            fprintf(file, "%d\n", args_s[0].v[i]);
        }
        fclose(file);
    }

    return 0;
}
