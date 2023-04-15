#ifndef operators_h
#define operators_h

const int NUMBER        = -1;
const int OPER          = -2;
const int VAR           = -3;
const int FUNC          = -4;
const int NotAType      = -5;

enum Operations {

    OP_ADD  = '+',
    OP_SUB  = '-',
    OP_MUL  = '*',
    OP_DIV  = '/',
    OP_POW  = '^',

};

const int NUM = -10;

enum Functions {

    #define CMP(func_id, ...) _##func_id,
    #include "codegeneration.h"
    #undef CMP
};



enum Brackets {
    OPEN_BRACKET   = '(',
    CLOSE_BRACKET  = ')',
};
#endif