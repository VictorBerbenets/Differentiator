#ifndef Node_h
#define Node_h

typedef union {
    double number;
    char* var;
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