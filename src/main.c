#include <stdio.h>
#include "header/debugger.h"
#include "header/preprocessor.h"

//info
const unsigned long BuildVersion = 14;
const char CodeName[] = "Sprout";

int main(int argc, char* argv[]) {

    if (argc <= 2) {
        printf("Resin %s V.0.1 \nBuild : %lu\n" , CodeName , BuildVersion);
        return 0;
    }

    FILE *sourceFile = fdopen(*argv[1] , "r");

    // CleanSource(sourceFile);
    
    return 0;
}