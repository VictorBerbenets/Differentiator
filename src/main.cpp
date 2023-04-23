#include "include/differentiator.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

// Xelatex - из консооли сделать .tex -> .pdf 
// 1. много срет в консоль можно перенаправить вывод
// 2. Жестко обсирается при малейшей ошибке
// 3. Нужно долбиться с русским языком

                                   

int main(int argc, char** argv) {

    if (argc > 2) {
        printf("Too many arguments of the command line\n");
        return -1;
    }
    Tree tree = {};

    ConstructTree(argv[1], &tree);
    TreeDump(tree.Root);

    DerivativeOnALLVars(&tree);
    ConverteTreeToPdf(&tree);

    DeleteAllTrees(&tree);
    return 0;
}

