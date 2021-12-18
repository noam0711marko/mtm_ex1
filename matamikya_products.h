#ifndef PRODUCTS_H_
#define PRODUCTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "matamikya.h"
#include "matamikya_print.h"


/**
 * Product In Product Set
 *
 * The following functions are available:
 *   createProduct        - Allocates a new empty product
 *   freeProduct          - Deallocates an existing product. Clears all allocated data.
 *   copyProduct          - Creates a copy of existing product.
 *   getProductId         - Returns the id of a non-void product.
 *   getProductName       - Returns the name of a non-void product.
 *   getProductProfit     - Returns the profit from a non-void product.
 *   getProductAmount     - Returns the quantity of a non-void product.
 *   getProductAmountType - Returns the type of measurement of a non-void product.
 *   getProductData       - Returns the custom data of a non-void product.
 *   updateProductAmount  - Updates product's quantity.
 *   updateProductProfit  - Adds profit from product's total profit.
 *   productCmp           - Compare between two products by their IDs.
 *   printProduct         - Prints product data in wanted output file.
 *   getPrice             - Returns product's total price.
 */

/** Type for defining the product */
typedef struct products_t * Product;

/**
 * createProduct: Allocates a new empty product.
 *
 * @param id - Target id of the new product.
 * @param name - Target name of the new product.
 * @param amount - Target quantity of the new product.
 * @param amountType - Target type of measurement of the new product's quantity.
 * @param data - Custom data which is inserted by user to product's data.
 * @param get_price - Function pointer to be used for calculate product's price.
 * @param copy - Function pointer to be used for copying product's data.
 * @param free_data - Function pointer to be used for removing product's data.
 *
 * @return
 *     NULL - if one of the parameters is NULL or one of allocations failed.
 *     A new product in case of success.
 */
Product createProduct(const unsigned int id, const char* name, double amount,MatamikyaAmountType amountType, MtmProductData data,
                       MtmGetProductPrice get_price,MtmCopyData copy,MtmFreeData free_data);

/**
 * freeProduct: Deallocates an existing product. Clears all allocated data.
 *
 * @param to_destroy - Target product to be deallocated. If product is NULL nothing will be done.
 */
void freeProduct(Product to_destroy);

/**
 * copyProduct: Creates a copy of existing product.
 *
 * @param to_copy - Target product which it's copy is requested..
 *
 * @return
 *     NULL if a NULL was sent or a memory allocation failed.
 *     A new product containing the same data as target product, otherwise.
 */
Product copyProduct(Product to_copy);

/**
 * getProductId: Returns the id of a non-void product.
 *
 * @param product - The non-void product which it's ID is requested.
 *
 * @return
 *     The ID of the product.
 */
unsigned int getProductId(Product product);

/**
 * getProductName: Returns a copy of the name of a non-void product.
 *
 * @param product - The non-void product which it's name is requested.
 *
 * @return
 *     A copy of the name of the product.
 */
char* getProductName(Product product);

/**
 * getProductProfit: Returns the profit from a non-void product.
 *
 * @param product - The non-void product which it's profit is requested.
 *
 * @return
 *     The profit from the product.
 */
double getProductProfit(Product product);

/**
 * getProductAmount: Returns the quantity of a non-void product.
 *
 * @param product - The non-void product which it's quantity is requested.
 *
 * @return
 *     The quantity of the product.
 */
double getProductAmount(Product product);

/**
 * getProductAmountType: Returns the type of measurement of a non-void product.
 *
 * @param product - The non-void product which it's measurement type is requested.
 *
 * @return
 *     The type of measurement of the product.
 */
MatamikyaAmountType getProductAmountType(Product product);

/**
 * getProductData: Returns the custom data of a non-void product.
 *
 * @param product - The non-void product which it's custom data is requested.
 *
 * @return
 *     The custom data of the product.
 */
MtmProductData getProductData(Product product);

/**
 * updateProductAmount: Updates product's quantity.
 *
 * @param product - The non-void product which it's quantity is needing an update.
 * @param amount - The new quantity of the product.
 */
void updateProductAmount(Product product,double amount);

/**
 * updateProductProfit: Adds profit from product's total profit.
 *
 * @param product - The non-void product which the profit from is needing an update.
 * @param profit - The profit to add to product's total profit.
 */
void updateProductProfit(Product product,double profit);

/**
 * productCmp: Compare between two products by their IDs.
 *
 * @param product_a - The first non-void product to compare.
 * @param product_b - The second non-void product to compare.
*
 * @return
 *     positive number (>0) - if first's ID is bigger.
 *     negative number (<0) - if second's ID is bigger.
 *     zero (=0) - if their IDs are equal.
 */
int productCmp(Product product_a,Product product_b);

/**
 * printProduct: Prints product data in wanted output file.
 *
 * @param product - The non-void product which it's data is needing to be printed.
 * @param output - The file we are willing to print to.
 */
void printProduct(Product product,FILE * output);

/**
 * getPrice: Returns product's total price.
 *
 * @param product - The non-void product which it's total price (by amount) is requested.
 * @param amount - The quantity of the product.
 *
 * @return
 *     The total price of the product.
 */
double getPrice(Product product,double amount);

#endif //PRODUCTS_H_

