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
    
    // printf("EBAL = %lg\n", Ebal(&treee, 0, "x"));
    // TreeDump(tree.Root);

    ConverteTreeToPdf(&tree);
    PrintMakloren(&tree, "data//makloren.tex", 0, 5);
    DeleteAllTrees(&tree);

    return 0;
}


