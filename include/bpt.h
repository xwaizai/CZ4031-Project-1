#ifndef BPT_H_
#define BPT_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEYS 3  // 39
#define PTR 4   // 40

typedef struct node {
    unsigned int keys[KEYS];
    void* pointers[PTR];
    bool isLeaf;
    int size;
    struct node* parent;
} node;

node* bulkloadbpt(unsigned int* numVotesArr,
                  char** blkAddArr,
                  int numVotesLength,
                  node* root,int totalnumVotes);
void printbpt(node* root);

#endif