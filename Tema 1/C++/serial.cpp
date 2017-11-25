#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace std;

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
        int max = i;
        for (int j = i - 1; j >= 0; --j) {
            if (arr[j] > arr[max]) {
                max = j;
            }
        }

        // Swap the found maximum element with the first element
        swap(arr[i], arr[max]);
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
    int num;
    vector<int> arr;

    while(cin >> num) {
        arr.push_back(num);
    }

    selectionSort(arr, arr.size());
    printf("Sorted array: \n");
    printArray(arr, arr.size());

    return 0;
}
