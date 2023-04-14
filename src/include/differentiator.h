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
#include <math.h>

typedef double elem_t;

typedef struct {
    char* buffer;
    size_t buffer_size;
} Buffer;

enum TreeConsts {
    ALL_RIGHT     = 0,
    TREE_INITED   = 1,
    ONE_NODE      = 2,
    VAR_IS_FOUND  = 10,
};

enum Position {
    LEFT,
    RIGHT,
};

enum TreeErrors {
    INVALID_STRING_DATA     = -10,
    INVALID_STRING_POINTER  = 1,
    ERROR_IN_READING_FILE   = 2, 
    INVALID_TREE_POINTER    = 3,
    CLOSING_FILE_ERROR      = 4,
    READING_FILE_ERROR      = 5,
    FREAD_READING_ERROR     = 6,   
    INVALID_PRINT_TYPE      = 7,
    MEMORY_ALLOC_ERR        = 8,
    INVALID_OPERATOR        = 9,
    INVALID_NODE            = 10,
    DIVIDE_ERROR            = 11,
}; 

Buffer ReadFile(const char* file_name);
Node* ConstructTree(const char* file_name);
Node* BuildTree(Node* tree, Buffer* tree_buffer);
Node* CreateNewNode(int TYPE_NUM, const void* value, Node* left_node = nullptr, Node* right_node = nullptr);

void ConverteTreeToPdf(Node** tree);
void WriteDerivativeToPdf(Node** tree, FILE* tree_pdf);

void CreateGraphNode(FILE* dot_file, Node* ptr, int* node_counter);
void CreateNextGraphNode(FILE* dot_file, Node* ptr, int* node_head, int* node_next, Position position);

Node* Diff(Node* node, const char* var_name);
elem_t Ebal(Node* node_ptr, elem_t value);
elem_t CalculateNumbers(Node* node_ptr);
elem_t GetPower(Node* base, Node* degree, elem_t value);
elem_t GetDiv(Node* dividend, Node* divisor, elem_t value);
Node* SimplifyTree(Node* tree);

void PreOrder(Node* tree, FILE* file_name);
void InOrder(Node* tree, FILE* file_name);
void PostOrder(Node* tree, FILE* file_name);

int PrintTreeToFile(Node* tree, PrintType type);

void DeleteTree(Node* tree);

int TreeDump(Node* tree);

size_t GetFileSize (const char *file);

int IsEqual(elem_t value1, elem_t value2);

Node* CopyTree(Node* tree_to_copy, Node* duplic_tree);

void PrintTree(Node* tree);

#endif