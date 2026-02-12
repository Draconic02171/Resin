#include <stdio.h>
void CleanSource(FILE *sourceFile) {

    char buffer[256];
    int line = 0;

    while (fgets(buffer, sizeof(buffer), sourceFile)) {
        printf("%s" , buffer);
    }

    return;
}