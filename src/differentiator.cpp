#include "..//include//differentiator.h"
#include "..//include//graphviz.h"



Node* CreateNewNode(int TYPE_NUM, elem_t value, Node* left_node, Node* right_node) {

    Node* new_node = (Node*) calloc(ONE_NODE, sizeof(Node));
    Validator(new_node == nullptr, link error: calloc could not give memory, exit(EXIT_FAILURE));

    if (TYPE_NUM == NUMBER) {
        new_node->type = NUMBER;
        new_node->value.number = value;
    }
    else if (TYPE_NUM == OPER) {
        new_node->type = OPER;
        new_node->value.oper = (int)value;
        new_node->left_branch  = left_node;
        new_node->right_branch = right_node;
    }
    else {
        printf("Error: invalid value type: %d\n", TYPE_NUM);
        return nullptr;
    }
    
    return new_node;
}


int queue_is_empty(Queue* queue) {

    return (queue->size == 0);
}

void queue_init(Queue* queue, int size) {

    if (size < 0) {
        size = 10;
    }
    queue->capacity = size;
    queue->data = (Node*) calloc(queue->capacity + 10, sizeof(Node));
    queue->head = 1;
    queue->tail = 0;
    queue->size = 0;
}
void queue_enqueue(Queue* queue, Node value) {

    queue->tail++;
    queue->data[queue->tail] = value;
    queue->size++;
}
Node queue_dequeue(Queue* queue) {

    Node ret_value = queue->data[queue->head];
    queue->head++;
    queue->size--;

    return ret_value;
}
void queue_dtor(Queue* queue) {

    free(queue->data);
}
void queue_print(Queue queue) {

    printf("head: %d;   tail: %d;   size: %d\n", queue.head, queue.tail, queue.size);
    // for (int i = 1; i <= queue.size; i++) {
    //     printf("%d ", queue.data[i].val);
    // }
    printf("\n");
}

int TreeDump(Node* tree, Queue* q) {

    DotStartGraph("data//list.dot");
    Validator(dot_file == nullptr, in opening file:'data//list.dot', return -1;);

    const char dot_header[] = "digraph List {\n"
                              "\tdpi = 100;\n"
                              "\tfontname = \"Comic Sans MS\";\n"
                              "\tfontsize = 20;\n"
                              "\trankdir  = TB;\n";
    DotPrint(dot_header);

    DotSetGraph("lightgreen", 1.3, 0.5, "rounded", "green", 2.);
    DotSetEdge("darkgrey", "onormal", 1., 1.2);

    queue_enqueue(q, *tree);
    int node_counter = 1;
    while (q->size) {
    Node queue_node = queue_dequeue(q);
    Node* ptr = &queue_node;
    
    if (ptr->type == NUMBER) {
        DotPrint("node%d [shape = record, color = red, style = solid, label = \"{left: %p| value: %lg| right: %p}\"]\n",
                node_counter, ptr->left_branch, ptr->value.number, ptr->right_branch);
        printf("%lf\n", ptr->value.number);
    }
    else {
        DotPrint("node%d [shape = record, color = blue, style = solid, label = \"{left: %p| operator: %c| right: %p}\"]\n",
                node_counter, ptr->left_branch, ptr->value.oper, ptr->right_branch);
        printf("%c\n", ptr->value.oper);
    }

    if (ptr->left_branch) {
        DotPrint("node%d -> node%d; ", node_counter, node_counter + 1);
        queue_enqueue(q, *(ptr->left_branch));
    }
    if (ptr->right_branch) {
        DotPrint("node%d -> node%d; ", node_counter, node_counter + 2);
        queue_enqueue(q, *(ptr->right_branch));
    }
    DotPrint("\n");
    node_counter++;
    }

    DotEndGraph(dot_file);
    DotPrintGraph(file, 1);
}
// //  pre_order постфиксная запись
// //  graphviz
// //  Сравнение
// //  
// //  голосовой интерфейс
// //   
void PrintTree(Node* tree) {

    if (!tree) { return; }
    TabsForTreePrint += 5;

    PrintTree(tree->left_branch);

    for (int tabs_counter = 0; tabs_counter < TabsForTreePrint; ++tabs_counter) {
        printf(" ");
    }
    if (tree->type == NUMBER) {
        printf("%lf\n", tree->value.number);
    }
    else {
        printf("%c\n", tree->value.oper);
    }

    PrintTree(tree->right_branch);

    TabsForTreePrint -= 5;
    return ;
}

// Node* FindNode(Node* real_node, elem_t value) {

//     if (!real_node) {
//         return nullptr;
//     }
//     if (!strcmp(value, real_node->value)) {
//         return real_node;
//     }

//     Node* find_left = FindNode (real_node->left_branch, value);
//     Node* find_right = FindNode (real_node->right_branch, value);

//     if (find_left) return find_left;
//     if (find_right) return find_right;

//     return nullptr;
// }

void DeleteTree(Node* tree) {

    if (!tree) { return ; }

    DeleteTree(tree->left_branch);
    DeleteTree(tree->right_branch);

    free(tree);
    return ;
}