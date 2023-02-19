#ifndef MEMBLOCK_H_
#define MEMBLOCK_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCKSIZE 200

#define DISKSIZE BLOCKSIZE * 1024 * 1024

typedef struct memBlock {
    char block[BLOCKSIZE];
    bool isFull;
    unsigned int blockIndex;
    struct memBlock* next;
} memBlock;

memBlock* fillmemBlock(char* input, int length, memBlock* current);

#endif