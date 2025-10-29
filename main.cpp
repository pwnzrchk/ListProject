#include "list.h"

int main(void) {

    List main_list = {};
    ListConstructor(&main_list);
    int adding_value = kPoisonValue;
    int index_after  = kTrashPrev;
    ListDump(&main_list);
    while(scanf("%d %d", &index_after, &adding_value) == 2) {
        AddNode(&main_list, index_after, adding_value);
        ListDump(&main_list);
    }
    return 0;
}
