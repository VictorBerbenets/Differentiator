#include "include/differentiator.h"

#define _print(...) fprintf(tree_pdf, __VA_ARGS__);

void ConverteTreeToPdf(Node** tree) {

    FILE* tree_pdf = fopen("data//derivative.pdf", "w+");
    Validator(tree_pdf == nullptr, "in oppening file", exit(READING_FILE_ERROR));

    WriteDerivativeToPdf(tree, tree_pdf);

    int is_fclosed = fclose(tree_pdf);
    Validator(is_fclosed != 0, "in closing file", exit(CLOSING_FILE_ERROR));    
}

void WriteDerivativeToPdf(Node** tree, FILE* tree_pdf) {
    if (!tree) {
        return ;
    }

    WriteDerivativeToPdf(&(*tree)->left_branch, tree_pdf);

    WriteDerivativeToPdf(&(*tree)->right_branch, tree_pdf);

}