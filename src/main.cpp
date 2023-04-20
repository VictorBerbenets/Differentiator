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

    printf("%zd\n", sizeof(Node));
    if (argc > 2) {
        printf("Too many arguments of the command line\n");
        return -1;
    }
    char* file_name = argv[1];
    printf("file_name = %s\n", file_name);
    Buffer buff = ReadFile(file_name);
    printf("BUFFER = <%s>\n", buff.buffer);
    Tree tree1 = {};
    Node* tree = BuildTree(&buff.buffer);
    // Node* tree = ConstructTree(file_name);
    TreeDump(tree);
    // int x = 0;

    Node* diff_tree = Diff(tree, "x");
    PrintTreeToFile(diff_tree, PRE_ORDER);
    PrintTreeToFile(diff_tree, IN_ORDER);
    PrintTreeToFile(diff_tree, POST_ORDER);
    TreeDump(diff_tree);
    
    Node* simple_tree = nullptr;
    simple_tree = SimplifyTree(diff_tree);
    TreeDump(simple_tree);
    ConverteTreeToPdf(tree, simple_tree, "x");

    printf("value = %lg\n", Ebal(simple_tree, 0.5, "x"));
    TreeDump(simple_tree);


    DeleteTree(tree);
    DeleteTree(simple_tree);


    return 0;
}