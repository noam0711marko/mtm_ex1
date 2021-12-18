#include "amount_set_str_tests.h"
#include "amount_set_str_test_utilities.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

static long NumTestsPassed = 0;

/*color printing functions*/
static void red () {
    printf("\033[1;31m");
    fflush(stdout);
}

static void green() {
    printf("\033[0;32m");
    fflush(stdout);
}

static void blue() {
    printf("\033[0;94m");
}

static void yellow () {
    printf("\033[0;33m");
    fflush(stdout);
}

static void purple () {
    printf("\033[0;95m");
    fflush(stdout);
}

static void reset () {
    printf("\033[0m");
    fflush(stdout);
}

/*If you would like to run an OUT_OF_MEMORY check, uncomment the #define
 * warning! the OUT_OF_MEMORY check contains an (almost) infinite loop.
 * therefore, the test might take long time to run.
 */

//#define WITH_NO_MEMORY_CHECK

static void printNumPassed(long num_tests){
    printf("####  Summary: Passed %ld out of %ld ####\n" ,NumTestsPassed, num_tests);

}

static void printTestSucceed(){
    printf("Congratulations! TEST SUCCEED :-)\n");
}

static void printTestFailed(){
    printf("Oh No! TEST FAILED :-(\n");
    printf("Check assertion failures and fix them!\n");
}

static void printMemCheck(){
    printf("Dont forget to run valgrind and check for memory leaks!!!\n");
}

static void printIfSuccess(long num_tests)
{
    if (0 == NumTestsPassed)
    {
        red();
        printNumPassed(num_tests);
        printTestFailed();
    }
    if (num_tests == NumTestsPassed)
    {
        green();
        printNumPassed(num_tests);
        printTestSucceed();
    }
    else
    {
        yellow();
        printNumPassed(num_tests);
        printTestFailed();
    }

    purple();
    printMemCheck();
    reset();
}

static void printCreatedBy(){
    printf("\033[1;36m");
    printf("\nTEST CREATED BY:\n");
    printf("\033[1;96m");
    printf("Noam Marco & Dolev Gefen\n");
    reset();
}

/*The functions for the tests should be added here*/
bool (*tests[]) (void) = {
        testAsCreateDestroy,
        testAsAddAndContainsAndSize,
        testChangeAndGetAmount,
        testAsDeleteAndClear,
        allAroundTest
};

#define NUMBER_TESTS ((long)(sizeof(tests)/sizeof(*tests)))

/*The names of the test functions should be added here*/
const char* testNames[NUMBER_TESTS] = {
        "testAsCreateDestroy",
        "testAsAddAndContainsAndSize",
        "testChangeAndGetAmount",
        "testAsDeleteAndClear",
        "allAroundTest"
};

int main(int argc, char *argv[])
{
    long number_tests = NUMBER_TESTS;
    if (argc == 1)
    {
        fprintf(stdout, "Running %ld tests:\n", number_tests);
        for (long test_idx = 0; test_idx < number_tests; test_idx++)
        {
            RUN_COLORFULL_TEST(tests[test_idx], testNames[test_idx], test_idx);
        }
        printIfSuccess(number_tests);
    }
    else if (argc != 2)
    {
        fprintf(stdout, "Usage: matamikya <test index>\n");
        return 0;
    }
    else
    {
        long test_idx = strtol(argv[1], NULL, 10);
        if (test_idx < 1 || test_idx > NUMBER_TESTS)
        {
            fprintf(stderr, "Invalid test index %ld\n", test_idx);
            return 0;
        }
        number_tests = 1;
        RUN_COLORFULL_TEST(tests[test_idx - 1], testNames[test_idx - 1], test_idx - 1);
    }
    printCreatedBy();
    return 0;
}
