#include "amount_set_str.h"
#include "amount_set_str_node.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

struct AmountSet_t {
    Node first;
    Node last;
    Node iterator;
    int size;
};

AmountSet asCreate() {
    AmountSet new_as = (AmountSet) malloc(sizeof(struct AmountSet_t));
    if (new_as == NULL) {
        return NULL;
    }
    new_as->first = NULL;
    new_as->iterator = NULL;
    new_as->last = NULL;
    new_as->size = 0;
    return new_as;
}

static void resetIterator(AmountSet set, Node tmp_iterator) {
    assert(set!=NULL);
    set->iterator = tmp_iterator;
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
    char* iterator = asGetFirst(set);
    copy_of_set->first = copyNode(set->iterator);
    iterator = asGetNext(set);
    asGetFirst(copy_of_set);
    while (iterator != NULL) {
        Node new_node = copyNode(set->iterator);
        if (new_node == NULL) {
            asDestroy(copy_of_set);
            return NULL;
        }
        updateNextNode(copy_of_set->iterator, new_node);
        asGetNext(copy_of_set);
        iterator = asGetNext(set);
    }
    copy_of_set->last = copy_of_set->iterator;
    copy_of_set->size = set->size;
    return copy_of_set;
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
            function_result = AS_ITEM_DOES_NOT_EXIST;
        }
        else {
            char *current = asGetFirst(set);
            while (current != NULL) {
                int compare_string_result = strcmp(element, current);
                if (compare_string_result == 0) {
                    NodeResult change_amount_result = updateQuantityNode(set->iterator, amount);
                    if (change_amount_result == NODE_INSUFFICIENT_AMOUNT) {
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
        set->first = getNextNode(set->first);
        destroyNode(set->iterator);
        set->iterator = NULL;
        set->size--;
        return AS_SUCCESS;
    }
    Node previous = set->first;
    current = asGetNext(set);
    while (strcmp(element, current) != 0) {
        previous = set->iterator;
        current = asGetNext(set);
    }
    updateNextNode(previous, getNextNode(set->iterator));
    destroyNode(set->iterator);
    set->iterator = NULL;
    if (getNextNode(previous) == NULL) {
        set->last = previous;
    }
    set->size--;
    return AS_SUCCESS;
}

AmountSetResult asClear(AmountSet set){
    if(set==NULL){
        return AS_NULL_ARGUMENT;
    }
    asGetFirst(set);
    Node Next=getNextNode(set->iterator);
    while(Next!=NULL){
        destroyNode(set->iterator);
        set->iterator=Next;
        Next=getNextNode(set->iterator);
    }
    destroyNode(set->iterator);
    set->iterator=NULL;
    set->last=NULL;
    set->first=NULL;
    set->size=0;
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
