#ifndef Queue_Struct
#define Queue_Struct

#include <stdio.h>
#include <stdlib.h>
#include "colors.h"
#include "validator.h"
#include <iostream>

typedef double Data;
typedef struct {
    Data* elem_address;
    int tail_max_value;
    int head;
    int tail;
    int size;
    int capacity;
} Queue;


enum Queue_Errors {
    
    MEMORY_ALLOC_ERROR,
    INVALID_DATA_SIZE = 0xDEADBEEF1,
    INVALID_POP_VALUE = 0xDEADBEEF2,
} ;

#define QueueLog(queue) _QueueLog(queue, __LINE__, __PRETTY_FUNCTION__);
void _QueueLog(Queue* queue, int line, const char* func_name);
int QueueResize(Queue* queue);
Data QueuePop(Queue* queue);
void QueuePush(Queue* queue, Data value);
void QueueInit(Queue* queue, int queue_size);
void QueueDtor (Queue* queue);

#endif