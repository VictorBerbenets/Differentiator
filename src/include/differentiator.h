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
#include "DSL.h"
#include <cmath>

typedef double elem_t;

typedef struct {
    char* buffer;
    size_t buffer_size;
} Buffer;

enum TreeConsts {
    ALL_RIGHT   = 0,
    TREE_INITED = 1,
    ONE_NODE    = 2
};

enum Position {
    LEFT,
    RIGHT,
};

enum TreeErrors {
    ERROR_IN_READING_FILE, 
    INVALID_TREE_POINTER,
    CLOSING_FILE_ERROR,
    READING_FILE_ERROR,
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
Node* BuildTree(Node* tree, Buffer* tree_buffer);
Node* CreateNewNode(int TYPE_NUM, const void* value, Node* left_node = nullptr, Node* right_node = nullptr);

void CreateGraphNode(FILE* dot_file, Node* ptr, int* node_counter);
void CreateNextGraphNode(FILE* dot_file, Node* ptr, int* node_head, int* node_next, Position position);

Node* Diff(Node* node);
elem_t Ebal(Node* node_ptr);
elem_t GetPower(Node* base, Node* degree);
elem_t GetDiv(Node* dividend, Node* divisor);

void PreOrder(Node* tree, FILE* file_name);
void InOrder(Node* tree, FILE* file_name);
void PostOrder(Node* tree, FILE* file_name);

int PrintTreeToFile(Node* tree, PrintType type);

void DeleteTree(Node* tree);

int TreeDump(Node* tree);

size_t GetFileSize (const char *file);

int IsEqual(elem_t value1, elem_t value2);

int IsDigitDigit(char* string);
#endif