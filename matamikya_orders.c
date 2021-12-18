#include "matamikya_orders.h"

#define POSITIVE 1
#define NEGATIVE -1
#define MERGE_OF_ERROR 0.001

struct order_t {
    int order_id;
    Set shopping_cart;
    bool finished;
};

static void *orderCopyProductInOrder(void *product_in_order) {
    Product_in_order to_copy = (Product_in_order) product_in_order;
    return (void *) copyProductInOrder(to_copy);
}

static void orderFreeProductInOrder(void *product_in_order) {
    destroyProductInOrder((Product_in_order) product_in_order);
}

static int orderCompareProductInOrder(void *product_a, void *product_b) {
    return compareProductsInOrder((Product_in_order) product_a, (Product_in_order) product_b);
}

Order createOrder(unsigned int id) {
    Order new_order = (Order) malloc(sizeof(struct order_t));
    if (new_order == NULL) {
        return NULL;
    }
    new_order->shopping_cart = setCreate(orderCopyProductInOrder, orderFreeProductInOrder, orderCompareProductInOrder);
    if (new_order->shopping_cart == NULL) {
        free(new_order);
        return NULL;
    }
    new_order->order_id = id;
    new_order->finished = false;
    return new_order;
}

void destroyOrder(Order to_destroy) {
    if(to_destroy==NULL){
        return;
    }
    setDestroy(to_destroy->shopping_cart);
    free(to_destroy);
}

Order copyOrder(Order to_copy) {
    if(to_copy==NULL){
        return NULL;
    }
    Order copy_of_order = createOrder(to_copy->order_id);
    assert(copy_of_order!=NULL);
    return copy_of_order;
}

int compareOrdersId(Order first, Order second) {
    assert(first!=NULL && second!=NULL);
    if(first->order_id>second->order_id){
        return POSITIVE;
    }
    else if(first->order_id<second->order_id){
        return NEGATIVE;
    }
    return 0;
}

unsigned int getOrderId(Order order) {
    assert(order!=NULL);
    return order->order_id;
}

bool getOrderStatus(Order order) {
    assert(order!=NULL);
    return order->finished;
}

void changeOrderStatus(Order order) {
    assert(order!=NULL);
    order->finished = true;
}

double calculateOrderTotalPrice(Order order,Product_set product_set) {
    assert(order!=NULL);
    double total_price = 0;
    Product_in_order current = (Product_in_order) setGetFirst(order->shopping_cart);
        while (current != NULL) {
            double curr_amount=getProductAmountFromOrderData(current);
            unsigned int curr_id =getProductIdFromOrder(current);
            Product current_in_inventory=getProductFromProductSet(product_set,curr_id);
        total_price += getPrice(current_in_inventory,curr_amount);
        current = (Product_in_order) setGetNext(order->shopping_cart);
    }
    return total_price;
}

bool isProductExistInOrder(Order order, int product_id) {
    assert(order!=NULL);
    Product_in_order current = (Product_in_order) setGetFirst(order->shopping_cart);
    while (current != NULL) {
        if ((unsigned) getProductIdFromOrder(current) == product_id) {
            return true;
        }
        current = (Product_in_order) setGetNext(order->shopping_cart);
    }
    return false;
}


Product_in_order getProductFromOrder(Order order, const int id) {
    assert(order!=NULL);
    Product_in_order current = (Product_in_order) setGetFirst(order->shopping_cart);
    while (current != NULL) {
        if (getProductIdFromOrder(current) == id) {
            return current;
        }
        current = (Product_in_order) setGetNext(order->shopping_cart);
    }
    return NULL;
}


MatamikyaResult changeProductAmountInOrder(Order order, const unsigned int id, const double amount, Product_set product_set) {
    if (order == NULL || product_set == NULL) {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    if (isProductExistInOrder(order, id) == false) {
        Product current = (getProductFromProductSet(product_set, id));
        if (current == NULL) {
            return MATAMIKYA_PRODUCT_NOT_EXIST;
        }
        if (amount <= MERGE_OF_ERROR && amount >=-MERGE_OF_ERROR) {
            return MATAMIKYA_SUCCESS;
        }
        if(amount < 0){
            if(checkIfAmountLegal(-amount,getProductAmountType(current))==false){
                return MATAMIKYA_INVALID_AMOUNT;
            }
            return MATAMIKYA_SUCCESS;
        }
        if(checkIfAmountLegal(amount,getProductAmountType(current))==false){
            return MATAMIKYA_INVALID_AMOUNT;
        }
        char* name= getProductName(current);
        Product_in_order new_product_in_order = createProductInOrder(name, id, amount, getProductAmountType(current));
        free(name);
        if(new_product_in_order==NULL){
            return MATAMIKYA_OUT_OF_MEMORY;
        }
        if(setAdd(order->shopping_cart,(void*)new_product_in_order)==SET_OUT_OF_MEMORY)
        {
            destroyProductInOrder(new_product_in_order);
            return MATAMIKYA_OUT_OF_MEMORY;
        }
        destroyProductInOrder(new_product_in_order);
        return MATAMIKYA_SUCCESS;
    }
    Product_in_order to_update=getProductFromOrder(order,id);
    assert(to_update!=NULL);
    MatamikyaAmountType type = getProductTypeFromOrderData(to_update);
    if(amount > 0){
        if(!checkIfAmountLegal(type,amount)){
            return MATAMIKYA_INVALID_AMOUNT;
        }
        updateProductAmountInOrder(to_update,amount);
        return MATAMIKYA_SUCCESS;
    }
    if(amount !=0 && !checkIfAmountLegal(-amount,type)){
        return MATAMIKYA_INVALID_AMOUNT;
    }
    updateProductAmountInOrder(to_update,amount);
    if(getProductAmountFromOrderData(to_update)<=0){
    clearProductFromOrder(order,id);
    }
    return MATAMIKYA_SUCCESS;
}

MatamikyaResult clearProductFromOrder(Order order, const unsigned int id) {
    if (order == NULL) {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    Product_in_order to_remove = getProductFromOrder(order, id);
    if (to_remove == NULL) {
        return MATAMIKYA_PRODUCT_NOT_EXIST;
    }
    setRemove(order->shopping_cart, (void *) to_remove);
    return MATAMIKYA_SUCCESS;
}

static bool checkAmountBeforeFinish(Order order, Product_set product_set) {
    assert(order!=NULL && product_set!=NULL);
    Product_in_order current = (Product_in_order) setGetFirst(order->shopping_cart);
    while (current != NULL) {
        int curr_id = getProductIdFromOrder(current);
        Product current_in_product_set = getProductFromProductSet(product_set, curr_id);
        if (getProductAmountFromOrderData(current) > getProductAmount(current_in_product_set)+MERGE_OF_ERROR) {
            return false;
        }
        current = (Product_in_order) setGetNext(order->shopping_cart);

    }
    return true;
}


MatamikyaResult finishOrder(Order order, Product_set product_set) {
    assert(order!=NULL && product_set!=NULL);
    if(order->finished){
        return MATAMIKYA_ORDER_NOT_EXIST;
    }
    if(!checkAmountBeforeFinish(order, product_set)){
        return MATAMIKYA_INSUFFICIENT_AMOUNT;
    }
    Product_in_order current = (Product_in_order) setGetFirst(order->shopping_cart);
       while (current != NULL) {
        int curr_id = getProductIdFromOrder(current);
        Product current_in_product_set = getProductFromProductSet(product_set, curr_id);
        double order_amount=getProductAmountFromOrderData(current);
        double order_price=getPrice(current_in_product_set,order_amount);
        double stock= getProductAmount(current_in_product_set);
        double new_amount=stock-order_amount;
        updateProductProfit(current_in_product_set, order_price);
        updateProductAmount(current_in_product_set,new_amount);
        current = (Product_in_order) setGetNext(order->shopping_cart);
      }
    changeOrderStatus(order);
    return MATAMIKYA_SUCCESS;
}

void printOrder(Order order, FILE *output,Product_set product_set) {
    assert(order!=NULL && output!=NULL && product_set!=NULL);
    unsigned int id = getOrderId(order);
    mtmPrintOrderHeading(id, output);
    Product_in_order current = (Product_in_order) setGetFirst(order->shopping_cart);
    while (current != NULL) {
        char *curr_name = getProductNameFromOrder(current);
        unsigned int curr_id = getProductIdFromOrder(current);
        double curr_amount = getProductAmountFromOrderData(current);
        Product curr_product=getProductFromProductSet(product_set,curr_id);
        double curr_price=getPrice(curr_product,curr_amount);
        mtmPrintProductDetails(curr_name, curr_id, curr_amount, curr_price, output);
        current = (Product_in_order) setGetNext(order->shopping_cart);
        free(curr_name);
    }
    double total_price = calculateOrderTotalPrice(order,product_set);
    mtmPrintOrderSummary(total_price, output);
  }

