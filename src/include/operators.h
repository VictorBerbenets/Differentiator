#ifndef operators_h
#define operators_h

const int NUMBER        = -1;
const int OPER          = -2;
const int VAR           = -3;
const int FUNC          = -4;
const int NotAType      = -5;
const int MaxVarSize    = 100;

enum Operations {

    OP_ADD  = '+',
    OP_SUB  = '-',
    OP_MUL  = '*',
    OP_DIV  = '/',

};

enum Functions {
    OP_POW  = '^',
    OP_SQRT,
    NUM,
    SIN,
    COS,
    TG,
    CTG,
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