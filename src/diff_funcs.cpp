#include "include//differentiator.h"
//------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------Constants and functions-------------------------------------------------- -----------------//
//Operation functions
static Node* CopyNode(Node* node_to_copy);
static Node* Digit(elem_t number);
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
static Node* DiffTh (Node* node);
static Node* DiffCth(Node* node);

//Operators
static const int Sub  = OP_SUB;
static const int Add = OP_ADD;
static const int Mul  = OP_MUL;
static const int Div  = OP_DIV;
static const int Pow  = OP_POW;

//Functions
static const int Sin  = SIN;
static const int Cos  = COS;
static const int Tg   = TG;
static const int Ctg  = CTG;
static const int Sh   = SH;
static const int Ch   = CH;
static const int Th   = TH;
static const int Cth  = CTH;
//******************************************************************************************************************************************//
//---------------------------------------------------Function   bodies----------------------------------------------------------------------//

// CreateNode (..., node* left_child, node* right_child)
//

static Node* duplic_tree = nullptr;

#define L  node->left_branch
#define R  node->right_branch

#define dL Diff(L)
#define dR Diff(R)
#define cL CopyTree(node->left_branch, duplic_tree)
#define cR CopyTree(node->right_branch, duplic_tree)
// #define dR Diff(cur_node->right)

#define ADD(L, R) CreateNewNode(OPER, &Add, L, R)
#define SUB(L, R) CreateNewNode(OPER, &Sub, L, R)
#define MUL(L, R) CreateNewNode(OPER, &Mul, L, R)
#define DIV(L, R) CreateNewNode(OPER, &Div, L, R)
#define POW(NODE, degree) CreateNewNode(OPER, &Pow, NODE, Digit(degree))

#define SIN_(L, R) CreateNewNode(FUNC, &Sin, L, R)
#define COS_(L, R) CreateNewNode(FUNC, &Cos, L, R)
#define TG_(L, R)  CreateNewNode(FUNC,  &Tg,  L, R)
#define CTG_(L, R) CreateNewNode(FUNC, &Ctg, L, R)
#define CH_(L, R)  CreateNewNode(FUNC,  &Ch,  L, R)
#define Sh_(L, R)  CreateNewNode(FUNC,  &Sh,  L, R)
#define TH_(L, R)  CreateNewNode(FUNC,  &Th,  L, R)
#define CTH_(L, R) CreateNewNode(FUNC, &Cth, L, R)
#define CONST_()   Digit(0)
#define VAR_()     Digit(1)
// #define SIN(L, R) MUL(MUL(DIGIT(-1), COS(cL)), dL)

static Node* Digit(elem_t number) {

    return CreateNewNode(NUMBER, &number);
}
// case OP_ADD:
//     return ADD (dL, dR)


Node* Diff(Node* node) {

    if (!node) {
        return nullptr;
    }
    switch(node->type) {
        case OPER:
            switch(node->value.oper) {
                case OP_ADD: return ADD(dL, dR);
                case OP_SUB: return SUB(dL, dR);
                case OP_MUL: return ADD(MUL(dL, cR), MUL(cL, dR));
                case OP_DIV: return DIV(SUB(MUL(dL, cR), MUL(cL, dR)),POW(cR, 2));  
                // case OP_POW: return POW(cL, )    
                default:  printf("No such operator: %d\n", node->value.oper);  
            }
        case FUNC:
            switch (node->value.func) {
                case SIN: return MUL(COS_(cL, nullptr), dL);
                case COS: return MUL(MUL(SIN_(cL, nullptr), dL), Digit(-1));
                case TG:  return DiffTg(node);
                case CTG: return DiffCtg(node);
                case LOG: return DiffLog(node);
                case LN:  return DiffLn(node);
                case EXP: return DiffExp(node);
                case SH:  return DiffSh(node);
                case CH:  return DiffCh(node);
                case CTH: return DiffCth(node);
                case TH:  return DiffTh(node);
                default:  break;
            } 
        case NUMBER: return CONST_(); 
        case VAR:    return VAR_(); 
        default:
            printf ("No such node\n");
            return nullptr;
    }
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

//==========================================================================================================================================//

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

//==========================================================================================================================================//

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

//==========================================================================================================================================//

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

//==========================================================================================================================================//

static Node* DiffLog(Node* node) {

}

//==========================================================================================================================================//

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

//==========================================================================================================================================//

static Node* DiffExp(Node* node) {
    Node* duplicate_tree = nullptr;
    elem_t delim   = 1;

    Node* new_node = CreateNewNode(OPER, &Mul);
    new_node->left_branch = CopyTree(node, duplicate_tree);
    new_node->right_branch = Diff(node->left_branch);

    return new_node;
}

//==========================================================================================================================================//

static Node* DiffSh (Node* node) {
    Node* duplicate_tree = nullptr;

    Node* new_node = CreateNewNode(OPER, &Mul);
    new_node->left_branch  = CreateNewNode(FUNC, &Ch);
    new_node->left_branch->left_branch = CopyTree(node->left_branch, duplicate_tree);
    new_node->right_branch = Diff(node->left_branch);

    return new_node;
}

//==========================================================================================================================================//

static Node* DiffCh (Node* node) {
    Node* duplicate_tree = nullptr;
    elem_t delim   = 1;

    Node* new_node = CreateNewNode(OPER, &Mul);
    new_node->left_branch  = CreateNewNode(FUNC, &Sh);
    new_node->left_branch->left_branch = CopyTree(node->left_branch, duplicate_tree);
    new_node->right_branch = Diff(node->left_branch);

    return new_node;
}

//==========================================================================================================================================//

static Node* DiffTh(Node* node) {
    Node* duplicate_tree = nullptr;
    elem_t delim   = 1;
    elem_t pow_val = 2;

    Node* new_node = CreateNewNode(OPER, &Mul);
    new_node->left_branch = CreateNewNode(OPER, &Div);
    new_node->left_branch->left_branch  = CreateNewNode(NUMBER, &delim);
    new_node->left_branch->right_branch = CreateNewNode(OPER, &Pow);
    new_node->left_branch->right_branch->left_branch  = CreateNewNode(FUNC, &Ch);
    new_node->left_branch->right_branch->right_branch = CreateNewNode(NUMBER, &pow_val);
    new_node->left_branch->right_branch->left_branch->left_branch = CopyTree(node->left_branch, duplicate_tree);
    new_node->right_branch = Diff(node->left_branch);

    return new_node;
}

//==========================================================================================================================================//

static Node* DiffCth(Node* node) {
    Node* duplicate_tree = nullptr;
    elem_t delim   = -1;
    elem_t pow_val =  2;

    Node* new_node = CreateNewNode(OPER, &Mul);
    new_node->left_branch = CreateNewNode(OPER, &Div);
    new_node->left_branch->left_branch  = CreateNewNode(NUMBER, &delim);
    new_node->left_branch->right_branch = CreateNewNode(OPER, &Pow);
    new_node->left_branch->right_branch->left_branch  = CreateNewNode(FUNC, &Sh);
    new_node->left_branch->right_branch->right_branch = CreateNewNode(NUMBER, &pow_val);
    new_node->left_branch->right_branch->left_branch->left_branch = CopyTree(node->left_branch, duplicate_tree);
    new_node->right_branch = Diff(node->left_branch);

    return new_node;
}
//==========================================================================================================================================//
