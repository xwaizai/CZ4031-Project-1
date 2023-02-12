#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 99

bool queueIsEmpty();
bool queueIsFull();
int queueSize();
void queueInsert(void* data);
void* queuePop();