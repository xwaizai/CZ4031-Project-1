#include "bpt.h"
#include <math.h>
#include "queue.h"
#include "group.h"

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
    if(parent == NULL){
        return;
    }
    //printf("Parent size %d Parent start %d\n",parent->size, parent->keys[0]);
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

void updateChildrenParent(node* parent){
    
    node* child = NULL;
    for (int i = 0; i <= parent->size; i++)
    {
        child = parent->pointers[i];
        child->parent = parent;
    }
    
    
}

void splitParent(node* parent1, node* parent2, node* childRight) {
    int mid = parent1->size / 2;

    if (parent1->size % 2 == 1)
        mid = (parent1->size / 2) + 1;

    int j = 0;
    for (int i = mid+1; i < KEYS; i++) {
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
    if(childRight->keys[0]>parent2->keys[0]){
        for (int i = 0; i < parent2->size; i++)
        {
            if(childRight->keys[0]<parent2->keys[i]){
                findloc = i;
                break;
            }else{
                findloc = i + 1;
            }
        }
        for(int i = parent2->size; i>findloc ; i--){
            parent2->keys[i] = parent2->keys[i-1];
            parent2->pointers[i+1] = parent2->pointers[i];
        }
        parent2->keys[findloc] = childRight->keys[0];
        parent2->pointers[findloc+1] = childRight;
        parent2->size++;
    }else{
        for (int i = 0; i < parent1->size; i++)
        {
            if(childRight->keys[0]<parent1->keys[i]){
                findloc = i;
                break;
            }else{
                findloc = i + 1;
            }
        }
        for(int i = parent1->size; i>findloc ; i--){
            parent1->keys[i] = parent1->keys[i-1];
            parent1->pointers[i+1] = parent1->pointers[i];
        }
        parent1->keys[findloc] = childRight->keys[0];
        parent1->pointers[findloc+1] = childRight;
        parent1->size++;
    }
    
    updateChildrenParent(parent2);

    
}

void insertParent(node* parent,unsigned int key,node* childLeft, node* childRight){
    int i = 0;
    while(i< parent->size){
        if(key<parent->keys[i]){
            break;
        }
        i++;
    }

    //shifting the array
    for(int j = parent->size; j>i ; j--){
        parent->keys[j] = parent->keys[j-1];
        parent->pointers[j+1] = parent->pointers[j];
    }

    parent->keys[i] = key;
    parent->pointers[i] = childLeft;
    parent->pointers[i+1] = childRight;
    parent->size+=1;
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
    //printf("Parent size %d Parent start %d\n",parent->size,parent->keys[0]);
    // case 1 parent exist and can insert
    if (parent->size < KEYS) {
        /*parent->keys[parent->size] = smallestRight;
        parent->pointers[parent->size] = childLeft;
        parent->pointers[parent->size + 1] = childRight;
        parent->size += 1;*/
        insertParent(parent,smallestRight,childLeft, childRight);
        return parent;
    }

    // case 2 parent exist but no space left split the parent node into 2 and
    // call updateParent
    if (parent->size + 1 > KEYS) {
        node* coparent = (node*)malloc(sizeof(node));
        coparent->isLeaf = false;
        coparent->parent = NULL;
        coparent->size = 0;
        splitParent(parent, coparent,childRight);
        //coparent->pointers[coparent->size] = childRight;
        updateParentKeys(parent);
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
                  group** blkAddArr,
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
            updateParentKeys(prev->parent);
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
        if (i == 0 && !ptr->isLeaf)
            queueInsert(ptr->pointers[i]);

        if(!ptr->isLeaf)
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

node* searchleafnode(unsigned int key, node* root){
    int i = 0;
    node* current = root;
    while(!current->isLeaf){
        i = 0;
        while(i<current->size){
            if(key<current->keys[i]){
                break;
            }
            i++;
        }
        current = (node*) current->pointers[i];
    }
    
    return current; 
}

void insertToGroup(group* keygroup, char* addr){
    group* current = keygroup;

    

    while(current->next!=NULL){
        current = current->next;
        
        
    }
    
    current->pointers[current->size] = addr;
    current->size+=1;

    if(current->size == GROUP_LEN){
        group* newgroup = (group*) malloc(sizeof(group));
        newgroup->key = current->key;
        newgroup->size = 0;
        newgroup->next = NULL;
        current->next = newgroup;
    }
    
}



void insertToLeaf(node* leaf,unsigned int key, char* addr){
    int i = 0;

    //looking for place to insert
    while(i< leaf->size){
        if(key == leaf->keys[i]){
            group* current = (group*)leaf->pointers[i];
            //printf("key %d\n",key);
            //printf("%d \n",current->key);
            //printf("%d \n",current->size);
            insertToGroup((group*)leaf->pointers[i],addr);
            return;
        }

        if(key<leaf->keys[i]){
            break;
        }
        i++;
    }

    //shifting the array
    for(int j = leaf->size; j>i ; j--){
        leaf->keys[j] = leaf->keys[j-1];
        leaf->pointers[j] = leaf->pointers[j-1];
    }

    group* newgroup = (group*) malloc(sizeof(group));
    newgroup->key = key;
    newgroup->pointers[0] = addr;
    newgroup->size = 1;
    newgroup->next = NULL;

    leaf->keys[i] = key;
    leaf->pointers[i] = newgroup;
    leaf->size+=1;

    updateParentKeys(leaf->parent);
}

bool existInLeaf(node* leaf, unsigned int key){
    for(int i = 0; i<leaf->size;i++){
        if (key == leaf->keys[i])
        {
            return true;
        }
        
    }
    return false;
}

void insertToSplitLeaf(node* left, node* right, unsigned int key, char* addr){
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
    group* newgroup = (group*) malloc(sizeof(group));
    newgroup->key = key;
    newgroup->pointers[0] = addr;
    newgroup->size = 1;
    newgroup->next = NULL;
    if(key>right->keys[0]){
        for (int i = 0; i < right->size; i++)
        {
            if(key<right->keys[i]){
                findloc = i;
                break;
            }else{
                findloc = i + 1;
            }
        }
        for(int i = right->size; i>findloc ; i--){
            right->keys[i] = right->keys[i-1];
            right->pointers[i] = right->pointers[i-1];
        }
        right->keys[findloc] = key;
        right->pointers[findloc] = newgroup;
        right->size++;
    }else{
        for (int i = 0; i < left->size; i++)
        {
            if(key<left->keys[i]){
                findloc = i;
                break;
            }else{
                findloc = i + 1;
            }
        }
        for(int i = left->size; i>findloc ; i--){
            left->keys[i] = left->keys[i-1];
            left->pointers[i] = left->pointers[i-1];
        }
        left->keys[findloc] = key;
        left->pointers[findloc] = newgroup;
        left->size++;
    }
    

    
    // unsigned int tempkeys[KEYS+1];
    // group* temppointers[KEYS+1];

    // int findloc = 0;

    // for (int i = 0; i < KEYS; i++)
    // {
    //     tempkeys[i] = left->keys[i];
    //     temppointers[i] = left->pointers[i];
    // }

    // for (int i = 0; i < KEYS; i++)
    // {
    //     if(key<tempkeys[i]){
    //         findloc = i;
    //         break;
    //     }
    // }
    // for(int i = KEYS; i>findloc ; i--){
    //     tempkeys[i] = tempkeys[i-1];
    //     temppointers[i] = temppointers[i-1];
    // }

    // tempkeys[findloc] = key;

    // group* newgroup = (group*) malloc(sizeof(group));
    // newgroup->key = key;
    // newgroup->pointers[0] = addr;
    // newgroup->size = 1;
    // newgroup->next = NULL;
    // temppointers[findloc] = newgroup;

    // int mid = left->size / 2;
    // int j = 0;
    // int k = 0;
    // for (int i = 0; i < KEYS+1; i++) {
    //     if(i<mid){
    //         left->keys[j] = tempkeys[i];
    //         left->pointers[j] = temppointers[i];
    //         j++;
    //     }else{
    //         right->keys[k] = tempkeys[i];
    //         right->pointers[k] = temppointers[i];
    //         k++;
    //     }
        
    // }

    // right->size=mid+1;
    // left->size=mid;

    // for (int i = mid; i < KEYS; i++)
    // {
    //     left->keys[i] = 0;
    //     left->pointers[i] = NULL;
    // }

    // for (int i = mid+1; i < KEYS; i++)
    // {
    //     right->keys[i] = 0;
    //     right->pointers[i] = NULL;
    // }
    

    node* parent = updateParent(left->parent,left,right,right->keys[0]);

    left->parent = parent;
    right->parent = parent;

    left->pointers[KEYS] = right;

    

}

node* insertbpt(node* root, unsigned int key, char* addr){

    if(root == NULL){
        root = (node*) malloc(sizeof(node));
        root->keys[0] = key;
        root->isLeaf = true;
        root->size = 1;
        root->parent = NULL;

        
        group* newgroup = (group*) malloc(sizeof(group));
        newgroup->key = key;
        newgroup->pointers[0] = addr;
        newgroup->size = 1;
        newgroup->next = NULL;

        root->pointers[0] = newgroup;

        return root;
    }

    node* leaf = searchleafnode(key,root);
    if(leaf->size == KEYS && !existInLeaf(leaf,key)){
        node* newleaf = (node*) malloc(sizeof(node));
        newleaf->isLeaf = true;
        newleaf->size = 0;
        newleaf->parent = NULL;
        insertToSplitLeaf(leaf, newleaf, key, addr);
    }else{
        insertToLeaf(leaf,key,addr);
    }
    //find root to return
    while (root->parent != NULL) {
        root = root->parent;
    }
    return root;
    
}
