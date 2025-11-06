#include "list.h"

//=================================================================================================================================================

#define CASE_TEMPLATE_ERROR_HANDLER(error)               \
        fprintf(stderr, "%s error in list\n", error);    \
        break;

//=================================================================================================================================================

tListError ListCtor(tList* reference_list, const char* logger_file_name) {
    assert(reference_list != NULL);

    reference_list->capacity = kNodesAmount;
    reference_list->nodes = (tNode*)calloc((size_t)reference_list->capacity, sizeof(tNode));

    if (reference_list->nodes == NULL) {
        return kNullPointer;
    }

    reference_list->nodes[kFictionalElement].prev = kFictionalElement;
    reference_list->nodes[kFictionalElement].next = kFictionalElement;
    reference_list->nodes[kFictionalElement].data = kShieldValue;
    reference_list->free = 1;

    for (int i = reference_list->free; i < reference_list->capacity; i++) {
        reference_list->nodes[i].data = kPoisonValue;
        reference_list->nodes[i].next = (i + 1 == reference_list->capacity) ? kFictionalElement : i + 1;
        reference_list->nodes[i].prev = kTrashPrev;
    }

    reference_list->log_file = fopen(logger_file_name, "w");
    reference_list->dump_counter = 0;
    return kNoErrors;
}

//=================================================================================================================================================

tListError AddNodeAfter(tList* reference_list, int index, tData value) {
    assert(reference_list != NULL);

    if (index < 0 || index >= reference_list->capacity) {
        return kIncorrectIndex;
    }

    if (reference_list->nodes[index].next < 0 || reference_list->nodes[index].data == kPoisonValue || reference_list->nodes[index].prev == kTrashPrev) {
        return kIncorrectIndex;
    }

    if (reference_list->free == kFictionalElement) {
        tListError error_for_realloc = kNoErrors;
        if ((error_for_realloc = ListIncrease(reference_list)) != kNoErrors) return error_for_realloc;
    }

    int next_address = reference_list->free;
    reference_list->free = reference_list->nodes[reference_list->free].next;

    reference_list->nodes[next_address] = {value, reference_list->nodes[index].next, index};
    reference_list->nodes[reference_list->nodes[index].next].prev = next_address;
    reference_list->nodes[index].next = next_address;

    return kNoErrors;
}

//=================================================================================================================================================

tListError ListIncrease (tList* reference_list) {
    int old_capacity = reference_list->capacity;
        int new_capacity = old_capacity * kSizeMultiplier;

        tNode* verify_buffer = (tNode*)realloc(reference_list->nodes, sizeof(tNode) * (size_t)new_capacity);
        if (verify_buffer == NULL) return kNullPointer;

        reference_list->nodes = verify_buffer;
        reference_list->capacity = new_capacity;

        for (int new_node = old_capacity; new_node < new_capacity; new_node++) {
            reference_list->nodes[new_node].data = kPoisonValue;
            reference_list->nodes[new_node].next = ((new_node + 1 == new_capacity) ? kFictionalElement : new_node + 1);
            reference_list->nodes[new_node].prev = kTrashPrev;
        }
        reference_list->free = old_capacity;

    return kNoErrors;
}

//=================================================================================================================================================

tListError DeleteNodeAt (tList* ref_list, int index) {
    assert(ref_list != NULL);

    if (index <= 0 || index >= ref_list->capacity) {
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

    if (ref_list->free >= ref_list->capacity) {
         fprintf(stderr, "Incorrect size of list\n");
         return kIncorrectSize;
    }

    if (ref_list->nodes[ref_list->nodes[kFictionalElement].next].prev != ref_list->nodes[ref_list->nodes[kFictionalElement].prev].next) {
        fprintf(stderr, "Incorrect previous indexes for head and tail\n");
        return kErrorHead;
    }


    for (int actual_node = NodeNext(ref_list, kFictionalElement);
             actual_node != kFictionalElement;
             actual_node = NodeNext(ref_list, actual_node)) {

        tNode current_node = ref_list->nodes[actual_node];
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
            if (ref_list->nodes[next_address].prev != actual_node || ref_list->nodes[prev_address].next != actual_node) {
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
    for (int i = 0; i < ref_list->capacity; i++) {

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
    if (ref_list == NULL) return;

    if (ref_list->nodes != NULL) {
        for (int i = 0; i < ref_list->capacity; i++) {
            ref_list->nodes[i].data = kFictionalElement;
            ref_list->nodes[i].prev = kFictionalElement;
            ref_list->nodes[i].next = kFictionalElement;
        }
        free(ref_list->nodes);
        ref_list->nodes = NULL;
    }

    if (ref_list->log_file != NULL) {
        fclose(ref_list->log_file);
        ref_list->log_file = NULL;
    }

    ref_list->free         = kPoisonValue;
    ref_list->capacity     = kPoisonValue;
    ref_list->dump_counter = kPoisonValue;
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

        case kNullNode:
            CASE_TEMPLATE_ERROR_HANDLER("Null node")

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
