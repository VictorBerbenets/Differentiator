#include "include//differentiator.h"
#include "include//graphviz.h"
// #include "include//stack.h"
#include "include//Queue.h"
#include <string.h>
#include <ctype.h>

//******************************************************************************************************************************************//
static void ReadBuffer(char** buffer, char* result_string, char* readed_symbol, ReadType type);
static void DeleteParentAndChild(Node** parent, Node** child);
static void LeaveOnlyLeftNode(Node** parent, Node** tree);
static void LeaveOnlyRightNode(Node** parent, Node** tree);
static int  RetFuncName(char* result_string);
static int  IsVariable(char* string);
static int  IsDigit(char* string);

// Node* CalculateChildes(Node** parent);
void CalculateChildes(Node** parent);

static int  ERROR_FLAG = 0;

//==========================================================================================================================================//

Node* ConstructTree(const char* file_name) {

    Buffer tree_buffer   = ReadFile(file_name);
    char* save_buff_addr = tree_buffer.buffer;
    Node* tree = CreateNewNode(NUMBER, nullptr);
    tree = BuildTree(tree, &tree_buffer);

    if (ERROR_FLAG) {
        DeleteTree(tree);
        tree = nullptr;
        printf("" Green "Tree was not created! " Gray "\n");
        exit(EXIT_FAILURE);
    }
    free(save_buff_addr);
    return tree;
}

//==========================================================================================================================================//

Buffer ReadFile(const char* file_name) {

    Buffer buff    = {};
    FILE* TreeFile = fopen(file_name, "r");
    Validator(TreeFile == nullptr, "reading file error", exit(READING_FILE_ERROR));

    buff.buffer_size = GetFileSize(file_name);
    buff.buffer      = (char*) calloc(buff.buffer_size + 1, sizeof(char));
    Validator(!buff.buffer, "memory giving error", exit(MEMORY_ALLOC_ERR));

    size_t fread_ret_value = fread(buff.buffer, sizeof(char), buff.buffer_size, TreeFile);
    Validator(fread_ret_value != buff.buffer_size, "fread reading error", exit(FREAD_READING_ERROR));
    buff.buffer[buff.buffer_size] = '\0';

    int is_file_closed = fclose(TreeFile);
    Validator(is_file_closed != 0, "closing file error", exit(CLOSING_FILE_ERROR));

    return buff;
}

//==========================================================================================================================================//

Node* BuildTree(Node* tree, Buffer* tree_buffer) {

    static char result_string[Max_variable_size] = {};
    static const char Operators[] = "+ - * / ^";        // const
    static elem_t value       = 0;
    static char readed_symbol = 0;
    static int counter        = 0;
    static int string_len     = 0;

    if (!tree) {
        return nullptr;
    }

    ReadBuffer(&(tree_buffer->buffer), result_string, &readed_symbol, SYMBOL);
    Validator(readed_symbol != OPEN_BRACKET, "expected open bracket", return nullptr);

    ReadBuffer(&(tree_buffer->buffer), result_string, &readed_symbol, STRING);
    if (strstr(Operators, result_string)) { // && (strlen(result_string) == 1)
        value = result_string[0];
        tree->type = OPER;
        tree->value.oper   = (int)value;
        tree->left_branch  = CreateNewNode(OPER, &value);
        tree->right_branch = CreateNewNode(OPER, &value);
    }
    else if ((tree->value.func = RetFuncName(result_string)) != INVALID_STRING_DATA){

        tree->type = FUNC;
        tree->left_branch = CreateNewNode(FUNC, &tree->value.func);
    }
    else if (IsDigit(result_string)) {
        tree->value.number = atof(result_string);
        tree->type = NUMBER;
    }
    else if (IsVariable(result_string)) {
        string_len = strlen(result_string);
        if (string_len < Max_variable_size) {
            memcpy((void*)tree->value.var, (const void*)result_string, sizeof(char) * string_len);
            tree->value.var[string_len] = '\0';
            tree->type = VAR;
        }
        else {
            fprintf(stderr, "Name of the variable '%s' is to long:%d. It have to < %d", result_string, string_len, Max_variable_size);
            ERROR_FLAG = 1;
            return nullptr;
        }
    }
    else {
        ERROR_FLAG = 1;
        printf("error: invalid data: %s\n", result_string);
        return nullptr;
    }

    tree->left_branch  = BuildTree(tree->left_branch,  tree_buffer); 
    tree->right_branch = BuildTree(tree->right_branch, tree_buffer);

    ReadBuffer(&(tree_buffer->buffer), result_string, &readed_symbol, SYMBOL);
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

static int RetFuncName(char* result_string) {

    Validator(result_string == nullptr, "invalid string pointer", exit(INVALID_STRING_POINTER));

    for (size_t struct_number = 0; struct_number < sizeof(_Diff_Functions_)/sizeof(_Diff_Functions_[0]); struct_number++) {
        if (!strcmp(_Diff_Functions_[struct_number].func_name, result_string)) {
            return _Diff_Functions_[struct_number].func_id;
        }
    }
    return INVALID_STRING_DATA;
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
            sscanf(*buffer, " %25[^ ()]%n", result_string, &counter); //make string size
            *buffer += counter;
            break;
        default: printf("Invalid type: %d\n", type);
    }
}

//==========================================================================================================================================//

static int IsDigit(char* string) {
    char* string_ptr  = string;
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

Node* CreateNewNode(int TYPE_NUM, const void* value, Node* left_node, Node* right_node) {

    Node* new_node = (Node*) calloc(ONE_NODE, sizeof(Node));
    Validator(new_node == nullptr, "link error: calloc could not give memory", exit(EXIT_FAILURE));

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
    }
    else if (TYPE_NUM == VAR) {
        new_node->type = VAR;
        if (value) {
            memcpy((void*) new_node->value.var, value, sizeof(char)*strlen((char*)value));
        }
        new_node->left_branch  = left_node;
        new_node->right_branch = right_node;
    }
    else if (TYPE_NUM == FUNC) {
        new_node->type = FUNC;
        new_node->value.func = *(int*)value;
    }
    else {
        printf("Error: invalid value type: %d\n", TYPE_NUM);
        new_node->type = NotAType;
    }
    new_node->left_branch  = left_node;
    new_node->right_branch = right_node;

    if (new_node->left_branch) {
        new_node->left_branch->parent = new_node;
    }
    if (new_node->right_branch) {
        new_node->right_branch->parent = new_node;
    }

    return new_node;
}

//==========================================================================================================================================//
// calculate tree
elem_t Ebal(Node* node_ptr) { 

    if (node_ptr->type == NUMBER) {
        return node_ptr->value.number;
    }
    switch(node_ptr->value.oper) {
        case OP_ADD : return Ebal(node_ptr->left_branch) + Ebal(node_ptr->right_branch);
        case OP_SUB : return Ebal(node_ptr->left_branch) - Ebal(node_ptr->right_branch);
        case OP_MUL : return Ebal(node_ptr->left_branch) * Ebal(node_ptr->right_branch);
        case OP_DIV : return GetDiv(node_ptr->left_branch, node_ptr->right_branch);
        case OP_POW : return GetPower(node_ptr->left_branch, node_ptr->right_branch); 
        case SQRT:
        default: PrintWarningInvalidOper(); return INVALID_OPERATOR;
    }
}

//==========================================================================================================================================//

elem_t GetPower(Node* base, Node* degree) {

    return pow(Ebal(base), Ebal(degree));
}

//==========================================================================================================================================//

elem_t GetDiv(Node* dividend, Node* divisor) {
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

    // fprintf(stderr, "tree address = <%p>\n", tree);
    // fprintf(stderr, "right address = <%p>\n", tree->right_branch);
    // fprintf(stderr, "left  address = <%p>\n", tree->left_branch);
 
    // tree->parent       = nullptr;
    // tree->left_branch  = nullptr;
    // tree->right_branch = nullptr;
    free(tree);
    // tree = nullptr;
    return ;
}
//==========================================================================================================================================//

//==========================================================================================================================================//
void PrintTree(Node* tree) {

    if (!tree) {
        return;
    }

    PrintTree(tree->left_branch);
    PrintTree(tree->right_branch);

    if (tree->type == OPER) {
        printf("%d \n", tree->value.oper);
    }
    if (tree->type == NUMBER) {
        printf("%lg \n", tree->value.number);
    } 
    if (tree->type == VAR) {
        printf("%s \n", tree->value.var);
    }
    if (tree->type == FUNC) {
        printf("%s \n", _Diff_Functions_[tree->value.func].func_name );
    }
    // printf("parent = <%p>\n", tree->parent);
}

Node* SimplifyTree(Node* tree, int* flag_) {

    if (!tree) {
        return nullptr;
    }

    Queue queue = {};
    Queue queue_save = {};
    QueueInit(&queue, QueueInitSize);
    QueueInit(&queue_save, QueueInitSize);
    QueuePush(&queue, tree);
    QueuePush(&queue_save, tree);
    
    while (queue.size) {

        Node* ptr = QueuePop(&queue);
        if (ptr->left_branch) {
                QueuePush(&queue, ptr->left_branch);
            if (ptr->left_branch->type == OPER) {
                printf("PUSH ADDRESS = %p\n", ptr->left_branch);

                QueuePush(&queue_save, ptr->left_branch);
            }
        }
        if (ptr->right_branch) {
                QueuePush(&queue, ptr->right_branch);
            if (ptr->right_branch->type == OPER) {
                printf("PUSH ADDRESS = %p\n", ptr->right_branch);
                QueuePush(&queue_save, ptr->right_branch);
            }
        }
    }

    int node_ptrs_size = queue_save.size;
    Node** node_ptrs   = (Node**) calloc(node_ptrs_size, sizeof(Node*));

    for (int node_counter = node_ptrs_size; node_counter > 0; --node_counter) {
        node_ptrs[node_counter - 1] = QueuePop(&queue_save);

    }
    // for (int i = 0; i < node_ptrs_size; i++) {
    //     printf("address[%d] = %p\n",i, node_ptrs[i]);
    // }
    for (int node_counter = 0; node_counter < node_ptrs_size; ++node_counter) {
        // printf("address[%d] = %p\n",node_counter, node_ptrs[node_counter]);
        printf("address left  = %p\n", node_ptrs[node_counter]->left_branch);
        printf("address       = %p\n", node_ptrs[node_counter]);
        printf("address right = %p\n", node_ptrs[node_counter]->right_branch);
        printf("counter = %d\n", node_counter);
        switch(node_ptrs[node_counter]->value.oper) {
            case OP_SUB:
            case OP_ADD:
                if (node_ptrs[node_counter]->left_branch->type == NUMBER && node_ptrs[node_counter]->right_branch->type == NUMBER) {
                    printf("ADRESS = %p\n", node_ptrs[node_counter]);
                    CalculateChildes(&(node_ptrs[node_counter]));
                    printf("ADRESS&&&&& = %p\n", node_ptrs[node_counter]);
                    break;
                }
                if (node_ptrs[node_counter]->left_branch->type == NUMBER) {
                    if (IsEqual(node_ptrs[node_counter]->left_branch->value.number, 0)) {
                        LeaveOnlyRightNode(&(node_ptrs[node_counter]), &tree);
                        break;                   
                    }
                }
                if (node_ptrs[node_counter]->right_branch->type == NUMBER) {
                    if (IsEqual(node_ptrs[node_counter]->right_branch->value.number, 0)) {
                        LeaveOnlyLeftNode(&(node_ptrs[node_counter]), &tree);
                        break;
                    }
                }
                break;
            case OP_MUL:
                if (node_ptrs[node_counter]->left_branch->type == NUMBER && node_ptrs[node_counter]->right_branch->type == NUMBER) {
                    printf("ADRESS = %p\n", node_ptrs[node_counter]);
                    // node_ptrs[node_counter] = CalculateChildes((node_ptrs[node_counter]));
                    break;
                }
                if (node_ptrs[node_counter]->left_branch->type == NUMBER) {
                    if (IsEqual(node_ptrs[node_counter]->left_branch->value.number, 0)) {
                        if (!node_ptrs[node_counter]->parent) {
                            tree = node_ptrs[node_counter]->left_branch;
                            DeleteParentAndChild(&(node_ptrs[node_counter]), &(node_ptrs[node_counter]->right_branch));
                            return tree;
                        }
                        node_ptrs[node_counter]->left_branch->parent = node_ptrs[node_counter]->parent;

                        if (node_ptrs[node_counter]->parent->left_branch == node_ptrs[node_counter]) {
                            node_ptrs[node_counter]->parent->left_branch = node_ptrs[node_counter]->left_branch;
                        }
                        else if (node_ptrs[node_counter]->parent->right_branch == node_ptrs[node_counter]) {
                            node_ptrs[node_counter]->parent->right_branch = node_ptrs[node_counter]->left_branch;
                        }
                        DeleteParentAndChild(&(node_ptrs[node_counter]), &(node_ptrs[node_counter]->right_branch));
                        break;                   
                    }                       
                }
                if (node_ptrs[node_counter]->right_branch->type == NUMBER) {
                    if (IsEqual(node_ptrs[node_counter]->right_branch->value.number, 0)) {
                        if (!node_ptrs[node_counter]->parent) {
                            tree = node_ptrs[node_counter]->right_branch;
                            DeleteParentAndChild(&(node_ptrs[node_counter]), &(node_ptrs[node_counter]->left_branch));
                            return tree;
                        }
                        node_ptrs[node_counter]->right_branch->parent = node_ptrs[node_counter]->parent;

                        if (node_ptrs[node_counter]->parent->left_branch == node_ptrs[node_counter]) {
                            node_ptrs[node_counter]->parent->left_branch = node_ptrs[node_counter]->right_branch;
                        }
                        else if (node_ptrs[node_counter]->parent->right_branch == node_ptrs[node_counter]) {
                            node_ptrs[node_counter]->parent->right_branch = node_ptrs[node_counter]->right_branch;
                        }
                        DeleteParentAndChild(&(node_ptrs[node_counter]), &(node_ptrs[node_counter]->left_branch));
                        break;                   
                    }
                }
                 if (node_ptrs[node_counter]->left_branch->type == NUMBER) {

                    if (IsEqual(node_ptrs[node_counter]->left_branch->value.number, 1)) {
                        LeaveOnlyRightNode(&(node_ptrs[node_counter]), &tree);
                    break;                   
                    }
                }
                if (node_ptrs[node_counter]->right_branch->type == NUMBER) {
                    if (IsEqual(node_ptrs[node_counter]->right_branch->value.number, 1)) {
                        LeaveOnlyLeftNode(&(node_ptrs[node_counter]), &tree);
                    break;
                    }
                }
                break;

            case OP_DIV:

            case OP_POW:

            default: break;
            }
        }
    free(node_ptrs);
    QueueDtor(&queue);
    QueueDtor(&queue_save);

   

    return tree;
}

void CalculateChildes(Node** parent) {

    elem_t calculated_value = Ebal(*parent);

    Node* grand_parent = (*parent)->parent;
    Node* new_parent   = CreateNewNode(NUMBER, &calculated_value);
    new_parent->parent = grand_parent;

    if (grand_parent->left_branch == *parent) {
        grand_parent->left_branch = new_parent;
    }
    else if (grand_parent->right_branch == *parent) {
        grand_parent->right_branch = new_parent;
    }

    free((*parent)->left_branch);
    free((*parent)->right_branch);
    free(*parent);

    *parent = new_parent;
}

static void LeaveOnlyRightNode(Node** parent, Node** tree) {

    if (!(*parent)->parent) {
        *tree = (*parent)->right_branch;
        DeleteParentAndChild(parent, &((*parent)->left_branch));
        return ;

    }
    (*parent)->right_branch->parent = (*parent)->parent;
    if ((*parent)->parent->left_branch == (*parent)) {
        (*parent)->parent->left_branch = (*parent)->right_branch;
    }
    else if ((*parent)->parent->right_branch == (*parent)) {
        (*parent)->parent->right_branch = (*parent)->right_branch;
    }
    DeleteParentAndChild(&((*parent)), &((*parent)->left_branch));

}

static void LeaveOnlyLeftNode(Node** parent, Node** tree) {

    if (!(*parent)->parent) {
        *tree = (*parent)->left_branch;
        DeleteParentAndChild(parent, &((*parent)->right_branch));
        return ;

    }
    (*parent)->left_branch->parent = (*parent)->parent;
    if ((*parent)->parent->left_branch == (*parent)) {
        (*parent)->parent->left_branch = (*parent)->left_branch;
    }
    else if ((*parent)->parent->right_branch == (*parent)) {
        (*parent)->parent->right_branch = (*parent)->left_branch;
    }
    DeleteParentAndChild(&((*parent)), &((*parent)->right_branch));

}

static void DeleteParentAndChild(Node** parent, Node** child) {

    if (*child) {
        free(*child);
        *child = nullptr;
    }
    if (*parent) {
        free(*parent);
        *parent = nullptr;
    }
}


//simple
//pdflatex