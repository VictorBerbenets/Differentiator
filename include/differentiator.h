#ifndef AKINATOR_H
#define AKINATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "colors.h"
#include "validator.h"
#include "operators.h"

static int TabsForTreePrint = 0;

typedef double elem_t;

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
    MEMORY_ALLOC_ERR,
    INVALID_OPERATOR,
    INVALID_NODE,
    DIVIDE_ERROR,
};

// void TreeCtor(Node** tree);

// Node* InsertNode(Node* prev_node, elem_t value);
Node* CreateTree(const char* file_name);

Node* CreateNewNode(int TYPE_NUM, elem_t value, Node* left_node, Node* right_node);

void CreateGraphNode(FILE* dot_file, Node* ptr, int* node_counter);
void CreateNextGraphNode(FILE* dot_file, Node* ptr, int* node_head, int* node_next, Position position);

double Ebal(const Node* node_ptr);

void PrintTree(const Node* tree);

void DeleteTree(const Node* tree);

int TreeDump(const Node* tree);

size_t GetFileSize (const char *file);

#endif