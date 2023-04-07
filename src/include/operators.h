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

enum Functions {
    NUM     = -10,
    SQRT    = 0,
    SIN     = 1,
    COS     = 2,
    TG      = 3,
    CTG     = 4,
    LOG     = 5,
    LN      = 6,
    EXP     = 7,
    SH      = 8,
    CH      = 9,
    CTH     = 10,
    TH      = 11,
};

enum Brackets {
    OPEN_BRACKET   = '(',
    CLOSE_BRACKET  = ')',
};

#endif