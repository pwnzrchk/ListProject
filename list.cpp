#include "list.h"

//=================================================================================================================================================

#define CASE_TEMPLATE_ERROR_HANDLER(error)               \
        fprintf(stderr, "%s error in list\n", error);    \
        break;

//=================================================================================================================================================

tListError ListCtor(tList* reference_list, const char* logger_file_name) {
    assert(reference_list != NULL);

    reference_list->nodes = (tNode*)calloc(kNodesAmount, sizeof(tNode));
    if (reference_list->nodes == NULL) {
        return kNullPointer;
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
    return kNoErrors;
}

//=================================================================================================================================================

tListError AddNodeAfter(tList* reference_list, int index, tData value) {
    assert(reference_list != NULL);

    if (index < 0 || index >= kNodesAmount) {
        return kIncorrectIndex;
    }

    if (reference_list->nodes[index].next < 0 || reference_list->nodes[index].data == kPoisonValue || reference_list->nodes[index].prev == kTrashPrev) {

        return kIncorrectIndex;
    }

    int next_address = reference_list->free;
    reference_list->free = reference_list->nodes[reference_list->free].next;

    if(reference_list->free == kNodesAmount) {
        tNode* verify_buffer = (tNode*)realloc(reference_list->nodes, sizeof(tNode) * kNodesAmount * kSizeMultiplier);
        if (verify_buffer == NULL) return kNullPointer;

        reference_list->nodes = verify_buffer;
    }

    reference_list->nodes[next_address] = {value, reference_list->nodes[index].next, index};
    reference_list->nodes[reference_list->nodes[index].next].prev = next_address;
    reference_list->nodes[index].next = next_address;

    return kNoErrors;
}

//=================================================================================================================================================

tListError DeleteNodeAt (tList* ref_list, int index) {
    assert(ref_list != NULL);

    if (index < 0 || index >= kNodesAmount) {
        return kIncorrectIndex;
    }

    if (ref_list->nodes[kFictionalElement].next == 0 || ref_list->nodes[kFictionalElement].prev == 0 || ref_list->free == 1) {
        return kBlankList;
    }

    if (ref_list->nodes[index].next < 0 || ref_list->nodes[index].prev == kTrashPrev || ref_list->nodes[index].data == kPoisonValue) {
        if (ref_list->nodes[index].next == 0) {
            return kNullNode;
        }
        return kEmptyNode;
    }

    ref_list->nodes[ref_list->nodes[index].prev].next = ref_list->nodes[index].next;
    ref_list->nodes[ref_list->nodes[index].next].prev = ref_list->nodes[index].prev;

    ref_list->nodes[index].data = kPoisonValue;
    ref_list->nodes[index].next = ref_list->free;

    ref_list->free = index;
    ref_list->nodes[index].prev = kTrashPrev;

    return kNoErrors;

}

//=================================================================================================================================================

tListError ListVerify(tList* ref_list) {
    assert(ref_list != NULL);

    if (ref_list->nodes[kFictionalElement].data != kShieldValue) {
        fprintf(stderr, "Canary was fucked sir...\n");
        return kShieldError;
    }

    if (ref_list->free >= kNodesAmount) {
        fprintf(stderr, "Incorrect size of list\n");
        return kIncorrectSize;
    }

    if (ref_list->nodes[ref_list->nodes[kFictionalElement].next].prev != ref_list->nodes[ref_list->nodes[kFictionalElement].prev].next) {
        fprintf(stderr, "Incorrect previous indexes for head and tail\n");
        return kErrorHead;
    }


    for (int actual_node = NodeNext(ref_list, kFictionalElement);
             actual_node != kFictionalElement;
             actual_node = NodeNext(ref_list, i)) {

        tNode current_node = ref_list->nodes[actual_nodeН];
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

tListError ListDump(tList* ref_list) {
    assert(ref_list != NULL);

    printf("\nHead of list: %d\nTail of list: %d\n", ref_list->nodes[kFictionalElement].next, ref_list->nodes[kFictionalElement].prev);
    printf("Free in list - [%d]\n\n", ref_list->free);
    printf("Number    Next:    Data:    Prev:\t\n");
    for (int i = 0; i < kNodesAmount; i++) {

        tNode current_node = ref_list->nodes[i];
        int next_address = current_node.next;
        int prev_address = current_node.prev;
        tData current_data = current_node.data;

        printf("[%d]  -     %d        %d        %d\n", i,  next_address, current_data, prev_address);
        if (i % 9 == 0 && i > 0) printf("\n");
    }
    return kNoErrors;
}

//=================================================================================================================================================

int ListHead(tList* ref_list) {
    return ref_list->nodes[0].next;
}

//=================================================================================================================================================

int ListTail(tList* ref_list) {
    return ref_list->nodes[0].prev;
}

//=================================================================================================================================================

tListError AddFront(tList* ref_list, tData value) {
    return AddNodeAfter(ref_list, kFictionalElement, value);
}

//=================================================================================================================================================

tListError AddBack (tList* ref_list, tData value) {
    tListError add_error = kNoErrors;
    if ((add_error = AddNodeAfter(ref_list, ref_list->nodes[kFictionalElement].prev, value)) != kNoErrors) return add_error;
    return kNoErrors;
}

//=================================================================================================================================================

int NodePrev(tList* ref_list, int index) {
    return ref_list->nodes[index].prev;
}

//=================================================================================================================================================

int NodeNext(tList* ref_list, int index) {
    return ref_list->nodes[index].next;
}

//=================================================================================================================================================

tData GetData (tList* ref_list, int index) {
    tData ret_data = ref_list->nodes[index].data;
    if (!ValidData(ret_data)) {
        return kPoisonValue;
    }
    return ret_data;

}

//=================================================================================================================================================

tListError SetData (tList* ref_list, int index, tData value) {
    if (index == 0 || ref_list->nodes[index].data == kPoisonValue || ref_list->nodes[index].prev == kTrashPrev) {
        return kIncorrectIndex;
    }
    ref_list->nodes[index].data = value;
    return kNoErrors;

}

//=================================================================================================================================================

void ListDtor (tList* ref_list) {
    ref_list->free = kPoisonValue;
    for (int i = NodeNext(ref_list, kFictionalElement); i != kFictionalElement; i = NodeNext(ref_list, i)) {
        ref_list->nodes[i].data = kPoisonValue;
        ref_list->nodes[i].prev = kTrashPrev;
        ref_list->nodes[i].next = i + 1;
    }
    free(ref_list->nodes);
    fclose(ref_list->log_file);
    return;
}

//=================================================================================================================================================

void ErrorHandler (tListError error) {

    switch(error) {

        case kNullPointer:
            CASE_TEMPLATE_ERROR_HANDLER("Null pointer")

        case kIncorrectIndex:
            CASE_TEMPLATE_ERROR_HANDLER("Incorrect index given")

        case kIncorrectInput:
            CASE_TEMPLATE_ERROR_HANDLER("Incorrect input given")

        case kErrorLinking:
            CASE_TEMPLATE_ERROR_HANDLER("Linking nodes")

        case kBlankList:
            CASE_TEMPLATE_ERROR_HANDLER("Filling blank list")

        case kEmptyNode:
            CASE_TEMPLATE_ERROR_HANDLER("Empty node insert")

        case kShieldError:
            CASE_TEMPLATE_ERROR_HANDLER("Shield")

        case kIncorrectSize:
            CASE_TEMPLATE_ERROR_HANDLER("Incorrect size")

        case kIncorrectData:
            CASE_TEMPLATE_ERROR_HANDLER("Insert in incorrect data")

        case kErrorHead:
            CASE_TEMPLATE_ERROR_HANDLER("Incorrect head")

        case kErrorTail:
             CASE_TEMPLATE_ERROR_HANDLER("Incorrect tail")

        case kUserError:
            CASE_TEMPLATE_ERROR_HANDLER("User")

        case kNoErrors:

        default:
            CASE_TEMPLATE_ERROR_HANDLER("<unknown error>")
    }

    return;
}

//=================================================================================================================================================

bool ValidData(tData ref_data) {
    if (ref_data == kPoisonValue || ref_data == kShieldValue) return false;
    return true;
}

//=================================================================================================================================================
//=================================================================================================================================================
