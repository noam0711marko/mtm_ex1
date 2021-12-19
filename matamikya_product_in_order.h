#ifndef PRODUCT_IN_ORDER_H_
#define PRODUCT_IN_ORDER_H_

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "matamikya.h"
#include "matamikya_products_set.h"

/**
 * Product In Order
 *
 * The following functions are available:
 *   createProductInOrder          - Allocates a new empty product
 *   destroyProductInOrder         - Deallocates an existing product in order. Clears all allocated data.
 *   copyProductInOrder            - Creates a copy of existing product.
 *   compareProductsInOrder        - Compare between two products by their IDs.
 *   getProductNameFromOrder       - Returns a copy of the name of a non-void product.
 *   getProductIdFromOrder         - Returns the id of a non-void product.
 *   getProductAmountFromOrderData - Returns the quantity of a non-void product in the order.
 *   getProductTypeFromOrderData   - Returns the type of measurement of a non-void product.
 *   updateProductAmountInOrder    - Updates product's quantity in order.
 */

/** Type for defining the product */
typedef struct product_in_order_t *Product_in_order;

/**
 * createProductInOrder: Allocates a new empty Product.
 *
 * @param name - Target name of the new product.
 * @param id - Target id of the new product.
 * @param amount - Target quantity of the new product.
 * @param price - Target price in order of the new product (calculated by amount).
 * @param type - Target type of measurement of the new product's quantity.
 *
 * @return
 *     NULL - if one of the parameters is NULL or one of allocations failed.
 *     A new product in case of success.
 */
Product_in_order createProductInOrder(char* name, unsigned int id, double amount, MatamikyaAmountType type);

/**
 * destroyProductInOrder: Deallocates an existing product in order. Clears all allocated data.
 *
 * @param to_destroy - Target product to be deallocated. If product is NULL nothing will be done.
 */
void destroyProductInOrder(Product_in_order to_destroy);

/**
 * copyProductInOrder: Creates a copy of existing product.
 *
 * @param to_copy - Target non-void product which it's copy is requested..
 *
 * @return
 *     NULL if a NULL was sent or a memory allocation failed.
 *     A new product containing the same data as target product, otherwise.
 */
Product_in_order copyProductInOrder(Product_in_order to_copy);

/**
 * compareProductsInOrder: Compare between two products by their IDs.
 *
 * @param first - The first non-void product to compare.
 * @param second - The second non-void product to compare.
*
 * @return
 *     positive number (>0) - if first's ID is bigger.
 *     negative number (<0) - if second's ID is bigger.
 *     zero (=0) - if their IDs are equal.
 */
int compareProductsInOrder(Product_in_order first, Product_in_order second);

/**
 * getProductNameFromOrder: Returns a copy of the name of a non-void product.
 *
 * @param product - The non-void product which it's name is requested.
 *
 * @return
 *     A copy of the name of the product.
 */
char* getProductNameFromOrder(Product_in_order product_in_order);

/**
 * getProductIdFromOrder: Returns the id of a non-void product.
 *
 * @param product - The non-void product which it's ID is requested.
 *
 * @return
 *     The ID of the product.
 */
unsigned int getProductIdFromOrder(Product_in_order product_in_order);

/**
 * getProductAmountFromOrderData: Returns the quantity of a non-void product in the order.
 *
 * @param product - The non-void product which it's quantity is requested.
 *
 * @return
 *     The quantity of the product in the order.
 */
double getProductAmountFromOrderData(Product_in_order product_in_order);


/**
 * getProductTypeFromOrderData: Returns the type of measurement of a non-void product.
 *
 * @param product - The non-void product which it's measurement type is requested.
 *
 * @return
 *     The type of measurement of the product.
 */
MatamikyaAmountType getProductTypeFromOrderData(Product_in_order product_in_order);

/**
 * updateProductAmountInOrder: Updates product's quantity in order.
 *
 * @param product - The non-void product which it's quantity is needing an update.
 * @param amount - The quantity to increase to/decrease from product's quantity in order.
 */
void updateProductAmountInOrder(Product_in_order product_in_order, double amount);

#endif /*PRODUCT_IN_ORDER_H_*/
