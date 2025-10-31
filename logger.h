#ifndef logger_h
#define logger_h

#include <stdio.h>
#include "list.h"


void PrintNodes  (List* ref_list);
void EndGraph    (List* ref_list);
void BeginGraph  (List* ref_list);
void PrintFictive(List* ref_list);
void PrintNext   (List* ref_list);

#endif //logger_h
