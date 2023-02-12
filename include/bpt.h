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

int buildbpt(unsigned int* numVotesArr, char** blkAddArr);
node* searchbpt(node* root, int key);
void insertbpt(int key, char* record, node* root);

#endif