#ifndef Node_h
#define Node_h

const static int Max_variable_size = 25;

typedef union {
    double number;
    char var[Max_variable_size] = {};
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