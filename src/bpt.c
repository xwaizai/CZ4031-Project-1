#include "bpt.h"

void printRecordHere(char* record) {
    for (int i = 0; i < 14; i++) {
        printf("%c", *(record + i));
        if (i == 8)
            printf(" ");
    }
    printf("\n");
}

int buildbpt(unsigned int* numVotes, char** blkAddr) {
    printf("build bpt\n");
    node* root = (node*)malloc(sizeof(node));
    printf("%d\n", sizeof(root));
    // for (int i = 0; i < 4; i++) {
    //     printf("%d ", numVotes[i]);
    //     printRecordHere(blkAddr[i]);
    //     insertbpt(numVotes[i], blkAddr[i], root);
    // }
    // for (int i = 0; i < 5; i++) {
    //     char* ptr = root->pointers[i];
    //     printRecordHere(ptr);
    // }
}

void insertbpt(int key, char* record, node* root) {
    printf("%d\n", root->size);
    if (root->size == NULL)
        root->size = 0;
    // if (root->size >= 5) {
    //     printf("here\n");
    //     // node* nextNode = (node*)malloc(sizeof(node));
    //     int mid = (root->size / 2) - 1;
    //     int nextNodePtr = 0;
    //     int size = root->size, i;
    //     // for (i = mid; i < size; i++) {
    //     //     nextNode->keys[nextNodePtr++] = root->keys[i];
    //     //     nextNode->pointers[nextNodePtr++] = root->pointers[i];
    //     //     root->keys[i] = -1;
    //     //     root->pointers[i] = -1;
    //     //     root->size--;
    //     // }
    //     // root->pointers[i] = nextNode;
    // } else {
    // int lastIdx = root->size;
    // root->keys[lastIdx] = key;
    // root->pointers[lastIdx] = record;
    // root->size++;
    // root->isLeaf = true;
    // }
}
