#include "../header/structruntime.h"
#include <stdlib.h>


#define true 1
#define false 0

int StartRuntime() {
    RuntimeEnv newRuntimeEnv;

    newRuntimeEnv.Heap = malloc(5);
    newRuntimeEnv.Stack = malloc(1);
    newRuntimeEnv.Stack->type = ARRAY;

    return 0;
}