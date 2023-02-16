#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bpt.h"
#include "memBlock.h"
#include "readFile.h"
#include "sorting.h"
#include "group.h"

/*Solely for debuging purposes*/
void printRecord(char* record) {
    for (int i = 0; i < 20; i++) {
        printf("%c", *(record + i));
        if (i == 8 || i == 12)
            printf(" ");
    }
    printf("\n");
}

void printGroup(group* cur){
    do{
        for (int i=0; i<cur->size; i++){
            printf("%d :",i);
            printRecord(cur->pointers[i]);
        }
        cur=cur->next;
    }while(cur);
}

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
        if(current->next==NULL){
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

    printf("Experiment 1\n");
    printf("Number of records: %lu\n", 10 * (blocks - 1) + numRec);
    printf("Size of records: 20\n");
    printf("Number of records stored in a block: %d\n", BLOCKSIZE / 20);
    printf("Number of blocks: %d\n", blocks);

    printf("size of unsigned int %d\n", sizeof(unsigned int));

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
    //printf("Number of records %d\n", noOfRec);

    /*using quicksort algo to sort the arrays*/
    //printf("Sorting array...\n");
    //quickSort(numVotes, blockaddr, 0, noOfRec - 1);

    // for (int i = 0; i < noOfRec; i++) {
    //     if (i<=46){
    //         printf("%d ", numVotes[i]);
    //         printRecord(blockaddr[i]);
    //     }
    // }

    /*initialize uniqueNumVotes (key) array for grouping*/
    //printf("Grouping votes...\n");
    //unsigned int* uniqueNumVotes = (int*)malloc(noOfRec * sizeof(int));
    //unsigned int countUniqueVotes = countUnique(numVotes, noOfRec, uniqueNumVotes);
    //group** groupVotes = createGroups(numVotes, blockaddr, noOfRec);

    //group* cur = groupVotes[0];
    //printGroup(cur);
    
    // printf("Number of unique votes: %d\n", countUniqueVotes);
    // for (int i = 0; i < countUniqueVotes; i++) {
    //     printf("%d ",uniqueNumVotes[i]);
    // }

    // unsigned int data[] = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11,
    //                        12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22};
    // char* dataC[] = {
    //     "tt99166906.60", "tt99163168.40", "tt99165388.40", "tt99155207.00",
    //     "tt99155106.00", "tt99155178.00", "tt99234117.00", "tt99234127.00",
    //     "tt99234137.00", "tt99234127.00", "tt99232137.00", "tt99232117.00",
    //     "tt99232117.00", "tt99232117.00", "tt99232117.00", "tt99232117.00",
    //     "tt99232117.00", "tt99232117.00", "tt99232117.00", "tt99232117.00",
    //     "tt99232117.00", "tt99232117.00"};
    
    // int size = sizeof(data) / sizeof(unsigned int);

    printf("---Experiment 2---\n");
    //node* root = bulkloadbpt(uniqueNumVotes, groupVotes, countUniqueVotes, NULL);
    // node* root = bulkloadbpt(data, dataC, size, NULL);

    // printf("%d %d\n", root->keys[0], root->keys[1]);

    node* root = NULL;
    for (int i = 0; i < noOfRec; i++)
    {   
        //printf("Inserting %d\n", numVotes[i]);
        root = insertbpt(root,numVotes[i],blockaddr[i]);
        /*for (int i = 0; i < root->size; i++)
        {
            printf("%d ",root->keys[i]);
        }
        printf("\n");*/
        //printbpt(root);
    }
    

    //printbpt(root);
    saveToFile(root);
    

    return 0;
}