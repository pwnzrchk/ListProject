#ifndef logger_h
#define logger_h

#include <stdio.h>
#include "list.h"


void PrintNodes  (tList* ref_list);
void EndGraph    (tList* ref_list);
void BeginGraph  (tList* ref_list);
void PrintFictive(tList* ref_list);
void PrintEdges  (tList* ref_list);

#endif //logger_h
