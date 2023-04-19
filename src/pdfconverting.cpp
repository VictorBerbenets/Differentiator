#include "include/differentiator.h"

#define _print(...) fprintf(tree_pdf, __VA_ARGS__)

void ConverteTreeToPdf(Node* tree, Node* tree_diff) {
    const char header[] = "\\documentclass{article}\n"
                          "\\usepackage[utf8]{inputenc}\n"
                          "\\usepackage{float}\n"
                          "\\usepackage{pgfplots}\n"
                          "\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools}\n"
                          "\\usepackage{graphicx}\n"
                          "\\usepackage{comment}\n"

    "\\begin{document}\n";

    FILE* tree_pdf = fopen("data//derivative.tex", "w+");
    Validator(tree_pdf == nullptr, "in oppening file", exit(READING_FILE_ERROR));
    _print("%s\n", header);
    _print("Let's do this shit man and go to drink beer and play World Of Tanks like normal workers after hard day!\n\n");
    _print("$(");

    WriteTreeToPdf(tree, tree_pdf);
    _print(")^\\prime = ");
    WriteTreeToPdf(tree_diff, tree_pdf);
    _print("$\n");
    _print("\n\tThat's all, I hope your ass is satisfied\n");
    _print("\n\\end{document}");

    int is_fclosed = fclose(tree_pdf);

    system("cd data");
    system("pdflatex data//derivative.tex");
    system("cd ..");

    Validator(is_fclosed != 0, "in closing file", exit(CLOSING_FILE_ERROR));    
}

void WriteTreeToPdf(Node* tree, FILE* tree_pdf) {
    if (!tree) {
        return ;
    }
    if (tree->type == OPER) {
        if (tree->value.oper == OP_DIV) {
            _print(" \\frac{ ");
        }
        else if (tree->value.oper == OP_POW) {
            _print("(");
        }
        else if (tree->value.oper == OP_ADD || tree->value.oper == OP_SUB) {
            _print("(");
        }
        WriteTreeToPdf(tree->left_branch, tree_pdf);
        if (tree->value.oper == OP_DIV) {
            _print("}");
        }
        if (tree->type == OPER) {
            if (tree->value.oper == OP_POW) {
                _print(" %c{ ", tree->value.oper);
            }
            else if (tree->value.oper == OP_MUL) {
                _print(" \\cdot ");
            }
            else if (tree->value.oper == OP_ADD) {
                _print(" + ");
            }
            else if (tree->value.oper == OP_SUB) {
                _print(" - ");
            }
        }
        if (tree->value.oper == OP_DIV) {
            _print("{");
        }
        WriteTreeToPdf(tree->right_branch, tree_pdf);
        if (tree->value.oper == OP_POW) {
            _print("})");
        }
        else if (tree->value.oper == OP_DIV) {
            _print("}");
        }
        else if (tree->value.oper == OP_ADD || tree->value.oper == OP_SUB) {
            _print(")");
        }
    }
    if (tree->type == FUNC) {
        if (!strcmp("sqrt", _Diff_Functions_[tree->value.func].func_name)) {
            _print("\\sqrt{");
        }
        else {
            _print(" %s( ", _Diff_Functions_[tree->value.func].func_name);
        }
        WriteTreeToPdf(tree->left_branch, tree_pdf);
        if (!strcmp("sqrt", _Diff_Functions_[tree->value.func].func_name)) {
            _print("}");
        }
        else {
            _print(")");
        }
    }

    if (tree->type == VAR) {
        _print("%s", tree->value.var);
    }
    if (tree->type == NUMBER) {
        if (tree->value.number < 0) {
            _print("(%lg)\n", tree->value.number);
        }
        else {
            _print("%lg", tree->value.number);
        }
    }

}

// if tree->left && tree->right
//left
//node
//right