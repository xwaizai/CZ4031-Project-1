#include <memBlock.h>
#include <sys/time.h>

memBlock* fillmemBlock(char* input, int length, memBlock* tail) {
    
    memBlock* current = tail;

    /*Case 1 when memory block is not full and input length is within the
     * BLOCKSIZE*/
    if (!current->isFull && current->blockIndex + length <= BLOCKSIZE) {
        memmove(current->block + current->blockIndex, input, length);
        current->blockIndex += length;
        if (current->blockIndex >= BLOCKSIZE) {
            current->isFull = true;
        }
        tail = current;
        /*Case 2 when memory block is full or input length exceeds the
         * BLOCKSIZE*/
    } else {
        memBlock* new = (memBlock*)malloc(sizeof(memBlock));
        new->isFull = false;
        new->next = NULL;
        current->next = new;
        memmove(new->block, input, length);
        new->blockIndex += length;
        tail = new;
    }
    
    return tail;
}

void bruteForceLinearScan(memBlock* current) {
    struct timeval start, end;
    double cpu_time_used;
    int noBlocks = 0;

    gettimeofday(&start, NULL);

    /*looping through all the memBlocks*/
    while (current) {
        noBlocks++;
        current = current->next;
    }

    gettimeofday(&end, NULL);
    
    cpu_time_used = (end.tv_sec - start.tv_sec)* 1000000;
    cpu_time_used += (end.tv_usec - start.tv_usec);

    printf("\nBrute Force Method\n");
    printf("Number of Data Blocks Accessed: %d\n", noBlocks);
    printf("Process Runtime: %.0f microseconds\n", cpu_time_used);
    printf("\n\n");
}
