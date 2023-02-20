#include "deleteBpt.h"

void deleteNodeLeaf(node* leafNode, unsigned int deleteKey) {

    // CASE 1: Easy Case - don't need to merge or borrow sibling
    if(leafNode->size > 1) {
        int i;
        for(i=0; i< leafNode->size; i++) {
            if(leafNode->keys[i] == deleteKey)
                break;
        }

        leafNode->keys[i] = NULL;
        free(leafNode->pointers[i]);
    }

    // CASE 2: Possible to borrow from sibling (left then right)

    // CASE 3: NOT possible to borrow from sibling - Merge

}