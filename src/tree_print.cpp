#include "include//differentiator.h"
#include "include//graphviz.h"
// #include "include//stack.h"
#include "include//Queue.h"
#include <string.h>
#include <ctype.h>

//******************************************************************************************************************************************//
static void PrintExpression(FILE* tex_file, Tree* tree, elem_t number, int degree);
//==========================================================================================================================================//
int TreeDump(Node* tree) {
    static int graph_number = 0;
    if (!tree) {
        fprintf(stderr, "Cant create Graph_Dump: tree pointer = %p\n", tree);
        return INVALID_TREE_POINTER;
    }
    DotStartGraph("data//list.dot");
    Validator(dot_file == nullptr, "in opening file:'data//list.dot'", return ERROR_IN_READING_FILE;);

    const char dot_header[] = "digraph List {\n"
                              "\tdpi = 100;\n"
                              "\tfontname = \"Comic Sans MS\";\n"
                              "\tfontsize = 20;\n"
                              "\trankdir  = TB;\n";
    DotPrint(dot_header);
    DotSetGraph("lightgreen", 1.3, 0.5, "rounded", "green", 2.);
    DotSetEdge("black", "diamond", 1., 1.2);

    Queue queue = {};
    
    QueueInit(&queue, QueueInitSize);
    QueuePush(&queue, (Node*)tree);
    
    int node_head = 1;
    int node_next = 2;

    while (queue.size) {

        Node* ptr = QueuePop(&queue);
        CreateGraphNode(dot_file, ptr, &node_head);
        if (ptr->left_branch) {
            CreateNextGraphNode(dot_file, ptr, &node_head, &node_next, LEFT);
            QueuePush(&queue, ptr->left_branch);
        }
        if (ptr->right_branch) {
            CreateNextGraphNode(dot_file, ptr, &node_head, &node_next, RIGHT);
            QueuePush(&queue, ptr->right_branch);

        }
        DotPrint("\n");
        node_head++;
    }

    DotEndGraph(dot_file);
    DotPrintGraph(file, graph_number);
    QueueDtor(&queue);
    graph_number++;

    return ALL_RIGHT;
}

//==========================================================================================================================================//

void CreateNextGraphNode(FILE* dot_file, Node* ptr, int* node_head, int* node_next, Position position) {
    if (!ptr) {
        return ;
    }
    CreateGraphNode(dot_file, ptr, node_next);
    DotPrint("node%d: <%s> -> node%d; ", *node_head, position == LEFT ? "ptr1":"ptr2", *node_next);
    (*node_next)++;
}

//==========================================================================================================================================//
void CreateGraphNode(FILE* dot_file, Node* ptr, int* node_counter) {
    if (ptr->type == NUMBER) {
        DotPrint("node%d [shape = Mrecord, style = filled, fillcolor = \"#FFD0D0\", label ="
                 "\"{address: %p|value: %lg| { <ptr1> left: %p| <ptr2> right: %p|parent: %p}}\"]\n",
                *node_counter,  ptr, ptr->value.number, ptr->left_branch, ptr->right_branch, ptr->parent);
    }
    else if(ptr->type == OPER) {
        DotPrint("node%d [shape = Mrecord, style = filled, fillcolor = \"#ABFFF1\", label ="
                 "\"{address: %p|operator: '%c'| { <ptr1> left: %p| <ptr2> right: %p|parent: %p}}\"]\n",
                *node_counter,  ptr, ptr->value.oper, ptr->left_branch, ptr->right_branch, ptr->parent); 
    }
    else if (ptr->type == VAR) {
        DotPrint("node%d [shape = Mrecord, style = filled, fillcolor = \"#EEAAF1\", label ="
            "\"{address: %p|variable: %s| { <ptr1> left: %p| <ptr2> right: %p |parent: %p}}\"]\n",
            *node_counter,  ptr, ptr->value.var, ptr->left_branch, ptr->right_branch, ptr->parent);
    }
    else if (ptr->type == FUNC) {
        DotPrint("node%d [shape = Mrecord, style = filled, fillcolor = \"#B91FAF\", label ="
            "\"{address: %p|function: %s()| { <ptr1> left: %p| <ptr2> right: %p|parent: %p}}\"]\n",
            *node_counter,  ptr, _Diff_Functions_[ptr->value.func].func_name, ptr->left_branch, ptr->right_branch, ptr->parent);
    }

    else {
        printf("Fucking error, man!!!\n");
    }
}

//==========================================================================================================================================//

int PrintTreeToFile(Node* tree, PrintType type) {

    const char* file_name = nullptr;
    switch (type) {
        case IN_ORDER  : file_name = "data//in_order_tree.txt";     break;
        case PRE_ORDER : file_name = "data//pre_order_tree.txt";    break;
        case POST_ORDER: file_name = "data//post_order_tree.txt";   break;
        default: fprintf(stderr, "%s:%d:error: invalid print type: '%d'\n", __PRETTY_FUNCTION__, __LINE__, type); 
                return INVALID_PRINT_TYPE;
    }

    FILE* TreeFile = fopen(file_name, "w");
    Validator(TreeFile == nullptr, "in opening file", return READING_FILE_ERROR;);

    switch(type) {
        case IN_ORDER  : InOrder(tree, TreeFile);         break;
        case PRE_ORDER : PreOrder(tree, TreeFile);        break;
        case POST_ORDER: PostOrder(tree, TreeFile);       break;
        default: fprintf(stderr,"What the fuck???\n");    break;
    }
    int is_file_closed = fclose(TreeFile);
    Validator(TreeFile == nullptr, "in closing file", return CLOSING_FILE_ERROR;);
    return ALL_RIGHT;
}

//==========================================================================================================================================//

void PreOrder(Node* tree,  FILE* Tree_file) {

    if (!tree) { 
        return; 
    }

    fprintf(Tree_file, "(");
    if (tree->type == VAR) {
        fprintf(Tree_file, "%s ", tree->value.var);         
    }
    else if (tree->type == FUNC) {
        fprintf(Tree_file, "%s ", _Diff_Functions_[tree->value.func].func_name);
    }
    else if (tree->type == OPER) {
        fprintf(Tree_file, "%c ", tree->value.oper);
    }
    else if (tree->type == NUMBER) {
        fprintf(Tree_file, "%lg ", tree->value.number);
    }
    else {
        fprintf(Tree_file, "Nuchai bebru, invalid tree type: %d\n", tree->type);
    }

    PreOrder(tree->left_branch, Tree_file);
    PreOrder(tree->right_branch, Tree_file);
    fprintf(Tree_file, ")");

    return ;
}

//==========================================================================================================================================//

void InOrder(Node* tree, FILE* Tree_file) {
    
    if (!tree) { 
        return; 
    }

    fprintf(Tree_file, "(");
    InOrder(tree->left_branch, Tree_file);
    if (tree->type == VAR) {
        fprintf(Tree_file, "%s ", tree->value.var);
    }
    else if (tree->type == FUNC) {
        fprintf(Tree_file, "%s ", _Diff_Functions_[tree->value.func].func_name);
    }
    else if (tree->type == OPER) {
        fprintf(Tree_file, "%c ", tree->value.oper);
    }
    else if (tree->type == NUMBER) {
        fprintf(Tree_file, "%lg ", tree->value.number);
    }
    else {
        fprintf(Tree_file, "Nuchai bebru, invalid tree type: %d\n", tree->type);
    }
    InOrder(tree->right_branch, Tree_file);
    fprintf(Tree_file, ")"); 

    return ;
}

//==========================================================================================================================================//

void PostOrder(Node* tree, FILE* Tree_file) {
    
    if (!tree) { 
        return; 
    }
    fprintf(Tree_file, "(");
    PostOrder(tree->left_branch, Tree_file);
    PostOrder(tree->right_branch, Tree_file);
    if (tree->type == VAR) {
        fprintf(Tree_file, "%s ", tree->value.var);
    }
    else if (tree->type == FUNC) {
        fprintf(Tree_file, "%s ", _Diff_Functions_[tree->value.func].func_name);
    }
    else if (tree->type == OPER) {
        fprintf(Tree_file, "%c ", tree->value.oper);
    }
    else if (tree->type == NUMBER) {
        fprintf(Tree_file, "%lg ", tree->value.number);
    }
    else {
        fprintf(Tree_file, "Nuchai bebru, invalid tree type: %d\n", tree->type);
    }

    fprintf(Tree_file, ")"); 

    return ;
}

//==========================================================================================================================================//

#define _print(...) fprintf(tree_pdf, __VA_ARGS__)

void ConverteTreeToPdf(Tree* tree) {
    if (!tree->Root) {
        fprintf(stderr, "Cant create Graph_Dump: tree pointer = %p\n", tree);
        return ;
    }
    static const int MaxSystemCommandLen = 150;

    const char header[] = "\\documentclass{article}\n"
                          "\\usepackage[utf8]{inputenc}\n"
                          "\\usepackage{float}\n"
                          "\\usepackage{pgfplots}\n"
                          "\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools}\n"
                          "\\usepackage{graphicx}\n"
                          "\\usepackage{comment}\n"

    "\\begin{document}\n";
    char file_name[MaxSystemCommandLen] = {0};
    
    FILE* tree_pdf = fopen("data//derivative.tex", "w+");
    Validator(tree_pdf == nullptr, "in oppening file", exit(READING_FILE_ERROR));
    _print("%s", header);
    _print("Let's do this shit man and go to drink beer and play World Of Tanks like normal workers after hard day!\\newline \\newline");

    for (int var_number = 0; var_number < tree->var_counter; var_number++) {
        _print("\n$f(%s)^\\prime = (", tree->variables[var_number].var_name);
        WriteTreeToPdf(tree->Root, tree_pdf);
        _print(")_{%s}^\\prime = ", tree->variables[var_number].var_name);
        WriteTreeToPdf(tree->Diff_trees[var_number], tree_pdf);
        _print("$ \\newline \\newline");
    }

    _print("\tThat's all, I hope your ass is satisfied\\newline");
    _print("\\end{document}");

    int is_fclosed = fclose(tree_pdf);

    system("cd data");
    system("pdflatex data//derivative.tex");
    system("cd ..");

    Validator(is_fclosed != 0, "in closing file", exit(CLOSING_FILE_ERROR));    
}
//==========================================================================================================================================//

void WriteTreeToPdf(Node* tree, FILE* tree_pdf) {
    if (!tree) {
        return ;
    }
    if (tree->type == OPER) {
        if (tree->value.oper == OP_DIV) {
            _print(" \\frac{ ");
        }
        else if (tree->value.oper == OP_POW) {
            _print("(");
        }
        else if (tree->value.oper == OP_ADD || tree->value.oper == OP_SUB) {
            _print("(");
        }
        WriteTreeToPdf(tree->left_branch, tree_pdf);
        if (tree->value.oper == OP_DIV) {
            _print("}");
        }
        if (tree->type == OPER) {
            if (tree->value.oper == OP_POW) {
                _print(" %c{ ", tree->value.oper);
            }
            else if (tree->value.oper == OP_MUL) {
                _print(" \\cdot ");
            }
            else if (tree->value.oper == OP_ADD) {
                _print(" + ");
            }
            else if (tree->value.oper == OP_SUB) {
                _print(" - ");
            }
        }
        if (tree->value.oper == OP_DIV) {
            _print("{");
        }
        WriteTreeToPdf(tree->right_branch, tree_pdf);
        if (tree->value.oper == OP_POW) {
            _print("})");
        }
        else if (tree->value.oper == OP_DIV) {
            _print("}");
        }
        else if (tree->value.oper == OP_ADD || tree->value.oper == OP_SUB) {
            _print(")");
        }
    }
    if (tree->type == FUNC) {
        if (!strcmp("sqrt", _Diff_Functions_[tree->value.func].func_name)) {
            _print("\\sqrt{");
        }
        else {
            _print(" %s( ", _Diff_Functions_[tree->value.func].func_name);
        }
        WriteTreeToPdf(tree->left_branch, tree_pdf);
        if (!strcmp("sqrt", _Diff_Functions_[tree->value.func].func_name)) {
            _print("}");
        }
        else {
            _print(")");
        }
    }

    if (tree->type == VAR) {
        _print("%s", tree->value.var);
    }
    if (tree->type == NUMBER) {
        if (tree->value.number < 0) {
            _print("(%lg)\n", tree->value.number);
        }
        else {
            _print("%lg", tree->value.number);
        }
    }
}

#undef _print

//==========================================================================================================================================//

#define _print(...) fprintf(tex_file, __VA_ARGS__)

void PrintMakloren(Tree* tree, const char* file_name, elem_t number, int decompos_number) {
    if (!tree->Root) {
        fprintf(stderr, "Cant create Graph_Dump: tree pointer = %p\n", tree);
        return ;
    }
    static const int MaxSystemCommandLen = 150;
    const char header[] = "\\documentclass{article}\n"
                          "\\usepackage[utf8]{inputenc}\n"
                          "\\usepackage{float}\n"
                          "\\usepackage{pgfplots}\n"
                          "\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools}\n"
                          "\\usepackage{graphicx}\n"
                          "\\usepackage{comment}\n"

    "\\begin{document}\n";

    FILE* tex_file = fopen(file_name, "w+");
    Validator(tex_file == nullptr, "in oppening file", exit(READING_FILE_ERROR));

    _print("%s", header);

    int var_number = 1;
    _print("$");
    WriteTreeToPdf(tree->Root, tex_file);
    _print(" = ");
    Node* Copy      = CopyTree(tree->Diff_trees[0], Copy);
    elem_t tree_val = Ebal(Copy, number, tree->variables[0].var_name);
    TreeDump(Copy);
    if (!IsEqual(tree_val, 0)) {
        if (!IsEqual(tree_val, 1) && !IsEqual(tree_val, -1)) {
            TreeDump(Copy);
            WriteTreeToPdf(Copy, tex_file);
        } else if (IsEqual(tree_val, 1)) {
            _print("1");
        } else {
            _print("-1");
        }
        _print(" + ");
    }

    int func_counter = 0;
    int degree       = 1;
    for (func_counter = 0; func_counter < decompos_number - 1; func_counter++, degree++) {
        PrintExpression(tex_file, tree, number, degree);
        DerivativeOnALLVars(tree);
    }
    Copy     = CopyTree(tree->Diff_trees[0], Copy);
    tree_val = Ebal(Copy, number, tree->variables[0].var_name);
    printf("tree value = <%lg>\n", tree_val);
    if (!IsEqual(tree_val, 0)) {
        _print("\\frac{");
        if (!IsEqual(tree_val, 1)) {
            
                TreeDump(Copy);
            WriteTreeToPdf(Copy, tex_file);
        } else {
            _print("1");
        }
        _print("}");
        _print("{%d!}", degree);
        
        if (!IsEqual(number, 0)) {
            _print("\\cdot(%s - %lg)^{%d}", tree->variables[0].var_name, number, degree);
        } else {
            _print("\\cdot %s^{%d}", tree->variables[0].var_name, degree);
        }
    }
    if (!IsEqual(number, 0)) {
        _print(" + o(%s - %lg)^{%d}", tree->variables[0].var_name, number, decompos_number);
    } else {
        _print(" + o(%s)^{%d}", tree->variables[0].var_name, decompos_number);
    }

    _print("$\\newline");
    _print("\\newline ");
    _print("\\end{document} ");

    int is_fclosed = fclose(tex_file);

    system("cd data");
    system("pdflatex data//makloren.tex");
    system("cd ..");

    Validator(is_fclosed != 0, "in closing file", exit(CLOSING_FILE_ERROR));
}
//==========================================================================================================================================//

static void PrintExpression(FILE* tex_file, Tree* tree, elem_t number, int degree) {
    Node* Copy      = CopyTree(tree->Diff_trees[0], Copy);
    elem_t tree_val = Ebal(Copy, number, tree->variables[0].var_name);
    printf("tree value = <%lg>\n", tree_val);
    if (!IsEqual(tree_val, 0)) {
        _print("\\frac{");
        if (!IsEqual(tree_val, 1) && !IsEqual(tree_val, -1)) {
            TreeDump(Copy);
            WriteTreeToPdf(Copy, tex_file);
        } else if (IsEqual(tree_val, 1)) {
            _print("1");
        } else {
            _print("-1");
        }
        _print("}");
        _print("{%d!}", degree);
        if (!IsEqual(number, 0)) {
            _print("\\cdot(%s - %lg)^{%d}", tree->variables[0].var_name, number, degree);
        } else {
            _print("\\cdot %s^{%d}", tree->variables[0].var_name, degree);
        }
    _print("+");

    }
}
#undef _print

//==========================================================================================================================================//

void PrintTree(Node* tree) {
    if (!tree) {
        return;
    }

    PrintTree(tree->left_branch);
    PrintTree(tree->right_branch);

    if (tree->type == OPER) {
        printf("%d \n", tree->value.oper);
    }
    if (tree->type == NUMBER) {
        printf("%lg \n", tree->value.number);
    }
    if (tree->type == VAR) {
        printf("%s \n", tree->value.var);
    }
    if (tree->type == FUNC) {
        printf("%s \n", _Diff_Functions_[tree->value.func].func_name);
    }
}
//==========================================================================================================================================//
