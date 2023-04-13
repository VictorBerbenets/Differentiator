#include "include//differentiator.h"
//------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------Constants and functions-------------------------------------------------- -----------------//
//functions
static Node* FindVariable(Node* tree, int* is_function);
static int IsFunction(Node* tree);
static Node* CopyNode(Node* node_to_copy);
static Node* Digit(elem_t number);
static Node* duplic_tree = nullptr;


//Operators
static const int Sub  = OP_SUB;
static const int Add  = OP_ADD;
static const int Mul  = OP_MUL;
static const int Div  = OP_DIV;
static const int Pow  = OP_POW;

//Functions
#define CMP(func_id, func_name, body, address_name) static const int address_name = _##func_id;
    #include "codegeneration.h"
#undef CMP
//******************************************************************************************************************************************//
//---------------------------------------------------Function   bodies----------------------------------------------------------------------//


#define CMP(func_id, func_name, body, ...) case _##func_id: return body;

Node* Diff(Node* node, const char* var_name) {

    if (!node) {
        return nullptr;
    }
    switch(node->type) {
        case OPER:
            switch(node->value.oper) {
                case OP_ADD: return ADD(dL, dR);
                case OP_SUB: return SUB(dL, dR);
                case OP_MUL: return ADD(MUL(dL, cR), MUL(cL, dR));
                case OP_DIV: return DIV(SUB(MUL(dL, cR), MUL(cL, dR)),POW(cR, Digit(2)));  
                case OP_POW: 
                    if (R->type == NUMBER) {
                        return MUL(MUL(POW(cL, Digit(R->value.number - 1)), cR), dL);  
                    }
                    else if (IsFunction(R) && !IsFunction(L)) {
                        return MUL(MUL( LN_(cL, nullptr), POW(cL, cR) ), dR);
                    }
                    else if (IsFunction(R) && IsFunction(L)) {
                        return MUL( POW(cL, cR), ADD(MUL(dL, cR), MUL(cL, dR)) );
                    }
                default:  printf("No such operator: %d\n", node->value.oper);  return nullptr;
            }
        case FUNC:
            switch (node->value.func) {               
                #include "codegeneration.h"
                default:  break;
            } 
        case NUMBER: return CONST_(); 
        case VAR:    VAR_(); 
        default:
            printf ("No such node\n");
            return nullptr;
    }
}
#undef CMP
//==========================================================================================================================================//

 Node* CopyTree(Node* tree_to_copy, Node* duplic_tree) {

	if (tree_to_copy == nullptr){
		duplic_tree = nullptr;
		return nullptr;
	}
    duplic_tree = CopyNode(tree_to_copy);

	duplic_tree->left_branch  = CopyTree(tree_to_copy->left_branch, duplic_tree->left_branch);
    if (duplic_tree->left_branch) {
        duplic_tree->left_branch->parent = duplic_tree;
    }

	duplic_tree->right_branch = CopyTree(tree_to_copy->right_branch, duplic_tree->right_branch);
    if (duplic_tree->right_branch) {
        duplic_tree->right_branch->parent = duplic_tree;
    }

    return duplic_tree;
}

//==========================================================================================================================================//

static Node* CopyNode(Node* node_to_copy) {

    Node* copied_version  = (Node*) calloc(1, sizeof(Node));

    copied_version->type  = node_to_copy->type;
    copied_version->value = node_to_copy->value;

    return copied_version;
}

//==========================================================================================================================================//

static Node* Digit(elem_t number) {

    return CreateNewNode(NUMBER, &number);
}
//==========================================================================================================================================//


static int IsFunction(Node* tree) {
    int is_function = 0;
    FindVariable(tree, &is_function);
    return is_function;
}

static Node* FindVariable(Node* tree, int* is_function) {
    if (!tree) {
        return nullptr;
    }
    tree->left_branch  = FindVariable(tree->left_branch, is_function);
    tree->right_branch = FindVariable(tree->right_branch, is_function);

    if (tree->type == VAR) {
        *is_function = 1;
    }
    return tree;
}