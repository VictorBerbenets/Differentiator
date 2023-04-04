#include "differentiator.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "stack.h"


int main(int argc, char** argv) {

    Function_info f[] = {{"sin", 1}, {"cos", 2}};
    printf("%s   %d\n", f[1].func_name, f[1].func_id);
    if (argc > 2) {
        printf("Too many arguments of the command line\n");
        return -1;
    }
    char* file_name = argv[1];
    printf("file_name = %s\n", file_name);

    Node* tree = ConstructTree(file_name);
    TreeDump(tree);

    // printf("%lg\n", Ebal(tree));
    Node* diff_tree = Diff(tree);
    PrintTree(diff_tree);
    // for (int i = 0; i < 10; i++) {
    // DeleteTree(diff_tree);

    //     diff_tree = Diff(tree);
    // }
    PrintTreeToFile(diff_tree, PRE_ORDER);
    PrintTreeToFile(diff_tree, IN_ORDER);
    PrintTreeToFile(diff_tree, POST_ORDER);
    TreeDump(diff_tree);

    DeleteTree(tree);
    DeleteTree(diff_tree);
    // // PrintTreeToFile(tree, IN_ORDER);

    return 0;
}