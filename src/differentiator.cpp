#include "..//include//differentiator.h"
#include "..//include//graphviz.h"

Node* ConstructTree(const char* file_name) {


    Buffer buff = ReadFile(file_name);
    Node* tree = ConstructNode(&buff);
}

Buffer ReadFile(const char* file_name) {

    Buffer buff = {};
    FILE* TreeFile = fopen(file_name, "rb");

    Validator(!TreeFile, reading file error, exit(ERROR_IN_READING_FILE));

    buff.buffer_size = GetFileSize(file_name);
    buff.buffer = (char*) calloc(buff.buffer_size, sizeof(char));
    Validator(!buff.buffer, memory giving error, exit(MEMORY_ALLOC_ERR));

    int fread_ret_value = fread(buff.buffer, sizeof(char), buff.buffer_size, TreeFile);
    Validator(fread_ret_value != buff.buffer_size, fread reading error, exit(FREAD_READING_ERROR));

    fclose(TreeFile);
    return buff;
}

Node* ConstructNode(Buffer* buff) {

    int symb_position = 1;
    int bracket_count = 0;
    char symbol       = 0;
    elem_t value      = 0;
    Node* tree        = {};
//pre_order
    for (int symb_count = 0; symb_count < buff->buffer_size; symb_count++) {
        if (sscanf(buff->buffer, " %c", &symbol) && symbol == OPEN_BRACKET) {
            symb_position++;
            bracket_count++;
            if (sscanf(buff->buffer, " %lg", &value)) {

            }
        }
        else if (symbol == CLOSE_BRACKET) {
            symb_position++;
            bracket_count--;
        }
    }
}

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

int TreeDump(const Node* tree) {

    DotStartGraph("data//list.dot");
    Validator(dot_file == nullptr, in opening file:'data//list.dot', return -1;);

    const char dot_header[] = "digraph List {\n"
                              "\tdpi = 100;\n"
                              "\tfontname = \"Comic Sans MS\";\n"
                              "\tfontsize = 20;\n"
                              "\trankdir  = TB;\n";
    DotPrint(dot_header);
    DotSetGraph("lightgreen", 1.3, 0.5, "rounded", "green", 2.);
    DotSetEdge("black", "diamond", 1., 1.2);

    Queue q = {};
    queue_init(&q, QueueInitSize);
    queue_enqueue(&q, (Node*)tree);

    int node_head = 1;
    int node_next = 2;

    while (q.size) {

        Node* ptr = queue_dequeue(&q);
        CreateGraphNode(dot_file, ptr, &node_head);
        if (ptr->left_branch) {
            CreateNextGraphNode(dot_file, ptr, &node_head, &node_next, LEFT);
            queue_enqueue(&q, ptr->left_branch);
        }
        if (ptr->right_branch) {
            CreateNextGraphNode(dot_file, ptr, &node_head, &node_next, RIGHT);
            queue_enqueue(&q, ptr->right_branch);

        }
        DotPrint("\n");
        node_head++;
    }

    DotEndGraph(dot_file);
    DotPrintGraph(file, 1);
    queue_dtor(&q);
}

void CreateNextGraphNode(FILE* dot_file, Node* ptr, int* node_head, int* node_next, Position position) {

    CreateGraphNode(dot_file, ptr, node_next);
    DotPrint("node%d: <%s> -> node%d; ", *node_head, position == LEFT? "ptr1":"ptr2", *node_next);
    (*node_next)++;
}

void CreateGraphNode(FILE* dot_file, Node* ptr, int* node_counter) {

    if (ptr->type == NUMBER) {
        DotPrint("node%d [shape = Mrecord, style = filled, fillcolor = \"#FFD0D0\", label = \"{address: %p|value: %lg| { <ptr1> left: %p| <ptr2> right: %p}}\"]\n",
                *node_counter,  ptr, ptr->value.number, ptr->left_branch, ptr->right_branch);
    }
    else {
        DotPrint("node%d [shape = Mrecord, style = filled, fillcolor = \"#ABFFF1\", label = \"{address: %p|operator: '%c'| { <ptr1> left: %p| <ptr2> right: %p}}\"]\n",
                *node_counter,  ptr, ptr->value.oper, ptr->left_branch, ptr->right_branch); 
    }
}

int PrintTreeToFile(Node* tree, PrintType type) {

    const char* file_name = nullptr;
    switch (type) {
        case IN_ORDER: file_name = "data//in_order_tree.txt"; break;

        case PRE_ORDER: file_name = "data//pre_order_tree.txt"; break;

        case POST_ORDER: file_name = "data//post_order_tree.txt"; break;

        default: fprintf(stderr, "%s:%d:error: invalid print type: '%d'\n", __PRETTY_FUNCTION__, __LINE__, type); return INVALID_PRINT_TYPE;
    }

    FILE* TreeFile = fopen(file_name, "w");
    switch(type) {
        case IN_ORDER: InOrder(tree, TreeFile); break;

        case PRE_ORDER: PreOrder(tree, TreeFile); break;

        case POST_ORDER: PostOrder(tree, TreeFile); break;
    }
}

void PreOrder(Node* tree,  FILE* Tree_file) {

    if (!tree) { 
        return; 
    }

    fprintf(Tree_file, "(");
    if (tree->type == NUMBER) {
        fprintf(Tree_file, "%lg", tree->value.number);
    }
    else {
        fprintf(Tree_file, "%c", tree->value.oper);
    }

    PreOrder(tree->left_branch, Tree_file);
    PreOrder(tree->right_branch, Tree_file);
    fprintf(Tree_file, ")"); 

    return ;
}

void InOrder(Node* tree, FILE* Tree_file) {
    
    if (!tree) { 
        return; 
    }

    fprintf(Tree_file, "(");
    InOrder(tree->left_branch, Tree_file);

    if (tree->type == NUMBER) {
        fprintf(Tree_file, "%lg", tree->value.number);
    }
    else {
        fprintf(Tree_file, "%c", tree->value.oper);
    }

    InOrder(tree->right_branch, Tree_file);
    fprintf(Tree_file, ")"); 

    return ;
}

void PostOrder(Node* tree, FILE* Tree_file) {
    
    if (!tree) { 
        return; 
    }
    fprintf(Tree_file, "(");
    PostOrder(tree->left_branch, Tree_file);
    PostOrder(tree->right_branch, Tree_file);
    if (tree->type == NUMBER) {
        fprintf(Tree_file, "%lg", tree->value.number);
    }
    else {
        fprintf(Tree_file, "%c", tree->value.oper);
    }

    fprintf(Tree_file, ")"); 

    return ;
}

//Print to console
void PrintTree(const Node* tree) { 

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

// calculate tree
elem_t Ebal(const Node* node_ptr) { 

    if (node_ptr->type == NUMBER) {
        Validator(node_ptr->left_branch,  invalid node address, return INVALID_NODE;);
        Validator(node_ptr->right_branch, invalid node address, return INVALID_NODE;);
        return node_ptr->value.number;
    }

    switch(node_ptr->value.oper) {
        case OP_PLUS: return Ebal(node_ptr->left_branch) + Ebal(node_ptr->right_branch);
        case OP_SUB : return Ebal(node_ptr->left_branch) - Ebal(node_ptr->right_branch);
        case OP_MUL : return Ebal(node_ptr->left_branch) * Ebal(node_ptr->right_branch);
        case OP_DIV : {
            int divider = Ebal(node_ptr->right_branch);
            if (!divider) {
                fprintf(stderr, "" Purple "" White"Warning:" Grey "" White "%s:%d:" Grey "\n\t|Trying to divide by 0\n", \
                    __PRETTY_FUNCTION__, __LINE__);
                return DIVIDE_ERROR;
            }
            return Ebal(node_ptr->left_branch) / divider;
        }
        default: fprintf(stderr, "" Purple "" White"Warning:" Grey "" White "%s:%d:" Grey "\n\t|Invalid operator: %c\n",\
             __PRETTY_FUNCTION__, __LINE__, node_ptr->value.oper); 
                return INVALID_OPERATOR;
    }
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

void DeleteTree(const Node* tree) {

    if (!tree) { return ; }

    DeleteTree(tree->left_branch);
    DeleteTree(tree->right_branch);

    free((Node*)tree);
    return ;
}


int queue_is_empty(Queue* queue) {

    return (queue->size == 0);
}

void queue_init(Queue* queue, int size) {

    if (size < 0) {
        size = QueueInitSize;
    }
    queue->capacity = size;
    queue->elem_address = (Node**) calloc(queue->capacity, sizeof(Node*));
    queue->head = 1;
    queue->tail = 0;
    queue->size = 0;
}
void queue_enqueue(Queue* queue, Node* value) {

    queue->tail++;
    queue->elem_address[queue->tail] = value;
    queue->size++;
}
Node* queue_dequeue(Queue* queue) {

    queue->head++;
    queue->size--;
    return (queue->elem_address[queue->head - 1]);
}
void queue_dtor(Queue* queue) {

    free(queue->elem_address);
}