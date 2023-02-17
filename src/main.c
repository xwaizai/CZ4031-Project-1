#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bpt.h"
#include "group.h"
#include "memBlock.h"
#include "misc.h"
#include "readFile.h"
#include "sorting.h"

int main() {
    printf("\tStart Of Program \t\n");

    memBlock* head = (memBlock*)malloc(sizeof(memBlock));

    head->blockIndex = 0;
    head->isFull = false;
    head->next = NULL;

    head = readFromFile(head);

    int blocks = 0;
    int j = 0;
    int numRec = 0;
    memBlock* current = head;
    memBlock* last = NULL;
    while (current) {
        blocks++;
        /*for(int i =0 ; i < 200 ;i++){
            printf("%c", current->block[i]);
        }
        printf("\n");*/
        if (current->next == NULL) {
            last = current;
        }
        current = current->next;
    }
    while (j < 200) {
        if (last->block[j] == 't') {
            numRec++;
        }
        j += 20;
    }

    printf("--- Experiment 1: Store data on disk ---\n");
    printf("Number of records: %lu\n", 10 * (blocks - 1) + numRec);
    printf("Size of records: 20\n");
    printf("Number of records stored in a block: %d\n", BLOCKSIZE / 20);
    printf("Number of blocks: %d\n", blocks);

    printf("size of unsigned int %d\n", sizeof(unsigned int));
    printf("\n");

    /*store number of records*/
    unsigned int noOfRec = 10 * (blocks - 1) + numRec;

    /*initialise numVotes (key) array*/
    unsigned int* numVotes = (int*)malloc(noOfRec * sizeof(int));

    unsigned int blockaddrIndex = 0;

    /*initialize blockaddr array*/
    char** blockaddr = (char**)malloc(noOfRec * sizeof(char*));

    current = head;
    unsigned int numDigits = 0;
    unsigned int numVote = 0;

    /*looping through all the memBlocks to retrieve key and address of key
    for
     * indexing*/
    while (current) {
        for (int i = 0; i < 200; i += 20) {
            if (current->block[i] == 't') {
                numVote = atoi(&(current->block[13 + i]));
                // printf("numVote: %d\n", numVote);
                numVotes[blockaddrIndex] = numVote;
                blockaddr[blockaddrIndex] = &(current->block[i]);

                // if (numVotes[blockaddrIndex] == 0)
                //     printf("%d\n", numVotes[blockaddrIndex]);

                // printf("%d,%c\n", numVotes[blockaddrIndex],
                //        *(blockaddr[blockaddrIndex]));
                // printRecord((blockaddr[blockaddrIndex]));
                blockaddrIndex++;
            }
        }
        current = current->next;
    }

    printf("--- Experiment 2: Build B+ Tree on 'numVotes' ---\n");
    node* root = NULL;
    for (int i = 0; i < noOfRec; i++) {
        root = insertbpt(root, numVotes[i], blockaddr[i]);
    }
    printBPTStats(root);
    printf("\n");

    // printbpt(root);
    saveToFile(root);

    printf("--- Experiment 3: Movies with 'numVotes' equal to 500 ---\n");
    findNumVotes(500, root);
    printf("\n");

    printf("--- Experiment 4: Movies with 'numVotes' from 30,000 to 40,000 ---\n");
    findRangeNumVotes(30000, 40000, root);
    printf("\n");

    return 0;
}