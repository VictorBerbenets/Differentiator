#include "differentiator.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "stack.h"

// Xelatex - из консооли сделать .tex -> .pdf 
// 1. много срет в консоль можно перенаправить вывод
// 2. Жестко обсирается при малейшей ошибке
// 3. Нужно долбиться с русским языком

                                   

int main(int argc, char** argv) {

    printf("%zd\n", sizeof(Node));
    if (argc > 2) {
        printf("Too many arguments of the command line\n");
        return -1;
    }
    char* file_name = argv[1];
    printf("file_name = %s\n", file_name);

    Node* tree = ConstructTree(file_name);
    TreeDump(tree);

    // printf("%lg\n", Ebal(tree));
    Node* diff_tree = Diff(tree, "x");
    // PrintTree(diff_tree);
    // for (int i = 0; i < 10; i++) {
    // DeleteTree(diff_tree);

    //     diff_tree = Diff(tree);
    // }
    PrintTreeToFile(diff_tree, PRE_ORDER);
    PrintTreeToFile(diff_tree, IN_ORDER);
    PrintTreeToFile(diff_tree, POST_ORDER);
    TreeDump(diff_tree);
    
    Node* simple_tree = nullptr;
    int flag_ = 1;
    simple_tree = SimplifyTree(diff_tree, &flag_);

    // while(flag_ == 1) {
    //     flag_ = 0;
    //     simple_tree = SimplifyTree(simple_tree, &flag_);
    //     printf("flag = %d\n", flag_);
    TreeDump(simple_tree);

    // }

    // PrintTree(diff_tree);
    DeleteTree(tree);
    DeleteTree(simple_tree);
    // DeleteTree(diff_tree);
    // // PrintTreeToFile(tree, IN_ORDER);

    return 0;
}