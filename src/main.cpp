#include "differentiator.h"
#include <ctype.h>
// #include <string>
#include <string.h>
// #include <iostream>
// using std::string;

int main(int argc, char** argv) {
    // char arr[] = {'a', 'b', 'c', '\0'};
    // printf("str = %s\n", strstr(arr, "b"));
    // char arr1[543] = "   Hello   world   ";
    // char* ptr      = arr1;
    // char arr3[100] = {};
    // int n = 0;
    // sscanf(ptr, " %[^ *]%n", arr3, &n);
    // printf("arr3 = %s,   n = %d\n", arr3,  n);
    // ptr += 5;
    // sscanf(ptr, " %s", arr3);
    // printf("arr3 = %s\n", arr3);
    // ptr += 5;

    // sscanf(ptr, " <%s>", arr3);
    // printf("arr3 = <%s>\n", arr3);
    // char str[] = "54.23";
    // printf("%lg\n", atof(str));
    // printf("%d\n", std::stod(ptr));

    // char string[] = "+( 45 )(34))";
    // int n = 0;
    // sscanf(string, "%[^ ()]%n", result_string, &n);
    // printf("n = %d\n", n);
    // printf("str = <%s>\n", result_string);
    if (argc > 2) {
        printf("Too many arguments of the command line\n");
        return -1;
    }
    char* file_name = argv[1];
    printf("file_name = %s\n", file_name);

    Node* tree = ConstructTree(file_name);
    printf("ans = %lg\n", Ebal(tree));
    // Node* tree;
    // Node* node1 = CreateNewNode(NUMBER, 5);
    // Node* node2 = CreateNewNode(NUMBER, 7);
    // Node* node3 = CreateNewNode(OPER, '+', node1, node2);
    // Node* node4 = CreateNewNode(NUMBER, 10);
    // Node* node0 = CreateNewNode(NUMBER, -100);
    // Node* node324 = CreateNewNode(NUMBER, -532.87);
    // Node* node6 = CreateNewNode(OPER, '-', node4, node0);
    // Node* node300 = CreateNewNode(OPER, '+', node6, node324);
    // Node* node5 = CreateNewNode(OPER, '*', node3, node300); //node6
    // Node* node7 = CreateNewNode(NUMBER, -5.5);
    // Node* node8 = CreateNewNode(OPER, '/', node5, node7);
    // tree = node8;
    // PrintTreeToFile(tree, IN_ORDER);
    // PrintTreeToFile(tree, PRE_ORDER);
    // PrintTreeToFile(tree, POST_ORDER);

    // printf("ebal togo rot = %lg\n", Ebal(tree));
    TreeDump(tree);

    DeleteTree(tree);
    
    return 0;
}