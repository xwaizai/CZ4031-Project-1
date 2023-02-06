#include "sorting.h"

// function to swap elements
void swap(unsigned int* a, unsigned int* b) {
    unsigned int t = *a;
    *a = *b;
    *b = t;
}
void swapAddr(char** a, char** b) {
    char* t = *a;
    *a = *b;
    *b = t;
}

// function to find the partition position
unsigned int partition(unsigned int array[],
                       char** blockaddr,
                       unsigned int low,
                       unsigned int high) {
    // select the rightmost element as pivot
    unsigned int pivot = array[high];

    // pointer for greater element
    unsigned int i = (low - 1);

    // traverse each element of the array
    // compare them with the pivot
    for (unsigned int j = low; j < high; j++) {
        if (array[j] <= pivot) {
            // if element smaller than pivot is found
            // swap it with the greater element pointed by i
            i++;

            // swap element at i with element at j
            swap(&array[i], &array[j]);
            swapAddr(&blockaddr[i], &blockaddr[j]);
        }
    }

    // swap the pivot element with the greater element at i
    swap(&array[i + 1], &array[high]);
    swapAddr(&blockaddr[i + 1], &blockaddr[high]);

    // return the partition point
    return (i + 1);
}

void quickSort(unsigned int array[],
               char** blockaddr,
               unsigned int low,
               unsigned int high) {
    if (low < high) {
        // find the pivot element such that
        // elements smaller than pivot are on left of pivot
        // elements greater than pivot are on right of pivot
        unsigned int pi = partition(array, blockaddr, low, high);

        // recursive call on the left of pivot
        quickSort(array, blockaddr, low, pi - 1);

        // recursive call on the right of pivot
        quickSort(array, blockaddr, pi + 1, high);
    }
}