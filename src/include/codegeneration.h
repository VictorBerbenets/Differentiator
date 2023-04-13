
CMP(SIN, "sin",   MUL(COS_(cL, nullptr), dL), Sin)
CMP(COS, "cos",   MUL(MUL(SIN_(cL, nullptr), dL), Digit(-1)), Cos)
CMP(TG,  "tg",    MUL(DIV(Digit(1), POW(COS_(cL, nullptr), Digit(2))), dL), Tg)
CMP(CTG, "ctg",   MUL(DIV(Digit(-1), POW(SIN_(cL, nullptr), Digit(2))), dL), Ctg)
CMP(LN,  "ln",    MUL(DIV(Digit(1), cL), dL), Ln)
CMP(SH,  "sh",    MUL(CH_(cL, nullptr), dL), Sh)
CMP(CH,  "ch",    MUL(SH_(cL, nullptr), dL), Ch)
CMP(TH,  "th",    MUL(DIV(Digit(1), POW(CH_(cL, nullptr), Digit(2))), dL), Th)
CMP(CTH, "cth",   MUL(DIV(Digit(-1), POW(SH_(cL, nullptr), Digit(2))), dL), Cth)
CMP(EXP, "exp",   MUL(EXP_(cL, nullptr), dL), Exp)
CMP(SQRT, "sqrt", MUL(DIV(Digit(1), MUL(Digit(2), SQRT_(cL, nullptr))), dL), Sqrt)

