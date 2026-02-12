#include "datatype.h"

#ifndef StructRuntime
#define StructRuntime

typedef struct {

    Value *Stack;
    Value *Heap;
    int *VariableSlot;

} RuntimeEnv;

typedef struct {
    int opcode;
    Value operand;
} Instructions;

#endif