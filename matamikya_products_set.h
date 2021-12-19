#ifndef PRODUCTS_SET_H_
#define PRODUCTS_SET_H_

#include "set.h"
#include "matamikya_products.h"
#include "matamikya_validation_check.h"

/**
 * Products Container Set
 *
 * The following functions are available:
 *   createProductSet           - Allocates a new empty set for products.
 *   destroyProductSet          - Deallocates an existing set. Clears all allocated products in it.
 *   copyProductSet             - Creates a copy of existing products set.
 *   addProductToProductsSet    - Creates new product and adds it to the products set.
 *   getProductFromProductSet   - Returns a product from the products set.
 *   changeProductAmount        - Changes product's quantity in the products set.
 *   clearProductFromProductSet - Deletes a product (and deallocate it's data) from the products set.
 *   printProductSet            - Prints all product set's products and their data in an output file.
 *   printBestSellingProductSet - Prints product set's best selling product in an output file.
 */

/** Type for defining the products set */
typedef struct product_set_t* Product_set;

/**
 * createProductSet: Allocates a new empty set for products.
 *
 * @return
 *     NULL - if the allocation failed.
 *     A new empty set for products in case of success.
 */
Product_set createProductSet();

/**
 * destroyProductSet: Deallocates an existing set. Clears all allocated products in it.
 *
 * @param product_set - Target set to be deallocated. If set is NULL nothing will be done.
 */
void destroyProductSet(Product_set product_set);

/**
 * copyProductSet: Creates a copy of existing products set.
 *
 *  @param original - Target products set to be copied.
 *
 * @return
 *     NULL - if the allocation failed.
 *     A copy of given set in case of success.
 */
Product_set copyProductSet(Product_set original);

/**
 * addProductToProductsSet: Creates new product and adds it to the products set.
 *
 *  @param product_set - Target non-void products set to be copied.
 *  @param id - Target ID of the new product.
 *  @param name - Target name of the new product.
 *  @param amount - Target quantity of the new product.
 *  @param amountType - Target type of measurement for new product's quantity.
 *  @param customData - Target custom data for the new product.
 *  @param copyData - Pointer to copy function for new product.
 *  @param freeData - Pointer to free function for new product.
 *  @param prodPrice - Pointer to calculating function for new product's price.
 *
 * @return
 *     MATAMIKYA_OUT_OF_MEMORY - if the program ran out of memory.
 *     MATAMIKYA_NULL_ARGUMENT - if a NULL argument was sent to function.
 *     MATAMIKYA_INVALID_NAME - if the name doesn't start with a letter ot a number.
 *     MATAMIKYA_INVALID_AMOUNT - if the amount is non-positive, doesn't fit product's type of
 *                                measurement or doesn't stands between acceptable merge of error.
 *     MATAMIKYA_PRODUCT_ALREADY_EXIST - if the product already exists in the products set.
 *     MATAMIKYA_SUCCESS - if the product's addition went successfully.
 */
MatamikyaResult addProductToProductsSet(Product_set product_set, const unsigned int id, const char *name,
                                        const double amount, const MatamikyaAmountType amountType,
                                        const MtmProductData customData, MtmCopyData copyData,
                                        MtmFreeData freeData, MtmGetProductPrice prodPrice);

/**
 * getProductFromProductSet: Returns a product from the products set.
 *
 *  @param product_set - Target non-void products set to get product from.
 *  @param id - Target ID of the product requested.
 *
 * @return
 *     NULL - if the product doesn't exists in the products set.
 *     The requested product, otherwise.
 */
Product getProductFromProductSet(Product_set product_set, const int id);

/**
 * changeProductAmount: Changes product's quantity in the products set.
 *
 *  @param product_set - Target products set to change quantity of product in.
 *  @param id - Target ID of the product which needs the change.
 *  @param amount - the amount to increase to/decrease from product's quantity.
 *
 * @return
 *     MATAMIKYA_NULL_ARGUMENT - if a NULL argument was sent to function.
 *     MATAMIKYA_PRODUCT_NOT_EXIST - if the product doesn't exists in the products set.
 *     MATAMIKYA_INVALID_AMOUNT - if the amount doesn't fit product's type of measurement or
 *                                doesn't stands between acceptable merge of error.
 *     MATAMIKYA_INSUFFICIENT_AMOUNT - if the amount is negative and absolute bigger than current
 *                                     product's quantity in the products set.
 *     MATAMIKYA_SUCCESS - if the quantity update went successfully.
 */
MatamikyaResult changeProductAmount(Product_set productSet, const unsigned int id, const double amount);

/**
 * clearProductFromProductSet: Deletes a product (and deallocate it's data) from the products set.
 *
 *  @param product_set - Target products set to delete the product from.
 *  @param id - Target ID of the product which needs the be deleted.
 *
 * @return
 *     MATAMIKYA_NULL_ARGUMENT - if a NULL argument was sent to function.
 *     MATAMIKYA_PRODUCT_NOT_EXIST - if the product doesn't exists in the products set.
 *     MATAMIKYA_SUCCESS - if the delete went successfully.
 */
MatamikyaResult clearProductFromProductSet(Product_set product_set, const unsigned int id);

/**
 * printProductSet: Prints all product set's products and their data in an output file.
 *
 *  @param product_set - Target non-void products set to print.
 *  @param output - Target non-void output file to print in.
 */
void printProductSet(Product_set product_set, FILE *output);

/**
 * printBestSellingProductSet: Prints product set's best selling product in an output file.
 *
 *  @param product_set - Target products set which it's best selling product is requested.
 *  @param output - Target output file to print in.
 *
 * @return
 *     MATAMIKYA_NULL_ARGUMENT - if a NULL argument was sent to function.
 *     MATAMIKYA_SUCCESS - if the print went successfully.
 */
MatamikyaResult printBestSellingProductSet(Product_set product_set,FILE* output);

#endif /*PRODUCTS_SET_H_*/
