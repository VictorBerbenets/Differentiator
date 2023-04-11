#ifndef Node_h
#define Node_h
#include "operators.h"

const static int Max_variable_size = 25;
const static int Max_function_size = 25;

typedef struct {
    char func_name[Max_function_size];
    int func_id;
}Function_info;

const Function_info _Diff_Functions_[] = { {"sqrt", _SQRT}, {"sin", _SIN}, {"cos", _COS}, {"tg", _TG}, {"ctg", _CTG}, 
                                           {"ln", _LN}, {"exp", _EXP}, {"sh", _SH}, {"ch", _CH}, {"cth", _CTH}, {"th", _TH} };      // можна КОдеген

typedef union {
    double number;
    char var[Max_variable_size];
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