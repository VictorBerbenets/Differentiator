#include "..//include//differentiator.h"





int main() {

    int size = 3;
    Node* tree = {};
    printf("tree = %p\n", tree);
    Node* node1 = CreateNewNode(NUMBER, 5, NULL, NULL);
    Node* node2 = CreateNewNode(NUMBER, 7, NULL, NULL);
    Node* node3 = CreateNewNode(OPER, '+', node1, node2);
    Node* node4 = CreateNewNode(NUMBER, 10, NULL, NULL);
    Node* node5 = CreateNewNode(OPER, '*', node3, node4);
    tree = node5;
    PrintTree(tree);
    Queue q = {};
    queue_init(&q, 10);
    TreeDump(tree, &q);
//     Node* tree = {};
//     TreeCtor(&tree);
//     tree->value = "Init";
//     fprintf(stderr, "%d\n", tree == nullptr);
//     InsertNode(tree, "Text1", Right);
//     InsertNode(tree, "Text6" , Left);
//     InsertNode(tree, "Text2", Right);
//     InsertNode(tree, "Text3", Right);
//     InsertNode(tree, "Text4", Right);
//     // InsertNode(tree, "Text5", Right);
//     // InsertNode(tree, "Text6" , Left);
//     // InsertNode(tree, "Text7" , Left);
//     // InsertNode(tree, "Text8" , Left);
//     // InsertNode(tree, "Text9"  , Left);
//     // InsertNode(tree, "Text10"  , Left);

//     char name1[100] = "";
//     for (int i = 0; i < size; i++) {
//         scanf("%s", name1);
//         InsertNode(tree, name1, Left);
//         InsertNode(tree, name1, Right);
//     PrintTree(tree);

//     }
//     printf("Tree val = %s\n", tree->value);
//     printf("Tree val right = %s\n", tree->right_branch->value);
//     printf("Tree val left = %s\n", tree->left_branch->value);
//     PrintTree(tree);
    DeleteTree(tree);
    queue_dtor(&q);
    return 0;
}