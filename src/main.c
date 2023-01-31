#include <main.h>

memBlock* fillmemBlock(char* input, int length,memBlock* current){
    if(!current->isFull){
        memmove(current->block+current->blockIndex,input,length);
        current->blockIndex+=length;
        if(current->blockIndex>=200){
            current->isFull = true;
        }
    }else{
        memBlock* new = (memBlock*)malloc(sizeof(memBlock));
        new->next = current;
        current = new;
        memmove(current->block,input,length);
        current->blockIndex+=length;
    }
    return current;
}

int main()
{
    printf("Hello World\n");

    memBlock* head = (memBlock*)malloc(sizeof(memBlock));

    head->blockIndex = 0;
    head->isFull = false;
    head->next = NULL;

    char* test = "tt00000015.61645.000";

    for(int i = 0; i<11;i++){
        head = fillmemBlock(test,20,head);
    }

    memBlock* current = head;
    while (current)
    {
        printf("%s\n",current->block);
        current = current->next;
    }
    
    return 0;
}