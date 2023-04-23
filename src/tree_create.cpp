#include "include//differentiator.h"
#include "include//parsing.h"
#include "include//DSL.h"

//*****************************************************************************************************************************************//
static const int Sub  = OP_SUB;
static const int Add  = OP_ADD;
static const int Mul  = OP_MUL;
static const int Div  = OP_DIV;
static const int Pow  = OP_POW;

static Node* duplic_tree = nullptr;
static int _ERROR_FLAG_ = 0;

static void SkipSpaces(char** string);
static void PrintError(int err_id, char* string);
static int  GetFuncId(char* func_name);
static void ReadFileString(FILE* TreeFile, char** string_take);
//==========================================================================================================================================//

void ConstructTree(const char* file_name, Tree* tree) {
    char* expression = ReadFile(file_name, tree);
    Validator(expression == nullptr, "can't read expression from the file", exit(READING_FILE_ERROR));

    char* save_buff_addr = expression;
    (*tree).Root = CreateNewNode(NUMBER, nullptr);
    tree->Root   = BuildTree(&expression);

    free(save_buff_addr);
}

//==========================================================================================================================================//

char* ReadFile(const char* file_name, Tree* tree) {
    FILE* TreeFile   = fopen(file_name, "r");
    Validator(TreeFile == nullptr, "reading file error", exit(READING_FILE_ERROR));

    char* expression = nullptr;
    ReadFileString(TreeFile, &expression);

    FindVariables(TreeFile, tree);
    int is_file_closed = fclose(TreeFile);
    Validator(is_file_closed != 0, "closing file error", exit(CLOSING_FILE_ERROR));

    return expression;
}

//==========================================================================================================================================//

void  FindVariables(FILE* TreeFile, Tree* tree) {

    char* var_string = nullptr;
    ReadFileString(TreeFile, &var_string);
    char* save_address = var_string;

    char string[MAX_VARIABLE_SIZE + 1] = {0};
    int var_len = 0;
    int var_counter = 0;
    char ch  = 0;
    char end_symb = -1;

    SkipSpaces(&var_string);
    while((ch = sscanf(var_string, "%25[^ ]%n", string, &var_len))) {

        tree->variables = (VarInfo*) realloc(tree->variables, sizeof(VarInfo)*(var_counter + 1));
        tree->variables[var_counter].var_name = (char*) calloc(var_len + 1, sizeof(char));
        memcpy(tree->variables[var_counter].var_name, string, sizeof(char)*var_len);
        tree->variables[var_counter].var_name[var_len] = '\0';
        tree->variables[var_counter].var_id = var_counter;
        var_counter++;

        var_string += var_len;
        SkipSpaces(&var_string);

        if (sscanf(var_string, "%c", &end_symb) == EOF) {
            break;
        }
    }

    free(save_address);
    tree->var_counter = var_counter;
}

//==========================================================================================================================================//

static void ReadFileString(FILE* TreeFile, char** string_take) {
    int string_size = GetStringSize(TreeFile);
    int check_fseek = 0;
    if (!feof(TreeFile)) {
        if (fseek(TreeFile, -string_size - 1, SEEK_CUR)) {
            const char err_messege[] = "fseek error!";
            perror(err_messege);
            exit(EXIT_FAILURE);
        }
    } else {
        // if EOF: file ptr don't move and we should step back by string size number
        if (fseek(TreeFile, -string_size, SEEK_CUR)) {
            const char err_messege[] = "fseek error!";
            perror(err_messege);
            exit(EXIT_FAILURE);
        }
    }
    *string_take = (char*)calloc(string_size + 2, sizeof(char)); 
    Validator(!*string_take, "memory giving error", exit(MEMORY_ALLOC_ERR));

    fgets(*string_take, string_size + 2, TreeFile);  
    (*string_take)[string_size] = '\0';
}

//==========================================================================================================================================//

#define CMP(func_id, func_name, body, address_name) static const int address_name = _##func_id;
    #include "codegeneration.h"
#undef CMP

Node* BuildTree(char** string) {

    SkipSpaces(string);
    printf("I am GetG, i got such string: <%s>\n", *string);
    if(!strlen(*string)) {
        printf("String is empty\n");
        exit(EXIT_FAILURE);
    }
    Node* node = GetExp(string);
    SkipSpaces(string);
    if (**string != '\0') {
        printf("Error: didn't get end simbol <%s>\n", *string);
        // DeleteTree(node);
        exit(EXIT_FAILURE);
    }
    if (_ERROR_FLAG_) {
        PrintError(_ERROR_FLAG_, *string);
        exit(_ERROR_FLAG_);
    }
    return node;
}

//==========================================================================================================================================//

Node* GetExp(char** string) {

    SkipSpaces(string);
    printf("I am GetE, i got such string: <%s>\n", *string);
    
    Node* node = GetT(string);
    printf("I am GetE, i got such string after GetT in line:%d : <%s>\n", __LINE__, *string);

    SkipSpaces(string);

    while (**string == OP_ADD || **string == OP_SUB) {

        int save_symb   = **string;
        (*string)++;

        printf("Calling GetT, current string: <%s>\n", *string);
        Node* node_right = GetT(string);
        Node* node_left  = CopyTree(node, duplic_tree);

        printf("I am GetE, i got such string after GetT in line:%d : <%s>\n", __LINE__, *string);

        if (save_symb == OP_ADD) {
            node = ADD(node_left, node_right);
        }
        else {
            node =SUB(node_left, node_right);
        }

    }

    return node;
}

//==========================================================================================================================================//

Node* GetT(char** string) {

   SkipSpaces(string);
    printf("I am GetT, i got such string: <%s>\n", *string);
    
    Node* node = GetS(string);

    printf("I am GetT, i got such string after GetS: <%s>\n", *string);

    // if (_ERROR_FLAG_) {
    //     return node;
    // }

    while (**string == OP_MUL || **string == OP_DIV) {

        int save_symb   = **string;
        (*string)++;

        printf("Calling GetS, current string: <%s>\n", *string);
        Node* node_right = GetS(string);
        Node* node_left  = CopyTree(node, duplic_tree);

        printf("I am GetT, i got such string after GetS: <%s>\n", *string);

        // if (_ERROR_FLAG_) {
        //     return node;
        // }
        if (save_symb == OP_MUL) {
            node = MUL(node_left, node_right);
        }
        else {
            node = DIV(node_left, node_right);
        }
        SkipSpaces(string);

    }

    // printf("I am GetE, RETURN string: <%s>\n", *string);

    return node;
}

//==========================================================================================================================================//

Node*  GetS(char** string) {

    SkipSpaces(string);
    printf("I am GetS, i got such string: <%s>\n", *string);
    
    Node* node = GetP(string);

    printf("I am GetS, i got such string after GetP: <%s>\n", *string);

    while (**string == OP_POW) {

        (*string)++;

        printf("Calling GetP, current string: <%s>\n", *string);
        Node* node_right = GetP(string);
        Node* node_left  = CopyTree(node, duplic_tree);

        printf("I am GetS, i got such string after GetP: <%s>\n", *string);

        node = POW(node_left, node_right);
    }

    return node;
}

//==========================================================================================================================================//

Node* GetP(char** string) {
    
    SkipSpaces(string);
    printf("I am GetP, i got such string: <%s>\n", *string);
    if (**string == OPEN_BRACKET) {
        (*string)++;
        printf("Calling GetE, current string: <%s>\n", *string);
        Node* data = GetExp(string);
        if (**string != CLOSE_BRACKET) {
            printf("Close bracket is missed: %s\n", *string);
            _ERROR_FLAG_ = MISSING_CLOSE_BRACKET;
        }
        else {
            (*string)++;
        }
        SkipSpaces(string);

        printf("I am GetP, RETURN string: <%s>\n", *string);

        return data;
    }

    return GetFunc(string);
}

//==========================================================================================================================================//

Node* GetFunc(char** string) {
    printf("I am GetF, i got such string: <%s>\n", *string);
    SkipSpaces(string);

    char function_name[MaxFuncSize] = {0};
    int symbol_counter = 0;
    for( ; isalpha(*(*string + symbol_counter)) && symbol_counter < MaxFuncSize; symbol_counter++) {
        function_name[symbol_counter] = *(*string + symbol_counter);
    }

    if (symbol_counter == MaxFuncSize) {
       _ERROR_FLAG_ = INVALID_FUNCTION_NAME_SIZE;
    }

    int func_id = GetFuncId(function_name);
    if (func_id != NotFunction) {
        (*string) += symbol_counter;
    printf("I am GetF, i return such string to GetE: <%s>\n", *string);
        Node* node_left = GetExp(string);
    printf("I am GetF, i got such string after GetE: <%s>\n", *string);

        switch(func_id) {
            case _SIN:    return SIN_    (node_left, nullptr);
            case _COS:    return COS_    (node_left, nullptr);
            case _TG:     return TG_     (node_left, nullptr);
            case _CTG:    return CTG_    (node_left, nullptr);
            case _SH:     return SH_     (node_left, nullptr);
            case _CH:     return CH_     (node_left, nullptr);
            case _TH:     return TH_     (node_left, nullptr);
            case _CTH:    return CTH_    (node_left, nullptr);
            case _ARCSIN: return ARCSIN_ (node_left, nullptr);
            case _ARCCOS: return ARCCOS_ (node_left, nullptr);
            case _ARCTG:  return ARCTG_  (node_left, nullptr);
            case _ARCCTG: return ARCCTG_ (node_left, nullptr);
            case _SQRT:   return SQRT_   (node_left, nullptr);
            case _EXP:    return EXP_    (node_left, nullptr);
            case _LN:     return LN_     (node_left, nullptr);
            default: break;
        }
    }
    printf("I am GetF, i return such string to GetVar: <%s>\n", *string);

    return GetVar(string);
}

//==========================================================================================================================================//

Node* GetVar(char** string) {
    printf("I am GetVar, i got such string: <%s>\n", *string);
    SkipSpaces(string);

    if (isalpha(**string)) {
        char* var = (char*) calloc(MAX_VARIABLE_SIZE, sizeof(char));
        for (int i = 0; i < MAX_VARIABLE_SIZE && isalpha(**string); i++) {
            var[i] = **string;
            (*string)++;
        }
        return CreateNewNode(VAR, var);
    }
    return GetNumber(string);
}

//==========================================================================================================================================//

Node* GetNumber(char** string) {

    SkipSpaces(string);
    printf("I am GetN, i got such string: <%s>\n", *string);

    Node* ret_data   = nullptr;
    int inside_cicle = 0;
    int is_nagative  = 0;
    static int just_for_addres = 0;
    ret_data = CreateNewNode(NUMBER, &just_for_addres);
    if (**string == '-') {
        is_nagative = 1;
        (*string)++;
    }
    if (**string == 'p' && *(*string + 1) == 'i') {
        *string += 2;
        ret_data->value.number = M_PI;
        return ret_data;
    }
    while (isdigit(**string)) { 
        inside_cicle  = 1;
        ret_data->value.number = ret_data->value.number*10 + (**string - '0');
        (*string)++;
        //if floating point number
        if (**string == '.') {
            int divider = 10;
            (*string)++;
            while(isdigit(**string)) {
                ret_data->value.number += (**string - '0') / (elem_t)divider;
                divider *= 10;
                (*string)++;
            }
            break;
        }
    } 

    if (is_nagative) {
        ret_data->value.number *= -1;
    }

    SkipSpaces(string);

    printf("I am GetN, i RETURN such string: <%s>\n", *string);

    return ret_data;
}

static void SkipSpaces(char** string) {
    while (*string && isspace(**string)) {
        (*string)++;
    }
}

//==========================================================================================================================================//

static int GetFuncId(char* func_name) {
    for (size_t i = 0; i < sizeof(_Diff_Functions_)/sizeof(_Diff_Functions_[0]); i++) {
        if (!strcmp(func_name, _Diff_Functions_[i].func_name)) {
            return _Diff_Functions_[i].func_id;
        }
    }
    return NotFunction;
}

//==========================================================================================================================================//

static void PrintError(int err_id, char* string) {
    switch(err_id) {
        case MISSING_CLOSE_BRACKET:      printf("Missing close bracket: <%s>\n", string);                                   break;
        case DIVISION_BY_ZERO:           printf("Trying to divide by '0': <%s>\n", string);                                 break;
        case UNEXPECTED_SYMBOL:          printf("Unexpected symbol: <%s>\n", string);                                       break;
        case INVALID_DATA:               printf("Invalid data: <%s>\n", string);                                            break;
        case INVALID_FUNCTION_NAME_SIZE: printf("Invalid function name size, it must be <= %d: %s\n", MaxFuncSize, string); break;
        default: printf("invalid err_id: %d\n", err_id);
    }
}
//==========================================================================================================================================//

