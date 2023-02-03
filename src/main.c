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
            /*shifting the array*/
            for(int i = 20;i>11;i--){
                tobeinserted[i]=tobeinserted[i-1];
            }
            /*setting header for number of digits of numVotes*/
            tobeinserted[12]=tokenlen;
            
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
        printf("%c",*(record+i));
    }
    printf("\n");
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

    unsigned int noOfRec = 10*(blocks-1)+numRec;

    unsigned int numVotes[noOfRec];

    unsigned int blockaddrIndex = 0;
    char **blockaddr = (char**)malloc(noOfRec*sizeof(char**));

    current = head;
    unsigned int numDigits = 0;
    unsigned int numVote = 0;

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
    
    return 0;
}