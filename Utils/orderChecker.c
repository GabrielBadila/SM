#include <stdio.h>
#include <stdlib.h>

/* Function to check the order of the elements of an array */
void checkOrder(int *v, int n) {
    int i;
    for (i = 0; i < n - 1; ++i) {
        if (v[i] > v[i + 1]) {
            printf("The array is not ordered!\n");
            return;
        }
    }
    printf("The array is ordered!\n");
}

/* Driver program to test above function */
int main(int argc, char *argv[]) {
    int n, i;
    int *data = NULL;
    FILE * file = NULL;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <in_file>\n", argv[0]);
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

    checkOrder(data, n);

    return 0;
}
