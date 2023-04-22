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
    char string[100] = "     hello world, vitia!\n";
    char copy[100] = {0};
    int n = 0;
    char* string_ptr = string;
    while(sscanf(string_ptr, " %[^ \n]%n", copy, &n)) {
        printf("copy = <%s>; n = %d\n", copy, n);
        string_ptr += n;
        if (*string_ptr == '\n') {
            break;
        }
        printf("ptr = <%s>\n", string_ptr);
    }
    // printf("%zd\n", sizeof(Node));
    if (argc > 2) {
        printf("Too many arguments of the command line\n");
        return -1;
    }
    char* file_name = argv[1];
    Tree tree = {};

    ConstructTree(argv[1], &tree);
    TreeDump(tree.Root);

    DerivativeOnALLVars(&tree);
    ConverteTreeToPdf(&tree);

    DeleteAllTrees(&tree);
    return 0;
}

