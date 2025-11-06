#ifndef list_h
#define list_h


#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

//=================================================================================================================================================

enum tListError {
    kNoErrors       = 100,
    kNullPointer    = 1,
    kIncorrectIndex = 2,
    kIncorrectInput = 3,
    kErrorLinking   = 4,
    kBlankList      = 5,
    kEmptyNode      = 6,
    kShieldError    = 7,
    kIncorrectSize  = 8,
    kIncorrectData  = 9,
    kErrorHead      = 10,
    kErrorTail      = 11,
    kUserError      = 12,
    kNullNode       = 13
};

typedef int tData;

typedef struct{
    tData data;
    int next;
    int prev;
} tNode;

typedef struct{
    tNode* nodes;
    int    free;
    FILE*  log_file;
    int    capacity;
    int    dump_counter;
} tList;

static const int kNodesAmount      = 10;
static const int kShieldValue      = -666;
static const int kPoisonValue      = -228;
static const int kTrashPrev        = -1;
static const int kFictionalElement = 0;
static const int kSizeMultiplier   = 2;

//=================================================================================================================================================

tListError ListDump    (tList* ref_list);
tListError ListVerify  (tList* ref_list);

tListError DeleteNodeAt(tList* ref_list, int index);
tListError AddBack     (tList* ref_list, tData value);
tListError AddFront    (tList* ref_list, tData value);
tListError AddNodeAfter(tList* reference_list, int index, tData value);

tListError SetData(tList* ref_list, int index, tData value);
tData GetData     (tList* ref_list, int index);
tListError ListIncrease (tList* reference_list);

tListError ListCtor(tList* reference_list, const char* logger_file_name);
void ListDtor      (tList* ref_list);
void ErrorHandler  (tListError error);
bool ValidData     (tData ref_data);


int NodeNext (tList* ref_list, int index);
int NodePrev (tList* ref_list, int index);
int ListTail (tList* ref_list);
int ListHead (tList* ref_list);

//=================================================================================================================================================

#endif // list_h


