#ifndef MISC_H_
#define MISC_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bpt.h"
#include "group.h"
#include "queue.h"

void printbpt(node* root);
void saveToFile(node* root);
void printGroup(group* cur, int* blocksAcc, double* totalRate, double* count);
void printBPTStats(node* root);
unsigned int getnumVote(char* record);

#endif