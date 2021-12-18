#include "matamikya_validation_check.h"

#define HALF 0.5
#define MERGE_OF_ERROR 0.001

bool isNameLegal(const char *name) {
    if (name[0] >= 'a' && name[0] <= 'z') {
        return true;
    }
    if (name[0] >= 'A' && name[0] <= 'Z') {
        return true;
    }
    if (name[0] >= '0' && name[0] <= '9') {
        return true;
    }
    return false;
}

bool checkIfAmountLegal(double amount, MatamikyaAmountType type) {
    if (amount <= 0) {
        return false;
    }
    double num = (int) amount;
    double dif = amount - num;
    if(dif<0) {
        dif *= -1;
    }
    if (type == MATAMIKYA_INTEGER_AMOUNT) {
     if((dif>=-MERGE_OF_ERROR&&dif<=MERGE_OF_ERROR)||(dif>=1-MERGE_OF_ERROR&&dif<=1+MERGE_OF_ERROR)){
        return true;
    }
        return false;
    }
    if (type == MATAMIKYA_HALF_INTEGER_AMOUNT) {
        if ((dif>=-MERGE_OF_ERROR&&dif<=MERGE_OF_ERROR)||(dif>=HALF-MERGE_OF_ERROR&&dif<HALF+MERGE_OF_ERROR)||(dif>=1-MERGE_OF_ERROR&&dif<=+1+MERGE_OF_ERROR))
        {
            return true;
        }
        return false;
    }
    return true;
}
