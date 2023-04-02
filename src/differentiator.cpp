#include "include//differentiator.h"
#include "include//graphviz.h"
// #include "include//stack.h"
#include "include//Queue.h"
#include <string.h>
#include <ctype.h>

//******************************************************************************************************************************************//
static void ReadBuffer(char** buffer, char* result_string, char* readed_symbol, ReadType type);
static int IsVariable(char* string);
static int IsDigit(char* string);

//==========================================================================================================================================//
Node* ConstructTree(const char* file_name) {

    Buffer tree_buffer = ReadFile(file_name);
    Node* tree = CreateNewNode(NUMBER, nullptr);
    tree = BuildTree(tree, &tree_buffer);
    return tree;
}

//==========================================================================================================================================//

Node* BuildTree(Node* tree, Buffer* tree_buffer) {
    static char result_string[MaxVarSize] = {};
    static const char Operators[]         = {'+', '-', '*', '/', '^','\0'};
    static elem_t value       = 0;
    static char readed_symbol = 0;
    static int counter        = 0;
    static int ERROR_FLAG     = 0;
    Buffer* buff_ptr          = tree_buffer;

    if (ERROR_FLAG) {
        return nullptr;
    }
    if (!tree) {
        return nullptr;
    }

    ReadBuffer(&(buff_ptr->buffer), result_string, &readed_symbol, SYMBOL);
    Validator(readed_symbol != OPEN_BRACKET, expected open bracket, return nullptr);

    ReadBuffer(&(buff_ptr->buffer), result_string, &readed_symbol, STRING);
    if (strstr(Operators, result_string) && (strlen(result_string) == 1)) {
        value = result_string[0];
        tree->type = OPER;
        tree->value.oper   = (int)value;
        tree->left_branch  = CreateNewNode(OPER, &value);
        tree->right_branch = CreateNewNode(OPER, &value);
    }
    else if (IsDigit(result_string)) {
        tree->value.number = atof(result_string);
        tree->type = NUMBER;
    }
    else if (IsVariable(result_string)) {
        char* variable = (char*) calloc(strlen(result_string) + 1, sizeof(char));
        memcpy((void*)variable, (const void*)result_string, sizeof(char) * strlen(result_string));
        variable[strlen(result_string)] = '\0';
        tree->value.var = variable;
        tree->type      = VAR;
    }
    else {
        ERROR_FLAG = 1;
        printf("error");
        return nullptr;
    }

    tree->left_branch  = BuildTree(tree->left_branch,  buff_ptr); 
    tree->right_branch = BuildTree(tree->right_branch, buff_ptr);

    ReadBuffer(&(buff_ptr->buffer), result_string, &readed_symbol, SYMBOL);
    if (readed_symbol == ')') {
        return tree;
    }
    else {
        ERROR_FLAG = 1;
        printf("ERROR BLYAT: EBANIY V ROT GDE CLOSE BRACKET NAXUI, MUDILA???\nGet: %c, expecter: %c\n", readed_symbol, CLOSE_BRACKET);
        return nullptr;
    }
}

//==========================================================================================================================================//

static int IsVariable(char* string) {
    char* string_ptr  = string;
    if (!strlen(string)) {
        return 0;
    }
    for (int symb_id = 0; string_ptr[symb_id] != '\0'; symb_id++) {
        if (isalpha(string_ptr[symb_id]) || string_ptr[symb_id] == '_') {
            continue;
        }
        else {
            return 0;
        }
    }
    return 1;
}

//==========================================================================================================================================//

static void ReadBuffer(char** buffer, char* result_string, char* readed_symbol, ReadType type) {
    int counter = 0;
    switch(type) {
        case SYMBOL:
            sscanf(*buffer, " %c%n", readed_symbol, &counter);
            *buffer += counter;
            break;
        case STRING:
            sscanf(*buffer, " %[^ ()]%n", result_string, &counter);
            *buffer += counter;
            break;
        default: printf("Invalid type: %d\n", type);
    }
}

//==========================================================================================================================================//

static int IsDigit(char* string) {
    char* string_ptr         = string;
    int point_counter = 0;
    if (!strlen(string)) {
        return 0;
    }
    for (int symb_id = 0; string_ptr[symb_id] != '\0'; symb_id++) {
        if (isdigit(string_ptr[symb_id])) {
            continue;
        }
        else if ((string_ptr[symb_id] == '.' && symb_id && !point_counter)) {
            point_counter++;
        }
        else {
            return 0;
        }
    }
    return 1;
}

//==========================================================================================================================================//

Buffer ReadFile(const char* file_name) {

    Buffer buff    = {};
    FILE* TreeFile = fopen(file_name, "r");

    Validator(!TreeFile, reading file error, exit(ERROR_IN_READING_FILE));

    buff.buffer_size = GetFileSize(file_name);
    buff.buffer      = (char*) calloc(buff.buffer_size + 1, sizeof(char));
    Validator(!buff.buffer, memory giving error, exit(MEMORY_ALLOC_ERR));

    size_t fread_ret_value = fread(buff.buffer, sizeof(char), buff.buffer_size, TreeFile);
    Validator(fread_ret_value != buff.buffer_size, fread reading error, exit(FREAD_READING_ERROR));
    buff.buffer[buff.buffer_size] = '\0';

    fclose(TreeFile);

    return buff;
}

//==========================================================================================================================================//

Node* CreateNewNode(int TYPE_NUM, const void* value, Node* left_node, Node* right_node) {

    Node* new_node = (Node*) calloc(ONE_NODE, sizeof(Node));
    Validator(new_node == nullptr, link error: calloc could not give memory, exit(EXIT_FAILURE));

    if (TYPE_NUM == NUMBER) {
        new_node->type = NUMBER;
        if (value) {
            new_node->value.number = *(elem_t*)value;
        }
    }
    else if (TYPE_NUM == OPER) {
        new_node->type = OPER;
        if (value) { 
            new_node->value.oper = *(int*)value; 
        }
        new_node->left_branch  = left_node;
        new_node->right_branch = right_node;
    }
    else if (TYPE_NUM == VAR) {
        new_node->type = VAR;
        if (value) { 
            new_node->value.var = (char*)value; 
        }
        new_node->left_branch  = left_node;
        new_node->right_branch = right_node;
    }
    else {
        printf("Error: invalid value type: %d\n", TYPE_NUM);
        new_node->type = NotAType;
        // return nullptr;
    }
    
    return new_node;
}

//==========================================================================================================================================//

int TreeDump(Node* tree) {
    if (!tree) {
        printf("Cant create Graph_Dump: tree pointer = %p\n", tree);
        return INVALID_TREE_POINTER;
    }
    DotStartGraph("data//list.dot");
    Validator(dot_file == nullptr, in opening file:'data//list.dot', return ERROR_IN_READING_FILE;);

    const char dot_header[] = "digraph List {\n"
                              "\tdpi = 100;\n"
                              "\tfontname = \"Comic Sans MS\";\n"
                              "\tfontsize = 20;\n"
                              "\trankdir  = TB;\n";
    DotPrint(dot_header);
    DotSetGraph("lightgreen", 1.3, 0.5, "rounded", "green", 2.);
    DotSetEdge("black", "diamond", 1., 1.2);

    Queue queue = {};
    
    QueueInit(&queue, QueueInitSize);
    QueuePush(&queue, (Node*)tree);
    
    int node_head = 1;
    int node_next = 2;

    while (queue.size) {

        Node* ptr = QueuePop(&queue);
        CreateGraphNode(dot_file, ptr, &node_head);
        if (ptr->left_branch) {
            CreateNextGraphNode(dot_file, ptr, &node_head, &node_next, LEFT);
            QueuePush(&queue, ptr->left_branch);
        }
        if (ptr->right_branch) {
            CreateNextGraphNode(dot_file, ptr, &node_head, &node_next, RIGHT);
            QueuePush(&queue, ptr->right_branch);

        }
        DotPrint("\n");
        node_head++;
    }

    DotEndGraph(dot_file);
    DotPrintGraph(file, 1);
    QueueDtor(&queue);

    return ALL_RIGHT;
}

//==========================================================================================================================================//

void CreateNextGraphNode(FILE* dot_file, Node* ptr, int* node_head, int* node_next, Position position) {

    CreateGraphNode(dot_file, ptr, node_next);
    DotPrint("node%d: <%s> -> node%d; ", *node_head, position == LEFT ? "ptr1":"ptr2", *node_next);
    (*node_next)++;
}

void CreateGraphNode(FILE* dot_file, Node* ptr, int* node_counter) {

    if (ptr->type == NUMBER) {
        DotPrint("node%d [shape = Mrecord, style = filled, fillcolor = \"#FFD0D0\", label ="
                 "\"{address: %p|value: %lg| { <ptr1> left: %p| <ptr2> right: %p}}\"]\n",
                *node_counter,  ptr, ptr->value.number, ptr->left_branch, ptr->right_branch);
    }
    else if(ptr->type == OPER) {
        DotPrint("node%d [shape = Mrecord, style = filled, fillcolor = \"#ABFFF1\", label ="
                 "\"{address: %p|operator: '%c'| { <ptr1> left: %p| <ptr2> right: %p}}\"]\n",
                *node_counter,  ptr, ptr->value.oper, ptr->left_branch, ptr->right_branch); 
    }
    else if (ptr->type == VAR) {
        DotPrint("node%d [shape = Mrecord, style = filled, fillcolor = \"#ABFFF1\", label ="
            "\"{address: %p|variable: %s| { <ptr1> left: %p| <ptr2> right: %p}}\"]\n",
            *node_counter,  ptr, ptr->value.var, ptr->left_branch, ptr->right_branch);
    }
    else {
        printf("Fucking error, man!!!\n");
    }
}

//==========================================================================================================================================//

int PrintTreeToFile(Node* tree, PrintType type) {

    const char* file_name = nullptr;
    switch (type) {
        case IN_ORDER  : file_name = "data//in_order_tree.txt";     break;
        case PRE_ORDER : file_name = "data//pre_order_tree.txt";    break;
        case POST_ORDER: file_name = "data//post_order_tree.txt";   break;
        default: fprintf(stderr, "%s:%d:error: invalid print type: '%d'\n", __PRETTY_FUNCTION__, __LINE__, type); 
                return INVALID_PRINT_TYPE;
    }

    FILE* TreeFile = fopen(file_name, "w");
    switch(type) {
        case IN_ORDER  : InOrder(tree, TreeFile);         break;
        case PRE_ORDER : PreOrder(tree, TreeFile);        break;
        case POST_ORDER: PostOrder(tree, TreeFile);       break;
        default: fprintf(stderr,"What the fuck???\n");    break;
    }
    return ALL_RIGHT;
}

//==========================================================================================================================================//

void PreOrder(Node* tree,  FILE* Tree_file) {

    if (!tree) { 
        return; 
    }

    fprintf(Tree_file, "(");
    if (tree->type == NUMBER) {
        fprintf(Tree_file, "%lg", tree->value.number);
    }
    else if (tree->type == VAR) {
        fprintf(Tree_file, "%s", tree->value.var);
    }
    else if (tree->type == OPER) {
        fprintf(Tree_file, "%c", tree->value.oper);
    }
    else {
        fprintf(Tree_file, "Nuchai bebru, invalid tree type: %d\n", tree->type);
    }

    PreOrder(tree->left_branch, Tree_file);
    PreOrder(tree->right_branch, Tree_file);
    fprintf(Tree_file, ")");

    return ;
}

//==========================================================================================================================================//

void InOrder(Node* tree, FILE* Tree_file) {
    
    if (!tree) { 
        return; 
    }

    fprintf(Tree_file, "(");
    InOrder(tree->left_branch, Tree_file);

    if (tree->type == NUMBER) {
        fprintf(Tree_file, "%lg", tree->value.number);
    }
    else if (tree->type == VAR) {
        fprintf(Tree_file, "%s", tree->value.var);
    }
    else if (tree->type == OPER) {
        fprintf(Tree_file, "%c", tree->value.oper);
    }
    else {
        fprintf(Tree_file, "Nuchai bebru, invalid tree type: %d\n", tree->type);
    }

    InOrder(tree->right_branch, Tree_file);
    fprintf(Tree_file, ")"); 

    return ;
}

//==========================================================================================================================================//

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
    else if (tree->type == VAR) {
        fprintf(Tree_file, "%s", tree->value.var);
    }
    else if (tree->type == OPER) {
        fprintf(Tree_file, "%c", tree->value.oper);
    }
    else {
        fprintf(Tree_file, "Nuchai bebru, invalid tree type: %d\n", tree->type);
    }

    fprintf(Tree_file, ")"); 

    return ;
}

//==========================================================================================================================================//
// calculate tree
elem_t Ebal(Node* node_ptr) { 

    if (node_ptr->type == NUMBER) {
        Validator(node_ptr->left_branch,  invalid node address, return INVALID_NODE;);
        Validator(node_ptr->right_branch, invalid node address, return INVALID_NODE;);
        return node_ptr->value.number;
    }
    switch(node_ptr->value.oper) {
        case OP_ADD : return Ebal(node_ptr->left_branch) + Ebal(node_ptr->right_branch);
        case OP_SUB : return Ebal(node_ptr->left_branch) - Ebal(node_ptr->right_branch);
        case OP_MUL : return Ebal(node_ptr->left_branch) * Ebal(node_ptr->right_branch);
        case OP_POW : return GetPower(node_ptr->left_branch, node_ptr->right_branch); 
        case OP_DIV : return GetQuotient(node_ptr->left_branch, node_ptr->right_branch);
        default: PrintWarningInvalidOper(); return INVALID_OPERATOR;
    }
}

//==========================================================================================================================================//

elem_t GetPower(Node* base, Node* degree) {

    return pow(Ebal(base), Ebal(degree));
}

//==========================================================================================================================================//

elem_t GetQuotient(Node* dividend, Node* divisor) {
    elem_t div = Ebal(divisor->right_branch);
    if (IsEqual(div, 0)) { 
        PrintWarningForDivisor(); 
        return DIVIDE_ERROR;
    }
    return Ebal(dividend->left_branch) / div;
}

//==========================================================================================================================================//

int IsEqual(elem_t value1, elem_t value2) {

    const static elem_t Epsilon_ = 1e-17;
    return fabs(value1 - value2) <= Epsilon_;
}

//==========================================================================================================================================//

void DeleteTree(Node* tree) {

    if (!tree) { return ; }

    DeleteTree(tree->left_branch);
    DeleteTree(tree->right_branch);

    free((Node*)tree);
    return ;
}
//==========================================================================================================================================//
