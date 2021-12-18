#include "matamikya_products.h"

#define POSITIVE 1
#define NEGATIVE -1

struct products_t{
    unsigned int id;
    char *name;
    double amount;
    MatamikyaAmountType amount_type;
    MtmProductData data;
    double profit;
    MtmGetProductPrice get_price;
    MtmFreeData freeData;
    MtmCopyData CopyData;
    };

Product createProduct(const unsigned int id, const char* name, double amount,MatamikyaAmountType amountType,
                      MtmProductData data, MtmGetProductPrice get_price,MtmCopyData copy,MtmFreeData free_data){
    if(name==NULL || get_price==NULL || copy == NULL || free_data==NULL){
        return NULL;
    }
    Product new_product=(Product)malloc(sizeof(struct products_t));
    if(new_product==NULL) {
        return NULL;
    }
    new_product->data=copy(data);
    if (new_product->data==NULL){
        free(new_product);
        return NULL;
    }
    int len=(int)strlen(name);
    new_product->name=(char*)malloc(sizeof(char)*(len+1));
    if(new_product->name==NULL){
        free_data(new_product->data);
        free_data(new_product);
        return NULL;
    }
    strcpy(new_product->name,name);
    new_product->id=id;
    new_product->amount=amount;
    new_product->get_price=get_price;
    new_product->profit=0;
    new_product->amount_type=amountType;
    new_product->freeData=free_data;
    new_product->CopyData=copy;
    return new_product;
}

void freeProduct(Product to_destroy){
    if (to_destroy==NULL){
        return;
    }
    free(to_destroy->name);
    to_destroy->freeData(to_destroy->data);
    free(to_destroy);
}

Product copyProduct(Product to_copy)
{
    if(to_copy==NULL){
        return NULL;
    }
    Product product_copy= createProduct(to_copy->id,to_copy->name,to_copy->amount,
                                        to_copy->amount_type,to_copy->data,to_copy->get_price,
                                        to_copy->CopyData,to_copy->freeData);
    assert(product_copy!=NULL);
    return product_copy;
}

 unsigned int getProductId(Product product){
    assert(product!=NULL);
    return product->id;
}

char* getProductName(Product product){
    assert(product!=NULL);
    int len=(int)strlen(product->name);
    char* name=(char*)malloc(sizeof(char)*(len+1));
    if(name==NULL){
        return NULL;
    }
    strcpy(name,product->name);
    return name;
}

double getProductProfit(Product product){
    assert(product!=NULL);
    return product->profit;
}

double getProductAmount(Product product){
    assert(product!=NULL);
    return product->amount;
}

MatamikyaAmountType getProductAmountType(Product product){
    assert(product!=NULL);
    return product->amount_type;
}

MtmProductData getProductData(Product product){
    assert(product!=NULL);
    return product->data;
}

void updateProductAmount(Product product,double amount){
    assert(product!=NULL);
    product->amount=amount;
}

void updateProductProfit(Product product,double profit){
    assert(product!=NULL);
    product->profit+=profit;
}

int productCmp(Product product_a,Product product_b){
    assert(product_a!=NULL && product_b!=NULL);
    if(product_a->id>product_b->id){
        return POSITIVE;
    }
    else if(product_a->id<product_b->id){
        return NEGATIVE;
    }
    return 0;
}

void printProduct(Product product,FILE* output){
    assert(product!=NULL && output!=NULL);
    double price=product->get_price(product->data,1);
    mtmPrintProductDetails(product->name,product->id,product->amount,price,output);
}

double getPrice(Product product,double amount){
    assert(product!=NULL);
    return product->get_price(product->data,amount);
}
