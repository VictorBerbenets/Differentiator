#include "..//include//differentiator.h"





int main() {

    // int size = 3;
    Node* tree;
    Node* node1 = CreateNewNode(NUMBER, 5, NULL, NULL);
    Node* node2 = CreateNewNode(NUMBER, 7, NULL, NULL);
    Node* node3 = CreateNewNode(OPER, '+', node1, node2);
    Node* node4 = CreateNewNode(NUMBER, 10, NULL, NULL);
    Node* node5 = CreateNewNode(OPER, '*', node3, node4);
    Node* node7 = CreateNewNode(NUMBER, -2, NULL, NULL);
    Node* node8 = CreateNewNode(OPER, '//', node5, node7);
    tree = node8;
    PrintTree(tree);

    TreeDump(tree);

    DeleteTree(tree);
    
    return 0;
}