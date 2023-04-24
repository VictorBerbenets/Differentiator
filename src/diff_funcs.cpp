#include "include//differentiator.h"
//------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------Constants and functions-------------------------------------------------- -----------------//
//functions
static Node* PowFunction(Node* node, const char* var_name);
static Node* FindVariable(Node* tree, int* is_function);
static int IsFunction(Node* tree);
static Node* CopyNode(Node* node_to_copy);
static Node* Digit(elem_t number);
static Node* duplic_tree = nullptr;

#define POW_FUNCTION(node, var_name) PowFunction(node, var_name);

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
                case OP_POW: return POW_FUNCTION(node, var_name);
                default: printf("no such operator = %d\n", node->value.oper); return nullptr;
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

    if (node_to_copy->type == VAR) {
        int var_len = strlen(node_to_copy->value.var);
        copied_version->value.var = (char*) calloc(strlen(node_to_copy->value.var) + 1, sizeof(char));
        memcpy(copied_version->value.var, node_to_copy->value.var, sizeof(char) * var_len);
    }

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

//==========================================================================================================================================//

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

//==========================================================================================================================================//

static Node* PowFunction(Node* node, const char* var_name) {
    if (R->type == NUMBER) {
        return MUL(MUL(POW(cL, Digit(R->value.number - 1)), cR), dL);  
    }
    else if (IsFunction(R) && !IsFunction(L)) {
        return MUL(MUL( LN_(cL, nullptr), POW(cL, cR) ), dR);
    }
    else if (IsFunction(R) && IsFunction(L)) {
        return MUL( POW(cL, cR), ADD(MUL(dR, LN_(cL, nullptr)), MUL( MUL(DIV(Digit(1), cL), dL ) , cR))) ; // MUL( POW(cL, cR), ADD(MUL(dL, LN_(cR, nullptr)), MUL(cL, MUL(DIV(Digit(1), cR), dR)))
    }
    return nullptr;
}

//==========================================================================================================================================//

void DerivativeOnALLVars(Tree* tree) {
    Validator(tree == nullptr, "tree is nullptr", return ;);
    for(int tree_number = 0; tree_number < tree->var_counter; tree_number++) {
        tree->Diff_trees[tree_number] = Diff(tree->Diff_trees[tree_number], tree->variables[tree_number].var_name);
        tree->Diff_trees[tree_number] = SimplifyTree(tree->Diff_trees[tree_number]);
    }
}
//==========================================================================================================================================//

void CreatePartialDerivatives(Tree* tree) {
    Validator(tree == nullptr, "tree is nullptr", return ;);
    if (!tree->var_counter) {
        printf("I have't any vars. Can't derivative\n");
        return ;
    }

    Node** check_calloc = (Node**) calloc(tree->var_counter, sizeof(Node*));
    Validator(check_calloc == nullptr, "in allocating memory by calloc", return ;);
    tree->Diff_trees = check_calloc;

    Node* diff_tree   = nullptr;
    for(int tree_number = 0; tree_number < tree->var_counter; tree_number++) {
        diff_tree = Diff(tree->Root, tree->variables[tree_number].var_name);
        tree->Diff_trees[tree_number] = SimplifyTree(diff_tree);
    }
}