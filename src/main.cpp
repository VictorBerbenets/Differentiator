#include "differentiator.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "stack.h"


int main(int argc, char** argv) {

    if (argc > 2) {
        printf("Too many arguments of the command line\n");
        return -1;
    }
    char* file_name = argv[1];
    printf("file_name = %s\n", file_name);

    Node* tree = ConstructTree(file_name);
    // printf("%lg\n", Ebal(tree));
    // Node* copy_tree = nullptr;
    // CopyTree(tree, &copy_tree);
    Node* diff_tree = Diff(tree);
    PrintTreeToFile(diff_tree, IN_ORDER);
    PrintTreeToFile(diff_tree, PRE_ORDER);
    PrintTreeToFile(diff_tree, POST_ORDER);
    TreeDump(diff_tree);
    TreeDump(tree);
    // TreeDump(copy_tree);

    // DeleteTree(copy_tree);
    DeleteTree(tree);
    DeleteTree(diff_tree);
    // PrintTreeToFile(tree, IN_ORDER);

    return 0;
}