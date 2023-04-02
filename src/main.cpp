#include "differentiator.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>


int main(int argc, char** argv) {

    if (argc > 2) {
        printf("Too many arguments of the command line\n");
        return -1;
    }
    char* file_name = argv[1];
    printf("file_name = %s\n", file_name);

    Node* tree = ConstructTree(file_name);
    // printf("%lg\n", Ebal(tree));
    PrintTreeToFile(tree, IN_ORDER);
    PrintTreeToFile(tree, PRE_ORDER);
    PrintTreeToFile(tree, POST_ORDER);

    TreeDump(tree);
    DeleteTree(tree);
    
    return 0;
}