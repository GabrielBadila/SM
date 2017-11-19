#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <omp.h>
#include <iostream>
#include <vector>

using namespace std;

// Structure for enabling reduction on the index of elements
struct Compare { int val = INT_MIN; int index; };
// Custom reduction for finding the index of the max element
#pragma omp declare reduction(maximum : struct Compare : omp_out = omp_in.val > omp_out.val ? omp_in : omp_out)

/* Function to interchange two variables */
void swap(int &x, int &y) {
    int aux = x;
    x = y;
    y = aux;
}

/* Function to sort an array */
void selectionSort(vector<int> &arr, int n) {
    // One by one move boundary of unsorted subarray
    for (int i = n - 1; i > 0; --i) {
        // Find the maximum element in unsorted array
        struct Compare max;
        max.val = arr[i];
        max.index = i;
        // Use pragma omp paralle for reduction to parallelize work
        #pragma omp parallel for reduction(maximum:max)
        for (int j = i - 1; j >= 0; --j) {
            if (arr[j] > max.val) {
                max.val = arr[j];
                max.index = j;
            }
        }

        // Swap the found maximum element with the first element
        swap(arr[i], arr[max.index]);
    }
}

/* Function to print an array */
void printArray(vector<int> arr, int size) {
    int i;
    for (i=0; i < size; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

/* Driver program to test above functions */
int main() {
    int num, tid;
    vector<int> arr;

    omp_set_num_threads(4);

    #pragma omp parallel
	{
		tid = omp_get_thread_num();
		if (tid == 0) {
			printf("Number of threads: %d\n", omp_get_num_threads());
		}
	}

    while(cin >> num) {
        arr.push_back(num);
    }

    selectionSort(arr, arr.size());
    printf("Sorted array: \n");
    printArray(arr, arr.size());

    return 0;
}
