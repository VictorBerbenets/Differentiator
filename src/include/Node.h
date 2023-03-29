#ifndef Node_h
#define Node_h

struct Node{
    int type;  
    union data{
        int oper;
        double number;
    } value;

    Node* left_branch  = nullptr;
    Node* right_branch = nullptr;
};
typedef struct Node Node;

#endif