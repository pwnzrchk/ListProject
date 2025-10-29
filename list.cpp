#include "list.h"

void ListConstructor(List* reference_list) {
    assert(reference_list);

    reference_list->nodes = (Node*)calloc(kNodesAmount, sizeof(Node));
    reference_list->nodes[0].prev = 0;
    reference_list->nodes[0].next = 0;
    reference_list->nodes[0].data = kShieldValue;

    reference_list->tail = 0;
    reference_list->head = 0;
    reference_list->size = 0;

    for (int i = 1; i < kNodesAmount; i++) {
        reference_list->nodes[i].data = kPoisonValue;
        reference_list->nodes[i].next = -(i + 1);
        reference_list->nodes[i].prev = kTrashPrev;
    }

    return;
}

//=================================================================================================================================================
//Add node after node under index number
tListError AddNode(List* reference_list, int index, tData value) {
    assert(reference_list != NULL);

    if (index < 0 || index >= kNodesAmount) {
        fprintf(stderr, "Wrong input index for add\n");
        return kIncorrectIndex;
    }

    if (reference_list->head == 0 || reference_list->tail == 0 || reference_list->size == 0) {
        if (index != 0) return kIncorrectInput;

        reference_list->size++;
        reference_list->head = reference_list->size;
        reference_list->tail = reference_list->size;
        reference_list->nodes[reference_list->size] = {value, reference_list->head, reference_list->tail};
        return kNoErrors;
    }

    if (reference_list->nodes[index].next < 0) {
        fprintf(stderr, "Error, not an initialized node before\n");
        return kUserError;
        //WARNING - Are we doing like that?

//         reference_list->nodes[index + 1].next = 0;
//         reference_list->nodes[index + 1].prev = 0;
//         reference_list->nodes[index + 1].data = value;
//
//         if (reference_list->tail == index) reference_list->tail = index + 1;
        //WARNING -

        return kNoErrors;
    }

    if (reference_list->nodes[reference_list->nodes[index].next].prev == kTrashPrev) {
        fprintf(stderr, "Error in linking nodes\n");
        return kErrorLinking;
    //WARNING - Такого быть не может ведь? Верно
    }

    if (reference_list->nodes[reference_list->head].prev != reference_list->tail) reference_list->nodes[reference_list->head].prev = reference_list->tail;
    if (reference_list->nodes[reference_list->tail].next != reference_list->head) reference_list->nodes[reference_list->tail].next = reference_list->head;

    int next_address = ++reference_list->size;
    reference_list->nodes[reference_list->nodes[index].next].prev = next_address;   //WARNING -
    reference_list->nodes[next_address] = {value, reference_list->nodes[index].next, index};
    reference_list->nodes[index].next = next_address;

    if (index == reference_list->tail) {
        reference_list->tail = next_address;
        reference_list->nodes[next_address].prev = index;
        reference_list->nodes[next_address].next = reference_list->head; //WARNING - Do we really need this?
        reference_list->nodes[next_address].data = value;
        return kNoErrors;

    }

    return kNoErrors;
}

//=================================================================================================================================================
//Delete node no(index) from List
tListError DeleteNode (List* ref_list, int index) {
    assert(ref_list != NULL);

    if (index < 0 || index >= kNodesAmount) {
        fprintf(stderr, "Wrong input index for add\n");
        return kIncorrectIndex;
    }

    if (ref_list->head == 0 || ref_list->tail == 0 || ref_list->size == 0) {
        fprintf(stderr, "Blank list, nothing to delete\n");
        return kBlankList;
    }

    if (ref_list->nodes[index].next < 1 || ref_list->nodes[index].prev == kTrashPrev || ref_list->nodes[index].data == kPoisonValue) {
        if (ref_list->nodes[index].next == 0) {
            fprintf(stderr, "Trying to delete null-node\n");
            return kEmptyNode;
        }
        fprintf(stderr, "Trying to delete empty node\n");
        return kEmptyNode;
    }

    if (ref_list->tail == index && ref_list->nodes[index].next == ref_list->head) {
        ref_list->tail = ref_list->nodes[index].prev;

        ref_list->nodes[index].data = kPoisonValue;
        ref_list->nodes[index].next = (index != kNodesAmount ? -(index + 1) : -kNodesAmount);
        ref_list->nodes[index].prev = kTrashPrev;

        ref_list->nodes[ref_list->tail].next = ref_list->head;
        ref_list->size--;
        return kNoErrors;
    }

    if (index == ref_list->head && ref_list->nodes[index].prev == ref_list->tail) {
        ref_list->head = ref_list->nodes[index].next;

        ref_list->nodes[index].data = kPoisonValue;
        ref_list->nodes[index].next = (index != kNodesAmount ? -(index + 1) : -kNodesAmount);
        ref_list->nodes[index].prev = kTrashPrev;
        ref_list->nodes[ref_list->nodes[index].next].prev = ref_list->tail;
        ref_list->size--;

        return kNoErrors;
    }

    ref_list->nodes[index].data = kPoisonValue;
    ref_list->nodes[ref_list->nodes[index].prev].next = ref_list->nodes[index].next;
    ref_list->nodes[ref_list->nodes[index].next].prev = ref_list->nodes[index].prev;

    ref_list->nodes[index].next = (index != kNodesAmount ? -(index + 1) : -kNodesAmount);
    ref_list->nodes[index].prev = kTrashPrev;
    ref_list->size--;

    return kNoErrors;
}

//=================================================================================================================================================

tListError ListVerify(List* ref_list) {
    assert(ref_list != NULL);

    if (ref_list->nodes[0].data != kShieldValue || ref_list->nodes[0].next != 0 || ref_list->nodes[0].prev != 0) {
        fprintf(stderr, "Canary was fucked sir...\n");
        return kShiedError;
    }

    if (ref_list->size >= kNodesAmount) {
        fprintf(stderr, "Incorrect size of list\n");
        return kIncorrectSize;
    }


    if (ref_list->nodes[ref_list->head].prev != ref_list->tail) {
        fprintf(stderr, "Incorrect previous index for head\n");
        return kErrorHead;
    }
    if (ref_list->nodes[ref_list->tail].next != ref_list->head) {
        fprintf(stderr, "Incorrect next index for tail\n");
        return kErrorTail;
    }

    for (int i = 1; i < kNodesAmount; i++) {
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
                fprintf(stderr, "Error in linking lists\n");
                return kErrorLinking;
            }
        }
    }
    return kNoErrors;
}

//=================================================================================================================================================

tListError ListDump(List* ref_list) {
    assert(ref_list != NULL);

    printf("\nHead of list: %d\nTail of list: %d\n", ref_list->head, ref_list->tail);
    printf("Size of list - [%d]\n\n", ref_list->size);
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
