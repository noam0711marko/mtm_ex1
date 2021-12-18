#ifndef AMOUNT_SET_STR_TEST_UTILITIES_H_
#define AMOUNT_SET_STR_TEST_UTILITIES_H_

#include <stdbool.h>
#include <stdio.h>

/**
 * These macros are here to help you create tests more easily and keep them
 * clear.
 *
 * The basic idea with unit-testing is create a test function for every real
 * function and inside the test function declare some variables and execute the
 * function under test.
 *
 * Use the ASSERT_TEST and ASSERT_TEST_WITH_FREE to verify correctness of
 * values.
 */

/**
 * Evaluates expr and continues if expr is true.
 * If expr is false, ends the test by returning false, prints a detailed
 * message about the failure, and frees resources by evaluating destroy.
 */
#define ASSERT_TEST_WITH_FREE(expr, destroy)                                      \
    do {                                                                          \
        if (!(expr)) {                                                            \
            printf("\033[31m");\
            printf("\nAssertion failed at %s:%d ", __FILE__, __LINE__);           \
            printf("\033[31m");                                                                      \
            /*destroy; */                                                             \
            return false;                                                         \
        }                                                                         \
    } while (0)

/**
 * Evaluates expr and continues if expr is true.
 * If expr is false, ends the test by returning false and prints a detailed
 * message about the failure.
 */
/* #define ASSERT_TEST(expr)                                                         \ */
/*     do {                                                                          \ */
/*         if (!(expr)) {                                                            \ */
/*             printf("\nAssertion failed at %s:%d %s ", __FILE__, __LINE__, #expr); \ */
/*             return false;                                                         \ */
/*         }                                                                         \ */
/*     } while (0) */
#define ASSERT_TEST(expr) ASSERT_TEST_WITH_FREE(expr, NULL)

/**
 * Macro used for running a test from the main function
 */

#define RUN_COLORFULL_TEST(test, name, id)    \
\
    do {                                 \
        blue();      printf("Running test #%ld %s ... ", id + 1, name);  reset(); \
        fflush(stdout); \
        if (test()) {                    \
            green();\
            printf("[OK]\n");            \
            reset();\
            ++NumTestsPassed;   \
        }                                     \
        else {    \
            red();\
            printf("[Failed]\n");        \
            reset();\
        }                                \
    } while (0)

#endif /* AMOUNT_SET_STR_TEST_UTILITIES_H_ */
