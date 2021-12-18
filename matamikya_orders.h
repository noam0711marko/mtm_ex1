#ifndef ORDERS_H_
#define ORDERS_H_

#include "matamikya_products_set.h"
#include "matamikya_products.h"
#include "matamikya_product_in_order.h"

/**
 * Order In Orders Set
 *
 * The following functions are available:
 *   createOrder                - Allocates a new empty order
 *   destroyOrder               - Deallocates an existing order. Clears all allocated data.
 *   copyOrder                  - Creates a copy of existing order.
 *   getOrderId                 - Returns the id of a non-void order.
 *   getOrderStatus             - Returns the status of a non-void order.
 *   changeOrderStatus          - Changes order status from active to finished.
 *   calculateOrderTotalPrice   - Returns the total price of a non-void order.
 *   changeProductAmountInOrder - Changes product's amount in the order.
 *   clearProductFromOrder      - Deletes a product (and deallocate it's data) from the order.
 *   finishOrder                - Finish an order.
 *   printOrder                 - Prints order's products and their data in an output file.
 */

/** Type for defining the order */
typedef struct order_t* Order;

/**
 * createOrder: Allocates a new empty order.
 *
 * @param id - Target id of the new order.
 *
 * @return
 *     NULL - if one of allocations failed.
 *     A new empty order in case of success.
 */
Order createOrder(unsigned int id);

/**
 * destroyOrder: Deallocates an existing order. Clears all allocated data.
 *
 * @param to_destroy - Target order to be deallocated. If order is NULL nothing will be done.
 */
void destroyOrder(Order to_destroy);

/**
 * copyOrder: Creates a copy of existing order.
 *
 * @param to_copy - Target order which it's copy is requested..
 *
 * @return
 *     NULL if a NULL was sent or a memory allocation failed.
 *     A new order containing the same data as target order, otherwise.
 */
Order copyOrder(Order to_copy);

/**
 * compareOrdersId: Compare between two orders by their IDs.
 *
 * @param first - The first non-void order to compare.
 * @param second - The second non-void order to compare.
*
 * @return
 *     positive number (>0) - if first's ID is bigger.
 *     negative number (<0) - if second's ID is bigger.
 *     zero (=0) - if their IDs are equal.
 */
int compareOrdersId (Order first, Order second);

/**
 * getOrderId: Returns the id of a non-void order.
 *
 * @param order - The non-void order which it's ID is requested.
 *
 * @return
 *     The ID of the order.
 */
unsigned int getOrderId (Order order);

/**
 * getOrderStatus: Returns the status of a non-void order.
 *
 * @param order - The non-void order which it's status is requested.
 *
 * @return
 *     False - if order is still active.
 *     True - if order is finished.
 */
bool getOrderStatus(Order order);

/**
 * changeOrderStatus: Changes order status from active to finished.
 *
 * @param order - The non-void order which it's status is need to change.
 */
void changeOrderStatus(Order order);

/**
 * calculateOrderTotalPrice: Returns the total price of a non-void order.
 *
 * @param order - The non-void order which it's total price is requested.
 *
 * @return
 *     The total price of the order.
 */
double calculateOrderTotalPrice(Order orderm,Product_set product_set);

/**
 * isProductExistInOrder: Returns if a product exists in a non-void order.
 *
 * @param order - The non-void order which it's total price is requested.
 * @param product_id - ID of the product that is requested.
 *
 * @return
 *     True - if the product exists in the order.
 *     False - otherwise
 */
bool isProductExistInOrder(Order order, int product_id);

/**
 * getProductFromOrder: Returns a product from the order.
 *
 *  @param order - Target non-void order to get product from.
 *  @param id - Target ID of the product requested.
 *
 * @return
 *     NULL - if the product doesn't exists in the products set.
 *     The requested product, otherwise.
 */
Product_in_order getProductFromOrder(Order order, const int id);

/**
 * changeProductAmountInOrder: Changes product's amount in the order.
 *
 *  @param order - Target order to change quantity of product in.
 *  @param id - Target ID of the product which needs the change.
 *  @param amount - the amount to increase to/decrease from product's quantity.
 *  @param product_set - The products set which contains warehouse info about the products.
 *
 * @attention: if amount<0 and absolute smaller than current quantity -> quantity decrease in order.
 *             if amount <0 and absolute bigger or equal the current quantity -> delete product from order.
 *             **only for before-existing products!!**
 *
 * @return
 *     MATAMIKYA_OUT_OF_MEMORY- if the program ran out of memory.
 *     MATAMIKYA_NULL_ARGUMENT - if a NULL argument was sent to function.
 *     MATAMIKYA_PRODUCT_NOT_EXIST - if the product doesn't exists in the order.
 *     MATAMIKYA_INVALID_AMOUNT - if the amount doesn't fit product's type of measurement,
 *                                doesn't stands between acceptable merge of error, or, if it is a
 *                                new product, if amount is non-positive.
 *     MATAMIKYA_SUCCESS - if the quantity update went successfully.
 */
MatamikyaResult changeProductAmountInOrder(Order order, const unsigned int id, const double amount, Product_set product_set);

/**
 * clearProductFromOrder: Deletes a product (and deallocate it's data) from the order.
 *
 *  @param order - Target order to delete the product from.
 *  @param id - Target ID of the product which needs the be deleted.
 *
 * @return
 *     MATAMIKYA_NULL_ARGUMENT - if a NULL argument was sent to function.
 *     MATAMIKYA_PRODUCT_NOT_EXIST - if the product doesn't exists in the order.
 *     MATAMIKYA_SUCCESS - if the delete went successfully.
 */
MatamikyaResult clearProductFromOrder(Order order, const unsigned int id);

/**
 * finishOrder: Finish an order.
 *
 *  @param order - Target non-void order to delete the product from.
 *  @param product_set - The products set which contains warehouse info about the products.
 *
 * @return
 *     MATAMIKYA_INSUFFICIENT_AMOUNT - if (at least one of) the product's quantity in the
 *                                     order is bigger than warehouse quantity (stock).
 *     MATAMIKYA_SUCCESS - if the process went successfully.
 */
MatamikyaResult finishOrder(Order order, Product_set product_set);

/**
 * printOrder: Prints order's products and their data in an output file.
 *
 *  @param order - Target non-void order to print.
 *  @param output - Target non-void output file to print in.
 */
void printOrder(Order order, FILE* output,Product_set product_set);

#endif //ORDERS_H_
