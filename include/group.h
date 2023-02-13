#ifndef GROUP_H_
#define GROUP_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GROUP_LEN 24

typedef struct group {
    unsigned int key;
    int size;
    char* pointers[GROUP_LEN];
    struct group* next;
} group;

unsigned int countUnique(unsigned int* array, 
                        unsigned int arraySize, 
                        unsigned int* unique);

group** createGroups(unsigned int* numVotesArr,
                  char** blkAddArr,
                  int numVotesLength);

#endif