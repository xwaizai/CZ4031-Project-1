#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bpt.h"
#include "memBlock.h"
#include "readFile.h"
#include "sorting.h"

/*Solely for debuging purposes*/
void printRecord(char* record) {
    for (int i = 0; i < 20; i++) {
        printf("%c", *(record + i));
        if (i == 8 || i == 12)
            printf(" ");
    }
    printf("\n");
}

int main() {
    printf("\tStart Of Program \t\n");

    // memBlock* head = (memBlock*)malloc(sizeof(memBlock));

    // head->blockIndex = 0;
    // head->isFull = false;
    // head->next = NULL;

    // head = readFromFile(head);

    // int blocks = 0;
    // int j = 0;
    // int numRec = 0;
    // memBlock* current = head;
    // while (current) {
    //     blocks++;
    //     /*for(int i =0 ; i < 200 ;i++){
    //         printf("%c", current->block[i]);
    //     }
    //     printf("\n");*/
    //     current = current->next;
    // }
    // while (j < 200) {
    //     if (head->block[j] == 't') {
    //         numRec++;
    //     }
    //     j += 20;
    // }

    // printf("Experiment 1\n");
    // printf("Number of records: %lu\n", 10 * (blocks - 1) + numRec);
    // printf("Size of records: 20\n");
    // printf("Number of records stored in a block: %d\n", BLOCKSIZE / 20);
    // printf("Number of blocks: %d\n", blocks);

    // printf("size of unsigned int %d\n", sizeof(unsigned int));

    // /*store number of records*/
    // unsigned int noOfRec = 10 * (blocks - 1) + numRec;

    // /*initialise numVotes (key) array*/
    // unsigned int* numVotes = (int*)malloc(noOfRec * sizeof(int));

    // unsigned int blockaddrIndex = 0;

    // /*initialize blockaddr array*/
    // char** blockaddr = (char**)malloc(noOfRec * sizeof(char*));

    // current = head;
    // unsigned int numDigits = 0;
    // unsigned int numVote = 0;

    // /*looping through all the memBlocks to retrieve key and address of key
    // for
    //  * indexing*/
    // while (current) {
    //     for (int i = 0; i < 200; i += 20) {
    //         if (current->block[i] == 't') {
    //             numVote = atoi(&(current->block[13 + i]));
    //             printf("numVote: %d\n", numVote);
    //             numVotes[blockaddrIndex] = numVote;
    //             blockaddr[blockaddrIndex] = &(current->block[i]);

    //             printf("%d,%c\n", numVotes[blockaddrIndex],
    //                    *(blockaddr[blockaddrIndex]));
    //             printRecord((blockaddr[blockaddrIndex]));
    //             blockaddrIndex++;
    //         }
    //     }
    //     current = current->next;
    // }
    // printf("%d\n", noOfRec);

    // /*using quicksort algo to sort the arrays*/
    // printf("Sorting array...\n");
    // //quickSort(numVotes, blockaddr, 0, noOfRec - 1);

    // for (int i = 0; i < 1000; i++) {
    //     printf("%d ", numVotes[i]);
    //     printRecord(blockaddr[i]);
    // }

    unsigned int data[] = {5, 5, 6, 7, 8};
    char* dataC[] = {"tt99166906.60", "tt99163168.40", "tt99165388.40",
                     "tt99155207.00", "tt99155106.00"};

    unsigned int* fakeNumVotes = (int*)malloc(5 * sizeof(int));
    char** fakeBlkAddr = (char**)malloc(5 * sizeof(char*));

    for (int i = 0; i < sizeof(data)/data[0]; i++) {
        fakeNumVotes[i] = data[i];
        fakeBlkAddr[i] = dataC[i];
    }

    printf("Experiment 2\n");
    buildbpt(fakeNumVotes, fakeBlkAddr);

    return 0;
}