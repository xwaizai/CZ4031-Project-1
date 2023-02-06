#ifndef SORTING_H_
#define SORTING_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(unsigned int* a, unsigned int* b);

void swapAddr(char** a, char** b);

unsigned int partition(unsigned int array[],
                       char** blockaddr,
                       unsigned int low,
                       unsigned int high);

void quickSort(unsigned int array[],
               char** blockaddr,
               unsigned int low,
               unsigned int high);

#endif