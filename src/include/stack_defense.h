#ifndef Stack_defense
#define Stack_defense

typedef unsigned long long Ull;

const Ull Canary_Value =  0XDEDFEED69;
const Ull Canary_left  =  0xDEADBEEF1;
const Ull Canary_right =  0xDEADBEEF9;

#ifdef FULL_STACK_DEFENSE
#define STACK_HASH
#define STACK_CANARY
#define STACK_HASH_DATA
#define STACK_DATA_CANARY_PROTECT
#endif

enum canary_errors {

    STACK_CANARY_LEFT    = (1 << 5),
    STACK_CANARY_RIGHT   = (1 << 6),
    DATA_CANARY_LEFT     = (1 << 7),
    DATA_CANARY_RIGHT    = (1 << 8),
    HASH_DATA_ERROR      = (1 << 9), 
    HASH_STACK_ERROR     = (1 << 10), 
} ;

#endif 