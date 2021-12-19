#ifndef ORDERS_SET_H_
#define ORDERS_SET_H_

#include "matamikya_orders.h"

/**
 * Orders Container Set
 *
 * The following functions are available:
 *   createOrderSet               - Allocates a new empty set for orders.
 *   destroyOrderSet              - Deallocates an existing set. Clears all allocated products in it.
 *   copyProductSet               - Creates a copy of existing orders set.
 *   addOrderToOrderSet           - Adds a new order to the orders set.
 *   getOrderFromOrdersSet        - Returns an order from the orders set.
 *   changeAmountOfProductInOrder - Changes product's quantity in an order from the set.
 *   cancelOrderFromOrdersSet     - Deletes an order (and deallocate it's data) from the orders set.
 *   printOrderFromSet            - Prints order's from set products and their data in an output file.
 *   OrderSetFinishOrder          - Finish an order from the set.
 *   clearProductFromOrderSet     - Deletes a product from all of the orders in the set.
 */

/** Type for defining the orders set */
typedef struct orders_set_t* Order_set;

/**
 * createOrderSet: Allocates a new empty set for orders.
 *
 * @return
 *     NULL - if the allocation failed.
 *     A new empty set for orders in case of success.
 */
Order_set createOrderSet();

/**
 * destroyOrderSet: Deallocates an existing set. Clears all allocated products in it.
 *
 * @param product_set - Target set to be deallocated. If set is NULL nothing will be done.
 */
void destroyOrderSet(Order_set order_set);

/**
 * copyProductSet: Creates a copy of existing orders set.
 *
 *  @param original - Target orders set to be copied.
 *
 * @return
 *     NULL - if the allocation failed.
 *     A copy of given set in case of success.
 */
Order_set copyOrdersSet(Order_set original);

/**
 * addOrderToOrderSet: Adds a new order to the orders set.
 *
 *  @param orders_set - Target orders set to add new order into.
 *
 * @return
 *     0 (zero) - if the new order addition to set has failed.
 *     Positive number - otherwise.
 */
int addOrderToOrderSet(Order_set orders_set);

/**
 * getOrderFromOrdersSet: Returns an order from the orders set.
 *
 *  @param order_set - Target non-void orders set to get order from.
 *  @param id - Target ID of the order requested.
 *
 * @return
 *     NULL - if the order doesn't exists in the orders set.
 *     The requested order, otherwise.
 */
Order getOrderFromOrdersSet(Order_set order_set, const unsigned int id);

/**
 * changeAmountOfProductInOrder: Changes product's quantity in an order from the set.
 *
 *  @param order_set - Target orders set which contains the order.
 *  @param order_id - Target ID of the order which contains the product.
 *  @param product_id - Target ID of the product which it's quantity needs change.
 *  @param amount - the amount to increase to/decrease from product's quantity.
 *  @param product_set - The products set which contains warehouse info about the products.
 *
 * @return
 *     MATAMIKYA_ORDER_NOT_EXIST - if the order doesn't exists in the set.
 *     MATAMIKYA_OUT_OF_MEMORY- if the program ran out of memory.
 *     MATAMIKYA_NULL_ARGUMENT - if a NULL argument was sent to function.
 *     MATAMIKYA_PRODUCT_NOT_EXIST - if the product doesn't exists in the order.
 *     MATAMIKYA_INVALID_AMOUNT - if the amount doesn't fit product's type of measurement,
 *                                doesn't stands between acceptable merge of error, or, if it is a
 *                                new product, if amount is non-positive.
 *     MATAMIKYA_SUCCESS - if the quantity update went successfully.
 */
MatamikyaResult changeAmountOfProductInOrder(Order_set order_set, const unsigned int order_id,
                                             const unsigned int product_id, const double amount,
                                             Product_set product_set);

/**
 * cancelOrderFromOrdersSet: Deletes an order (and deallocate it's data) from the orders set.
 *
 *  @param order_set - Target orders set to delete the order from.
 *  @param id - Target ID of the order which needs the be deleted.
 *
 * @return
 *     MATAMIKYA_NULL_ARGUMENT - if a NULL argument was sent to function.
 *     MATAMIKYA_ORDER_NOT_EXIST - if the order doesn't exists in the orders set.
 *     MATAMIKYA_SUCCESS - if the delete went successfully.
 */
MatamikyaResult cancelOrderFromOrdersSet(Order_set order_set, const unsigned int id);

/**
 * printOrderFromSet: Prints order's from set products and their data in an output file.
 *
 *  @param order_set - Target non-void orders set which contains the requested order.
 *  @param order_id - Target ID of the requested order.
 *  @param output - Target non-void output file to print in.
 *
 *  @return
 *     MATAMIKYA_ORDER_NOT_EXIST - if the order doesn't exists in the orders set.
 *     MATAMIKYA_SUCCESS - if the print went successfully.
 */
MatamikyaResult printOrderFromSet(Order_set order_set,unsigned int order_id, FILE *output,Product_set product_set);

/**
 * OrderSetFinishOrder: Finish an order from the set.
 *
 *  @param order_set - Target non-void orders set which contains the requested order.
 *  @param product_set - The products set which contains warehouse info about the products.
 *  @param id - Target ID of the requested order.
 *
 * @return
 *     MATAMIKYA_ORDER_NOT_EXIST - if the order doesn't exists in the orders set.
 *     MATAMIKYA_SUCCESS - if the process went successfully.
 */
MatamikyaResult OrderSetFinishOrder(Order_set order_set,Product_set product_set, const unsigned int id);

/**
 * clearProductFromOrderSet: Deletes a product from all of the orders in the set.
 *
 *  @param order_set - Target order set to delete the product from.
 *  @param id - Target ID of the product to delete.
 */
void clearProductFromOrderSet(Order_set order_set,unsigned int id);

#endif /*ORDERS_SET_H_*/
