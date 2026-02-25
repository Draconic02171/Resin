//Resin Skeletion version
const int BuildVersion = 63; 
const char CodeName[] = "spore";
const char StartTime[] = "2026-02-22 20:03:37";
const char TimeSpent[] = "2026-02-24 21:30:47";

#include <stdlib.h>
#include <iso646.h>
#include <stdio.h>
#include <stdint.h>

#define true 1
#define false 0
#define TypeNull ((void*)0)

typedef uint8_t usigned8 ;
typedef uint16_t usigned16 ;
typedef uint32_t usigned32 ;


//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
typedef enum {enum_type_null,enum_type_number} Datatype;
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
    _end,

//IO
    _read,
    _print,
    _peek,
    _halt,

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
    int ProgramCounter;
    int* VariableSlot;
} FuncFrame;

// typedef struct {
//     funcCall
// } ;

typedef struct {

    Value Stack[10];
    int StackPointer;
    int StackSize;

    Instruction* Program;
    int ProgramSize;
    int ProgramCounter;

    int IsRunning;

} RuntimeEnv;


static const char* opcode_to_str(Opcode op) {
    switch (op) {
        case _push: return "_push"; 
        case _pop:  return "_pop";  
        case _dup:  return "_dup";
        case _load: return "_load"; 
        case _store: return "_store";
        case _add:  return "_add";  
        case _sub:   return "_sub";  
        case _mul:  return "_mul";
        case _div:  return "_div";  
        case _mod:   return "_mod";
        case _equal: return "_eq";  
        case _more:  return "_more";   
        case _less: return "_less";
        case _moreOrEqual: return "_moreOrEqual"; 
        case _lessOrEqual: return "_lessOrEqual";
        case _jump: return "_jump";  
        case _jumpIfTrue: return "_jumpIfTrue"; 
        case _jumpIfFalse: return "_jumpIfFalse";
        case _print: return "_print"; 
        case _peek: return "_peek";
        case _halt: return "_halt";
        default: return "UNKNOWN_OP";
    }
}

/**
 * Logs the VM state including the Opcode name.
 */
static inline void log_vm_step(const char* filename, int pc, Opcode op, Value* stack, int sp) {
    FILE* f = fopen(filename, "a");
    if (!f) return;

    // Header showing PC and the Opcode that just executed
    fprintf(f, "Step [PC: %03d | Op: %-10s | SP: %d]\n", pc, opcode_to_str(op), sp);
    fprintf(f, "Stack State:\n");

    if (sp < 0) {
        fprintf(f, "  (Empty)\n");
    } else {
        for (int i = 0; i <= sp; i++) {
            if (stack[i].type == 1) {
                fprintf(f, "  [%d]: %lf\n", i, stack[i].as.number);
            } else if (stack[i].type == 0) {
                fprintf(f, "  [%d]: NULL\n", i);
            } else {
                fprintf(f, "  [%d]: (Other/Pointer Type)\n", i);
            }
        }
    }
    fprintf(f, "----------------------------------\n");
    fclose(f);
}



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

void print_numeric(RuntimeEnv* VM);
void peek_numeric(RuntimeEnv* VM);

void InitializeRuntime(RuntimeEnv* VM);
int PauseExecution(RuntimeEnv* VM);
int Execute(RuntimeEnv* VM) ;

//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■



// MAIN FUNCTION

Instruction program[] = {
    //counter program
    //start by set init value
    { .Opcode = _push,          .Operand = { .type = enum_type_number, .as.number = 20.0 } },

    //then push 1 and sub
    { .Opcode = _push,          .Operand = { .type = enum_type_number, .as.number = 1.0 } },
    { .Opcode = _sub,           .Operand = { .type = enum_type_null} },

    //peek
    { .Opcode = _peek,          .Operand = { .type = enum_type_null} },

    //dup for compare, push 0 for compare
    { .Opcode = _dup,           .Operand = { .type = enum_type_null} },
    { .Opcode = _push,          .Operand = { .type = enum_type_number, .as.number = 0 } },
    { .Opcode = _more,         .Operand = { .type = enum_type_null} },

    //jump if equal results 0?
    { .Opcode = _jumpIfTrue,    .Operand = { .type = enum_type_number, .as.number = 1.0 } },
};

int main(int argc, char* argv[]) {

    RuntimeEnv VirtualMachine;
    InitializeRuntime(&VirtualMachine);
    
    VirtualMachine.Program = program;
    VirtualMachine.ProgramSize = 8;
    
    Execute(&VirtualMachine);
    
    return 0;
}



//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

void InitializeRuntime(RuntimeEnv* VM) {

    VM->IsRunning = false;
    VM->StackPointer = -1;
    VM->ProgramCounter = 0;
    VM->StackSize = 256;

}

void ExecuteLoop(RuntimeEnv* VM) {

    while (VM->IsRunning == true && VM->ProgramCounter < VM->ProgramSize) {

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
            case _jumpIfTrue     : jumpIfTrue(VM, &operand); break;
            case _jumpIfFalse    : jumpIfFalse(VM, &operand); break;
            
            case _function      : break;
            case _call          : break;
            case _end           : break;
            
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

    if (value->type == enum_type_null) {exit(1);}
    VM->ProgramCounter = value->as.number - 1;

}

void jumpIfTrue(RuntimeEnv* VM , Value* value) {

    if (value->type == enum_type_null) {exit(1);}
    Value Check = pop_numeric(VM);
    
    if (!Check.as.number) {return;}

    VM->ProgramCounter = value->as.number - 1;

}

void jumpIfFalse(RuntimeEnv* VM , Value* value) {

    if (value->type == enum_type_null) {exit(1);}
    Value Check = pop_numeric(VM);
    
    if (Check.as.number != 0) {return;}

    VM->ProgramCounter = value->as.number - 1;

}

