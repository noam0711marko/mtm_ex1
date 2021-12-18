#ifndef VALIDATION_CHECK_H_
#define VALIDATION_CHECK_H_

#include <stdbool.h>

#include "matamikya.h"

/**
 * Validation Checks for Name and Amount
 *
 * The following functions are available:
 *   isNameLegal               - Checks if name meets conditions
 *   checkIfAmountLegal        - Deallocates an existing set. Clears all allocated products in it.
 *
 */

/**
 * isNameLegal: Checks if name meets conditions
 *
 * @return
 *     true - if name starts with a letter or a number.
 *     false - otherwise.
 */
bool isNameLegal(const char *name);

/**
 * checkIfAmountLegal: Checks if amount meets conditions
 *
 * @attention send only positive amount!
 *            to check type and merge of error validation of negative amount, send -amount to function!
 * @return
 *     true - if amount is positive, fits type the type of measurement of product and stand between
 *            acceptable merge of error.
 *     false - otherwise.
 */
bool checkIfAmountLegal(double amount, MatamikyaAmountType type);

#endif //VALIDATION_CHECK_H_
