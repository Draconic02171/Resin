#ifndef Opcode_H
#define Opcode_H

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
    _jmpIfTrue,
    _jmpIfFalse,

//function
    _function,
    _call,
    _end,

//IO
    _read,
    _print,
    _peek,

//array
    _allocArray,
    _pushArray,
    _storeArray,
    _loadArray
} Opcode;

#endif