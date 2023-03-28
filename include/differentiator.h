#ifndef AKINATOR_H
#define AKINATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "colors.h"
#include "validator.h"
#include "operators.h"
#include "..//include//print.h"
// #include "Queue.h"

static int TabsForTreePrint = 0;

typedef double elem_t;

typedef struct {
    char* buffer;
    unsigned int buffer_size;
} Buffer;

struct Node{
    int type;  
    union data{
        int oper;
        double number;
    } value;

    Node* left_branch  = nullptr;
    Node* right_branch = nullptr;
};
typedef struct Node Node;

typedef struct {
    Node** elem_address;
    int head;
    int tail;
    int size;
    int capacity;
} Queue;

const int QueueInitSize = 10;
void queue_init(Queue* queue, int size);
void queue_enqueue(Queue* queue, Node* value);
Node* queue_dequeue(Queue* queue);
void queue_dtor(Queue* queue);
int queue_is_empty(Queue* queue);


enum ListConsts {

    TREE_INITED,
    ONE_NODE,
    ALL_RIGHT,
};

enum Position {
    LEFT,
    RIGHT,
};

enum ListErrors {
    ERROR_IN_READING_FILE,
    FREAD_READING_ERROR,
    INVALID_PRINT_TYPE,
    MEMORY_ALLOC_ERR,
    INVALID_OPERATOR,
    INVALID_NODE,
    DIVIDE_ERROR,
};

// void TreeCtor(Node** tree);
Buffer ReadFile(const char* file_name);
// Node* InsertNode(Node* prev_node, elem_t value);
Node* ConstructTree(const char* file_name);
Node* ConstructNode(Buffer* buff);

Node* CreateNewNode(int TYPE_NUM, elem_t value, Node* left_node, Node* right_node);

void CreateGraphNode(FILE* dot_file, Node* ptr, int* node_counter);
void CreateNextGraphNode(FILE* dot_file, Node* ptr, int* node_head, int* node_next, Position position);

elem_t Ebal(const Node* node_ptr);

void PreOrder(Node* tree, FILE* file_name);
void InOrder(Node* tree, FILE* file_name);
void PostOrder(Node* tree, FILE* file_name);

int PrintTreeToFile(Node* tree, PrintType type);
void PrintTree(const Node* tree);

void DeleteTree(const Node* tree);

int TreeDump(const Node* tree);

size_t GetFileSize (const char *file);

#endif