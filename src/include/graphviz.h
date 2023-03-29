#ifndef GRAPH_H
#define GRAPH_H

#define DotPrint(...) fprintf(dot_file, __VA_ARGS__);

#define DotStartGraph(file_name)         \
    const int Name_Size = 100;            \
    char file[Name_Size] = file_name;      \
    FILE* dot_file = fopen(file, "w+");     

#define DotSetRankdir(dir) fprintf(dot_file, "rankdir = %s\n", #dir);

#define DotSetGraph(fillcolor, ranksep, nodesep, style, color, penwidth) \
    fprintf(dot_file, "graph [fillcolor = %s, ranksep = %lg, nodesep = %lg, style = \"%s\", color = %s, penwidth = %lg];\n",\
    fillcolor, ranksep, nodesep, style, color, penwidth);

#define DotSetEdge(color, arrowhead, arrowsize, penwidth) \
    fprintf(dot_file, "edge [color = %s, arrowhead = %s, arrowsize = %lg, penwidth = %lg];\n", color, arrowhead, arrowsize,\
    penwidth);

#define DotSetNodeStyle(shape, style, fillcolor, fontcolor, margin) \
    fprintf(dot_file, "node [shape=%s, style=%s, fillcolor=%s, fontcolor=%s, margin = %s];\n", \
    #shape, #style, #fillcolor, #fontcolor, #margin);

#define DotSetNode(node_id, shape, style, color, label) \
    fprintf(dot_file, "node%d [shape=%s, style=%s, color=%s, label = %s];\n", \
    node_id, shape, style, color, label);

#define DotSetLinkStyle()

#define DotCreateConnection(first_id, second_id) \
    fprintf(dot_file, "node%d -> node%d\n", first_id, second_id);


#define DotEndGraph(file_name)                 \
    DotPrint("}")                               \
    fclose(file_name);                     

#define DotPrintGraph(file, picture_numb)                                           \
    const int dot_len = 150;                                                         \
    char dot_png[dot_len] = "";                                                       \
    sprintf(dot_png, "dot -Tpng %s -o data//graph_%d.png", file, picture_numb);        \
    system(dot_png);

#endif