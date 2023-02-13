#include "group.h"

unsigned int countUnique(unsigned int* array, unsigned int arraySize, unsigned int* unique) {
    int i, j, count = 0;
    for (i = 0; i < arraySize; i++) {
        unique[count++] = array[i];
        for (j = i + 1; j < arraySize; j++) {
            if (array[i] != array[j]) {
                i = j-1;
                break;
            }
        }
    }

    return count;
}

group** createGroups(unsigned int* numVotesArr,
                  char** blkAddArr,
                  int numVotesLength) {
    int i, j = 0;
    unsigned int* uniqueNumVotes = (int*)malloc(numVotesLength * sizeof(int));
    unsigned int countUniqueVotes = countUnique(numVotesArr, numVotesLength, uniqueNumVotes);
    group** groupVotes = (group**)malloc(countUniqueVotes * sizeof(group*));

    for (i=0; i< countUniqueVotes; i++) {
        group* voteGroup = (group*)malloc(sizeof(group));
        groupVotes[i] = voteGroup;
        voteGroup->key = uniqueNumVotes[i];
        voteGroup->size = 0;
        while(j < numVotesLength) {
            if (uniqueNumVotes[i] == numVotesArr[j]) {
                if (voteGroup->size < GROUP_LEN){
                    voteGroup->pointers[voteGroup->size] = *(blkAddArr + j);
                    voteGroup->size++;
                }else{
                    voteGroup->next = (group*)malloc(sizeof(group));
                    voteGroup = voteGroup->next;
                    voteGroup->key = uniqueNumVotes[i];
                    voteGroup->size = 0;
                    voteGroup->pointers[voteGroup->size] = *(blkAddArr + j);
                    voteGroup->size++;
                }
                j++;
            }else{
                break;
            }
        }
    }

    printf("No. of unique votes: %d\n", countUniqueVotes);
    return groupVotes;
}
