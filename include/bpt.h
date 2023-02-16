#ifndef BPT_H_
#define BPT_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "group.h"

#define KEYS 16  // 16
#define PTR 17   // 17

typedef struct node {
    unsigned int keys[KEYS];
    void* pointers[PTR];
    bool isLeaf;
    int size;
    struct node* parent;
} node;

node* searchleafnode(unsigned int key, node* root);
node* insertbpt(node* root, unsigned int key, char* addr);

#endif