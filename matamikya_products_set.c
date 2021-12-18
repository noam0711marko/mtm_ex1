#include "matamikya_products_set.h"
#include "matamikya_products.h"
struct product_set_t{
    Set product_set;
};

static void *productSetCopyProduct(void *product) {
    Product to_copy = (Product) product;
    return (void *) copyProduct(to_copy);
}

static void freeProductVoid(void *product) {
    freeProduct((Product) product);
}

static int productCmpVoidInPS(void *product_a, void *product_b) {
    return productCmp((Product) product_a, (Product) product_b);
}

Product_set createProductSet() {
    Product_set new_product_set = (Product_set) malloc(sizeof(struct product_set_t));
    if (new_product_set == NULL) {
        return NULL;
    }
    new_product_set->product_set = setCreate(productSetCopyProduct, freeProductVoid, productCmpVoidInPS);
    if (new_product_set->product_set == NULL) {
        free(new_product_set);
        return NULL;
    }
    return new_product_set;
}

void destroyProductSet(Product_set product_set) {
    setDestroy(product_set->product_set);
    free(product_set);
}

Product_set copyProductSet(Product_set original) {
    assert(original!=NULL);
    Product_set copy = (Product_set) malloc(sizeof(struct product_set_t));
    if (copy == NULL) {
        return NULL;
    }
    copy->product_set = setCopy(original->product_set);
    if (copy->product_set == NULL) {
        free(copy);
        return NULL;
    }
    return copy;
}

MatamikyaResult addProductToProductsSet(Product_set product_set, const unsigned int id, const char *name,
                                        const double amount, const MatamikyaAmountType amountType,
                                        const MtmProductData customData, MtmCopyData copyData,
                                        MtmFreeData freeData, MtmGetProductPrice prodPrice) {
    assert(product_set!=NULL);
    if ((name == NULL) || (copyData == NULL) || (customData==NULL)||(freeData == NULL) || (prodPrice == NULL)) {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    if (isNameLegal(name) == false) {
        return MATAMIKYA_INVALID_NAME;
    }
    if (checkIfAmountLegal(amount, amountType) == false) {
        return MATAMIKYA_INVALID_AMOUNT;
    }
    Product new_prod = createProduct(id, name, amount, amountType, customData, prodPrice, copyData, freeData);
    if (new_prod == NULL) {
        return MATAMIKYA_OUT_OF_MEMORY;
    }
    SetResult set_res = setAdd(product_set->product_set, (void*)new_prod);
    freeProduct(new_prod);
    if (set_res == SET_ITEM_ALREADY_EXISTS) {
        return MATAMIKYA_PRODUCT_ALREADY_EXIST;
    }
    if (set_res == SET_NULL_ARGUMENT) {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    if (set_res == SET_OUT_OF_MEMORY) {
        return MATAMIKYA_OUT_OF_MEMORY;
    }
    return MATAMIKYA_SUCCESS;
}

Product getProductFromProductSet(Product_set product_set, const int id) {
    Product current = (Product) setGetFirst(product_set->product_set);
    while (current != NULL) {
        if ((unsigned) getProductId(current) == id) {
            return current;
        }
        current = (Product) setGetNext(product_set->product_set);
    }
    return NULL;
}

MatamikyaResult changeProductAmount(Product_set productSet, const unsigned int id, const double amount) {
    if (productSet == NULL) {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    if(amount==0){
        return MATAMIKYA_SUCCESS;
    }
    Product to_change = getProductFromProductSet(productSet, id);
    if (to_change == NULL) {
        return MATAMIKYA_PRODUCT_NOT_EXIST;
    }
    if (amount > 0) {
        if (!checkIfAmountLegal(amount, getProductAmountType(to_change))) {
            return MATAMIKYA_INVALID_AMOUNT;
        }
    }
    if (amount <= 0) {
        if (!checkIfAmountLegal(-amount, getProductAmountType(to_change))) {
            return MATAMIKYA_INVALID_AMOUNT;
        }
    }
    double new_amount = getProductAmount(to_change) + amount;
    if (new_amount < 0) {
        if (!checkIfAmountLegal(-new_amount, getProductAmountType(to_change))) {
            return MATAMIKYA_INVALID_AMOUNT;
        }
        return MATAMIKYA_INSUFFICIENT_AMOUNT;
    }
    if(new_amount!=0) {
        if (!checkIfAmountLegal(new_amount, getProductAmountType(to_change))) {
            return MATAMIKYA_INVALID_AMOUNT;
        }
    }
    updateProductAmount(to_change, new_amount);
    return MATAMIKYA_SUCCESS;
}

MatamikyaResult clearProductFromProductSet(Product_set product_set, const unsigned int id){
    if(product_set==NULL){
        return MATAMIKYA_NULL_ARGUMENT;
    }
    Product to_remove=getProductFromProductSet(product_set,id);
    if(to_remove==NULL){
        return MATAMIKYA_PRODUCT_NOT_EXIST;
    }
    setRemove(product_set->product_set,(void*)to_remove);
    return MATAMIKYA_SUCCESS;
}

void printProductSet(Product_set product_set, FILE *output){
    assert(product_set!=NULL && output != NULL);
    fprintf(output,"Inventory Status:\n");
    Product current=(Product)setGetFirst(product_set->product_set);
    while(current!=NULL)
    {
        printProduct(current,output);
        current=(Product)setGetNext(product_set->product_set);
    }
}

MatamikyaResult printBestSellingProductSet(Product_set product_set,FILE* output){
    if(product_set==NULL||output==NULL){
        return MATAMIKYA_NULL_ARGUMENT;
    }
    fprintf(output,"Best Selling Product:\n");
    if(setGetSize(product_set->product_set)==0){
    fprintf(output,"none\n");
        return MATAMIKYA_SUCCESS;
    }
    Product current=(Product)setGetFirst(product_set->product_set);
    Product highest=current;
    double best_selling=getProductProfit(current);
    while(current!=NULL) {
        if (getProductProfit(current) > best_selling) {
            best_selling = getProductProfit(current);
            highest = current;
        }
        current = (Product) setGetNext(product_set->product_set);
    }
    if(best_selling==0){
        fprintf(output,"none\n");
        return MATAMIKYA_SUCCESS;
    }
    char *name = getProductName(highest);
    mtmPrintIncomeLine(name, getProductId(highest), best_selling, output);
    free(name);
    return MATAMIKYA_SUCCESS;
}
