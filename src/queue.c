#include "queue.h"

void* queue[MAX];
int front = 0;
int rear = -1;
int itemCnt = 0;

bool queueIsEmpty() {
    return itemCnt == 0;
}

bool queueIsFull() {
    return itemCnt == MAX;
}

int queueSize() {
    return itemCnt;
}

void queueInsert(void* data) {
    if (!queueIsFull()) {
        if (rear == MAX - 1) {
            rear = -1;
        }

        queue[++rear] = data;
        itemCnt++;
    }
}

void* queuePop() {
    void* data = queue[front++];

    if (front == MAX) {
        front = 0;
    }

    itemCnt--;
    return data;
}