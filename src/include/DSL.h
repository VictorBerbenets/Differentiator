#ifndef DSL_H
#define DSL_H

#define PrintWarningForDivisor() fprintf(stderr, "" Purple "" White"Warning:" Gray "" White "%s:%d:" Gray "\n\t|Trying to divide by 0\n",\
                                __PRETTY_FUNCTION__, __LINE__);

#define PrintWarningInvalidOper() fprintf(stderr, "" Purple "" White"Warning:" Gray "" White "%s:%d:" Gray "\n\t|Invalid operator: %c\n",\
                                __PRETTY_FUNCTION__, __LINE__, node_ptr->value.oper); 
#define L  node->left_branch
#define R  node->right_branch

#define dL Diff(L, var_name)
#define dR Diff(R, var_name)
#define cL CopyTree(node->left_branch, duplic_tree)
#define cR CopyTree(node->right_branch, duplic_tree)

#define ADD(L, R) CreateNewNode(OPER, &Add, L, R)
#define SUB(L, R) CreateNewNode(OPER, &Sub, L, R)
#define MUL(L, R) CreateNewNode(OPER, &Mul, L, R)
#define DIV(L, R) CreateNewNode(OPER, &Div, L, R)
#define POW(L, R) CreateNewNode(OPER, &Pow, L, R)

#define SIN_(L, R)    CreateNewNode(FUNC,  &Sin,    L, R)
#define COS_(L, R)    CreateNewNode(FUNC,  &Cos,    L, R)
#define TG_(L, R)     CreateNewNode(FUNC,  &Tg,     L, R)
#define CTG_(L, R)    CreateNewNode(FUNC,  &Ctg,    L, R)
#define CH_(L, R)     CreateNewNode(FUNC,  &Ch,     L, R)
#define SH_(L, R)     CreateNewNode(FUNC,  &Sh,     L, R)
#define TH_(L, R)     CreateNewNode(FUNC,  &Th,     L, R)
#define CTH_(L, R)    CreateNewNode(FUNC,  &Cth,    L, R)
#define EXP_(L, R)    CreateNewNode(FUNC,  &Exp,    L, R)
#define LN_(L, R)     CreateNewNode(FUNC,  &Ln,     L, R)
#define SQRT_(L, R)   CreateNewNode(FUNC,  &Sqrt,   L, R)
#define ARCSIN_(L, R) CreateNewNode(FUNC,  &Arcsin, L, R)
#define ARCCOS_(L, R) CreateNewNode(FUNC,  &Arccos, L, R)
#define ARCTG_(L, R)  CreateNewNode(FUNC,  &Arctg,  L, R)
#define ARCCTG_(L, R) CreateNewNode(FUNC,  &Arcctg, L, R)
#define SQRT_(L, R)   CreateNewNode(FUNC,  &Sqrt,   L, R)

#define CONST_()    Digit(0)
#define VAR_()     if (!strcmp(node->value.var, var_name)) { \
                       return Digit(1);                       \
                   }                                           \
                   return Digit(0);

#define GetValue() Ebal(node_ptr->left_branch, value, var_name)
#endif