#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 999999

bool queueIsEmpty();
bool queueIsFull();
int queueSize();
void queueInsert(void* data);
void* queuePop();