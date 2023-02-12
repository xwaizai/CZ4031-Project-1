#include "bpt.h"
#include <math.h>
#include "queue.h"

void printRecordHere(char* record) {
    for (int i = 0; i < 14; i++) {
        printf("%c", *(record + i));
        if (i == 8)
            printf(" ");
    }
    printf(" ");
    // printf("\n");
}

void populateLeaf(unsigned int* numVotesArr,
                  char** blkAddArr,
                  node* leaf,
                  int noKeys) {
    for (int i = 0; i < noKeys; i++) {
        leaf->keys[i] = *(numVotesArr + i);
        leaf->pointers[i] = *(blkAddArr + i);
        leaf->size++;
    }
}

void updateParentKeys(node* parent) {
    node* finder = NULL;
    for (int i = 0; i < parent->size; i++) {
        finder = parent->pointers[i + 1];
        while (!finder->isLeaf) {
            finder = finder->pointers[0];
        }
        parent->keys[i] = finder->keys[0];
    }
}

void splitParent(node* parent1, node* parent2) {
    int mid = parent1->size / 2;

    if (parent1->size % 2 == 1)
        mid = (parent1->size / 2) + 1;

    int j = 0;
    for (int i = mid; i < KEYS; i++) {
        parent2->keys[j] = parent1->keys[i];
        parent2->pointers[j] = parent1->pointers[i + 1];
        parent1->pointers[i + 1] = NULL;
        j++;
        parent2->size++;
        parent1->size--;
    }
}

node* updateParent(node* parent,
                   node* childLeft,
                   node* childRight,
                   int smallestRight) {
    // case 3 no parent, make parent
    if (parent == NULL) {
        parent = (node*)malloc(sizeof(node));
        parent->isLeaf = false;
        parent->parent = NULL;
        parent->size = 0;
    }

    // case 1 parent exist and can insert
    if (parent->size < KEYS) {
        parent->keys[parent->size] = smallestRight;
        parent->pointers[parent->size] = childLeft;
        parent->pointers[parent->size + 1] = childRight;
        parent->size += 1;
        return parent;
    }

    // case 2 parent exist but no space left split the parent node into 2 and
    // call updateParent
    if (parent->size + 1 > KEYS) {
        node* coparent = (node*)malloc(sizeof(node));
        coparent->isLeaf = false;
        coparent->parent = NULL;
        coparent->size = 0;
        splitParent(parent, coparent);
        coparent->pointers[coparent->size] = childRight;
        updateParentKeys(coparent);
        node* grandparent =
            updateParent(parent->parent, parent, coparent, coparent->keys[0]);
        parent->parent = grandparent;
        coparent->parent = grandparent;

        return coparent;
    }
    return ((void*)0);
}

node* bulkloadbpt(unsigned int* numVotesArr,
                  char** blkAddArr,
                  int numVotesLength,
                  node* root) {
    node* headLeaf = (node*)malloc(sizeof(node));
    headLeaf->isLeaf = true;
    headLeaf->size = 0;
    headLeaf->parent = NULL;
    node* tailLeaf = headLeaf;
    node* prev = NULL;

    int totalNumVotes = numVotesLength;
    int numLeaf = 0;
    int mid = (KEYS / 2) + 1;
    int keysToInsert = KEYS;
    int i = 0;

    while (i < numVotesLength) {
        // printf("%d\n", i);
        numLeaf++;
        // printf("%d : %d \n", totalNumVotes, totalNumVotes % KEYS);

        if ((totalNumVotes - 2 * KEYS) < mid && (totalNumVotes % KEYS) != 0)
            keysToInsert = mid;
        else
            keysToInsert = KEYS;

        if (numLeaf > 1) {
            prev = tailLeaf;
            tailLeaf = (node*)malloc(sizeof(node));
            tailLeaf->isLeaf = true;
            tailLeaf->size = 0;
            tailLeaf->parent = NULL;

            prev->pointers[keysToInsert] = tailLeaf;

            totalNumVotes -= keysToInsert;

            if (totalNumVotes - keysToInsert > 0) {
                populateLeaf(&numVotesArr[i], &blkAddArr[i], tailLeaf,
                             keysToInsert);
            } else {
                populateLeaf(&numVotesArr[i], &blkAddArr[i], tailLeaf,
                             totalNumVotes);
            }

            // updateParent
            root =
                updateParent(prev->parent, prev, tailLeaf, tailLeaf->keys[0]);
            prev->parent = root;
            tailLeaf->parent = root;
        } else {
            populateLeaf(&numVotesArr[i], &blkAddArr[i], tailLeaf, KEYS);
        }
        i += keysToInsert;
    }
    while (root->parent != NULL) {
        root = root->parent;
    }

    return root;
}

void printbpt(node* root) {
    printf("------Printing BPT------\n");

    node* ptr = root;
    for (int i = 0; i < ptr->size; i++) {
        printf("%d ", ptr->keys[i]);
        if (i == 0)
            queueInsert(ptr->pointers[i]);
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
