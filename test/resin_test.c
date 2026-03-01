//Resin Skeletion version
const int BuildVersion = 173; 
const char CodeName[] = "Seedling";
const char StartTime[] = "2026-02-22 20:03:37";
const char TimeSpent[] = "2026-03-01 19:02:56";

#include <string.h>
#include <stdlib.h>
#include <iso646.h>
#include <stdio.h>
#include <stdint.h>

#define true 1
#define false 0
#define TypeNull ((void*)0)

#define MAX_STACK_SIZE 15 //test size
#define MAX_FUNC_SLOT 256 //test size
#define MAX_FUNCNAME_SIZE 256 //test size

typedef uint8_t usigned8 ;
typedef uint16_t usigned16 ;
typedef uint32_t usigned32 ;

#include <unistd.h>  // for usleep


//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
typedef enum {
    enum_type_null,
    enum_type_number,
    enum_type_string,
    enum_type_function
} Datatype;
typedef enum {
// basic stack operation
    _push,
    _pop,
    _dup,
    _load,
    _store,

// arithmetic operation
    _add,
    _sub,
    _mul,
    _div,
    _mod,

//comparision
    _equal,
    _more,
    _less,
    _moreOrEqual,
    _lessOrEqual,

//control flow
    _jump,
    _jumpIfTrue,
    _jumpIfFalse,

//function
    _function,
    _call,
    _return,

//IO
    _read,
    _print,
    _peek,
    _halt, //not sure if I need this since _return is enough

//array
    _allocArray,
    _pushArray,
    _storeArray,
    _loadArray
} Opcode;

//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
typedef struct {
    usigned8 type; 
    union {double number; void* value;} as;
} Value;

typedef struct {Opcode Opcode; Value Operand;} Instruction;

typedef struct {
    char Name[100];
    int ArgsAmout;
    int VariableAmount;
} MetaData;

typedef struct {

    Value Stack[MAX_STACK_SIZE];
    int StackPointer;

    int CallStackPointer;
    MetaData FuncSlot[MAX_FUNC_SLOT];

    Instruction* Program;
    int ProgramSize;
    int ProgramCounter;
    
    int IsRunning;

} RuntimeEnv;


//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
void push_numeric(RuntimeEnv* VM , Value value);
Value pop_numeric(RuntimeEnv* VM);
void dup_numeric(RuntimeEnv* VM);

void add_numeric(RuntimeEnv* VM);
void sub_numeric(RuntimeEnv* VM);
void mul_numeric(RuntimeEnv* VM);
void div_numeric(RuntimeEnv* VM);
void mod_numeric(RuntimeEnv* VM);

void equal_numeric(RuntimeEnv* VM);
void more_numeric(RuntimeEnv* VM);
void less_numeric(RuntimeEnv* VM);
void moreEqual_numeric(RuntimeEnv* VM);
void lessEqual_numeric(RuntimeEnv* VM);

void jump(RuntimeEnv* VM , Value* value);
void jumpIfTrue(RuntimeEnv* VM , Value* value);
void jumpIfFalse(RuntimeEnv* VM , Value* value);

// void PushFuncCall(RuntimeEnv* VM, Value* value);
// Value PopFuncCall(RuntimeEnv* VM);
int GetFuncIndexByName(RuntimeEnv* VM , char* String);

void CallFunc(RuntimeEnv* VM, Value* FunctionIndex);
void FuncBody(RuntimeEnv* VM, Value* FunctionIndex);
void ReturnFunc(RuntimeEnv* VM, Value* PopCount);

void print_numeric(RuntimeEnv* VM);
void peek_numeric(RuntimeEnv* VM);

void InitializeRuntime(RuntimeEnv* VM);
int PauseExecFuncution(RuntimeEnv* VM);
int Execute(RuntimeEnv* VM) ;

char* log_btyeCode_to_enum(double value) {
    switch ((int) value) {

            case _push          : return "push";
            case _pop           : return "pop";
            case _dup           : return "dup";
            case _load          : return "load";
            case _store         : return "store";

            case _add           : return "add";
            case _sub           : return "sub"; 
            case _mul           : return "mul";
            case _div           : return "div";
            case _mod           : return "mod";

            case _equal         : return "equal";
            case _more          : return "more";
            case _less          : return "less";
            case _moreOrEqual   : return "moreOrEqual";
            case _lessOrEqual   : return "lessOrEqual";

            case _jump          : return "jump";
            case _jumpIfTrue    : return "jumpIfTrue";
            case _jumpIfFalse   : return "jumpIfFalse";
            
            case _function      : return "function";
            case _call          : return "call";
            case _return        : return "return";
            
            case _read          : return "read";
            case _print         : return "print";
            case _peek          : return "peek";

            default             : return "Unknown opcode";
            
        }
}

const char* log_opcode_description(int opcode) {
    switch (opcode) {
        case _push          : return "Push immediate value onto the stack";
        case _pop           : return "Pop the top value off the stack";
        case _dup           : return "Duplicate the top stack value";
        case _load          : return "Load a variable onto the stack";
        case _store         : return "Store top of stack into a variable";

        case _add           : return "Pop two values, push their sum";
        case _sub           : return "Pop two values, push their difference";
        case _mul           : return "Pop two values, push their product";
        case _div           : return "Pop two values, push their quotient";
        case _mod           : return "Pop two values, push their remainder";

        case _equal         : return "Push 1 if top two values are equal";
        case _more          : return "Push 1 if second > top";
        case _less          : return "Push 1 if second < top";
        case _moreOrEqual   : return "Push 1 if second >= top";
        case _lessOrEqual   : return "Push 1 if second <= top";

        case _jump          : return "Unconditional jump to address";
        case _jumpIfTrue    : return "Jump to address if top of stack is truthy";
        case _jumpIfFalse   : return "Jump to address if top of stack is falsy";

        case _function      : return "Define a function at current position";
        case _call          : return "Call a function, push return frame";
        case _return        : return "Return from function, restore frame";

        case _read          : return "Read input from user onto stack";
        case _print         : return "Pop and print top of stack";
        case _peek          : return "Print top of stack without popping";

        default             : return "Unknown opcode";
    }
}

void log_vm_step(RuntimeEnv* VM) {

    FILE* file = fopen("stack_debug.txt", "w");
    if (!file) { perror("log_vm_step: fopen"); return; }

    int opcode = VM->Program[VM->ProgramCounter].Opcode;
    int call_stack_bottom = MAX_STACK_SIZE - VM->CallStackPointer;

    fprintf(file, "=================================================\n");
    fprintf(file, "PC: %d | Opcode: %d (%s)\n",
            VM->ProgramCounter, opcode, log_btyeCode_to_enum((double)opcode));
    fprintf(file, "DESC: %s\n", log_opcode_description(opcode));
    fprintf(file, "=================================================\n");
    fprintf(file, "%-6s| %-8s| %-20s| %s\n", "Index", "Type", "Value", "Section");
    fprintf(file, "-------------------------------------------------\n");

    for (int i = 0; i < MAX_STACK_SIZE; i++) {

        int is_call_region  = (i >= call_stack_bottom);
        int is_value_region = (i <= VM->StackPointer);

        if (is_call_region) {
            int call_slot      = (MAX_STACK_SIZE - 1) - i;
            int frame_index    = call_slot / 2;
            int slot_in_frame  = call_slot % 2;
            const char* slot_label = (slot_in_frame == 0) ? "saved PC" : "saved SP";

            const char* type_str =
                VM->Stack[i].type == enum_type_number ? "num"  :
                VM->Stack[i].type == enum_type_null   ? "null" : "other";

            fprintf(file, "%-6d| %-8s| %-20.4f| CALL frame[%d] %s\n",
                i, type_str, VM->Stack[i].as.number, frame_index, slot_label);

        } else if (is_value_region) {
            const char* type_str =
                VM->Stack[i].type == enum_type_number   ? "num"  :
                VM->Stack[i].type == enum_type_string   ? "str"  :
                VM->Stack[i].type == enum_type_function ? "func" : "null";

            fprintf(file, "%-6d| %-8s| %-20.4f| VALUE\n",
                i, type_str, VM->Stack[i].as.number);

        } else {
            int danger = (i > VM->StackPointer && i < call_stack_bottom);
            fprintf(file, "%-6d| ---- free %s----\n", i, danger ? "" : "");
        }
    }

    fprintf(file, "=================================================\n");
    fprintf(file, "StackPointer    : %d\n",  VM->StackPointer);
    fprintf(file, "CallStackPointer: %d\n",  VM->CallStackPointer);
    fprintf(file, "Call stack top  : %d  (index MAX-1 = %d)\n",
            call_stack_bottom, MAX_STACK_SIZE - 1);
    fprintf(file, "Free slots      : %d\n",
            call_stack_bottom - VM->StackPointer - 1);
    fprintf(file, "=================================================\n");

    fclose(file);
    usleep(2000000);
}


//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■


// MAIN FUNCTION

// Instruction program[] = {
//     // function 0
//     //main function
//     {.Opcode = _function, .Operand = {.type = enum_type_function, .as.number = 0 }},
//     {.Opcode = _push, .Operand = {.type = enum_type_function, .as.number = 5}},
//     {.Opcode = _push, .Operand = {.type = enum_type_number, .as.number = 1}},
//     {.Opcode = _sub, .Operand = {.type = enum_type_null}},
//     {.Opcode = _dup, .Operand = {.type = enum_type_null}},
//     {.Opcode = _push, .Operand = {.type = enum_type_number, .as.number = 0}},
//     {.Opcode = _equal, .Operand = {.type = enum_type_number, .as.number = 0}},
//     {.Opcode = _jumpIfFalse, .Operand = {.type = enum_type_number, .as.number = 2}},
//     {.Opcode = _return, .Operand = {.type = enum_type_null}},
// };

Instruction program[] = {

    /* ── main (FuncSlot 0) ─── PC 0–9 ──────────────────────── */
    /* 00 */ {.Opcode = _function, .Operand = {.type = enum_type_function, .as.number = 0}},
    /* 01 */ {.Opcode = _call,     .Operand = {.type = enum_type_number,   .as.number = 10}}, // → funcA
    /* 02 */ {.Opcode = _peek,     .Operand = {.type = enum_type_null}},                       // expect: 7
    /* 03 */ {.Opcode = _pop,      .Operand = {.type = enum_type_null}},
    /* 04 */ {.Opcode = _call,     .Operand = {.type = enum_type_number,   .as.number = 15}}, // → funcB
    /* 05 */ {.Opcode = _peek,     .Operand = {.type = enum_type_null}},                       // expect: 20
    /* 06 */ {.Opcode = _pop,      .Operand = {.type = enum_type_null}},
    /* 07 */ {.Opcode = _call,     .Operand = {.type = enum_type_number,   .as.number = 20}}, // → funcC
    /* 08 */ {.Opcode = _peek,     .Operand = {.type = enum_type_null}},                       // expect: 27
    /* 09 */ {.Opcode = _return,   .Operand = {.type = enum_type_number,   .as.number = 0}},

    /* ── funcA (FuncSlot 1) ─── PC 10–14 ── returns 3+4 = 7 ── */
    /* 10 */ {.Opcode = _function, .Operand = {.type = enum_type_function, .as.number = 1}},
    /* 11 */ {.Opcode = _push,     .Operand = {.type = enum_type_number,   .as.number = 3}},
    /* 12 */ {.Opcode = _push,     .Operand = {.type = enum_type_number,   .as.number = 4}},
    /* 13 */ {.Opcode = _add,      .Operand = {.type = enum_type_null}},
    /* 14 */ {.Opcode = _return,   .Operand = {.type = enum_type_number,   .as.number = 1}},

    /* ── funcB (FuncSlot 2) ─── PC 15–19 ── returns 10×2 = 20 ─ */
    /* 15 */ {.Opcode = _function, .Operand = {.type = enum_type_function, .as.number = 2}},
    /* 16 */ {.Opcode = _push,     .Operand = {.type = enum_type_number,   .as.number = 10}},
    /* 17 */ {.Opcode = _push,     .Operand = {.type = enum_type_number,   .as.number = 2}},
    /* 18 */ {.Opcode = _mul,      .Operand = {.type = enum_type_null}},
    /* 19 */ {.Opcode = _return,   .Operand = {.type = enum_type_number,   .as.number = 1}},

    /* ── funcC (FuncSlot 3) ─── PC 20–24 ── calls A+B, returns 27 */
    /* 20 */ {.Opcode = _function, .Operand = {.type = enum_type_function, .as.number = 3}},
    /* 21 */ {.Opcode = _call,     .Operand = {.type = enum_type_number,   .as.number = 10}}, // → funcA (7)
    /* 22 */ {.Opcode = _call,     .Operand = {.type = enum_type_number,   .as.number = 15}}, // → funcB (20)
    /* 23 */ {.Opcode = _add,      .Operand = {.type = enum_type_null}},                       // 7+20 = 27
    /* 24 */ {.Opcode = _return,   .Operand = {.type = enum_type_number,   .as.number = 1}},
};


// Instruction program[] = {
//     // function 0
//     //random Function
//     {.Opcode = _function, .Operand = {.type = enum_type_function, .as.number = 1}},
//         {.Opcode = _push, .Operand = {.type = enum_type_function, .as.number = 5}},
//         {.Opcode = _push, .Operand = {.type = enum_type_function, .as.number = 5}},
//         {.Opcode = _add, .Operand = {.type = enum_type_null}},
//         {.Opcode = -_call, .Operand = {.type = enum_type_function, .as.number = 1}},
//         {.Opcode = -_return, .Operand = {.type = enum_type_null}},

//     //function main 
//     {.Opcode = _function, .Operand = {.type = enum_type_function, .as.number = 0}},
//         {.Opcode = _push, .Operand = {.type = enum_type_function, .as.number = 5}},
//         {.Opcode = _push, .Operand = {.type = enum_type_number, .as.number = 1}},
//         {.Opcode = _sub, .Operand = {.type = enum_type_null}},
//         {.Opcode = _dup, .Operand = {.type = enum_type_null}},
//         {.Opcode = _push, .Operand = {.type = enum_type_number, .as.number = 0}},
//         {.Opcode = _equal, .Operand = {.type = enum_type_number, .as.number = 0}},
//         {.Opcode = _jumpIfFalse, .Operand = {.type = enum_type_number, .as.number = 2}},
// };


int main(int argc, char* argv[]) {

    RuntimeEnv VirtualMachine;
    InitializeRuntime(&VirtualMachine);
    
    VirtualMachine.Program = program;
    VirtualMachine.ProgramSize = sizeof(program) / sizeof(Instruction);

    for (int index = 0; index < MAX_FUNC_SLOT; index ++) {
        VirtualMachine.FuncSlot[index].VariableAmount = 0;
    }

    // for (int index = 0; index < VirtualMachine.ProgramSize; index ++) {
    //     if (VirtualMachine.Program[index].Opcode == _function) {
    //         VirtualMachine.FuncSlot[(int)VirtualMachine.Program->Operand.as.number].ArgsAmout
    //     }
    // }
    
    //run `_main` first
    // not practical for now
    // MainFunc.as.number = (double)GetFuncIndexByName(&VirtualMachine , "_main");
    
    Value MainFunc;
    MainFunc.type = enum_type_number;
    MainFunc.as.number = 0;
    
    CallFunc(&VirtualMachine , &MainFunc);
    log_vm_step(&VirtualMachine);
    Execute(&VirtualMachine);

    
    return 0;
}



//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

void InitializeRuntime(RuntimeEnv* VM) {
    VM->IsRunning = false;
    VM->StackPointer = -1;
    VM->ProgramCounter = 0;
    VM->CallStackPointer = 0;
}

// void MountProgram(RuntimeEnv* VM , Instruction* Program , int TotalProgramLine) {
//     VM->Program = Program;
//     VM->ProgramSize = TotalProgramLine;

//     for (int index = 0; index <= TotalProgramLine; index ++) {
//         // if() ++;
//     }
// }

void ExecuteLoop(RuntimeEnv* VM) {

    while (VM->IsRunning == true && VM->ProgramCounter < VM->ProgramSize && VM->CallStackPointer > 0) {

        int opcode = VM->Program[VM->ProgramCounter].Opcode;
        Value operand = VM->Program[VM->ProgramCounter].Operand;

        // printf("%d\t %lf\n" , opcode , operand.as.number);

        switch (opcode) {

            case _push          : push_numeric(VM , operand); break;
            case _pop           : pop_numeric(VM); break;
            case _dup           : dup_numeric(VM); break;
            case _load          : break;
            case _store         : break;

            case _add           : add_numeric(VM); break;
            case _sub           : sub_numeric(VM); break; 
            case _mul           : mul_numeric(VM); break; 
            case _div           : div_numeric(VM); break;
            case _mod           : mod_numeric(VM); break;

            case _equal         : equal_numeric(VM); break;
            case _more          : more_numeric(VM); break;
            case _less          : less_numeric(VM); break;
            case _moreOrEqual   : moreEqual_numeric(VM); break;
            case _lessOrEqual   : lessEqual_numeric(VM); break;

            case _jump          : jump(VM, &operand); break;
            case _jumpIfTrue    : jumpIfTrue(VM, &operand); break;
            case _jumpIfFalse   : jumpIfFalse(VM, &operand); break;
            
            case _function      : FuncBody(VM, &operand); break;
            case _call          : CallFunc(VM, &operand); break;
            case _return        : ReturnFunc(VM, &operand);  break;
            
            case _read          : break;
            case _print         : print_numeric(VM); break;
            case _peek          : peek_numeric(VM); break;
            
        }
        // printf("%p\n" , VM->Stack[]);
        // peek_numeric(VM);
        // log_vm_step(
        //     "logcheck.txt",
        //     VM->ProgramCounter,
        //     opcode,
        //     VM->Stack,
        //     VM->StackPointer
        // );
        log_vm_step(VM);
        VM->ProgramCounter ++;
    }
}

int PauseExecution(RuntimeEnv* VM) {
    VM->IsRunning = false;
    return 0;
}

int Execute(RuntimeEnv* VM) {
    VM->IsRunning = true;
    ExecuteLoop(VM);
    return 0;
}

void push_numeric(RuntimeEnv* VM , Value value) {
    VM->StackPointer ++;
    VM->Stack[VM->StackPointer] = value;
}

Value pop_numeric(RuntimeEnv* VM) {

    Value _NULL;
    _NULL.type = enum_type_null;

    Value ExportValue = VM->Stack[VM->StackPointer];

    VM->Stack[VM->StackPointer] = _NULL;
    VM->StackPointer --;

    return ExportValue;
}

void dup_numeric(RuntimeEnv* VM) {
    push_numeric(VM , VM->Stack[VM->StackPointer]);
}

void peek_numeric(RuntimeEnv* VM) {
    printf("%lf\n" , VM->Stack[VM->StackPointer].as.number);
}

void print_numeric(RuntimeEnv* VM) {
    peek_numeric(VM);
    pop_numeric(VM);
}

void add_numeric(RuntimeEnv* VM) {

    Value Right = pop_numeric(VM);
    Value Left = pop_numeric(VM);

    Value ExportValue;
    ExportValue.type = enum_type_number;
    ExportValue.as.number = Left.as.number + Right.as.number;

    push_numeric(VM, ExportValue);

}

void sub_numeric(RuntimeEnv* VM) {

    Value Right = pop_numeric(VM);
    Value Left = pop_numeric(VM);

    Value ExportValue;
    ExportValue.type = enum_type_number;
    ExportValue.as.number = Left.as.number - Right.as.number;

    push_numeric(VM, ExportValue);

}

void mul_numeric(RuntimeEnv* VM) {

    Value Right = pop_numeric(VM);
    Value Left = pop_numeric(VM);

    Value ExportValue;
    ExportValue.type = enum_type_number;
    ExportValue.as.number = Left.as.number * Right.as.number;

    push_numeric(VM, ExportValue);

}

void div_numeric(RuntimeEnv* VM) {

    Value Right = pop_numeric(VM);
    Value Left = pop_numeric(VM);

    Value ExportValue;
    ExportValue.type = enum_type_number;
    ExportValue.as.number = Left.as.number / Right.as.number;

    push_numeric(VM, ExportValue);

}

void mod_numeric(RuntimeEnv* VM) {

    Value Right = pop_numeric(VM);
    Value Left = pop_numeric(VM);

    Value ExportValue;
    ExportValue.type = enum_type_number;
    ExportValue.as.number = (double)((int)Left.as.number % (int)Right.as.number);

    push_numeric(VM, ExportValue);

}

void equal_numeric(RuntimeEnv* VM) {

    Value Right = pop_numeric(VM);
    Value Left = pop_numeric(VM);

    Value ExportValue;
    ExportValue.type = enum_type_number;
    ExportValue.as.number = (double)(Left.as.number == Right.as.number);

    push_numeric(VM, ExportValue);

}

void more_numeric(RuntimeEnv* VM) {

    Value Right = pop_numeric(VM);
    Value Left = pop_numeric(VM);

    Value ExportValue;
    ExportValue.type = enum_type_number;
    ExportValue.as.number = (double)(Left.as.number > Right.as.number);

    push_numeric(VM, ExportValue);

}

void less_numeric(RuntimeEnv* VM) {

    Value Right = pop_numeric(VM);
    Value Left = pop_numeric(VM);

    Value ExportValue;
    ExportValue.type = enum_type_number;
    ExportValue.as.number = (double)(Left.as.number < Right.as.number);

    push_numeric(VM, ExportValue);

}

void moreEqual_numeric(RuntimeEnv* VM) {

    Value Right = pop_numeric(VM);
    Value Left = pop_numeric(VM);

    Value ExportValue;
    ExportValue.type = enum_type_number;
    ExportValue.as.number = (double)(Left.as.number >= Right.as.number);

    push_numeric(VM, ExportValue);

}

void lessEqual_numeric(RuntimeEnv* VM) {

    Value Right = pop_numeric(VM);
    Value Left = pop_numeric(VM);

    Value ExportValue;
    ExportValue.type = enum_type_number;
    ExportValue.as.number = (double)(Left.as.number <= Right.as.number);

    push_numeric(VM, ExportValue);

}

void jump(RuntimeEnv* VM , Value* value) {

    if (value->type == enum_type_null) {
        printf("no jump value\n");
        exit(1);
    }
    VM->ProgramCounter = value->as.number - 1;

}

void jumpIfTrue(RuntimeEnv* VM , Value* value) {

    if (value->type == enum_type_null) {
        printf("no jump value\n");
        exit(1);
    }
    Value Check = pop_numeric(VM);
    
    if (!Check.as.number) {return;}

    VM->ProgramCounter = value->as.number - 1;

}

void jumpIfFalse(RuntimeEnv* VM , Value* value) {

    if (value->type == enum_type_null) {
        printf("no jump value\n");
        exit(1);
    }
    Value Check = pop_numeric(VM);
    
    if (Check.as.number != 0) {return;}

    VM->ProgramCounter = value->as.number - 1;

}

//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
int GetFuncIndexByName(RuntimeEnv* VM , char* String) {

    for (int index = 0; index < MAX_FUNC_SLOT; index ++) {

        if (!strcmp(VM->FuncSlot[index].Name, String)) {return index;}

    }

    return -1;
}

//TODO : NEXT TIME ADD A SAFTY CHECK !!!!!
void PushFuncCall(RuntimeEnv* VM, Value* value) {
    int ActualCallStackPointer = (MAX_STACK_SIZE - 1) - VM->CallStackPointer;
    VM->Stack[ActualCallStackPointer] = *value;
    VM->CallStackPointer ++;
}

//TODO : THIS ONE TOO!!!
Value PopFuncCall(RuntimeEnv* VM) {
    int ActualCallStackPointer = (MAX_STACK_SIZE) - VM->CallStackPointer;
    
    Value ReturnFuncCall = VM->Stack[ActualCallStackPointer];

    Value _NULL;
    _NULL.type = enum_type_null;
    VM->Stack[ActualCallStackPointer] = _NULL;

    VM->CallStackPointer --;
    
    return ReturnFuncCall;
}

void CallFunc(RuntimeEnv* VM, Value* FunctionIndex) {


    //can collapes into just one call but for now
    //having a variable stored like this is easier
    Value CurrentPC = {
        .type = enum_type_number,
        .as.number = VM->ProgramCounter
    };

    //not sure between pass by value or pass by ref
    // which one is better
    PushFuncCall(VM, &CurrentPC);
    jump(VM, FunctionIndex);

}

//I need to make a variable slot for a func frame
//but for now lets make it usable first
void FuncBody(RuntimeEnv* VM, Value* FunctionIndex) {

    //TODO : patitioning a variable slots;
    //can collapes into just one call but for now
    //having a variable stored like this is easier
    Value CurrentStackPointer = {
        .type = enum_type_number,
        .as.number = VM->StackPointer
    };

    //not sure between pass by value or pass by ref
    // which one is better
    PushFuncCall(VM, &CurrentStackPointer);

    // VM->StackPointer += VM->FuncSlot[(int)FunctionIndex->as.number].VariableAmount;s

}

void ReturnFunc(RuntimeEnv* VM, Value* PopCount) {

    Value PoppedValue[(int)PopCount->as.number];

    for (int count = (int)PopCount->as.number - 1; count >= 0; count--) {  // fix
        PoppedValue[count] = pop_numeric(VM);
    }

    Value OriginalStackPointer  = PopFuncCall(VM);
    Value OriginalProgramCounter = PopFuncCall(VM);

    VM->ProgramCounter = (int)OriginalProgramCounter.as.number;
    VM->StackPointer   = (int)OriginalStackPointer.as.number;

    for (int count = (int)PopCount->as.number - 1; count >= 0; count--) {  // fix
        push_numeric(VM, PoppedValue[count]);
    }
}