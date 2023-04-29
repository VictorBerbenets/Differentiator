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
#include "parsing.h"
#include "DSL.h"
#include <math.h>

typedef double elem_t;

typedef struct {
    char* var_name;
    int  var_id;
} VarInfo;

typedef struct {
    Node* Root = nullptr;
    int var_counter    = 0;
    VarInfo* variables = nullptr;
    Node** Diff_trees  = nullptr;
} Tree;

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

char* ReadFile(const char* file_name, Tree* tree);

void ConstructTree(const char* file_name, Tree* tree);

Node* CreateNewNode(int TYPE_NUM, const void* value, Node* left_node = nullptr, Node* right_node = nullptr);

void ConverteTreeToPdf(Tree* tree);

void WriteTreeToPdf(Node* tree, FILE* tree_pdf);

void CreateGraphNode(FILE* dot_file, Node* ptr, int* node_counter);
void CreateNextGraphNode(FILE* dot_file, Node* ptr, int* node_head, int* node_next, Position position);

Node* Diff(Node* node, const char* var_name);
elem_t Ebal(Node* node_ptr, elem_t value, const char* var_name);
elem_t CalculateNumbers(Node* node_ptr);
elem_t GetPower(Node* base, Node* degree, elem_t value);
elem_t GetDiv(Node* dividend, Node* divisor, elem_t value);
Node* SimplifyTree(Node* tree);

void PreOrder(Node* tree, FILE* file_name);
void InOrder(Node* tree, FILE* file_name);
void PostOrder(Node* tree, FILE* file_name);

int PrintTreeToFile(Node* tree, PrintType type);

void DeleteAllTrees(Tree* tree);
void DeleteTree(Node* tree);

int TreeDump(Node* tree);

size_t GetStringSize (FILE* TreeFile);

int IsEqual(elem_t value1, elem_t value2);

Node* CopyTree(Node* tree_to_copy, Node* duplic_tree);

void PrintTree(Node* tree);

void PrintMakloren(Tree* tree, const char* file_name, elem_t number, int decompos_number);

void  FindVariables(FILE* TreeFile, Tree* tree);

void CreatePartialDerivatives(Tree* tree);
void DerivativeOnALLVars(Tree* tree);
#endif