#include "console_handler.h"

int main(void) {

    tList main_list = {};
    const char* main_file = "files/9mice.dot";

    if (ListCtor(&main_list, main_file) != kNoErrors) {
        fprintf(stderr, "Failed to construct list.\n");
        return EXIT_FAILURE;
    }
    RunInteractiveSession(&main_list);
    ListDtor(&main_list);
    return EXIT_SUCCESS;
}

