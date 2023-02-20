#include "deleteBpt.h"

#define MINKEYS KEYS / 2

void removeKeyFromNode(node* leafNode, unsigned int deleteKey) {
    int i, j;
    for(i=0; i< leafNode->size; i++)
        if(leafNode->keys[i] == deleteKey)
            break;
    
    for(j=i; j<leafNode->size ; j++) {
        leafNode->keys[j] = leafNode->keys[j+1];
        leafNode->pointers[j] = leafNode->pointers[j+1];
    }
    leafNode->keys[j] = NULL;
    leafNode->pointers[j] = NULL;
    leafNode->size--;
}

// node* findLeftSibling(node* curNode) {
//     node* parentNode = curNode->parent;
//     while(parentNode->parent != NULL) {
//         parent


//         parentNode = curNode->parent;
//     }
// }

void deleteMain(node* leafNode, unsigned int deleteKey) {

    // CASE 1: Easy Case - don't need to merge or borrow sibling
    if(leafNode->size > MINKEYS) {
        removeKeyFromNode(leafNode, deleteKey);
        // Need to update parent if is the first one
    }
    // CASE 2: Possible to borrow from sibling (left then right)
    // else if(leaf->size < MINKEYS) {
    //     nodeS
    //     removeKeyFromNode(leafNode, deleteKey);

    //     node* leftSibiling = findLeftSibling()
    // }


    // CASE 3: NOT possible to borrow from sibling - Merge

}
