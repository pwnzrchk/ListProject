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
} Node;

typedef struct{
    Node* nodes;
    int free;
    FILE* log_file;
} List;

static const int kNodesAmount      = 10; // FIXME расширять динамически
static const int kShieldValue      = -666;
static const int kPoisonValue      = -228;
static const int kTrashPrev        = -1;
static const int kFictionalElement = 0;

//=================================================================================================================================================

tListError ListDump  (List* ref_list);
tListError ListVerify(List* ref_list);
tListError DeleteNodeAt(List* ref_list, int index);
tListError AddBehind (List* ref_list, tData value);
tListError AddFront  (List* ref_list, tData value);
tListError AddNodeAfter   (List* reference_list, int index, tData value);
void ListConstructor (List* reference_list, const char* logger_file_name);
void ListDtor        (List* ref_list);

tListError SetData (List* ref_list, int index, tData value);
tData GetData      (List* ref_list, int index);
int NodeNext (List* ref_list, int index);
int NodePrev (List* ref_list, int index);
int ListTail (List* ref_list);
int ListHead (List* ref_list);

//=================================================================================================================================================

#endif // list_h


