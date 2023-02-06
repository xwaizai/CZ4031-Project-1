#include "readFile.h"

memBlock* readFromFile(memBlock* current) {
    FILE* fp;
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
    fp = fopen("data.tsv", "r");
    if (fp == NULL) {
        perror("Error opening file");
        return current;
    }
    while (fgets(line, 60, fp) != NULL) {
        if (initial == 0) {
            initial = 1;
        } else {
            /* removing newline */
            token = strtok(line, newline);
            /* tokenizing based on \t*/
            token = strtok(token, d);
            while (token != NULL) {
                memmove(tobeinserted + index, token, strlen(token));
                index += strlen(token);
                tokenlen = strlen(token);

                token = strtok(NULL, d);
            }
            /*check if rating is below 10*/
            if (tobeinserted[10] == '.') {
                /*shifting the array*/
                for (int i = 20; i > 11; i--) {
                    tobeinserted[i] = tobeinserted[i - 1];
                }
                /*adding 0 if it is not 10.0 eg. 5.5 becomes 5.50*/
                tobeinserted[12] = '0';
            }

            /*Insert to memory block*/
            current = fillmemBlock(tobeinserted, sizeof(tobeinserted), current);

            /*reset the tobeinserted array*/
            memset(tobeinserted, 0, sizeof(tobeinserted));

            index = 0;
        }
    }
    fclose(fp);
    return current;
}