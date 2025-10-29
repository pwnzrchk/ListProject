#ifndef list_h
#define list_h


#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

//=================================================================================================================================================

enum tListError {
    kNoErrors       = 100,
    kNullPointer    = 1,
    kIncorrectIndex = 2,
    kIncorrectInput = 3,
    kErrorLinking   = 4,
    kBlankList      = 5,
    kEmptyNode      = 6,
    kShiedError     = 7,
    kIncorrectSize  = 8,
    kIncorrectData  = 9,
    kErrorHead      = 10,
    kErrorTail      = 11,
    kUserError      = 12
};

typedef int tData;

typedef struct{
    tData data;
    int next;
    int prev;
}Node;

typedef struct{
    Node* nodes;
    int size;
    int head;
    int tail;
}List;

static const int kNodesAmount = 10;
static const int kShieldValue = -666;
static const int kPoisonValue = -228;
static const int kTrashPrev   = -1;

//=================================================================================================================================================

tListError ListDump  (List* ref_list);
tListError ListVerify(List* ref_list);
tListError DeleteNode(List* ref_list, int index);
tListError AddNode   (List* reference_list, int index, tData value);
void ListConstructor (List* reference_list);

#endif //list_h


