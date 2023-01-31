#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define BLOCKSIZE 200

#define DISKSIZE BLOCKSIZE*1024*1024

typedef struct memBlock
{
    char block[BLOCKSIZE];
    bool isFull;
    unsigned int blockIndex;
	struct memBlock* next;

}memBlock;


#endif