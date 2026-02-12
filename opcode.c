//its for shortnote, easy to look up enum values

// basic stack operation
enum Opcode {
    push,
    pop,
    dup,
    load,
    store,

// arithmetic operation
    add,
    sub,
    mul,
    div,
    mod,

//comparision
    equal,
    more,
    less,
    moreOrEqual,
    lessOrEqual,

//control flow
    jump,
    jmpIfTrue,
    jmpIfFalse,

//function
    function,
    call,
    end,

//IO
    read,
    print,
    peek,

//array
    allocArray,
    pushArray,
    storeArray,
    loadArray
};