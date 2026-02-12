#ifndef DATATYPE
#define DATATYPE

typedef enum {
    NULL,
    NUMBER,
    STRING,
    ARRAY
} Datatype;

typedef struct {
    Datatype type;
    union {
        double   number;
        void*    value;
    } as;
} Value;

#endif