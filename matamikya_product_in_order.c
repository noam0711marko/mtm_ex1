#include "matamikya_product_in_order.h"

#define POSITIVE 1
#define NEGATIVE -1

struct product_in_order_t{
    char* product_name;
    unsigned int product_id;
    double product_amount;
    MatamikyaAmountType product_type;
    };

Product_in_order createProductInOrder(char* name, unsigned int id, double amount, MatamikyaAmountType type){
    if(name==NULL){
        return NULL;
    }
    Product_in_order new_product_in_order=(Product_in_order)malloc(sizeof(struct product_in_order_t));
    if(new_product_in_order==NULL){
        return NULL;
    }
    int name_len=(int)strlen(name);
    new_product_in_order->product_name=(char*)malloc(sizeof(char)*(name_len+1));
    if(new_product_in_order->product_name==NULL){
        free(new_product_in_order);
        return NULL;
    }
    strcpy(new_product_in_order->product_name, name);
    new_product_in_order->product_id=id;
    new_product_in_order->product_amount=amount;
    new_product_in_order->product_type=type;
    return new_product_in_order;
}

void destroyProductInOrder(Product_in_order to_destroy){
    if(to_destroy==NULL){
        return;
    }
    free(to_destroy->product_name);
    free(to_destroy);
}

Product_in_order copyProductInOrder(Product_in_order to_copy){
    if(to_copy==NULL){
        return NULL;
    }
    Product_in_order copy_of_product_in_order=createProductInOrder(to_copy->product_name,to_copy->product_id,
                                                                   to_copy->product_amount,
                                                                   to_copy->product_type);
    assert(copy_of_product_in_order!=NULL);
    return copy_of_product_in_order;
}

int compareProductsInOrder(Product_in_order first, Product_in_order second){
    assert(first!=NULL && second!=NULL);
    if(first->product_id>second->product_id){
        return POSITIVE;
    }
    else if(first->product_id<second->product_id){
        return NEGATIVE;
    }
    return 0;
}

char* getProductNameFromOrder(Product_in_order product_in_order){
    assert(product_in_order!=NULL);
    int len=(int)strlen(product_in_order->product_name);
    char* name=(char*)malloc(sizeof(char)*(len+1));
    if(name==NULL){
        return NULL;
    }
    strcpy(name,product_in_order->product_name);
    return name;
}

unsigned int getProductIdFromOrder(Product_in_order product_in_order){
    assert(product_in_order!=NULL);
    return product_in_order->product_id;
}

double getProductAmountFromOrderData(Product_in_order product_in_order){
    assert(product_in_order!=NULL);
    return product_in_order->product_amount;
}

MatamikyaAmountType getProductTypeFromOrderData(Product_in_order product_in_order){
    assert(product_in_order!=NULL);
    return product_in_order->product_type;
}

void updateProductAmountInOrder(Product_in_order product_in_order, double amount){
    assert(product_in_order!=NULL);
    product_in_order->product_amount+=amount;
}
