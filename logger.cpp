#include "logger.h"

//=================================================================================================================================================
//PRINT_TO_FILE - PTF
#define LOG_FILE ref_list->log_file

//=================================================================================================================================================

void BeginGraph(List* ref_list) {
    fprintf(LOG_FILE, "digraph A{\n"
                      "\trankdir=LR;\n"
                      "\tnode[color=\"red\",fontsize=14];\n"
                      "\tedge[color=\"darkgreen\",fontcolor=\"blue\",fontsize=12];\n");
    return;
}

void EndGraph(List* ref_list) {
    fprintf(LOG_FILE, "}");
    return;
}

void PrintNodes(List* ref_list) {
    for (int i = 0; i < ref_list->size + 1; i++) {
        fprintf(LOG_FILE, "\t%d [shape=record,label=\"   %d %d %d\" ];\n", i, ref_list->nodes[i].data, ref_list->nodes[i].next, ref_list->nodes[i].prev);
    }
    return;
}

