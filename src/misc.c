#include "misc.h"
#include "group.h"
#include "bpt.h"
#include <stdio.h>
#include <string.h>

void printRecord(char* record) {
    for (int i = 0; i < 20; i++) {
        printf("%c", *(record + i));
        if (i == 8 || i == 12)
            printf(" ");
    }
    printf("\n");
}

void printGroup(group* cur, int* blocksAcc, double* totalRate, double* count) {
    char rate[5];

    do {
        for (int i = 0; i < cur->size; i++) {
            strncpy(rate, cur->pointers[i]+9, 4);
            rate[4] = '\0';
            *totalRate += strtod(rate, NULL);
            *count += 1;
        }
        cur = cur->next;
        *blocksAcc += 1;
    } while (cur);
}

void printBPTStats(node* root){
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

    printf("Parameter n: %d\n", KEYS);
    printf("Number of nodes of B+ Tree: %d\n", noNodes);
    printf("Number of levels: %d\n", noLevels);
    printf("Content of root node: ");
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

void saveToFile(node* root) {
    int num;
    FILE* fptr;

    // use appropriate location if you are using MacOS or Linux
    fptr = fopen("out.txt", "w");

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