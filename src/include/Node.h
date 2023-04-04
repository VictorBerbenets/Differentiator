#ifndef Node_h
#define Node_h

const static int Max_variable_size = 25;
const static int Max_function_size = 25;

// typedef struct {
//     char func_name[Max_function_size];
//     int func_id;
// }Function_name;

typedef union {
    double number;
    char var[Max_variable_size];
    int func_name;
    int oper;
} Params;

struct Node{
    int type;  
    Params value;
    Node* left_branch ;
    Node* right_branch;
};
typedef struct Node Node;

#endif