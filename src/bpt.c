#include "bpt.h"
#include <math.h>
#include "group.h"
#include "misc.h"
#include "queue.h"

void populateLeaf(unsigned int* numVotesArr,
                  group** blkAddArr,
                  node* leaf,
                  int noKeys) {
    for (int i = 0; i < noKeys; i++) {
        leaf->keys[i] = *(numVotesArr + i);
        leaf->pointers[i] = *(blkAddArr + i);
        leaf->size++;
    }
}

void updateParentKeys(node* parent) {
    if (parent == NULL)
        return;

    node* finder = NULL;
    for (int i = 0; i < parent->size; i++) {
        finder = parent->pointers[i + 1];
        while (!finder->isLeaf) {
            finder = finder->pointers[0];
        }
        parent->keys[i] = finder->keys[0];
    }
    updateParentKeys(parent->parent);
}

void updateChildrenParent(node* parent) {
    node* child = NULL;
    for (int i = 0; i <= parent->size; i++) {
        child = parent->pointers[i];
        child->parent = parent;
    }
}

int findIdxToInsert(node* parent, int key) {
    int p;
    for (int i = 0; i < parent->size; i++) {
        if (key < parent->keys[i]) {
            p = i;
            break;
        } else
            p = i + 1;
    }
    return p;
}

void shiftKeysInLeaf(node* parent, int p) {
    for (int i = parent->size; i > p; i--) {
        parent->keys[i] = parent->keys[i - 1];
        parent->pointers[i] = parent->pointers[i - 1];
    }
}

void shiftKeysInNonLeaf(node* parent, int p) {
    for (int i = parent->size; i > p; i--) {
        parent->keys[i] = parent->keys[i - 1];
        parent->pointers[i + 1] = parent->pointers[i];
    }
}

void splitParent(node* parent1, node* parent2, node* childRight) {
    int mid = parent1->size / 2;

    if (parent1->size % 2 == 1)
        mid = (parent1->size / 2) + 1;

    int j = 0;
    for (int i = mid + 1; i < KEYS; i++) {
        parent2->keys[j] = parent1->keys[i];
        parent2->pointers[j] = parent1->pointers[i];
        parent1->pointers[i] = NULL;
        j++;
        parent2->size++;
        parent1->size--;
    }
    parent1->size--;
    parent2->pointers[parent2->size] = parent1->pointers[KEYS];
    parent1->pointers[KEYS] = NULL;

    int findloc = 0;
    if (childRight->keys[0] > parent2->keys[0]) {
        findloc = findIdxToInsert(parent2, childRight->keys[0]);
        shiftKeysInNonLeaf(parent2, findloc);

        parent2->keys[findloc] = childRight->keys[0];
        parent2->pointers[findloc + 1] = childRight;
        parent2->size++;
    } else {
        findloc = findIdxToInsert(parent1, childRight->keys[0]);
        shiftKeysInNonLeaf(parent1, findloc);

        parent1->keys[findloc] = childRight->keys[0];
        parent1->pointers[findloc + 1] = childRight;
        parent1->size++;
    }
    updateChildrenParent(parent2);
}

void insertParent(node* parent,
                  unsigned int key,
                  node* childLeft,
                  node* childRight) {
    int i = 0;
    while (i < parent->size) {
        if (key < parent->keys[i]) {
            break;
        }
        i++;
    }

    shiftKeysInNonLeaf(parent, i);

    parent->keys[i] = key;
    parent->pointers[i] = childLeft;
    parent->pointers[i + 1] = childRight;
    parent->size += 1;
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

    //  case 1 parent exist and can insert
    if (parent->size < KEYS) {
        insertParent(parent, smallestRight, childLeft, childRight);
        updateParentKeys(parent);
        return parent;
    }

    // case 2 parent exist but no space left split the parent node into 2 and
    // call updateParent
    if (parent->size + 1 > KEYS) {
        node* coparent = (node*)malloc(sizeof(node));
        coparent->isLeaf = false;
        coparent->parent = NULL;
        coparent->size = 0;
        splitParent(parent, coparent, childRight);
        // coparent->pointers[coparent->size] = childRight;
        updateParentKeys(parent);
        updateParentKeys(coparent);
        node* grandparent =
            updateParent(parent->parent, parent, coparent, coparent->keys[0]);
        updateParentKeys(grandparent);
        parent->parent = grandparent;
        coparent->parent = grandparent;

        return coparent;
    }
    return ((void*)0);
}

node* searchLeafNode(unsigned int key, node* root) {
    int i = 0;
    node* current = root;
    while (!current->isLeaf) {
        i = 0;
        while (i < current->size) {
            if (key < current->keys[i]) {
                break;
            }
            i++;
        }
        current = (node*)current->pointers[i];
    }

    return current;
}

node* searchLeafNodeNoIndex(unsigned int key, node* root, int* noIndex) {
    int i = 0;
    *noIndex = 1;
    node* current = root;
    while (!current->isLeaf) {
        i = 0;
        while (i < current->size) {
            if (key < current->keys[i]) {
                break;
            }
            i++;
        }
        current = (node*)current->pointers[i];
        *noIndex += 1;
    }

    return current;
}

void findNumVotes(unsigned int key, node* root) {
    int i = 0, noBlocks = 0;
    double totalRate = 0, count = 0;
    printf("keys to search: %d\n", key);
    node* leaf = searchLeafNodeNoIndex(key, root, &noBlocks);
    printf("Number of index nodes accessed: %d\n", noBlocks);

    while (i < leaf->size) {
        if (key == leaf->keys[i])
            break;
        i++;
    }

    printf("current leaf: %d\n", leaf->keys[i]);

    printGroup(leaf->pointers[i], &noBlocks, &totalRate, &count);

    printf("%.2f / %.2f\n", totalRate, count);
    printf("Number of data blocks accessed: %d\n", noBlocks);
    printf("Average of averageRating: %.2f\n", totalRate / count);
}

void findRangeNumVotes(unsigned int min, unsigned int max, node* root) {
    int i = 0, noBlocks = 0;
    double totalRate = 0, count = 0;
    node* leaf = searchLeafNodeNoIndex(min, root, &noBlocks);
    printf("Number of index nodes accessed: %d\n", noBlocks);

    while (i < leaf->size) {
        if (leaf->keys[i] >= min && leaf->keys[i] <= max) {
            printGroup(leaf->pointers[i], &noBlocks, &totalRate, &count);
        }
        i++;
        if (i == leaf->size) {
            if (!leaf->pointers[KEYS]) {
                break;
            }
            leaf = (node*)leaf->pointers[KEYS];
            i = 0;
        }
        if (leaf->keys[i] > max) {
            break;
        }
    }

    printf("Number of data blocks accessed: %d\n", noBlocks);
    printf("Average of averageRating: %.2f\n", totalRate / count);
}

void insertToGroup(group* keygroup, char* addr) {
    group* current = keygroup;

    while (current->next != NULL) {
        current = current->next;
    }

    current->pointers[current->size] = addr;
    current->size++;

    if (current->size == GROUP_LEN) {
        group* newgroup = (group*)malloc(sizeof(group));
        newgroup->key = current->key;
        newgroup->size = 0;
        newgroup->next = NULL;
        current->next = newgroup;
    }
}

void insertToLeaf(node* leaf, unsigned int key, char* addr) {
    int i = 0;

    // looking for place to insert
    while (i < leaf->size) {
        if (key == leaf->keys[i]) {
            group* current = (group*)leaf->pointers[i];
            insertToGroup((group*)leaf->pointers[i], addr);
            return;
        }

        if (key < leaf->keys[i])
            break;

        i++;
    }

    shiftKeysInLeaf(leaf, i);

    group* newgroup = (group*)malloc(sizeof(group));
    newgroup->key = key;
    newgroup->pointers[0] = addr;
    newgroup->size = 1;
    newgroup->next = NULL;

    leaf->keys[i] = key;
    leaf->pointers[i] = newgroup;
    leaf->size += 1;

    updateParentKeys(leaf->parent);
}

bool existInLeaf(node* leaf, unsigned int key) {
    for (int i = 0; i < leaf->size; i++) {
        if (key == leaf->keys[i]) {
            return true;
        }
    }
    return false;
}

void insertToSplitLeaf(node* left, node* right, unsigned int key, char* addr) {
    int mid = left->size / 2;

    if (left->size % 2 == 1)
        mid = (left->size / 2) + 1;

    int j = 0;
    for (int i = mid; i < KEYS; i++) {
        right->keys[j] = left->keys[i];
        right->pointers[j] = left->pointers[i];
        left->pointers[i] = NULL;
        j++;
        right->size++;
        left->size--;
    }

    int findloc = 0;
    group* newgroup = (group*)malloc(sizeof(group));
    newgroup->key = key;
    newgroup->pointers[0] = addr;
    newgroup->size = 1;
    newgroup->next = NULL;

    if (key > right->keys[0]) {
        findloc = findIdxToInsert(right, key);
        shiftKeysInLeaf(right, findloc);

        right->keys[findloc] = key;
        right->pointers[findloc] = newgroup;
        right->size++;
    } else {
        findloc = findIdxToInsert(left, key);
        shiftKeysInLeaf(left, findloc);

        left->keys[findloc] = key;
        left->pointers[findloc] = newgroup;
        left->size++;
    }

    node* parent = updateParent(left->parent, left, right, right->keys[0]);

    left->parent = parent;
    right->parent = parent;

    right->pointers[KEYS] = left->pointers[KEYS];
    left->pointers[KEYS] = right;
}

node* insertbpt(node* root, unsigned int key, char* addr) {
    if (root == NULL) {
        root = (node*)malloc(sizeof(node));
        root->keys[0] = key;
        root->isLeaf = true;
        root->size = 1;
        root->parent = NULL;

        group* newgroup = (group*)malloc(sizeof(group));
        newgroup->key = key;
        newgroup->pointers[0] = addr;
        newgroup->size = 1;
        newgroup->next = NULL;

        root->pointers[0] = newgroup;

        return root;
    }

    node* leaf = searchLeafNode(key, root);
    if (leaf->size == KEYS && !existInLeaf(leaf, key)) {
        node* newleaf = (node*)malloc(sizeof(node));
        newleaf->isLeaf = true;
        newleaf->size = 0;
        newleaf->parent = NULL;
        insertToSplitLeaf(leaf, newleaf, key, addr);
    } else {
        insertToLeaf(leaf, key, addr);
    }
    // find root to return
    while (root->parent != NULL) {
        root = root->parent;
    }
    return root;
}
