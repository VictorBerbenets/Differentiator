#include "include//stack.h"

#define Grey   "\033[0m"
#define Red    "\033[0;31m"
#define White  "\033[1m"
#define Blue   "\033[0;36m"
#define Under  "\033[4m"

const int Two_canarys = 2;
const int One_canary  = 1;

#define STACK_ASSERT(expression, ...) do {if (!(expression)) {                                 \
        FILE* Errors = fopen("Errors.txt", "a");                                                \
        fprintf(Errors, "%s:%d: In function '%s':\n", __FILE__, __LINE__, __FUNCTION__);         \
        fprintf(Errors, "%s:%d: error:STACK_ASSERT expression is false\n\t|\t%s"                  \
        "\n\t|\n", __FILE__, __LINE__, #expression);                                               \
        fclose(Errors);                                                                             \
        __VA_ARGS__;                                                                                 \
}                                                                                                     \
} while (0)



void StackCtor(stack* st){

#ifdef STACK_CANARY
    st -> canary_left      = Canary_Value;
#endif

#ifdef STACK_HASH_DATA
    st -> hash_data        = 0;
#endif 

#ifdef STACK_HASH
    st -> hash_stk         = 0;
#endif
    st -> data             = nullptr;
    st -> capacity         = 0;
    st -> size             = 0;
    st -> stack_is_damaged = 0;
#ifdef STACK_CANARY
    st -> canary_right     = Canary_Value;
#endif
}

void StackDtor(stack* st){ 

    if (st -> size != 0 && st -> data) {
        DoPoison(st);
    }
    
    free(st -> data);

    st -> data             = nullptr;
    st -> size             = 0;
#ifdef STACK_HASH_DATA
    st -> hash_data        = 0;
#endif
#ifdef STACK_HASH
    st -> hash_stk         = 0;
#endif
    st -> stack_is_damaged = 0;
    st -> capacity         = 0;
#ifdef STACK_CANARY
    st -> canary_left      = 0;
    st -> canary_right     = 0;
#endif
}

void StackPush(stack* st, Data value){

    _StackCheck(st);
    if (IsStackFull(st) == StackFull || st->capacity == 0) {

        StackResize(st, PUSH);
        if (st -> size != 0 && st -> data) {
            DoPoison(st);
        }
    }
#ifdef STACK_DATA_CANARY_PROTECT
    st -> size ++;
    st -> data[st -> size] = value;
#else
    st -> data[st -> size] = value;
    st -> size ++;
#endif

#ifdef STACK_HASH_DATA
    st -> hash_data = hash_data(st);
#endif
#ifdef STACK_HASH
    st -> hash_stk  = 0;
    st -> hash_stk  = hash_stack(st);
#endif

    _StackCheck(st);
}

Data StackPop (stack* st){

    _StackCheck(st);
    if (st -> size > 0) {

        Data res = st -> data[(st -> size)];

        if ((st->capacity - st->size) > (int)MaxStayedSize){
            StackResize(st, POP);
        }
        if (IsStackEmpty(st) == StackNotEmpty){
            st -> size --;

#ifdef  STACK_HASH_DATA
            st -> hash_data = hash_data(st);
#endif
#ifdef  STACK_HASH
            st -> hash_stk  = 0;
            st -> hash_stk  = hash_stack(st);
#endif
        if (st -> size != 0 && st -> data) {
            DoPoison(st);
        }
            return res;
        }
    }
    return StackIsEmpty;   
}

void StackResize(stack* st, int Push_or_Pop){

    _StackCheck(st);

    switch(Push_or_Pop){

        case PUSH:{ 
            StackRecalloc(Push_or_Pop, st, st->capacity + (st->size / 2) + 1); // увеличиваем capacity, когда нужно место для Push
            break;
        }
        case POP:{ 
            StackRecalloc(Push_or_Pop, st, st->capacity - MinusSize); // уменьшаем capacity, когда достали много элементов из стэка
            break;
        }   
    }
}

void StackRecalloc (int Push_or_POP, stack* st, int new_size){

    _StackCheck(st);

    st -> capacity           = new_size;
    Data* check_memory_give  = nullptr;

#ifdef STACK_DATA_CANARY_PROTECT
    check_memory_give = (Data*)realloc(st->data, sizeof(Data) * (st -> capacity + Two_canarys));
    STACK_ASSERT(check_memory_give != nullptr, fprintf(stderr, "%s:%d: " Red "error: " Grey" in function '%s' - realloc returned "
    "nullptr.\n", __FILE__, __LINE__, __FUNCTION__); return ;);

    st -> data    = check_memory_give;
    st -> data[0] = Canary_left;
    st -> data[st -> capacity + 1] = Canary_right;

#else
    check_memory_give = ((Data*)realloc(st->data, sizeof(Data) * (st -> capacity)));
    STACK_ASSERT(check_memory_give != nullptr, fprintf(stderr, "%s:%d: " Red "error: " Grey" in function '%s' - realloc returned "
    "nullptr.\n", __FILE__, __LINE__, __FUNCTION__); return ;);

    st -> data   = check_memory_give;
#endif

#ifdef STACK_HASH
    st -> hash_stk  = 0;
    st -> hash_stk  = hash_stack(st);
#endif
}


void StackCheck(stack* st, int line, const char* func, const char* file){ // StackVerificate()

if(st -> size != 0 || st -> capacity != 0) {

    int err_code = 0;

    if (!(st -> data) && (st -> size || st -> capacity)) { 
        err_code += STACK_IS_NULLPTR; 
    } 
    else {
#ifdef STACK_DATA_CANARY_PROTECT
        if (st -> data[st -> capacity + 1] != Canary_right) { err_code += DATA_CANARY_RIGHT; }

        if (st -> data[0] != Canary_left)                   { err_code += DATA_CANARY_LEFT; }
#endif
#ifdef STACK_CANARY
        if (st -> canary_left  != Canary_Value)             { err_code += STACK_CANARY_LEFT;  }

        if (st -> canary_right != Canary_Value)             { err_code += STACK_CANARY_RIGHT; }
#endif
        if (st -> size < 0)                                 { err_code += INVALID_SIZE; }
        else {
            if   (st -> size > st -> capacity)                  { err_code += INVALID_CAPACITY; }
            else {
                if (IsDataValid(st) == ElemIsPoison)            { err_code += ElemOfStackIsPoison; }
#ifdef STACK_HASH_DATA
                if (!is_equal(st -> hash_data, hash_data(st)))  { err_code += HASH_DATA_ERROR; }
#endif                                                                          
#ifdef STACK_HASH
                if (!is_equal(st -> hash_stk, hash_stack(st)))  { err_code += HASH_STACK_ERROR; } 
#endif
            }
        }
    }
    if (err_code) { 
        st -> stack_is_damaged = 1;
        PrintError(st, err_code, line, func, file);
    }
}
}

void PrintError (stack* st, int ErrCode, int line, const char* func, const char* file){

    int number_of_errors = 0;

    if (ErrCode & INVALID_CAPACITY) { 
        fprintf(stderr, "" White "%s:%d:" Grey "In function " White "'%s':" Grey "\n", file, line, func);
        fprintf(stderr, "" White "%s:%d:" Red " error: " Grey "Size of the Stack > capacity. Size of stack must be '<= capacity' "
        "(check log.txt for more information)\n\t|\t" Blue "'st -> size = %d > st ->capacity = %d'" Grey "\n\t|"
        "\n", file, line, st -> size, st ->capacity);
        number_of_errors ++;
    }
    if (ErrCode & INVALID_SIZE) {
        fprintf(stderr, "" White "%s:%d:" Grey "In function " White "'%s':" Grey "\n", file, line, func);
        fprintf(stderr, "" White "%s:%d:" Red " error: " Grey "Invalid size of stack. Size of stack must be '> 0' "
        "(check log.txt for more information)\n\t|\t" Blue "'st -> size = %d < 0'" Grey "\n\t|\n", file, line, st -> size);
        number_of_errors ++;
    }
    if (ErrCode & ElemOfStackIsPoison) {
        fprintf(stderr, "" White "%s:%d:" Grey "In function " White "'%s':" Grey "\n", file, line, func);
        fprintf(stderr, "" White "%s:%d:" Red " error: " Grey "Element of stack is poison(check log.txt for more information)\n", \
        file, line);
        number_of_errors ++;
    }
    if (ErrCode & STACK_IS_NULLPTR) {
        fprintf(stderr, "" White "%s:%d:" Grey "In function " White "'%s':" Grey "\n", file, line, func);
        fprintf(stderr, "" White "%s:%d:" Red " error: " Grey "Stack is nullptr(check log.txt for more information)\n\t|\t" Blue ""
        "'st = %p'" Grey "\n\t|\n", file, line, st -> data);
        number_of_errors ++;
    }
#ifdef STACK_DATA_CANARY_PROTECT
    if (ErrCode & DATA_CANARY_LEFT) { 
        fprintf(stderr, "" White "%s:%d:" Grey "In function " White "'%s':" Grey "\n", file, line, func);
        fprintf(stderr, "" White "%s:%d:" Red " error: " Grey "Canary left was damaged(check log.txt for more information)\n\t|\t" Blue ""
        "'st[%d] = %lg != %lld'" Grey "\n\t|\n", file, line, -1, st->data[0], Canary_Value);
        number_of_errors ++;
    }
    if (ErrCode & DATA_CANARY_RIGHT) { 
        fprintf(stderr, "" White "%s:%d:" Grey "In function " White "'%s':" Grey "\n", file, line, func);
        fprintf(stderr, "" White "%s:%d:" Red " error: " Grey "Canary right was damaged(check log.txt for more information)\n\t|\t" Blue ""
        "'st[%d] = %lg != %lld'" Grey "\n\t|\n", file, line, st -> capacity, st->data[st -> capacity + 1], Canary_Value);
        number_of_errors ++;
    }
#endif
#ifdef STACK_CANARY
    if (ErrCode & STACK_CANARY_LEFT) { 
        fprintf(stderr, "" White "%s:%d:" Grey "In function " White "'%s':" Grey "\n",file, line, func);
        fprintf(stderr, "" White "%s:%d:" Red " error: " Grey "Stack canary 1 was damaged(check log.txt for more information)\n\t|\t" Blue ""
        "'st->canary_2 = %lld != %lld'" Grey "\n\t|\n",file, line, st->canary_left, Canary_Value);
        number_of_errors ++;
    }
    if (ErrCode & STACK_CANARY_RIGHT) { 
        fprintf(stderr, "" White "%s:%d:" Grey "In function " White "'%s':" Grey "\n", file, line, func);
        fprintf(stderr, "" White "%s:%d:" Red " error: " Grey "Stack canary 2 was damaged(check log.txt for more information)\n\t|\t" Blue ""
        "'st->canary_2 = %lld != %lld'" Grey "\n\t|\n", file, line, st->canary_right, Canary_Value);
        number_of_errors ++;
    }
#endif
#ifdef STACK_HASH
    if (ErrCode & HASH_STACK_ERROR) {
        fprintf(stderr, "" White "%s:%d:" Grey "In function " White "'%s':" Grey "\n", file, line, func);
        fprintf(stderr, "" White "%s:%d:" Red " error: " Grey "Hash of stack was damaged(check log.txt for more information)\n\t|\t" Blue ""
        "'hash_stk = %lld != %lld'" Grey "\n\t|\n", file, line, st -> hash_stk, hash_stack(st));
        number_of_errors ++;
    }
#endif
#ifdef STACK_HASH_DATA
    if (ErrCode & HASH_DATA_ERROR) {
        fprintf(stderr, "" White "%s:%d:" Grey "In function " White "'%s':" Grey "\n", file, line, func);
        fprintf(stderr, "" White "%s:%d:" Red " error: " Grey "Hash of data was damaged(check log.txt for more information)\n\t|\t" Blue ""
        "'hash_data = %lld != %lld'" Grey "\n\t|\n", file, line, st -> hash_data, hash_data(st));
        number_of_errors ++;
    }
#endif
    if (ErrCode) {

        if(!(ErrCode & STACK_IS_NULLPTR)) {
            _StackDump(st);
        }
        StackDtor(st);
        exit(EXIT_FAILURE);
    }
}

int IsStackFull(stack* st){

    _StackCheck(st);
    
    if (st -> size ==  st -> capacity)
        return StackFull;
    return StackNotFull;
}


int IsStackEmpty(stack* st){

    _StackCheck(st);

    if ((st -> size == 0))
        return StackIsEmpty;
    return StackNotEmpty;
}

void DoPoison(stack*st){

    int difference = st->capacity - st->size;
    int stack_number = 0;
#ifdef STACK_DATA_CANARY_PROTECT
    stack_number ++;
    difference   ++;
#endif
    for ( ; stack_number < difference; stack_number++) {
        st->data[st->size + stack_number] = POISON;
    }
}

int IsDataValid(stack* st){

    int stack_number       = 0;
    int max_st_number_size = st -> size;
#ifdef STACK_DATA_CANARY_PROTECT
    stack_number      ++;
    max_st_number_size ++;
#endif
    for ( ;stack_number < max_st_number_size; stack_number++){
        if (st->data[stack_number] == POISON){
            return ElemIsPoison;
        }
    }
    return NoPoison;
}

Ull hash_data(stack* st) {

    double mult    = 0.5;
    double hash    = 0;

    for (int st_number = 1; st_number <= st -> size; st_number++) {

        hash *= mult;
        hash += st -> data[st_number];
    }
    return hash;
}

int is_equal(Data value1, Data value2) {

    return (fabs(value1 - value2) < Epsilon);
}

#ifdef STACK_HASH
Ull hash_stack(stack* st) {


    size_t mult      = 0.5;
    double hash_stack  = 0;

    double stack_hash  = st -> hash_stk;
    st -> hash_stk   = 0;

    char* hash_ptr   = (char*)st;

    for (size_t counter = 0; counter < sizeof(*st); counter++, hash_ptr++) {

        hash_stack *= mult;
        hash_stack += *hash_ptr;
        
    }
    st -> hash_stk = stack_hash;
    return hash_stack / (hash_stack + 1);
}
#endif

/*Вывод общей информации*/
void StackDump (stack* st, int line, const char func[], const char* stack_name)
{          
    FILE* log_txt = fopen("log.txt", "a");
    
    fprintf(log_txt, "\n***********************************************************************************\n");

    fprintf(log_txt, "StackDump was called from '%d' line and function: '%s'\n", line, func);
    fprintf(log_txt, "Stack name: %s\n",                          stack_name);
    fprintf(log_txt, "\t\tElements in stack(st.size):   %d\n",     st->size);
#ifdef STACK_CANARY
    fprintf(log_txt, "\t\tStack Canary left            = %lld        \n",     st->canary_left);
    fprintf(log_txt, "\t\tStack Canary right           = %lld        \n",     st->canary_right);
#endif
    fprintf(log_txt, "\t\tStack capacity:               %d\n", st->capacity); 
    fprintf(log_txt, "\t\tStack pointer(data):          %p\n", st->data);
    fprintf(log_txt, "\t\tStack is damaged?:           '%s'\n", st -> stack_is_damaged == 1 ? "YES":"NO");
    fprintf(log_txt, "%s\n", st->capacity == 0 ? "Stack is empty":"Elements in stack:");

    if (st -> data) {

    int stack_number = 0;
    int max_counter  = st -> capacity;
#ifdef STACK_DATA_CANARY_PROTECT
    fprintf(log_txt, " [%d] = %lg Data Canary_left\n", 0, st ->data[0]);
    max_counter  ++;
    stack_number = 1;
#endif
    for ( ; stack_number < max_counter; stack_number++){

        if (st->data[stack_number] != POISON){
            fprintf(log_txt, "*[%d] = %c\n", stack_number, st->data[stack_number]);
        }
        else {
            fprintf(log_txt, " [%d] = %c POISON\n", stack_number, st->data[stack_number]);
        }
    }
#ifdef STACK_DATA_CANARY_PROTECT
    fprintf(log_txt, " [%d] = %lg Data Canary_right\n", st ->capacity + 1, st ->data[st -> capacity + 1]);
#endif
    }
    fprintf(log_txt, "************************************************************************************\n\n");
    fclose(log_txt);
}