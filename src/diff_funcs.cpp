#include "include//differentiator.h"
//------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------Constants and functions-------------------------------------------------- -----------------//
// void CopyTree(Node* node_to_copy, Node* duplic_tree);
static Node* CopyNode(Node* node_to_copy);
static Node* MulDerivative(Node* node);
static Node* AddDerivative(Node* node);
static Node* DivDerivative(Node* node);
static Node* PowDerivative(Node* node);
static const int Sub  = OP_SUB;
static const int Plus = OP_ADD;
static const int Mul  = OP_MUL;
static const int Div  = OP_DIV;
static const int Pow  = OP_POW;
//******************************************************************************************************************************************//
//---------------------------------------------------Function   bodies----------------------------------------------------------------------//

Node* Diff(Node* node) {

    static int const_deriv    = 0;
    static elem_t var_deriv   = 1;
    static Node* duplic_tree  = nullptr;
    Node* new_node = nullptr;

    if (!node) {
        return nullptr;
    }
    switch(node->type) {
        case OPER:
            switch(node->value.oper) {
                case OP_ADD: 
                case OP_SUB: return AddDerivative(node);
                case OP_MUL: return MulDerivative(node);
                case OP_DIV: return DivDerivative(node);           
                case OP_POW: return PowDerivative(node);     
                default:
                    break;   
            }
        case NUMBER: return CreateNewNode(NUMBER, &const_deriv); 
        case VAR:    return CreateNewNode(NUMBER, &var_deriv);
        case FUNC:
            switch (node->value.func_name)
            {
                case SIN:
                case COS:
                case TG:
                case CTG:
                case LOG:
                case LN:
                case EXP:
                case SH:
                case CH:
                case CTH:
                case TH: printf("fucker!\n");
                default:
                    break;
            }   

    }
    return nullptr;
}

//==========================================================================================================================================//

static Node* CopyTree(Node* tree_to_copy, Node* duplic_tree) {

	if (tree_to_copy == nullptr){
		duplic_tree = nullptr;
		return nullptr;
	}
    duplic_tree = CopyNode(tree_to_copy);
	duplic_tree->left_branch  = CopyTree(tree_to_copy->left_branch,  duplic_tree->left_branch);
	duplic_tree->right_branch = CopyTree(tree_to_copy->right_branch, duplic_tree->right_branch);

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

static Node* MulDerivative(Node* node) {

    Node* new_node = CreateNewNode(OPER, &Mul);
    Node* duplicate_tree = nullptr;
    new_node->left_branch  = CreateNewNode(OPER, &Mul);
    new_node->right_branch = CreateNewNode(OPER, &Mul);  

    
    new_node->left_branch->right_branch = CopyTree(node->right_branch, duplicate_tree);
    new_node->right_branch->left_branch = CopyTree(node->left_branch, duplicate_tree);


    new_node->left_branch->left_branch   = Diff(node->left_branch);  
    new_node->right_branch->right_branch = Diff(node->right_branch); 
    return new_node;
}

//==========================================================================================================================================//

static Node* DivDerivative(Node* node) {

    Node* duplicate_tree = nullptr;
    Node* new_node = CreateNewNode(OPER, &Div);
    new_node->left_branch  = CreateNewNode(OPER, &Sub);
    new_node->left_branch->left_branch  = CreateNewNode(OPER, &Mul);
    new_node->left_branch->right_branch = CreateNewNode(OPER, &Mul);

    new_node->left_branch->right_branch->left_branch = CopyTree(node->left_branch, duplicate_tree);
    new_node->left_branch->left_branch->right_branch = CopyTree(node->right_branch, duplicate_tree);

    new_node->left_branch->right_branch->right_branch = Diff(node->right_branch);
    new_node->left_branch->left_branch->left_branch   = Diff(node->left_branch);

    new_node->right_branch = CreateNewNode(OPER, &Pow);
    new_node->right_branch->left_branch  = CopyTree(node->right_branch, duplicate_tree);;
    elem_t degree = 2;
    new_node->right_branch->right_branch = CreateNewNode(NUMBER, &degree);

    return new_node;
}

//==========================================================================================================================================//

static Node* PowDerivative(Node* node) {
    Node* new_node    = nullptr;
    Node* duplic_tree = nullptr;
    if (node->right_branch->type == NUMBER) {
        new_node = CreateNewNode(OPER, &Mul);

        new_node->left_branch  = CopyTree(node->right_branch, duplic_tree);;
        new_node->right_branch = CopyTree(node, duplic_tree);;

        new_node->right_branch->left_branch = CopyTree(node->left_branch, duplic_tree);;

        new_node->right_branch->right_branch = CopyTree(node->right_branch, duplic_tree);;
        new_node->right_branch->right_branch->value.number--;
        return new_node;
    }

}

//==========================================================================================================================================//

static Node* AddDerivative(Node* node) {

    Node* new_node = CopyNode(node);
    new_node->left_branch  = Diff(node->left_branch);
    new_node->right_branch = Diff(node->right_branch);
    return new_node;
}

//==========================================================================================================================================//
