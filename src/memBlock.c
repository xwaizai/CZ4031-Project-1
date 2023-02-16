#include <memBlock.h>

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
