#include "include//differentiator.h"
//------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------Constants and functions-------------------------------------------------- -----------------//
//functions
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
                case OP_DIV: return DIV(SUB(MUL(dL, cR), MUL(cL, dR)),POW(cR, Digit(2)));  
                case OP_POW: 
                    if (R->type == NUMBER) {
                        return MUL(MUL(POW(cL, Digit(R->value.number - 1)), cR), dL);  
                    }
                default:  printf("No such operator: %d\n", node->value.oper);  return nullptr;
            }
        case FUNC:
            switch (node->value.func) {
                case SIN: return MUL(COS_(cL, nullptr), dL);
                case COS: return MUL(MUL(SIN_(cL, nullptr), dL), Digit(-1));
                case TG:  return MUL(DIV(Digit(1), POW(COS_(cL, nullptr), Digit(2))), dL);
                case CTG: return MUL(DIV(Digit(-1), POW(SIN_(cL, nullptr), Digit(2))), dL);
                case LN:  return MUL(DIV(Digit(1), cL), dL);
                case SH:  return MUL(CH_(cL, nullptr), dL);
                case CH:  return MUL(SH_(cL, nullptr), dL);
                case TH:  return MUL(DIV(Digit(1), POW(CH_(cL, nullptr), Digit(2))), dL);
                case CTH: return MUL(DIV(Digit(-1), POW(SH_(cL, nullptr), Digit(2))), dL);
                case EXP: return MUL(node, dL);
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
