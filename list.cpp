#include "list.h"

void ListConstructor(List* reference_list, const char* logger_file_name) {
    assert(reference_list);

    reference_list->nodes = (Node*)calloc(kNodesAmount, sizeof(Node));
    if (reference_list->nodes == NULL) {
        fprintf(stderr, "Error calloc memory in constructor\n");
        return;
    }

    reference_list->nodes[kFictionalElement].prev = kFictionalElement;
    reference_list->nodes[kFictionalElement].next = kFictionalElement;
    reference_list->nodes[kFictionalElement].data = kShieldValue;
    reference_list->free = 1;


    for (int i = reference_list->free; i < kNodesAmount; i++) {
        reference_list->nodes[i].data = kPoisonValue;
        reference_list->nodes[i].next = i + 1;
        reference_list->nodes[i].prev = kTrashPrev;
    }

    reference_list->log_file = fopen(logger_file_name, "w");
    return;
}

//=================================================================================================================================================
//Add node after node under index number
tListError AddNodeAfter(List* reference_list, int index, tData value) {
    assert(reference_list != NULL);

    if (index < 0 || index >= kNodesAmount) {
        // fprintf(stderr, "Wrong input index for add\n");  //FIXME -
        return kIncorrectIndex;
    }

    if (reference_list->nodes[index].next < 0 || reference_list->nodes[index].data == kPoisonValue || reference_list->nodes[index].prev == kTrashPrev) {
        // fprintf(stderr, "Error, not an initialized node before\n");
        return kIncorrectIndex;
    }


    int next_address = reference_list->free;
    reference_list->free = reference_list->nodes[reference_list->free].next;




    reference_list->nodes[next_address] = {value, reference_list->nodes[index].next, index};
    reference_list->nodes[reference_list->nodes[index].next].prev = next_address;
    reference_list->nodes[index].next = next_address;


    return kNoErrors;
}

//=================================================================================================================================================

tListError DeleteNodeAt (List* ref_list, int index) {
    assert(ref_list != NULL);

    if (index < 0 || index >= kNodesAmount) {
        fprintf(stderr, "Wrong input index for add\n");
        return kIncorrectIndex;
    }

    if (ref_list->nodes[kFictionalElement].next == 0 || ref_list->nodes[kFictionalElement].prev == 0 || ref_list->free == 1) {
        fprintf(stderr, "Blank list, nothing to delete\n");
        return kBlankList;
    }

    if (ref_list->nodes[index].next < 0 || ref_list->nodes[index].prev == kTrashPrev || ref_list->nodes[index].data == kPoisonValue) {
        if (ref_list->nodes[index].next == 0) {
            fprintf(stderr, "Trying to delete null-node\n");
            return kEmptyNode;
        }
        // fprintf(stderr, "Trying to delete empty node\n"); //FIXME - handler
        return kEmptyNode;
    }


    ref_list->nodes[ref_list->nodes[index].prev].next = ref_list->nodes[index].next;
    ref_list->nodes[ref_list->nodes[index].next].prev = ref_list->nodes[index].prev;

    ref_list->nodes[index].data = kPoisonValue;
    ref_list->nodes[index].next = ref_list->free;
    // ref_list->nodes[ref_list->free].next = index;
    ref_list->free = index;
    ref_list->nodes[index].prev = kTrashPrev;

    return kNoErrors;

}

//=================================================================================================================================================

tListError ListVerify(List* ref_list) {
    assert(ref_list != NULL);

    if (ref_list->nodes[kFictionalElement].data != kShieldValue) {
        fprintf(stderr, "Canary was fucked sir...\n");
        return kShiedError;
    }

    if (ref_list->free >= kNodesAmount) {
        fprintf(stderr, "Incorrect size of list\n");
        return kIncorrectSize;
    }


    if (ref_list->nodes[ref_list->nodes[kFictionalElement].next].prev != ref_list->nodes[ref_list->nodes[kFictionalElement].prev].next) {
        fprintf(stderr, "Incorrect previous indexes for head and tail\n");
        return kErrorHead;
    }

    for (int i = 1; i != 1; i = NodeNext(ref_list, i)) {
        Node current_node = ref_list->nodes[i];
        int next_address = current_node.next;
        int prev_address = current_node.prev;
        tData current_data = current_node.data;

        if (current_data == kPoisonValue || prev_address == kTrashPrev || next_address < 0) {
            if (!(current_data == kPoisonValue && prev_address == kTrashPrev && next_address < 0)) {
                fprintf(stderr, "Incorrect data in empty node\n");
                return kIncorrectData;
            }
        }

        if (current_data != kPoisonValue || prev_address != kTrashPrev || next_address > 0) {
            if (ref_list->nodes[next_address].prev != i || ref_list->nodes[prev_address].next != i) {
                fprintf(stderr, "Error in linking nodes\n");
                return kErrorLinking;
            }

        }
    }
    return kNoErrors;
}

//=================================================================================================================================================

tListError ListDump(List* ref_list) {
    assert(ref_list != NULL);

    printf("\nHead of list: %d\nTail of list: %d\n", ref_list->nodes[kFictionalElement].next, ref_list->nodes[kFictionalElement].prev);
    printf("Free in list - [%d]\n\n", ref_list->free);
    printf("Number    Next:    Data:    Prev:\t\n");
    for (int i = 0; i < kNodesAmount; i++) {

        Node current_node = ref_list->nodes[i];
        int next_address = current_node.next;
        int prev_address = current_node.prev;
        tData current_data = current_node.data;

        printf("[%d]  -     %d        %d        %d\n", i,  next_address, current_data, prev_address);
        if (i % 9 == 0 && i > 0) printf("\n");
    }
    return kNoErrors;
}

//=================================================================================================================================================

int ListHead(List* ref_list) {
    return ref_list->nodes[0].next;
}

//=================================================================================================================================================

int ListTail(List* ref_list) {
    return ref_list->nodes[0].prev;
}

//=================================================================================================================================================

tListError AddFront(List* ref_list, tData value) {
    tListError add_error = kNoErrors;
    if ((add_error = AddNodeAfter(ref_list, ref_list->nodes[kFictionalElement].next, value)) != kNoErrors) return add_error;
    return kNoErrors;
}

//=================================================================================================================================================

tListError AddBehind (List* ref_list, tData value) {
    tListError add_error = kNoErrors;
    if ((add_error = AddNodeAfter(ref_list, ref_list->nodes[kFictionalElement].prev, value)) != kNoErrors) return add_error;
    return kNoErrors;
}

//=================================================================================================================================================

int NodePrev(List* ref_list, int index) {
    return ref_list->nodes[index].prev;
}

//=================================================================================================================================================

int NodeNext(List* ref_list, int index) {
    return ref_list->nodes[index].next;
}

//=================================================================================================================================================

tData GetData (List* ref_list, int index) {
    tData ret_data = ref_list->nodes[index].data;
    if (ret_data == kPoisonValue || ret_data == kShieldValue) {
        fprintf(stderr, "Incorrect index to for getdata\n");
        return kPoisonValue;
    }
    return ret_data;

}

//=================================================================================================================================================

tListError SetData (List* ref_list, int index, tData value) {
    if (index == 0 || ref_list->nodes[index].data == kPoisonValue || ref_list->nodes[index].prev == kTrashPrev) {
        return kIncorrectIndex;
    }
    ref_list->nodes[index].data = value;
    return kNoErrors;

}

//=================================================================================================================================================

void ListDtor (List* ref_list) {
    ref_list->free = kPoisonValue;
    free(ref_list->nodes);
    return;
}

//=================================================================================================================================================
