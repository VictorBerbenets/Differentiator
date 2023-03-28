// #include "..//include//differentiator.h"
#include "..//include//Queue.h"




int main(int argc, char** argv) {

    Queue q = {};
    QueueInit(&q, 64);
        QueueLog(&q);

    for (int i = 0; i < 1000; i++) {
        QueuePush(&q, i);
        if (q.size % 5 == 0) {
            QueuePop(&q);
        }
        
    }
        QueueLog(&q);
    while(q.size) {
        QueuePop(&q);
    }
        QueueLog(&q);

    QueueDtor(&q);
    // int size = 3;
    // Node* tree;
    // Node* node1 = CreateNewNode(NUMBER, 5, NULL, NULL);
    // Node* node2 = CreateNewNode(NUMBER, 7, NULL, NULL);
    // Node* node3 = CreateNewNode(OPER, '+', node1, node2);
    // Node* node4 = CreateNewNode(NUMBER, 10, NULL, NULL);
    // Node* node0 = CreateNewNode(NUMBER, -100, NULL, NULL);
    // Node* node324 = CreateNewNode(NUMBER, -532.87, NULL, NULL);
    // Node* node6 = CreateNewNode(OPER, '-', node4, node0);
    // Node* node300 = CreateNewNode(OPER, '+', node6, node324);
    // Node* node5 = CreateNewNode(OPER, '*', node3, node300); //node6
    // Node* node7 = CreateNewNode(NUMBER, -5.5, NULL, NULL);
    // Node* node8 = CreateNewNode(OPER, '/', node5, node7);
    // tree = node8;
    // PrintTreeToFile(tree, IN_ORDER);
    // PrintTreeToFile(tree, PRE_ORDER);
    // PrintTreeToFile(tree, POST_ORDER);

    // printf("ebal togo rot = %lg\n", Ebal(tree));
    // TreeDump(tree);

    // DeleteTree(tree);
    
    return 0;
}