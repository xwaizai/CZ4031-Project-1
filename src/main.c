#include <main.h>

memBlock* fillmemBlock(char* input, int length,memBlock* current){
    if(!current->isFull && current->blockIndex+length <= BLOCKSIZE){
        memmove(current->block+current->blockIndex, input, length);
        current->blockIndex+=length;
        if(current->blockIndex>=BLOCKSIZE){
            current->isFull = true;
        }
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
    memset(tobeinserted, 48, sizeof(tobeinserted));

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

            /*
            for(int i = 0;i<sizeof(tobeinserted);i++)
                printf("%c",tobeinserted[i]);
            printf("\n");
            printf("%d\n",tobeinserted[12]);*/
            
            current = fillmemBlock(tobeinserted, sizeof(tobeinserted),current);

            memset(tobeinserted, 48, sizeof(tobeinserted));

            index = 0;
        }
        
    }
    fclose(fp);
    return current;
}

int main()
{
    printf("\tStart Of Program \t\n");

    memBlock* head = (memBlock*)malloc(sizeof(memBlock));

    head->blockIndex = 0;
    head->isFull = false;
    head->next = NULL;

    head = readFromFile(head);

    
    /*
    char* test = "tt00000015.61645.000";

    for(int i = 0; i<11; i++){
        head = fillmemBlock(test, 20, head);
    }
*/
    memBlock* current = head;
    while (current)
    {
        
        for(int i =0 ; i < 200 ;i++){
            printf("%c", current->block[i]);
        }
        printf("\n");
        current = current->next;
    }
    
    return 0;
}