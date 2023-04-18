#ifndef parsing_h
#define parsing_h

#include "Node.h"
#include <ctype.h>

const int MaxFuncSize = 100;
const int NotFunction = -10;

enum Errors {

    DIVISION_BY_ZERO           = 1,
    MISSING_CLOSE_BRACKET      = 2,
    UNEXPECTED_SYMBOL          = 3,
    INVALID_DATA               = 4,
    INVALID_FUNCTION_NAME_SIZE = 5,
};



// enum FUNCTIONS {
//     _SIN = 0,
//     _COS = 1,
//     _TG  = 2,
//     _CTG = 3,
//     _SH  = 4,
//     _CH  = 5,
//     _TH  = 6,
//     _CTH = 7,
//     _ARCSIN = 8,
//     _ARCCOS = 9,
//     _ARCTG  = 10,
//     _ARCCTG = 11,
//     _EXP    = 12,
//     _LN     = 13,
//     _SQRT   = 14,
// };

typedef struct {
    const char func_name[MaxFuncSize];
    int func_id;
} FuncInfo;


Node*  GetG(char** string);
Node*  GetE(char** string);
Node*  GetT(char** string);
Node*  GetS(char** string);
Node*  GetP(char** string);
Node*  GetF(char** string);
Node*  GetN(char** string);
Node* GetVar(char** string);

#endif