#include "include//differentiator.h"
//------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------Constants and functions-------------------------------------------------- -----------------//
//Operation functions
static Node* CopyNode(Node* node_to_copy);
static Node* MulDerivative(Node* node);
static Node* AddDerivative(Node* node);
static Node* DivDerivative(Node* node);
static Node* PowDerivative(Node* node);

//Differentiation functions
static Node* DiffSin(Node* node);
static Node* DiffCos(Node* node);
static Node* DiffTg (Node* node);
static Node* DiffCtg(Node* node);
static Node* DiffLog(Node* node);
static Node* DiffLn (Node* node);
static Node* DiffExp(Node* node);
static Node* DiffSh (Node* node);
static Node* DiffCh (Node* node);
static Node* DiffCth(Node* node);

//Operators
static const int Sub  = OP_SUB;
static const int Add = OP_ADD;
static const int Mul  = OP_MUL;
static const int Div  = OP_DIV;
static const int Pow  = POW;

//Functions
static const int Sin  = SIN;
static const int Cos  = COS;
static const int Tg   = TG;
static const int Ctg  = CTG;
static const int Exp  = EXP;
//******************************************************************************************************************************************//
//---------------------------------------------------Function   bodies----------------------------------------------------------------------//

Node* Diff(Node* node) {

    static int const_deriv    = 0;
    static elem_t var_deriv   = 1;
    // Node* new_node = nullptr;

    if (!node) {
        return nullptr;
    }
    switch(node->type) {
        case NUMBER: return CreateNewNode(NUMBER, &const_deriv); 
        case VAR:    return CreateNewNode(NUMBER, &var_deriv);
        case OPER:
            switch(node->value.oper) {
                case OP_ADD: 
                case OP_SUB: return AddDerivative(node);
                case OP_MUL: return MulDerivative(node);
                case OP_DIV: return DivDerivative(node);           
                case POW:    return PowDerivative(node);     
                default:     break;   
            }
        case FUNC:
            switch (node->value.func) {
                case SIN: return DiffSin(node);
                case COS: return DiffCos(node);
                case TG:  return DiffTg(node);
                case CTG: return DiffCtg(node);
                case LOG: return DiffLog(node);
                case LN:  return DiffLn(node);
                case EXP: return DiffExp(node);
                case SH:  return DiffSh(node);
                case CH:  return DiffCh(node);
                case CTH: return DiffCth(node);
                case TH:  printf("fucker!\n");
                default:  break;
            }  
    }
    return nullptr;
}

//==========================================================================================================================================//

 Node* CopyTree(Node* tree_to_copy, Node* duplic_tree) {

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

    Node* new_node = CreateNewNode(OPER, &Add);
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


static Node* DiffSin(Node* node) {

    Node* duplicate_tree = nullptr;

    Node* new_node = CreateNewNode(OPER, &Mul);
    new_node->left_branch = CreateNewNode(FUNC, &Cos);
    new_node->left_branch->left_branch = CopyTree(node->left_branch, duplicate_tree);
    new_node->right_branch = Diff(node->left_branch);
    return new_node;
}

static Node* DiffCos(Node* node) {
    Node* duplicate_tree = nullptr;
    elem_t multiplier = -1;

    Node* new_node = CreateNewNode(OPER, &Mul);
    new_node->left_branch = CreateNewNode(OPER, &Mul);
    new_node->left_branch->left_branch  = CreateNewNode(FUNC, &Sin);
    new_node->left_branch->right_branch = CreateNewNode(NUMBER, &multiplier);
    new_node->left_branch->left_branch->left_branch = CopyTree(node->left_branch, duplicate_tree);
    new_node->right_branch = Diff(node->left_branch);
    return new_node;
}
static Node* DiffTg (Node* node) {
    Node* duplicate_tree = nullptr;
    elem_t delim   = 1;
    elem_t pow_val = 2;

    Node* new_node = CreateNewNode(OPER, &Mul);
    new_node->left_branch = CreateNewNode(OPER, &Div);
    new_node->left_branch->left_branch  = CreateNewNode(NUMBER, &delim);
    new_node->left_branch->right_branch = CreateNewNode(OPER, &Pow);
    new_node->left_branch->right_branch->left_branch  = CreateNewNode(FUNC, &Cos);
    new_node->left_branch->right_branch->right_branch = CreateNewNode(NUMBER, &pow_val);
    new_node->left_branch->right_branch->left_branch->left_branch = CopyTree(node->left_branch, duplicate_tree);
    new_node->right_branch = Diff(node->left_branch);

    return new_node;
}
static Node* DiffCtg(Node* node) {
    Node* duplicate_tree = nullptr;
    elem_t delim   = -1;
    elem_t pow_val =  2;

    Node* new_node = CreateNewNode(OPER, &Mul);
    new_node->left_branch = CreateNewNode(OPER, &Div);
    new_node->left_branch->left_branch  = CreateNewNode(NUMBER, &delim);
    new_node->left_branch->right_branch = CreateNewNode(OPER, &Pow);
    new_node->left_branch->right_branch->left_branch  = CreateNewNode(FUNC, &Sin);
    new_node->left_branch->right_branch->right_branch = CreateNewNode(NUMBER, &pow_val);
    new_node->left_branch->right_branch->left_branch->left_branch = CopyTree(node->left_branch, duplicate_tree);
    new_node->right_branch = Diff(node->left_branch);

    return new_node;
}
static Node* DiffLog(Node* node) {

}
static Node* DiffLn (Node* node) {
    Node* duplicate_tree = nullptr;
    elem_t delim   = 1;

    Node* new_node = CreateNewNode(OPER, &Mul);
    new_node->left_branch = CreateNewNode(OPER, &Div);
    new_node->left_branch->left_branch  = CreateNewNode(NUMBER, &delim);
    new_node->left_branch->right_branch = CopyTree(node->left_branch, duplicate_tree);
    new_node->right_branch = Diff(node->left_branch);

    return new_node;
}
static Node* DiffExp(Node* node) {
    Node* duplicate_tree = nullptr;
    elem_t delim   = 1;

    Node* new_node = CreateNewNode(OPER, &Mul);
    new_node->left_branch = CopyTree(node, duplicate_tree);
    new_node->right_branch = Diff(node->left_branch);

    return new_node;
}
static Node* DiffSh (Node* node) {

}
static Node* DiffCh (Node* node) {

}
static Node* DiffCth(Node* node) {

}