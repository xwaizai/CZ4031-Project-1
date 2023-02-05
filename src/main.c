#include <main.h>

memBlock* fillmemBlock(char* input, int length,memBlock* current){
    /*Case 1 when memory block is not full and input length is within the BLOCKSIZE*/
    if(!current->isFull && current->blockIndex+length <= BLOCKSIZE){
        memmove(current->block+current->blockIndex, input, length);
        current->blockIndex+=length;
        if(current->blockIndex>=BLOCKSIZE){
            current->isFull = true;
        }
    /*Case 2 when memory block is full or input length exceeds the BLOCKSIZE*/
    }else{
        memBlock* new = (memBlock*)malloc(sizeof(memBlock));
        new->next = current;
        current = new;
        memmove(current->block, input, length);
        current->blockIndex+=length;
    }
    return current;
}

memBlock* readFromFile(memBlock* current){
    FILE *fp;
    char line[60];

    const char d[2] = "\t";
    const char newline[2] = "\n"; 

    char* token;
    int tokenlen;
    
    char tobeinserted[20];
    memset(tobeinserted, 0, sizeof(tobeinserted));

    int index = 0;
    int initial = 0;

    /* opening file for reading */
    fp = fopen("data.tsv" , "r");
    if(fp == NULL) {
        perror("Error opening file");
        return current;
    }
    while( fgets (line, 60, fp)!=NULL ) {
        if(initial == 0){
            initial = 1;
        }else{
            /* removing newline */
            token = strtok(line,newline);
            /* tokenizing based on \t*/
            token = strtok(token,d);
            while( token != NULL ) {
                memmove(tobeinserted+index,token,strlen(token));
                index+=strlen(token);
                tokenlen= strlen(token);

                token = strtok(NULL, d);
            }
            /*check if rating is below 10*/
            if(tobeinserted[10] == '.'){
                /*shifting the array*/
                for(int i = 20;i>11;i--){
                    tobeinserted[i]=tobeinserted[i-1];
                }
                /*adding 0 if it is not 10.0 eg. 5.5 becomes 5.50*/
                tobeinserted[12]='0';
            }
            
            /*Insert to memory block*/
            current = fillmemBlock(tobeinserted, sizeof(tobeinserted),current);

            /*reset the tobeinserted array*/
            memset(tobeinserted, 0, sizeof(tobeinserted));

            index = 0;
        }
        
    }
    fclose(fp);
    return current;
}

/*Solely for debuging purposes*/
void printRecord(char * record){
    for(int i = 0 ; i < 20; i++){
        printf("%c ",*(record+i));
    }
    printf("\n");
}

// function to swap elements
void swap(unsigned int *a, unsigned int *b) {
  unsigned int t = *a;
  *a = *b;
  *b = t;
}
void swapAddr(char** a, char** b){
    char* t = *a;
    *a = *b;
    *b = t;
}

// function to find the partition position
unsigned int partition(unsigned int array[], char** blockaddr,unsigned int low, unsigned int high) {
  
  // select the rightmost element as pivot
  unsigned int pivot = array[high];
  
  // pointer for greater element
  unsigned int i = (low - 1);

  // traverse each element of the array
  // compare them with the pivot
  for (unsigned int j = low; j < high; j++) {
    if (array[j] <= pivot) {
        
      // if element smaller than pivot is found
      // swap it with the greater element pointed by i
      i++;
      
      // swap element at i with element at j
      swap(&array[i], &array[j]);
      swapAddr(&blockaddr[i],&blockaddr[j]);
    }
  }

  // swap the pivot element with the greater element at i
  swap(&array[i + 1], &array[high]);
  swapAddr(&blockaddr[i + 1], &blockaddr[high]);
  
  // return the partition point
  return (i + 1);
}

void quickSort(unsigned int array[],char** blockaddr,unsigned int low, unsigned int high) {
  if (low < high) {
    
    // find the pivot element such that
    // elements smaller than pivot are on left of pivot
    // elements greater than pivot are on right of pivot
    unsigned int pi = partition(array,blockaddr, low, high);
    
    // recursive call on the left of pivot
    quickSort(array,blockaddr, low, pi - 1);
    
    // recursive call on the right of pivot
    quickSort(array,blockaddr, pi + 1, high);
  }
}

int main()
{
    printf("\tStart Of Program \t\n");

    memBlock* head = (memBlock*)malloc(sizeof(memBlock));

    head->blockIndex = 0;
    head->isFull = false;
    head->next = NULL;

    head = readFromFile(head);

    int blocks = 0;
    int j=0;
    int numRec = 0;
    memBlock* current = head;
    while (current)
    {
        blocks++;
        /*for(int i =0 ; i < 200 ;i++){
            printf("%c", current->block[i]);
        }
        printf("\n");*/
        current = current->next;
    }
    while(j<200){
        if(head->block[j]=='t'){
            numRec++;
        }
        j+=20;
    }

    printf("Experiment 1\n");
    printf("Number of records: %d\n",10*(blocks-1)+numRec);
    printf("Size of records: 20\n");
    printf("Number of records stored in a block: %d\n",BLOCKSIZE/20);
    printf("Number of blocks: %d\n",blocks);

    printf("size of unsigned int %d\n",sizeof(unsigned int));

    /*store number of records*/
    unsigned int noOfRec = 10*(blocks-1)+numRec;

    /*initialise numVotes (key) array*/
    unsigned int *numVotes = (int *) malloc(noOfRec*sizeof(int *));

    unsigned int blockaddrIndex = 0;

    /*initialize blockaddr array*/
    char **blockaddr = (char**)malloc(noOfRec*sizeof(char**));

    current = head;
    unsigned int numDigits = 0;
    unsigned int numVote = 0;

    /*looping through all the memBlocks to retrieve key and address of key for indexing*/
    while (current)
    {
        for(int i =0 ; i < 200 ;i+=20){
            if(current->block[i]=='t'){
                numVote = atoi(&(current->block[13+i]));
                printf("numVote: %d\n",numVote);
                numVotes[blockaddrIndex]=numVote;
                blockaddr[blockaddrIndex]=&(current->block[i]);

                printf("%d,%c\n",numVotes[blockaddrIndex],*(blockaddr[blockaddrIndex]));
                printRecord((blockaddr[blockaddrIndex]));
                blockaddrIndex++;
            }
            
        }
        current = current->next;
    }
    printf("%d\n",noOfRec);

    /*using quicksort algo to sort the arrays*/
    quickSort(numVotes,blockaddr,0,noOfRec-1);

    for(int i = 0;i<1000;i++){
        printf("%d ",numVotes[i]);
        printRecord(blockaddr[i]);
    }
    
    return 0;
}