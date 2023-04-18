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

Node*  GetG(char** string);
Node*  GetE(char** string);
Node*  GetT(char** string);
Node*  GetS(char** string);
Node*  GetP(char** string);
Node*  GetF(char** string);
Node*  GetN(char** string);
Node* GetVar(char** string);

#endif