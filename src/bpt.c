#include "bpt.h"
#include <math.h>
#include "queue.h"

void printbpt(node* root);
void insertIntoSortedKeys(node* root, int key, char* record);

void printRecordHere(char* record) {
    for (int i = 0; i < 14; i++) {
        printf("%c", *(record + i));
        if (i == 8)
            printf(" ");
    }
    printf("\n");
}

node* findRootNode(node* root) {
    while (root->parent != NULL)
        root = root->parent;
    return root;
}

int buildbpt(unsigned int* numVotes, char** blkAddr) {
    printf("build bpt\n");
    node* root = (node*)malloc(sizeof(node));
    // newRoot need to change for searching purposes for subsequent insertion
    for (int i = 0; i < 7; i++) {
        printf("%d ", numVotes[i]);
        printRecordHere(blkAddr[i]);
        insertbpt(numVotes[i], blkAddr[i], root);
    }

    printbpt(findRootNode(root));

    return 1;
}

void insertToParent(node* childNode, node* nextChildNode, unsigned int key) {
    node* parentNode;

    if (childNode->parent == NULL) {
        printf("Creating a new parent node\n");
        parentNode = (node*)malloc(sizeof(node));

        nextChildNode->parent = parentNode;
        childNode->parent = parentNode;

        parentNode->isLeaf = false;

        parentNode->keys[0] = key;
        parentNode->size++;
        parentNode->pointers[0] = childNode;
        parentNode->pointers[1] = nextChildNode;
    } else {
        parentNode = childNode->parent;
        nextChildNode->parent = parentNode;
        printf("%d\n", key);
        printf("parent: %d\n", parentNode->size);

        // need to add sorting here
        int lastIdx = parentNode->size;
        parentNode->keys[lastIdx] = key;
        parentNode->size++;
        parentNode->pointers[lastIdx] = childNode;
        parentNode->pointers[lastIdx + 1] = nextChildNode;
    }
}

void insertbpt(int key, char* record, node* root) {
    printf("size in insert: %d\n", root->size);
    if (root->size + 1 > KEYS && root->pointers[KEYS] == NULL) {
        printf("Creating new node\n");
        node* nextNode = (node*)malloc(sizeof(node));
        int mid = root->size / 2;
        int nextNodePtr = 0;
        int size = root->size, i;
        bool inserted = false;
        printf("mid: %d\n", mid);
        for (i = mid + 1; i < size; i++) {
            printf("Transferring values over\n");
            if (key < root->keys[i] && !inserted) {
                nextNode->keys[nextNodePtr] = key;
                nextNode->pointers[nextNodePtr] = record;
                nextNode->size++;
                nextNodePtr++;
                inserted = true;
            }

            nextNode->keys[nextNodePtr] = root->keys[i];
            nextNode->pointers[nextNodePtr] = root->pointers[i];
            nextNode->size++;
            printf("%d\n", nextNode->keys[nextNodePtr]);
            nextNodePtr++;

            root->keys[i] = 0;
            root->pointers[i] = NULL;
            root->size--;
        }
        if (!inserted) {
            printf("key: %d\n", key);
            nextNode->keys[nextNodePtr] = key;
            nextNode->pointers[nextNodePtr] = record;
            nextNode->size++;
        }

        root->pointers[i] = nextNode;

        insertToParent(root, nextNode, nextNode->keys[0]);
    } else if (root->pointers[KEYS] != NULL) {
        printf("Going to another node\n");
        node* nextNode = root->pointers[KEYS];
        insertbpt(key, record, nextNode);
    } else {
        // need to add sorting here
        int lastIdx = root->size;
        if (key < root->keys[lastIdx - 1]) {
            insertIntoSortedKeys(root, key, record);
        } else {
            printf("Inserting %d at the end of node\n", key);
            root->keys[lastIdx] = key;
            root->pointers[lastIdx] = record;
        }
        root->size++;
        root->isLeaf = true;  // need to check if leaf

        // for (int i = 0; i < root->size; i++)
        //     printf("% 5d", root->keys[i]);
        // printf("\n");
        // for (int i = 0; i < root->size; i++)
        //     printRecordHere(root->pointers[i]);
        // printf("\n");
    }
}

// need to modify parentssssss
void modifyParent(node* root, int key, int oldKey) {
    node* parent = root->parent;
    printf("firstIdx: %d \n", parent->keys[0]);
}

void insertIntoSortedKeys(node* root, int key, char* record) {
    printf("SORTING...\n");
    int p = 0, i;
    int oldKey = root->keys[0];
    for (i = 0; i < root->size; i++) {
        if (key < root->keys[i]) {
            p = i;
            break;
        } else
            p = i + 1;
    }
    for (i = root->size; i > p; i--) {
        root->keys[i] = root->keys[i - 1];
        root->pointers[i] = root->pointers[i - 1];
    }

    root->keys[p] = key;
    root->pointers[p] = record;

    if (p == 0)
        modifyParent(root, root->keys[0], oldKey);
}

void printbpt(node* root) {
    node* ptr = root;
    printf("------Printing BPT------\n");
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
                printf("%d ", ptr->keys[i]);
            }
            printf("| ");
        }
        printf("\n");
    }
}
