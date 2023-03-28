#include "..//include//Queue.h"

void QueueInit(Queue* queue, int queue_size) {
        queue->capacity       = queue_size;
        queue->tail_max_value = queue_size - 1;
        queue->elem_address   = (Data*) calloc(queue->capacity, sizeof(Data));
        Validator(!queue->elem_address, in calloc: could not get memory, exit(MEMORY_ALLOC_ERROR));
}

void QueuePush(Queue* queue, Data value) {

    if ((queue->tail == queue->head && !queue->size) || queue->size == queue->capacity) {
        QueueResize(queue);
    }
    queue->elem_address[queue->tail] = value;
    queue->size++;
    queue->tail++;
    queue->tail = queue->tail & queue->tail_max_value;
}

Data QueuePop(Queue* queue) {

    Validator(!queue->size, fprintf(stderr, "\t| " Blue " \tQueue is empty: you can't pop elements from queue." Grey "\n\t|\n"
    ""); return -111;);

    Data ret_value = queue->elem_address[queue->head];
    queue->head++;
    queue->head = queue->head & queue->tail_max_value;
    queue->size--;

    return ret_value;
}

int QueueResize(Queue* queue) {

    queue->tail = queue->capacity;
    queue->capacity = queue->capacity*2;
    queue->tail_max_value = queue->capacity - 1;
    Data* data_ptr = (Data*) realloc(queue->elem_address, sizeof(Data)*queue->capacity);
    Validator(!data_ptr, in realloc: could not get memory, return MEMORY_ALLOC_ERROR);

    queue->elem_address = data_ptr;
}

void _QueueLog(Queue* queue, int line, const char* func_name) {

    FILE* Log_Queue = fopen("data//log_queue.txt", "a");
    Validator(!Log_Queue, in opening file, exit(EXIT_FAILURE););

    fprintf(Log_Queue, "\n*****************************************************Queue_Info********************************************"
    "*****************************************\n");
    
    fprintf(Log_Queue, "%s was called from '%d' line and function: '%s'\n", __FUNCTION__, line, func_name);
    fprintf(Log_Queue, "queue's tail      = %d\n", queue->tail);
    fprintf(Log_Queue, "queue's head      = %d\n", queue->head);
    fprintf(Log_Queue, "queue's size      = %d\n", queue->size);
    fprintf(Log_Queue, "queue's capacity  = %d\n", queue->capacity);
    fprintf(Log_Queue, "        Queue elemenst:\n");

    for (size_t number = 0; number < queue->size; number++) {
        fprintf(Log_Queue, "[%zd] = %lg\n", queue->head + number, queue->elem_address[queue->head  + number]);
    }

    fprintf(Log_Queue, "**************************************************************************************************************"
    "**************************************\n\n");
    char is_file_close = fclose(Log_Queue);
    Validator(is_file_close, in closing file, exit(EXIT_FAILURE););
}

void QueueDtor (Queue* queue) {
    queue->capacity = 0;
    queue->head     = 0;
    queue->tail     = 0;
    queue->size     = 0;
    queue->tail_max_value = 0;
    free(queue->elem_address);
}