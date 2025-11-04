#include "logger.h"

//=================================================================================================================================================
//PRINT_TO_FILE - PTF
#define LOG_FILE ref_list->log_file

//=================================================================================================================================================

void BeginGraph(tList* ref_list) {
    fprintf(LOG_FILE, "digraph A{\n"
                      "\trankdir=LR;\n"
                      "\tnode[color=\"red\",fontsize=14];\n"
                      "\tedge[color=\"darkgreen\",fontcolor=\"blue\",fontsize=12];\n");
    return;
}

void EndGraph(tList* ref_list) {
    fprintf(LOG_FILE, "}");
    return;
}

void PrintNodes(tList* ref_list) {
    for (int i = 1; NodeNext(ref_list, i) != 0; i = NodeNext(ref_list, i)) {
        fprintf(LOG_FILE, "\t%d [shape=record,label=\"   %d\\n %d\\n %d\\n \" ];\n", i, ref_list->nodes[i].data, ref_list->nodes[i].next, ref_list->nodes[i].prev);
    }
    int tail = ref_list->nodes[kFictionalElement].prev;
    fprintf(LOG_FILE, "\t%d [shape=record,label=\"   %d\\n %d\\n %d\\n \" ];\n", tail, ref_list->nodes[tail].data, ref_list->nodes[tail].next, ref_list->nodes[tail].prev);
    return;
}

void PrintFictive(tList* ref_list) {
    fprintf(LOG_FILE, "\tsubgraph clusternull {\n"
            "\t\tnode [shape=ellipse,style=filled,color = white];\n"
            "\t\tstyle=filled;\n"
            "\t\tcolor=grey;\n"
            "\t\tNULL [label= \" %d\\n %d\\n %d\\n %d\\n \"];"
            "\t\tlabel = \"Fictive element\";\t\n}\n", kFictionalElement, ref_list->nodes[kFictionalElement].data,
            ref_list->nodes[kFictionalElement].next, ref_list->nodes[kFictionalElement].prev);
    return;
}


void PrintEdges(tList* ref_list) {

    fprintf(LOG_FILE, "\t\t");
    fprintf(LOG_FILE, "NULL->%d[dir=\"both\", label=\"Head\", color=\"black\"];\n", ref_list->nodes[kFictionalElement].next);
    fprintf(LOG_FILE, "\t\t");


    fprintf(LOG_FILE, "1");
    for (int i = 1; NodeNext(ref_list, i) != 0; i = NodeNext(ref_list, i)) {
        // if ( == 0) break;
        fprintf(LOG_FILE, "->%d", NodeNext(ref_list, i));
    }
    fprintf(LOG_FILE, " [dir=\"both\", label=\"prev-next\", color=\"black\"]");

    fprintf(LOG_FILE, "\n");
    fprintf(LOG_FILE, "\t\t");
    fprintf(LOG_FILE, "%d->NULL[dir=\"both\", label=\"Tail\", color=\"black\"];\n", ref_list->nodes[kFictionalElement].prev);
    return;
}
