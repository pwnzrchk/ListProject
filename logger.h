#ifndef logger_h
#define logger_h

#include <stdio.h>
#include "list.h"

static const int kMaxCommandLength = 64;

void GraphDump(tList* ref_list, const char* dot_file_name, const char* png_file_name, const char* html_filemane);

#endif //logger_h
