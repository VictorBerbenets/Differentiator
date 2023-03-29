#ifndef AKINATOR_H
#define AKINATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "colors.h"
#include "validator.h"
#include "operators.h"
#include "print.h"
#include "Node.h"
#include "Queue.h"

typedef double elem_t;

typedef struct {
    char* buffer;
    size_t buffer_size;
} Buffer;

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

elem_t Ebal(Node* node_ptr);

void PreOrder(Node* tree, FILE* file_name);
void InOrder(Node* tree, FILE* file_name);
void PostOrder(Node* tree, FILE* file_name);

int PrintTreeToFile(Node* tree, PrintType type);

void DeleteTree(Node* tree);

int TreeDump(Node* tree);

size_t GetFileSize (const char *file);

int is_equal(elem_t value1, elem_t value2);
#endif