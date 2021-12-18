#include "matamikya_orders_set.h"

struct orders_set_t{
    Set orders_set;
    unsigned int next_order_id;
};

static void *ordersSetCopyOrder(void *order) {
    Order to_copy = (Order) order;
    return (void *) copyOrder(to_copy);
}

static void freeOrderFromSet(void *order) {
    destroyOrder((Order) order);
}

static int productCmpVoid(void *order_a, void *order_b) {
    return compareOrdersId((Order) order_a, (Order) order_b);
}

Order_set createOrderSet() {
    Order_set new_order_set = (Order_set) malloc(sizeof(struct orders_set_t));
    if (new_order_set == NULL) {
        return NULL;
    }
    new_order_set->orders_set = setCreate(ordersSetCopyOrder, freeOrderFromSet, productCmpVoid);
    if ( new_order_set->orders_set == NULL) {
        free(new_order_set);
        return NULL;
    }
    new_order_set->next_order_id=1;
    return new_order_set;
}

void destroyOrderSet(Order_set order_set) {
    setDestroy(order_set->orders_set);
    free(order_set);
}

Order_set copyOrdersSet(Order_set original) {
    if (original==NULL){
        return NULL;
    }
    Order_set copy = (Order_set) malloc(sizeof(Order_set));
    if (copy == NULL) {
        return NULL;
    }
    copy->orders_set = setCopy(original->orders_set);
    if (copy->orders_set == NULL) {
        free(copy);
        return NULL;
    }

    return copy;
}

int addOrderToOrderSet(Order_set orders_set) {
    if(orders_set==NULL){
        return 0;
    }
    Order new_order = createOrder(orders_set->next_order_id);
    if (new_order == NULL) {
        return 0;
    }
    SetResult set_res = setAdd(orders_set->orders_set, (void*)new_order);
    destroyOrder(new_order);
    if (set_res != SET_SUCCESS) {
        return 0;
    }
    orders_set->next_order_id++;
    return (orders_set->next_order_id-1);
}

Order getOrderFromOrdersSet(Order_set order_set, const unsigned int id) {
    assert(order_set!=NULL);
    Order current = (Order) setGetFirst(order_set->orders_set);
    while (current != NULL) {
        if (getOrderId(current) == id) {
            return current;
        }
        current = (Order) setGetNext(order_set->orders_set);
    }
    return NULL;
}

MatamikyaResult changeAmountOfProductInOrder(Order_set order_set, const unsigned int order_id, const unsigned int product_id,
                                             const double amount, Product_set product_set){
    assert(order_set!=NULL && product_set!=NULL);
    Order order= getOrderFromOrdersSet(order_set, order_id);
    if(order==NULL){
        return MATAMIKYA_ORDER_NOT_EXIST;
    }
    return changeProductAmountInOrder(order,product_id,amount,product_set);
}

MatamikyaResult cancelOrderFromOrdersSet(Order_set order_set, const unsigned int id){
    if(order_set==NULL){
        return MATAMIKYA_NULL_ARGUMENT;
    }
    Order to_remove=getOrderFromOrdersSet(order_set,id);
    if(to_remove==NULL){
        return MATAMIKYA_ORDER_NOT_EXIST;
    }
    if(getOrderStatus(to_remove)){
        return MATAMIKYA_ORDER_NOT_EXIST;
    }
    setRemove(order_set->orders_set,(void*)to_remove);
    return MATAMIKYA_SUCCESS;
}

MatamikyaResult printOrderFromSet(Order_set order_set,unsigned int order_id, FILE *output,Product_set product_set){
    assert(order_set!=NULL && output!=NULL);
    Order to_print= getOrderFromOrdersSet(order_set, order_id);
    if(to_print==NULL){
        return MATAMIKYA_ORDER_NOT_EXIST;
    }
    printOrder(to_print, output,product_set);
    return MATAMIKYA_SUCCESS;
}

MatamikyaResult OrderSetFinishOrder(Order_set order_set,Product_set product_set, const unsigned int id){
    assert(order_set!=NULL && product_set!=NULL);
    Order to_finish=getOrderFromOrdersSet(order_set,id);
    if(to_finish==NULL){
        return MATAMIKYA_ORDER_NOT_EXIST;
    }
    return finishOrder(to_finish,product_set);
 }

void clearProductFromOrderSet(Order_set order_set,unsigned int id){
    if(order_set==NULL){
        return;
    }
    Order current=(Order)setGetFirst(order_set->orders_set);
    while(current!=NULL){
        clearProductFromOrder(current,id);
        current=(Order)setGetNext(order_set->orders_set);
    }
}
