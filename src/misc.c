#include "misc.h"
#include <stdio.h>
#include <string.h>
#include "bpt.h"
#include "group.h"
#include "readFile.h"

void printmemBlock(memBlock* current){
    for (int i = 0; i < BLOCKSIZE; i++) {
        printf("%c", current->block[i]);
    }
    printf("\n");
}

void printRecord(char* record) {
    for (int i = 0; i < TOBEINSERTEDSIZE; i++) {
        printf("%c", *(record + i));
        if (record[i] == ' ')
            printf(" ");
    }
    printf("\n");
}

void printRate(char* rate) {
    int i = 0;
    while (rate[i] != '\0') {
        printf("%c", rate[i++]);
    }
    printf("\n");
}

double getRateFromStr(char* record) {
    int commacount= 0;
    int index = 0;
    char temp[5];
    unsigned int numVote;
    for(int i =0 ; i<TOBEINSERTEDSIZE ; i++){
        if(commacount == 2){
            temp[index] = '\0';
            break;
        }
        if(commacount == 1){
            temp[index] = record[i];
            index++;
        }
        
        if(record[i] == ','){
            commacount++;
        }
    }

    return strtod(temp, NULL);
}

void printGroup(group* cur, int* blocksAcc, double* totalRate, double* count) {
    double value = 0;

    do {
        for (int i = 0; i < cur->size; i++) {
            value = getRateFromStr(cur->pointers[i]);
            (*totalRate) += value;
            (*count)++;
        }
        cur = cur->next;
        (*blocksAcc)++;
    } while (cur);
}

void printBPTStats(node* root) {
    int noNodes = 1, noLevels = 1;
    node* ptr = root;
    node* cur = root;

    for (int i = 0; i < ptr->size; i++) {
        if (i == 0 && !ptr->isLeaf)
            queueInsert(ptr->pointers[i]);
        if (!ptr->isLeaf)
            queueInsert(ptr->pointers[i + 1]);
    }
    while (!queueIsEmpty()) {
        int nodeOnThisLvl = queueSize();
        for (int s = 0; s < nodeOnThisLvl; s++) {
            ptr = (node*)queuePop();
            noNodes++;
            for (int i = 0; i < ptr->size; i++) {
                if (!ptr->isLeaf) {
                    queueInsert(ptr->pointers[i]);
                    if (i + 1 == ptr->size && ptr->pointers[i + 1] != NULL)
                        queueInsert(ptr->pointers[i + 1]);
                }
            }
        }
    }

    while (!cur->isLeaf) {
        cur = (node*)cur->pointers[0];
        noLevels++;
    }

    printf("Parameter n of B+ Tree: %d\n", KEYS);
    printf("Number of Nodes of B+ Tree: %d\n", noNodes);
    printf("Number of Levels of B+ Tree: %d\n", noLevels);
    printf("Content of Root Node (Keys): ");
    for (int i = 0; i < root->size; i++) {
        printf("%d ", root->keys[i]);
    }
    printf("\n");
}

void printbpt(node* root) {
    printf("------Printing BPT------\n");

    node* ptr = root;
    for (int i = 0; i < ptr->size; i++) {
        printf("%d ", ptr->keys[i]);
        if (i == 0 && !ptr->isLeaf)
            queueInsert(ptr->pointers[i]);

        if (!ptr->isLeaf)
            queueInsert(ptr->pointers[i + 1]);
    }
    printf("|\n");
    while (!queueIsEmpty()) {
        int nodeOnThisLvl = queueSize();
        for (int s = 0; s < nodeOnThisLvl; s++) {
            ptr = (node*)queuePop();
            for (int i = 0; i < ptr->size; i++) {
                // printf("hi\n");
                printf("%d ", ptr->keys[i]);
                // if (ptr->isLeaf) {
                //     printRecordHere((char*)ptr->pointers[i]);
                // }
                if (!ptr->isLeaf) {
                    queueInsert(ptr->pointers[i]);
                    if (i + 1 == ptr->size && ptr->pointers[i + 1] != NULL)
                        queueInsert(ptr->pointers[i + 1]);
                }
            }
            printf("| ");
        }
        printf("\n");
    }
}

void saveToFile(node* root, char* fileName) {
    int num;
    FILE* fptr;

    // use appropriate location if you are using MacOS or Linux
    fptr = fopen(fileName, "w");

    if (fptr == NULL) {
        printf("Error!");
        exit(1);
    }

    node* ptr = root;
    for (int i = 0; i < ptr->size; i++) {
        fprintf(fptr, "%d ", ptr->keys[i]);
        if (i == 0)
            queueInsert(ptr->pointers[i]);
        queueInsert(ptr->pointers[i + 1]);
    }
    fprintf(fptr, "|\n\n");
    while (!queueIsEmpty()) {
        int nodeOnThisLvl = queueSize();
        for (int s = 0; s < nodeOnThisLvl; s++) {
            ptr = (node*)queuePop();
            for (int i = 0; i < ptr->size; i++) {
                // printf("hi\n");
                fprintf(fptr, "%d ", ptr->keys[i]);
                // if (ptr->isLeaf) {
                //     printRecordHere((char*)ptr->pointers[i]);
                // }
                if (!ptr->isLeaf) {
                    queueInsert(ptr->pointers[i]);
                    if (i + 1 == ptr->size && ptr->pointers[i + 1] != NULL)
                        queueInsert(ptr->pointers[i + 1]);
                }
            }
            fprintf(fptr, "| ");
        }
        fprintf(fptr, "\n\n");
    }

    fclose(fptr);
}

unsigned int getnumVote(char* record){
    int commacount= 0;
    int index = 0;
    char temp[8];
    unsigned int numVote;
    for(int i =0 ; i<TOBEINSERTEDSIZE ; i++){
        if(commacount == 3){
            temp[index] = '\0';
            break;
        }
        if(commacount == 2){
            temp[index] = record[i];
            index++;
        }
        
        if(record[i] == ','){
            commacount++;
        }
    }
    
    numVote = atoi(temp);

    return numVote;
}