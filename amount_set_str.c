#include "amount_set_str.h"
#include "amount_set_str_node.h"

#include <stdbool.h>

struct AmountSet_t {
    Node first;
    Node last;
    Node iterator;
    int size;
};

///////////////////////////////////////////////// static help function /////////////////////////////////////////////////
///// declarations and documentation////////////////////////////////////////////////////////////////////////////////////

//sets new amount_set_str's fields to their initial values
static void asInit(AmountSet as);

//sets amount set's iterator to tmp_iterator. for set to NULL, send NULL as tmp_iterator.
static void resetIterator(AmountSet set, Node tmp_iterator);

//creates new item for the amount set
static Node createNewItem (const char* element, AmountSetResult *res);

//insertion function in case the new item should be inserted to an empty amount set
static void insertToEmptySet(AmountSet set, const char* element, AmountSetResult *res);

//insertion function in case the new item should be inserted first to the (non-empty) amount set
static void insertAsFirstToSet(AmountSet set, const char* element, AmountSetResult *res);

//insertion function in case the new item should be inserted last to the (non-empty) amount set
static void insertAsLastToSet(AmountSet set, const char* element,Node previous, AmountSetResult *res);

//insertion function in case the new item should be inserted to the (non-empty) amount set,
//but neither as first nor last.
static void insertInMiddleOfSet(AmountSet set, const char* element,Node previous, AmountSetResult *res);

//copies all original amount set's contents into a new amount set. if the copy fails, deallocates new amount set and
//returns NULL, otherwise returns the new amount set
static AmountSet copySetContents(AmountSet original, AmountSet copy);

//deletes the first item of the amount set, and takes care of the byproducts of this.
static void deleteFirst(AmountSet set);

//deletes an item (not the first) of the amount set, and takes care of the byproducts of this (including the case
//in which the item is the last of the amount set)
static void deleteNotFirst(AmountSet set, const char* current, const char* element);

//deallocates all amount set's contents
static void clearSetContents(AmountSet set);


///// implementations //////////////////////////////////////////////////////////////////////////////////////////////////

static void asInit(AmountSet as){
    as->first = NULL;
    resetIterator(as, NULL);
    as->last = NULL;
    as->size = 0;
}

static void resetIterator(AmountSet set, Node tmp_iterator) {
    assert(set!=NULL);
    set->iterator = tmp_iterator;
}

static Node createNewItem (const char* element, AmountSetResult *res){
    Node new_item= createNode(element);
    if(new_item == NULL) {
        *res=AS_OUT_OF_MEMORY;
    }
    *res=AS_SUCCESS;
    return new_item;
}

static void insertToEmptySet(AmountSet set, const char* element, AmountSetResult *res) {
    AmountSetResult creation_res;
    Node new_item= createNewItem(element, &creation_res);
    if (creation_res!=AS_SUCCESS) {
        *res=AS_OUT_OF_MEMORY;
        return;
    }
    set->first=new_item;
    set->last=new_item;
    set->size++;
}

static void insertAsFirstToSet(AmountSet set, const char* element, AmountSetResult *res) {
    AmountSetResult creation_res;
    Node new_item= createNewItem(element, &creation_res);
    if (creation_res!=AS_SUCCESS) {
        *res=AS_OUT_OF_MEMORY;
        return;
    }
    updateNextNode(new_item,set->iterator);
    set->first=new_item;
    set->size++;
    *res=AS_SUCCESS;
}

static void insertAsLastToSet(AmountSet set, const char* element,Node previous, AmountSetResult *res) {
    AmountSetResult creation_res;
    Node new_item= createNewItem(element, &creation_res);
    if (creation_res!=AS_SUCCESS) {
        *res=AS_OUT_OF_MEMORY;
        return;
    }
    updateNextNode(new_item,NULL);
    updateNextNode(previous, new_item);
    set->last=new_item;
    set->size++;
    *res=AS_SUCCESS;
}

static void insertInMiddleOfSet(AmountSet set, const char* element,Node previous, AmountSetResult *res) {
    AmountSetResult creation_res;
    Node new_item= createNewItem(element, &creation_res);
    if (creation_res!=AS_SUCCESS) {
        *res=AS_OUT_OF_MEMORY;
        return;
    }
    updateNextNode(new_item,set->iterator);
    updateNextNode(previous, new_item);
    set->size++;
    *res=AS_SUCCESS;
}

static AmountSet copySetContents(AmountSet original, AmountSet copy){
    char* iterator = asGetFirst(original);
    copy->first = copyNode(original->iterator);
    iterator = asGetNext(original);
    asGetFirst(copy);
    while (iterator != NULL) {
        Node new_node = copyNode(original->iterator);
        if (new_node == NULL) {
            asDestroy(copy);
            return NULL;
        }
        updateNextNode(copy->iterator, new_node);
        asGetNext(copy);
        iterator = asGetNext(original);
    }
    copy->last = copy->iterator;
    copy->size = original->size;

    return copy;
}

static void deleteFirst(AmountSet set){
    set->first = getNextNode(set->first);
    destroyNode(set->iterator);
    set->size--;
    resetIterator(set,NULL);
}

static void deleteNotFirst(AmountSet set, const char* current, const char* element){
    Node previous = set->first;
    current = asGetNext(set);
    while (strcmp(element, current) != 0) {
        previous = set->iterator;
        current = asGetNext(set);
    }
    updateNextNode(previous, getNextNode(set->iterator));
    destroyNode(set->iterator);
    resetIterator(set, NULL);
    if (getNextNode(previous) == NULL) {
        set->last = previous;
    }
}

static void clearSetContents(AmountSet set){
    asGetFirst(set);
    Node Next=getNextNode(set->iterator);
    while(Next!=NULL){
        destroyNode(set->iterator);
        resetIterator(set, Next);
        Next=getNextNode(set->iterator);
    }
    destroyNode(set->iterator);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AmountSet asCreate() {
    AmountSet new_as = (AmountSet) malloc(sizeof(struct AmountSet_t));
    if (new_as == NULL) {
        return NULL;
    }
    asInit(new_as);
    return new_as;
}

void asDestroy(AmountSet set) {
    asClear(set);
    free(set);
}

AmountSet asCopy(AmountSet set) {
    AmountSet copy_of_set = asCreate();
    if (copy_of_set == NULL) {
        return NULL;
    }
    return copySetContents(set, copy_of_set);
}

int asGetSize(AmountSet set) {
    if(set==NULL) {
        return -1;
    }
    return set->size;
}

bool asContains(AmountSet set, const char* element) {
    if (set == NULL || element == NULL) {
        return false;
    }
    Node tmp_iterator = set->iterator;
    char* current=asGetFirst(set);
    while (current != NULL) {
        int compare_string_result = strcmp(element, current);
        if (compare_string_result == 0) {
            return true;
        }
        current = asGetNext(set);
    }
    resetIterator(set, tmp_iterator);
    return false;
}

AmountSetResult asGetAmount(AmountSet set, const char* element, double* outAmount) {
    if (set==NULL || element == NULL || outAmount == NULL) {
        return AS_NULL_ARGUMENT;
    }
    if(asContains(set,element)==false) {
        return AS_ITEM_DOES_NOT_EXIST;
    }
    else {
        Node tmp_iterator = set->iterator;
        char* current=asGetFirst(set);
        while (current != NULL) {
            int compare_string_result = strcmp(element, current);
            if (compare_string_result == 0) {
                *outAmount = getQuantityNode(set->iterator);
                break;
            }
            current=asGetNext(set);
        }
        resetIterator(set,tmp_iterator);
    }
    return AS_SUCCESS;
}

AmountSetResult asRegister(AmountSet set, const char* element) {
    AmountSetResult insertion_result=AS_SUCCESS;
    if (set==NULL || element == NULL) {
        return AS_NULL_ARGUMENT;
    }
    if(asGetSize(set)==0) {
        insertToEmptySet(set,element,&insertion_result);
        return insertion_result;
    }
    if (asContains(set, element)) {
            insertion_result = AS_ITEM_ALREADY_EXISTS;
            return insertion_result;
    }
    char* current = asGetFirst(set);
    if (strcmp(element,current)<0){
        insertAsFirstToSet(set,element,&insertion_result);
        resetIterator(set,NULL);
        return insertion_result;
    }
    else if((strcmp(element,current)>0)&&asGetSize(set)==1) {
        insertAsLastToSet(set,element,set->first,&insertion_result);
        resetIterator(set,NULL);
        return insertion_result;
    }
    else {
        Node previous=set->iterator;
        current= asGetNext(set);
        while(current!=NULL){
            int compare_name_res= strcmp(element, current);
            if (compare_name_res<0){
                insertInMiddleOfSet(set, element, previous, &insertion_result);
                return insertion_result;
            }
            previous = set->iterator;
            current= asGetNext(set);
        }
    }
    insertAsLastToSet(set,element,set->last,&insertion_result);
    resetIterator(set,NULL);
    return insertion_result;
}

AmountSetResult asChangeAmount(AmountSet set, const char* element, double amount) {
    AmountSetResult function_result = AS_SUCCESS;
    if (set==NULL || element == NULL){
        function_result=AS_NULL_ARGUMENT;
    }
    else {
        Node tmp_iterator = set->iterator;
        if (asContains(set, element) == false) {
            function_result=AS_ITEM_DOES_NOT_EXIST;
        }
        else {
            char *current = asGetFirst(set);
            while (current != NULL) {
                if (strcmp(element, current) == 0) {
                    if (updateQuantityNode(set->iterator, amount) == NODE_INSUFFICIENT_AMOUNT){
                        function_result = AS_INSUFFICIENT_AMOUNT;
                        break;
                    }
                    function_result = AS_SUCCESS;
                }
                current = asGetNext(set);
            }
        }
        resetIterator(set,tmp_iterator);
    }
    return function_result;
}

AmountSetResult asDelete(AmountSet set, const char *element) {
    if (set == NULL || element == NULL) {
        return AS_NULL_ARGUMENT;
    }
    if (!asContains(set, element)) {
        return AS_ITEM_DOES_NOT_EXIST;
    }
    const char *current = asGetFirst(set);
    if (strcmp(element, current) == 0) {
        deleteFirst(set);
        return AS_SUCCESS;
    }
    deleteNotFirst(set, current, element);
    set->size--;
    return AS_SUCCESS;
}

AmountSetResult asClear(AmountSet set){
    if(set==NULL){
        return AS_NULL_ARGUMENT;
    }
    clearSetContents(set);
    asInit(set);
    return AS_SUCCESS;
}

char *asGetFirst(AmountSet set) {
    if (set == NULL) {
        return NULL;
    }
    if (set->first == NULL) {
        return NULL;
    }
    set->iterator=set->first;
    return getNameNode(set->iterator);
}

char *asGetNext(AmountSet set) {
    if ((set == NULL) || (set->iterator == NULL)) {
        return NULL;
    }
    set->iterator=getNextNode(set->iterator);
    return getNameNode(set->iterator);
}
