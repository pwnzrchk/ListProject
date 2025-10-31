#include "logger.h"

int main(void) {

    List main_list = {};
    const char* main_file = "logfile.dot";
    ListConstructor(&main_list, main_file);
    int adding_value = kPoisonValue;
    int index_after  = kTrashPrev;
    int comand       = kFictionalElement;

    ListDump(&main_list);
    while(scanf("%d %d %d", &comand, &index_after, &adding_value) == 3) {
        if(comand == 1) {
            AddNodeAfter(&main_list, index_after, adding_value);
            ListDump(&main_list);
        }
        if (comand == 2) {
            DeleteNodeAt(&main_list, index_after);
            ListDump(&main_list);
        }
    }

    BeginGraph(&main_list);
    PrintFictive(&main_list);
    PrintNodes(&main_list);
    PrintNext(&main_list);
    EndGraph(&main_list);

    return 0;
}
