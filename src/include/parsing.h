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

Node*  BuildTree(char** string);
Node*  GetExp(char** string);
Node*  GetT(char** string);
Node*  GetS(char** string);
Node*  GetP(char** string);
Node*  GetFunc(char** string);
Node*  GetNumber(char** string);
Node*  GetVar(char** string);

#endif