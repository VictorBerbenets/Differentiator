#ifndef operators_h
#define operators_h

const int NUMBER        = -1;
const int OPER          = -2;
const int VAR           = -3;
const int NotAType      = -4;
const int MaxVarSize = 100;

enum Operations {

    OP_ADD  = '+',
    OP_SUB  = '-',
    OP_MUL  = '*',
    OP_DIV  = '/',
    SIN,
    COS,
    CTG,
    TG,
    LOG,
    LN,
    EXP,
    SH,
    CH,
    CTH,
    TH,
};

enum Brackets {
    OPEN_BRACKET   = '(',
    CLOSE_BRACKET  = ')',
};

#endif