#ifndef Queue_Struct
#define Queue_Struct

#include <stdio.h>
#include <stdlib.h>
#include "colors.h"
#include "validator.h"
#include "Node.h"

const int QueueInitSize = 1024;

typedef Node* Data_;
typedef struct {
    Data_* elem_address;
    int tail_max_value;
    int head;
    int tail;
    int size;
    int capacity;
} Queue;


enum Queue_Errors {
    
    MEMORY_ALLOC_ERROR = 14888814,
    INVALID_DATA_SIZE  = 0xDEADBEEF1,
    INVALID_POP_VALUE  = 0xDEADBEEF2,
} ;

void QueuePush(Queue* queue, Data_ value);
Data_ QueuePop(Queue* queue);
void _QueueLog(Queue* queue, int line, const char* func_name);
int  QueueResize(Queue* queue);
void QueueInit(Queue* queue, int queue_size);
void QueueDtor (Queue* queue);

#define QueueLog(queue) _QueueLog(queue, __LINE__, __PRETTY_FUNCTION__);

#endif
