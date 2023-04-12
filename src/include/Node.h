#ifndef Node_h
#define Node_h
#include "operators.h"
#include "stdlib.h"

const int MAX_FUNCTION_ARRAY_SIZE = 100;
const int MAX_VARIABLE_SIZE       = 25;
const int MAX_FUNC_SIZE           = 45;
const int _SQRT = -435;

typedef struct {
    const char func_name[MAX_FUNC_SIZE];
    int func_id;
} Function_info;


#define CMP(func_id, func, body, address) {func, _##func_id},
const Function_info _Diff_Functions_[] = {
    #include "codegeneration.h"
    {"I am not a function, baby:)", 0}
};
#undef CMP

typedef union {
    double number;
    char var[MAX_VARIABLE_SIZE];
    int func;
    int oper;
} Params;

struct Node {
    int type;  
    Params value;
    Node* left_branch;
    Node* right_branch;
    Node* parent;
};
typedef struct Node Node;

#endif