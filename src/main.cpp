#include "include/differentiator.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>


int main(int argc, char** argv) {

    if (argc > 2) {
        printf("Too many arguments of the command line\n");
        return -1;
    }
    Tree tree = {};

    ConstructTree(argv[1], &tree);
    TreeDump(tree.Root);
    
    DerivativeOnALLVars(&tree);
    DerivativeOnALLVars(&tree);

    ConverteTreeToPdf(&tree);
    PrintMakloren(&tree, "data//makloren.tex", 3);
    DeleteAllTrees(&tree);

    return 0;
}


