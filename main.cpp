#include "logger.h"

int main(void) {

    List main_list = {};
    const char* main_file = "9mice.dot";
    ListConstructor(&main_list, main_file);
    int adding_value = kPoisonValue;
    int index_after  = kTrashPrev;
    int comand       = kFictionalElement;

    tListError error_for_handler = kNoErrors;

    ListDump(&main_list);
    while(scanf("%d %d %d", &comand, &index_after, &adding_value) == 3) {
        if(comand == 1) {
            if ((error_for_handler = AddNodeAfter(&main_list, index_after, adding_value)) != kNoErrors) {
                ErrorHandler(error_for_handler);
                return 1;
            }
            ListDump(&main_list);
        }
        if (comand == 2) {
            if ((error_for_handler = DeleteNodeAt(&main_list, index_after)) != kNoErrors) {
                ErrorHandler(error_for_handler);
                return 1;
            }
            ListDump(&main_list);
        }
    }

    BeginGraph(&main_list);
    PrintFictive(&main_list);
    PrintNodes(&main_list);
    PrintEdges(&main_list);
    EndGraph(&main_list);

    return 0;
}
