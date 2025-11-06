#include "logger.h"

//=================================================================================================================================================

static void BeginGraph(tList* ref_list);
static void EndGraph(tList* ref_list);
static void PrintFree(tList* ref_list);
static void PrintFictive(tList* ref_list);
static void PrintNodes(tList* ref_list);
static void PrintEdges(tList* ref_list);
static void CreateGraphImage(const char* dot_file_name, const char* png_file_name);
static void PrintHtml(const char* html_filename, const char* png_filename, int dump_count);
static void DrawGraph(tList* ref_list);

//=================================================================================================================================================

static void BeginGraph(tList* ref_list) {
    fprintf(ref_list->log_file, "digraph ListDump {\n"
                                "\tgraph [rankdir=LR, splines=curved, ranksep=1.2, nodesep=0.8];\n"
                                "\tnode [shape=record, style=\"rounded,filled\", fillcolor=\"#F5DEB3\"];\n"
                                "\tedge [fontsize=10];\n\n");
}

//=================================================================================================================================================

static void EndGraph(tList* ref_list) {
    fprintf(ref_list->log_file, "}\n");
    fflush(ref_list->log_file);
}

//=================================================================================================================================================

static void PrintFictive(tList* ref_list) {
    fprintf(ref_list->log_file, "\tsubgraph cluster_fictive {\n"
                                "\t\tstyle=filled;\n"
                                "\t\tcolor=lightgrey;\n"
                                "\t\tlabel=\"Fictive element\";\n"
                                "\t\tFictive [shape=record, style=filled, fillcolor=white, label=\""
                                "index: %d | data: %d | <p_next> next: %d | <p_prev> prev: %d\" ];\n"
                                "\t}\n",
            kFictionalElement,
            ref_list->nodes[kFictionalElement].data,
            ref_list->nodes[kFictionalElement].next,
            ref_list->nodes[kFictionalElement].prev);
}

//=================================================================================================================================================

static void PrintNodes(tList* ref_list) {
    for (int i = ListHead(ref_list); i != kFictionalElement; i = NodeNext(ref_list, i)) {
        fprintf(ref_list->log_file,
                "\tNode%d [shape=record, label=\""
                "index: %d | data: %d | <p_next> next: %d | <p_prev> prev: %d\" ];\n",
                i, i,
                ref_list->nodes[i].data,
                ref_list->nodes[i].next,
                ref_list->nodes[i].prev);
    }
}

//=================================================================================================================================================

static void PrintEdges(tList* ref_list) {
    int head_idx = ListHead(ref_list);
    int tail_idx = ListTail(ref_list);


    if (head_idx != kFictionalElement) {
        fprintf(ref_list->log_file, "\tFictive:p_next -> Node%d [tailport=e, headport=w, color=\"#0077be\", arrowhead=onormal, weight=100, xlabel=\"Head\"];\n", head_idx);
        fprintf(ref_list->log_file, "\tNode%d:p_prev -> Fictive [tailport=w, headport=e, color=\"#d73a49\", constraint=false];\n", head_idx);
    }

    if (tail_idx != kFictionalElement) {
        fprintf(ref_list->log_file, "\tNode%d:p_next -> Fictive [tailport=e, headport=e, color=\"#0077be\", constraint=false];\n", tail_idx);
        fprintf(ref_list->log_file, "\tFictive:p_prev -> Node%d [tailport=w, headport=e, color=\"#d73a49\", arrowhead=onormal, weight=100, xlabel=\"Tail\"];\n", tail_idx);
    }

    for (int i = ListHead(ref_list); i != kFictionalElement; i = NodeNext(ref_list, i)) {
        int next_node_idx = ref_list->nodes[i].next;
        if (next_node_idx != kFictionalElement) {
            fprintf(ref_list->log_file, "\tNode%d:p_next -> Node%d [tailport=n, headport=n, color=\"#0077be\", weight=100];\n", i, next_node_idx);

            fprintf(ref_list->log_file, "\tNode%d:p_prev -> Node%d [tailport=s, headport=s, color=\"#d73a49\", constraint=false];\n", next_node_idx, i);
        }
    }
}

//=================================================================================================================================================

static void CreateGraphImage(const char* dot_file_name, const char* png_file_name) {
    char command[kMaxCommandLength];

    snprintf(command, kMaxCommandLength, "dot -Tpng %s -o %s", dot_file_name, png_file_name);
    // printf("Executing command: %s\n", command);

    int result = system(command);
    if (result == 0) {
        printf("Successfully created %s\n", png_file_name);
    } else {
        fprintf(stderr, "Error: Failed to execute dot command. Is Graphviz installed and in your PATH?\n");
    }
}

//=================================================================================================================================================

static void PrintHtml(const char* html_filename, const char* png_filename, int dump_count) {

    FILE* html_file = fopen(html_filename, "a");
    if (html_file == NULL) {
        fprintf(stderr, "Error: Could not open HTML log file %s\n", html_filename);
        return;
    }

    fprintf(html_file, "<h2>List State Dump #%d</h2>\n", dump_count);
    fprintf(html_file, "<img src=\"%s\" width = 2000px>\n", png_filename);
    fprintf(html_file, "<hr>\n\n");

    fclose(html_file);
}

//=================================================================================================================================================

static void DrawGraph(tList* ref_list) {
    BeginGraph  (ref_list);
    PrintFictive(ref_list);
    PrintNodes  (ref_list);
    PrintEdges  (ref_list);
    PrintFree   (ref_list);
    EndGraph    (ref_list);
}

//=================================================================================================================================================

static void PrintFree(tList* ref_list) {

    fprintf(ref_list->log_file, "\tsubgraph cluster_free {\n"
                                "\t\tstyle=filled;\n"
                                "\t\tcolor=gray;\n"
                                "\t\tlabel=\"Free elements\";\n");

    for (int actual_free = ref_list->free; actual_free != kFictionalElement; actual_free = NodeNext(ref_list, actual_free)) {
        fprintf(ref_list->log_file, "\tNode%d [shape=record, label=\""
                "index: %d | data: %d | <p_next> next: %d | <p_prev> prev: %d\" ];\n",
                actual_free, actual_free,
                ref_list->nodes[actual_free].data,
                ref_list->nodes[actual_free].next,
                ref_list->nodes[actual_free].prev);

                if (actual_free == 1000) return;

    }

    for (int current_free = ref_list->free; current_free != kFictionalElement; current_free = NodeNext(ref_list, current_free)) {
        int next_free_idx = ref_list->nodes[current_free].next;
        if (next_free_idx == kFictionalElement) {
            fprintf(ref_list->log_file, "\tNode%d:p_next -> Fictive:p_prev [tailport=e, headport=n, color=\"#C71585\", weight=100];\n", current_free);
            fprintf(ref_list->log_file, "\t}\n");
            return;
        }
        fprintf(ref_list->log_file, "\tNode%d:p_next -> Node%d [tailport=e, headport=n, color=\"#C71585\", weight=100];\n", current_free, next_free_idx);
    }
}

//=================================================================================================================================================

void GraphDump(tList* ref_list, const char* dot_file_name, const char* png_file_name, const char* html_filemane) {
    ref_list->dump_counter++;
    FILE* dot_file = freopen(dot_file_name, "w", ref_list->log_file);
    if (!dot_file) {
         fprintf(stderr, "Failed to reopen dot file for writing\n");
         return;
    }
    ref_list->log_file = dot_file;
    DrawGraph(ref_list);
    CreateGraphImage(dot_file_name, png_file_name);

    const char* relative_png_mane = "logpict.png";
    PrintHtml(html_filemane, relative_png_mane, ref_list->dump_counter);
    return;
 }

//=================================================================================================================================================
