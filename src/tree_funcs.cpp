#include "include//differentiator.h"
#include "include//graphviz.h"
#include <ctype.h>
#include <string.h>

#include "include//Queue.h"

//******************************************************************************************************************************************//

static void ReplaceVarToNumber(Node** node_ptr, elem_t value);

//==========================================================================================================================================//

Node* CreateNewNode(int TYPE_NUM, const void* value, Node* left_node, Node* right_node) {
    Node* new_node = (Node*)calloc(ONE_NODE, sizeof(Node));
    Validator(new_node == nullptr, "link error: calloc could not give memory", exit(EXIT_FAILURE));

    if (TYPE_NUM == NUMBER) {
        new_node->type = NUMBER;
        if (value) {
            new_node->value.number = *(const elem_t*)value;
        }
    } else if (TYPE_NUM == OPER) {
        new_node->type = OPER;
        if (value) {
            new_node->value.oper = *(const int*)value;
        }
    } else if (TYPE_NUM == VAR) {
        new_node->type = VAR;
        if (value) {
            new_node->value.var = (char*)value;
        }
    } else if (TYPE_NUM == FUNC) {
        new_node->type = FUNC;
        new_node->value.func = *(const int*)value;
    } else {
        printf("Error: invalid value type: %d\n", TYPE_NUM);
        new_node->type = NotAType;
    }
    new_node->left_branch  = left_node;
    new_node->right_branch = right_node;

    if (new_node->left_branch) {
        new_node->left_branch->parent = new_node;
    }
    if (new_node->right_branch) {
        new_node->right_branch->parent = new_node;
    }

    return new_node;
}

//==========================================================================================================================================//
// calculate tree
elem_t CalculateNumbers(Node* node_ptr) {

    if (node_ptr->type == NUMBER) {
        return node_ptr->value.number;
    }
    switch (node_ptr->value.oper) {
        case OP_ADD:
            return CalculateNumbers(node_ptr->left_branch) + CalculateNumbers(node_ptr->right_branch);
        case OP_SUB:
            return CalculateNumbers(node_ptr->left_branch) - CalculateNumbers(node_ptr->right_branch);
        case OP_MUL:
            return CalculateNumbers(node_ptr->left_branch) * CalculateNumbers(node_ptr->right_branch);
        case OP_DIV:
            return GetDiv(node_ptr->left_branch, node_ptr->right_branch, 0);
        case OP_POW:
            return GetPower(node_ptr->left_branch, node_ptr->right_branch, 0);
        default:
            PrintWarningInvalidOper();
            return INVALID_OPERATOR;
    }
}

elem_t Ebal(Node* node_ptr, elem_t value, const char* var_name) {
    if (node_ptr->type == NUMBER) {
        return node_ptr->value.number;
    }
    ReplaceVarToNumber(&node_ptr, value);
    
    switch(node_ptr->type) {
        case OPER:
            switch (node_ptr->value.oper) {
                case OP_ADD:
                    return Ebal(node_ptr->left_branch, value, var_name) + Ebal(node_ptr->right_branch, value, var_name);
                case OP_SUB:
                    return Ebal(node_ptr->left_branch, value, var_name) - Ebal(node_ptr->right_branch, value, var_name);
                case OP_MUL:
                    return Ebal(node_ptr->left_branch, value, var_name) * Ebal(node_ptr->right_branch, value, var_name);
                case OP_DIV:
                    return GetDiv(node_ptr->left_branch, node_ptr->right_branch, value);
                case OP_POW:
                    return GetPower(node_ptr->left_branch, node_ptr->right_branch, value);
                default:
                    PrintWarningInvalidOper();
                    return INVALID_OPERATOR;
            }
            break;
        case FUNC:
            switch (node_ptr->value.func) {
                case SIN:    return  sin  (GetValue());
                case COS:    return  cos  (GetValue());
                case TG:     return  tan  (GetValue());
                case CTG:    return  1/tan(GetValue());
                case ARCSIN: return  asin (GetValue());
                case ARCCOS: return  acos (GetValue());
                case ARCTG:  return  atan (GetValue());
                case ARCCTG: return  M_PI/2 + atan(GetValue());
                case SH:     return  sinh  (GetValue());
                case CH:     return  cosh  (GetValue());
                case TH:     return  tanh  (GetValue());
                case CTH:    return  1/tanh(GetValue());
                case EXP:    return  exp   (GetValue());
                case LN:     return  log   (GetValue());
                case SQRT:   return  sqrt  (GetValue());
                default: printf("invalid func id: %d\n", node_ptr->value.func); break;
            }
            break;
        default: printf("invalid type: %d\n", node_ptr->type); break;
    }
    return 0;
}

static void ReplaceVarToNumber(Node** node_ptr, elem_t value) {
    if ((*node_ptr)->left_branch) {
        if ((*node_ptr)->left_branch->type == VAR  ) {                 
            (*node_ptr)->left_branch->value.number = value;
            (*node_ptr)->left_branch->type         = NUMBER;
        }
    }
    if ((*node_ptr)->right_branch ) {                               
        if ((*node_ptr)->right_branch->type == VAR) {
            (*node_ptr)->right_branch->value.number = value;
            (*node_ptr)->right_branch->type         = NUMBER;
        }
    }
    
}
//==========================================================================================================================================//

elem_t GetPower(Node* base, Node* degree, elem_t value) {
    return pow(Ebal(base, value, NULL), Ebal(degree, value, NULL));
}

//==========================================================================================================================================//

elem_t GetDiv(Node* dividend, Node* divisor, elem_t value) {
    elem_t div = Ebal(divisor, value, NULL);
    if (IsEqual(div, 0)) {
        PrintWarningForDivisor();
        exit(DIVIDE_ERROR);
    }
    return Ebal(dividend, value, NULL) / div;
}

//==========================================================================================================================================//

int IsEqual(elem_t value1, elem_t value2) {
    const static elem_t Epsilon_ = 1e-17;
    return fabs(value1 - value2) <= Epsilon_;
}

//==========================================================================================================================================//

void DeleteAllTrees(Tree* tree) {
    for (int tree_counter = 0; tree_counter < tree->var_counter; tree_counter++) {
        if (tree->Diff_trees) {
            DeleteTree(tree->Diff_trees[tree_counter]);
        }
        free(tree->variables[tree_counter].var_name);
    }
    free(tree->variables);
    free(tree->Diff_trees);
    free(tree->Root);
}

void DeleteTree(Node* tree) {
    static char* var_ptr = nullptr;

    if (!tree) {
        return;
    }
    DeleteTree(tree->left_branch);
    DeleteTree(tree->right_branch);
    
    if (tree->type == VAR) {
        if (tree->value.var) {
            var_ptr = (char*)tree->value.var;
            free(var_ptr);
            tree->value.var = nullptr;
        }
    }
    tree->parent = nullptr;
    tree->left_branch = nullptr;
    tree->right_branch = nullptr;

    free(tree);
    return;
}

//==========================================================================================================================================//
