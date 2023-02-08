#ifndef BPT_H_
#define BPT_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEYS 4  // 39
#define PTR 5   // 40

typedef struct node {
    unsigned int keys[KEYS];
    char* pointers[PTR];
    bool isLeaf;
    int size;
} node;

void buildbpt(void);

#endif