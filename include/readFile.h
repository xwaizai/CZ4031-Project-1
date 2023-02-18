#ifndef READFILE_H_
#define READFILE_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memBlock.h"

#define TOBEINSERTEDSIZE 24

memBlock* readFromFile(memBlock* current);

#endif