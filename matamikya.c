#include "matamikya_products_set.h"
#include "matamikya_orders_set.h"

struct Matamikya_t {
    Product_set products_set;
    Order_set orders;
};

Matamikya matamikyaCreate() {
    Matamikya matamikya = (Matamikya)malloc(sizeof(struct Matamikya_t));
    if (matamikya == NULL) {
        return NULL;
    }
    matamikya->products_set = createProductSet();
    if (matamikya->products_set == NULL) {
        free(matamikya);
        return NULL;
    }
    matamikya->orders = createOrderSet();
    if (matamikya->orders == NULL) {
        destroyProductSet(matamikya->products_set);
        free(matamikya);
        return NULL;
    }
    return matamikya;
}

void matamikyaDestroy(Matamikya matamikya) {
    if (matamikya == NULL) {
        return;
    }
    destroyProductSet(matamikya->products_set);
    destroyOrderSet(matamikya->orders);
    free(matamikya);
}

MatamikyaResult mtmNewProduct(Matamikya matamikya, const unsigned int id, const char *name,
                              const double amount, const MatamikyaAmountType amountType,
                              const MtmProductData customData, MtmCopyData copyData,
                              MtmFreeData freeData, MtmGetProductPrice prodPrice) {
    if(matamikya==NULL){
        return MATAMIKYA_NULL_ARGUMENT;
    }
    return addProductToProductsSet(matamikya->products_set, id, name, amount, amountType, customData,
                                   copyData, freeData, prodPrice);
}

MatamikyaResult mtmChangeProductAmount(Matamikya matamikya, const unsigned int id, const double amount) {
    if(matamikya==NULL){
        return MATAMIKYA_NULL_ARGUMENT;
    }
    return changeProductAmount(matamikya->products_set, id, amount);
}

MatamikyaResult mtmClearProduct(Matamikya matamikya, const unsigned int id) {
    if(matamikya==NULL) {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    MatamikyaResult res = clearProductFromProductSet(matamikya->products_set, id);
    if (res == MATAMIKYA_SUCCESS) {
        clearProductFromOrderSet(matamikya->orders, id);
    }
    return res;
}

MatamikyaResult mtmPrintInventory(Matamikya matamikya, FILE *output) {
    if ((output == NULL) || (matamikya == NULL)) {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    printProductSet(matamikya->products_set, output);
    return MATAMIKYA_SUCCESS;
}

unsigned int mtmCreateNewOrder(Matamikya matamikya)
{
    if(matamikya==NULL){
        return 0;
    }
    return addOrderToOrderSet(matamikya->orders);
}

MatamikyaResult mtmChangeProductAmountInOrder(Matamikya matamikya, const unsigned int orderId,
                                              const unsigned int productId, const double amount) {
    if (matamikya == NULL) {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    return changeAmountOfProductInOrder(matamikya->orders, orderId, productId, amount, matamikya->products_set);
}


MatamikyaResult mtmShipOrder(Matamikya matamikya, const unsigned int orderId) {
    if (matamikya == NULL) {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    return OrderSetFinishOrder(matamikya->orders, matamikya->products_set, orderId);
}

MatamikyaResult mtmCancelOrder(Matamikya matamikya, const unsigned int orderId) {
    if (matamikya == NULL) {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    return cancelOrderFromOrdersSet(matamikya->orders, orderId);
}

MatamikyaResult mtmPrintOrder(Matamikya matamikya, const unsigned int orderId, FILE *output) {
    if (matamikya == NULL || output == NULL) {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    return printOrderFromSet(matamikya->orders, orderId, output,matamikya->products_set);
}

MatamikyaResult mtmPrintBestSelling(Matamikya matamikya, FILE *output) {
    if (matamikya == NULL || output == NULL) {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    return printBestSellingProductSet(matamikya->products_set, output);
}
