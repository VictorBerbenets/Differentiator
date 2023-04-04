#include "include//differentiator.h"
#include "include//graphviz.h"
// #include "include//stack.h"
#include "include//Queue.h"
#include <string.h>
#include <ctype.h>

//******************************************************************************************************************************************//
static void ReadBuffer(char** buffer, char* result_string, char* readed_symbol, ReadType type);
static int  RetFuncName(char* result_string);
static int  IsVariable(char* string);
static int  IsDigit(char* string);
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
        printf("" Grin "Tree was not created! " Grey "\n");

    }

    free(save_buff_addr);
    return tree;
}

//==========================================================================================================================================//

Buffer ReadFile(const char* file_name) {

    Buffer buff    = {};
    FILE* TreeFile = fopen(file_name, "r");
    Validator(TreeFile == nullptr, reading file error, exit(READING_FILE_ERROR));

    buff.buffer_size = GetFileSize(file_name);
    buff.buffer      = (char*) calloc(buff.buffer_size + 1, sizeof(char));
    Validator(!buff.buffer, memory giving error, exit(MEMORY_ALLOC_ERR));

    size_t fread_ret_value = fread(buff.buffer, sizeof(char), buff.buffer_size, TreeFile);
    Validator(fread_ret_value != buff.buffer_size, fread reading error, exit(FREAD_READING_ERROR));
    buff.buffer[buff.buffer_size] = '\0';

    int is_file_closed = fclose(TreeFile);
    Validator(is_file_closed != 0, closing file error, exit(CLOSING_FILE_ERROR));

    return buff;
}

//==========================================================================================================================================//

Node* BuildTree(Node* tree, Buffer* tree_buffer) {

    static char result_string[Max_variable_size] = {};
    static const char Operators[] = "+ - * / ^";
    static const char Functions[] = "sqrt sin cos tg ctg ln log ch sh th cth";
    static elem_t value       = 0;
    static char readed_symbol = 0;
    static int counter        = 0;
    static int string_len     = 0;

    if (!tree) {
        return nullptr;
    }

    ReadBuffer(&(tree_buffer->buffer), result_string, &readed_symbol, SYMBOL);
    Validator(readed_symbol != OPEN_BRACKET, expected open bracket, return nullptr);

    ReadBuffer(&(tree_buffer->buffer), result_string, &readed_symbol, STRING);
    if (strstr(Operators, result_string)) { // && (strlen(result_string) == 1)
        value = result_string[0];
        tree->type = OPER;
        tree->value.oper   = (int)value;
        tree->left_branch  = CreateNewNode(OPER, &value);
        tree->right_branch = CreateNewNode(OPER, &value);
    }
    else if (strstr(Functions, result_string)){

        tree->type = FUNC;
        tree->value.func  = RetFuncName(result_string);
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

    Validator(result_string == nullptr, invalid string pointer, exit(INVALID_STRING_POINTER));

    for (int struct_number = 0; struct_number < sizeof(_Diff_Functions_)/sizeof(_Diff_Functions_[0]); struct_number++) {
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
            memcpy((void*) new_node->value.var, value, sizeof(char)*strlen((char*)value));
        }
        new_node->left_branch  = left_node;
        new_node->right_branch = right_node;
    }
    else if (TYPE_NUM == FUNC) {
        printf("value = %d\n", *(int*)value);
        new_node->type = FUNC;
        new_node->value.func = *(int*)value;
    }
    else {
        printf("Error: invalid value type: %d\n", TYPE_NUM);
        new_node->type = NotAType;
        // return nullptr;
    }
    
    return new_node;
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
        case OP_DIV : return GetDiv(node_ptr->left_branch, node_ptr->right_branch);
        case POW    : return GetPower(node_ptr->left_branch, node_ptr->right_branch); 
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
 
    free(tree);
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
        printf("%d ", tree->value.oper);
    }
    if (tree->type == NUMBER) {
        printf("%lg ", tree->value.number);
    } 
    if (tree->type == VAR) {
        printf("%s ", tree->value.var);
    }
    if (tree->type == FUNC) {
        printf("%s ", _Diff_Functions_[tree->value.func].func_name );
    }
}