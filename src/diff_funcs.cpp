#include "include//differentiator.h"

static Node* CopyNode(Node* node_to_copy);

//==========================================================================================================================================//

Node* Diff(Node* node) {

    static int Sub  = OP_SUB;
    static int Plus = OP_ADD;
    static int Mul  = OP_MUL;
    static int Div  = OP_DIV;
    static int const_deriv    = 0;
    static elem_t var_deriv   = 1;
    Node* new_node = nullptr;

    if (!node) {
        return nullptr;
    }
    switch(node->type) {
        case OPER:
            switch(node->value.oper) {
                case OP_ADD: case OP_SUB:

                    new_node = CopyNode(node);
                    new_node->left_branch  = Diff(node->left_branch);
                    new_node->right_branch = Diff(node->right_branch);
                    return new_node;
                case OP_MUL:
                    new_node = CreateNewNode(OPER, &Plus);

                    new_node->left_branch  = CreateNewNode(OPER, &Mul);
                    new_node->right_branch = CreateNewNode(OPER, &Mul);  

                    new_node->left_branch->right_branch = CopyNode(node->right_branch);
                    new_node->right_branch->left_branch = CopyNode(node->left_branch);

                    new_node->left_branch->left_branch   = Diff(node->left_branch);  
                    new_node->right_branch->right_branch = Diff(node->right_branch); 
                    return new_node;
                case OP_DIV:
                
                case OP_POW:
                    if (node->right_branch->type == NUMBER) {
                        new_node = CreateNewNode(OPER, &Mul);
                        new_node->left_branch  = CopyNode(node->right_branch);
                        new_node->right_branch = CopyNode(node);

                        new_node->right_branch->left_branch  = CopyNode(node->left_branch);
                        new_node->right_branch->right_branch = CopyNode(node->right_branch);
                        new_node->right_branch->right_branch->value.number--;
                        return new_node;
                    }
                break;     
            }         
        case NUMBER:
            return CreateNewNode(NUMBER, &const_deriv); 
        case VAR: 
            return CreateNewNode(NUMBER, &var_deriv);   

            // case SIN:
            // case COS:
            // case TG:
            // case CTG:
            // case LOG:
            // case LN:
            // case EXP:
            // case SH:
            // case CH:
            // case CTH:
            // case TH: printf("f");
    }
    return nullptr;
}

//==========================================================================================================================================//

static Node* CopyNode(Node* node_to_copy) {

    Node* copied_version  = (Node*) calloc(1, sizeof(Node));
    
    copied_version->type  = node_to_copy->type;
    copied_version->value = node_to_copy->value;
    return copied_version;
}
//==========================================================================================================================================//
