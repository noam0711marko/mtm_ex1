#include "amount_set_str_tests.h"

/////////////////////////////////////////////////////// defines ////////////////////////////////////////////////////////

/* can be any number you would like, as long as it's negative and bigger than -2,147,483,648 */
#define VERY_BIG_NEGATIVE -13232321

/* defines for the random generator function. please don't change! */
#define RAND_MAX 32767
#define RANDOM_MULTIPLY 1103515245
#define RANDOM_ADD 12345
#define RANDOM_DIVIDE 65536

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// random string generator/////////////////////////////////////////////////////////////
//rand() implementation
static unsigned long int next = 1;

static int random(void){
    next = next * RANDOM_MULTIPLY + RANDOM_ADD;
    return (unsigned int)(next/RANDOM_DIVIDE) % RAND_MAX;
}

//random string generator
static void rand_str(char *dest, size_t length) {
    char charset[] = "0123456789"
                     "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (length-- > 0) {
        size_t index = (double) random() / RAND_MAX * (sizeof(charset) - 1);
        *dest++ = charset[index];
    }
    *dest = '\0';
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// tests /////////////////////////////////////////////////////////

//test #1 - FULL
bool testAsCreateDestroy() {
    AmountSet set = asCreate();
    ASSERT_TEST(set != NULL);
    ASSERT_TEST(asGetSize(set) == 0);
    ASSERT_TEST(asGetFirst(set) == NULL);

    asDestroy(set);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//test #2 - FULL
bool testAsAddAndContainsAndSize() {
    AmountSet rand_set = asCreate();
    for (int key = 1 ; key < 1000 ; ++key) {
        char str[] = { [41] = '\1' };
        rand_str(str, sizeof str - 1);
        ASSERT_TEST(asRegister(rand_set, str) == AS_SUCCESS);
        ASSERT_TEST(asGetSize(rand_set) == key);
    }
    char* previous = asGetFirst(rand_set);
    char* current = asGetNext(rand_set);
    while (current != NULL) {
        ASSERT_TEST(strcmp(previous,current)<0);
        previous = current;
        current = asGetNext(rand_set);
    }
    asDestroy(rand_set);
    AmountSet set = asCreate();
    ASSERT_TEST(asRegister(NULL, "chair") == AS_NULL_ARGUMENT);
    ASSERT_TEST(asRegister(set, NULL) == AS_NULL_ARGUMENT);
    ASSERT_TEST(asRegister(set, "chair")==AS_SUCCESS);
    ASSERT_TEST(asRegister(set, "chair") == AS_ITEM_ALREADY_EXISTS);
    ASSERT_TEST(asRegister(set, "Chair") == AS_SUCCESS);
    char* chair = "CHAIR";
    ASSERT_TEST(asRegister(set, chair) == AS_SUCCESS);
    asRegister(set, "closet");
    char* desk = "desk";
    asRegister(set, desk);
    asRegister(set, "toilet seat #4328");
    ASSERT_TEST(asContains(set, desk) == true);
    ASSERT_TEST(asContains(set, "clOset") == false);
    ASSERT_TEST(asContains(set, "toilet seat #4329") == false);
    ASSERT_TEST(asContains(set, "toilet seat #4328") == true);
    ASSERT_TEST(asContains(set, "Gaming Chair") == false);
    ASSERT_TEST(asContains(NULL, "desk") == false);
    ASSERT_TEST(asContains(set, NULL) == false);
    desk="DESK";
    ASSERT_TEST(asContains(set, "DESK")==false);
    ASSERT_TEST(asContains(set, "desk")==true);
    chair="harta-barta";
    ASSERT_TEST(asContains(set,chair)==false);
    ASSERT_TEST(strcmp(asGetFirst(set),"CHAIR")==0);
    asDestroy(set);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//test #3 - initial settings
static void test3init(AmountSet set,char* desk){
    asRegister(set, "chair");
    asRegister(set, "closet");
    asRegister(set, desk);
    asRegister(set, "toilet seat #4328");
}

//test #3 - FULL
bool testChangeAndGetAmount() {
    AmountSet set = asCreate();
    char* desk = "desk";
    test3init(set,desk);
    double amount = random();
    ASSERT_TEST(asGetAmount(set, "chair", &amount) == AS_SUCCESS);
    ASSERT_TEST(amount == 0);
    amount = random();
    ASSERT_TEST(asGetAmount(set, "closet", &amount) == AS_SUCCESS);
    ASSERT_TEST(amount == 0);
    amount = random();
    ASSERT_TEST(asGetAmount(set, desk, &amount) == AS_SUCCESS);
    ASSERT_TEST(amount == 0);
    amount = random();
    ASSERT_TEST(asGetAmount(NULL, "toilet seat #4328", &amount) == AS_NULL_ARGUMENT);
    ASSERT_TEST(asGetAmount(set, NULL, &amount) == AS_NULL_ARGUMENT);
    ASSERT_TEST(asGetAmount(set, "toilet seat #4328", NULL) == AS_NULL_ARGUMENT);
    ASSERT_TEST(asGetAmount(set, "toilet seat #4329", NULL) == AS_NULL_ARGUMENT);
    ASSERT_TEST(asGetAmount(set, "toilet Seat #4329", &amount) == AS_ITEM_DOES_NOT_EXIST);
    ASSERT_TEST(asChangeAmount(NULL, "toilet Seat #4328", 10) == AS_NULL_ARGUMENT);
    ASSERT_TEST(asChangeAmount(set, NULL, 10) == AS_NULL_ARGUMENT);
    ASSERT_TEST(asChangeAmount(set, "toilet seat #4329", 10) == AS_ITEM_DOES_NOT_EXIST);
    ASSERT_TEST(asChangeAmount(set, "chair", 10) == AS_SUCCESS);
    ASSERT_TEST(asGetAmount(set, "chair", &amount) == AS_SUCCESS);
    ASSERT_TEST(amount == 10);
    amount=random();
    ASSERT_TEST(asChangeAmount(set, "chair", -100) == AS_INSUFFICIENT_AMOUNT);
    ASSERT_TEST(asGetAmount(set, "chair", &amount) == AS_SUCCESS);
    ASSERT_TEST(amount == 10);
    ASSERT_TEST(asChangeAmount(set, "chair", -9) == AS_SUCCESS);
    ASSERT_TEST(asGetAmount(set, "chair", &amount) == AS_SUCCESS);
    ASSERT_TEST(amount == 1);
    ASSERT_TEST(asChangeAmount(NULL, "chair", -100) == AS_NULL_ARGUMENT);
    ASSERT_TEST(asGetAmount(set, "chair", &amount) == AS_SUCCESS);
    ASSERT_TEST(amount == 1);
    ASSERT_TEST(asChangeAmount(set, NULL, -100) == AS_NULL_ARGUMENT);
    ASSERT_TEST(asGetAmount(set, "chair", &amount) == AS_SUCCESS);
    ASSERT_TEST(amount == 1);
    ASSERT_TEST(asChangeAmount(set, "GAMING CHAIR", -100) == AS_ITEM_DOES_NOT_EXIST);
    amount = random();
    ASSERT_TEST(asChangeAmount(set, "desk", 2) == AS_SUCCESS);
    ASSERT_TEST(asGetAmount(set, desk, &amount) == AS_SUCCESS);
    ASSERT_TEST(amount == 2);
    amount = random();
    ASSERT_TEST(asChangeAmount(set, desk, -2) == AS_SUCCESS);
    ASSERT_TEST(asGetAmount(set, "desk", &amount) == AS_SUCCESS);
    ASSERT_TEST(amount == 0);

    asDestroy(set);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//test #4 - FULL
bool testAsDeleteAndClear() {
    AmountSet office = asCreate();
    AmountSet bedroom = asCreate();
    AmountSet toilet_and_shower = asCreate();
    asRegister(office, "chair");
    asRegister(bedroom, "closet");
    char *desk = "desk";
    asRegister(office, desk);
    asRegister(toilet_and_shower, "toilet seat #4328");
    asRegister(office, "pencil holder");
    asRegister(office, "mouse pad");
    asRegister(bedroom, "bed");
    asRegister(bedroom, "dresser");
    asRegister(bedroom, "night light");
    asRegister(toilet_and_shower, "13th century Roman sink");
    asRegister(toilet_and_shower, "shower head (round)");
    asRegister(toilet_and_shower, "toilet heater");
    asRegister(toilet_and_shower, "shower head (square)");
    asRegister(toilet_and_shower, "towel holder");
    int office_set_size = asGetSize(office);
    int toilet_and_shower_set_size = asGetSize(toilet_and_shower);
    asRegister(office, "towel holder");
    ASSERT_TEST(asDelete(bedroom, "towel holder") == AS_ITEM_DOES_NOT_EXIST);
    ASSERT_TEST(asDelete(NULL, "towel holder") == AS_NULL_ARGUMENT);
    ASSERT_TEST(asDelete(office, NULL) == AS_NULL_ARGUMENT);
    ASSERT_TEST(asDelete(office, "towel holder") == AS_SUCCESS);
    ASSERT_TEST(asGetSize(office) == office_set_size);
    ASSERT_TEST(asContains(office, "towel holder") == false);
    ASSERT_TEST(asDelete(toilet_and_shower, "shower head (round)") == AS_SUCCESS);
    ASSERT_TEST(asGetSize(toilet_and_shower) == toilet_and_shower_set_size - 1);
    ASSERT_TEST(asContains(toilet_and_shower, "shower head (round)") == false);
    ASSERT_TEST(asClear(bedroom) == AS_SUCCESS);
    ASSERT_TEST(asContains(bedroom, "dresser") == false);
    ASSERT_TEST(asContains(bedroom, "night light") == false);
    ASSERT_TEST(asGetSize(bedroom) == 0);
    ASSERT_TEST(asGetFirst(bedroom) == NULL);
    ASSERT_TEST(asGetNext(bedroom) == NULL);
    ASSERT_TEST(strcmp(asGetFirst(office), "chair") == 0);
    ASSERT_TEST(asDelete(office, "chair") == AS_SUCCESS);
    ASSERT_TEST(strcmp(asGetFirst(office), "chair") > 0);
    ASSERT_TEST(strcmp(asGetFirst(office), "desk") == 0);
    ASSERT_TEST(asGetSize(office) == office_set_size - 1);

    asDestroy(office);
    asDestroy(bedroom);
    asDestroy(toilet_and_shower);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//test #5 - badroom set initial insertions
static void insertToBadroom(AmountSet badroom){
    asRegister(badroom, "king-size bed");
    asRegister(badroom, "queen-size bed");
    asRegister(badroom, "double bed");
    asRegister(badroom, "single bed");
    asRegister(badroom, "bunk beds");
    asRegister(badroom, "gallery bed");
    asRegister(badroom, "pillow #328");
    asRegister(badroom, "pillow #142");
    asRegister(badroom, "pillow #906");
    asRegister(badroom, "orthopedic pillow #086");
    asRegister(badroom, "orthopedic pillow #094");
    asRegister(badroom, "mattress 220X140");
    asRegister(badroom, "mattress 220X160");
    asRegister(badroom, "mattress 220X180");
    asRegister(badroom, "mattress 180X140");
}

//test #5 - badroom set sort checking
static bool checkBadroomSort(AmountSet badroom){
    ASSERT_TEST(strcmp("bunk beds", asGetFirst(badroom))==0);
    ASSERT_TEST(strcmp("double bed", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("gallery bed", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("king-size bed", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("mattress 180X140", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("mattress 220X140", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("mattress 220X160", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("mattress 220X180", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("orthopedic pillow #086", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("orthopedic pillow #094", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("pillow #142", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("pillow #328", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("pillow #906", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("queen-size bed", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("single bed", asGetNext(badroom))==0);
    return true;
}

//test #5 - badroom set error returns checking
static bool badroomErrorReturningCheck(AmountSet badroom){
    ASSERT_TEST(asRegister(badroom, "pillow #142")==AS_ITEM_ALREADY_EXISTS);
    ASSERT_TEST(asRegister(badroom, "king-size bed")==AS_ITEM_ALREADY_EXISTS);
    ASSERT_TEST(asRegister(badroom, NULL)==AS_NULL_ARGUMENT);
    char* null_string=NULL;
    ASSERT_TEST(asRegister(badroom, null_string)==AS_NULL_ARGUMENT);
    AmountSet null_set=NULL;
    ASSERT_TEST(asGetSize(null_set)==-1);
    ASSERT_TEST(asRegister(null_set, "regular bed")==AS_NULL_ARGUMENT);
    ASSERT_TEST(asClear(null_set)==AS_NULL_ARGUMENT);
    ASSERT_TEST(asGetFirst(null_set)==NULL);

    return true;
}

//test #5 - badroom set initial amounts and change amounts checking
static bool badroomAmountCheck(AmountSet badroom, double current_amount, char* current_item, double add_amount){
    while (current_item != NULL) {
        asGetAmount(badroom, current_item, &current_amount);
        ASSERT_TEST(current_amount==0);
        current_amount=random();
        current_item = asGetNext(badroom);
    }
    current_item=asGetFirst(badroom);
    asChangeAmount(badroom, current_item, add_amount);
    asGetAmount(badroom, current_item, &current_amount);
    ASSERT_TEST(current_amount==add_amount);
    for (int i=1; i< asGetSize(badroom); i++){
        add_amount=(double)random();
        current_item=asGetNext(badroom);
        asChangeAmount(badroom, current_item, add_amount);
        asGetAmount(badroom, current_item, &current_amount);
        ASSERT_TEST(current_amount==add_amount);
    }
    return true;
}

//test #5 - copy badroom set into a new set with correct name checking
static bool copyBadroomToBedroomCheck(AmountSet badroom, AmountSet bedroom){
    ASSERT_TEST(asRegister(bedroom, "pillow #142")==AS_ITEM_ALREADY_EXISTS);
    ASSERT_TEST(asRegister(bedroom, "king-size bed")==AS_ITEM_ALREADY_EXISTS);
    ASSERT_TEST(strcmp("bunk beds", asGetFirst(bedroom))==0);
    ASSERT_TEST(strcmp("double bed", asGetNext(bedroom))==0);
    ASSERT_TEST(strcmp("gallery bed", asGetNext(bedroom))==0);
    ASSERT_TEST(strcmp("king-size bed", asGetNext(bedroom))==0);
    ASSERT_TEST(strcmp("mattress 180X140", asGetNext(bedroom))==0);
    ASSERT_TEST(strcmp("mattress 220X140", asGetNext(bedroom))==0);
    ASSERT_TEST(strcmp("mattress 220X160", asGetNext(bedroom))==0);
    ASSERT_TEST(strcmp("mattress 220X180", asGetNext(bedroom))==0);
    ASSERT_TEST(strcmp("orthopedic pillow #086", asGetNext(bedroom))==0);
    ASSERT_TEST(strcmp("orthopedic pillow #094", asGetNext(bedroom))==0);
    ASSERT_TEST(strcmp("pillow #142", asGetNext(bedroom))==0);
    ASSERT_TEST(strcmp("pillow #328", asGetNext(bedroom))==0);
    ASSERT_TEST(strcmp("pillow #906", asGetNext(bedroom))==0);
    ASSERT_TEST(strcmp("queen-size bed", asGetNext(bedroom))==0);
    ASSERT_TEST(strcmp("single bed", asGetNext(bedroom))==0);
    double badroom_curr_amount;
    double bedroom_curr_amount;
    char* badroom_current=asGetFirst(badroom);
    char* bedroom_current=asGetFirst(bedroom);
    while (badroom_current != NULL && bedroom_current != NULL ) {
        asGetAmount(badroom, badroom_current, &badroom_curr_amount);
        asGetAmount(bedroom, bedroom_current, &bedroom_curr_amount);
        ASSERT_TEST(badroom_curr_amount==bedroom_curr_amount);
        badroom_current = asGetNext(badroom);
        bedroom_current = asGetNext(bedroom);
    }
    asDelete(badroom, "king-size bed");
    ASSERT_TEST(asGetSize(badroom)== asGetSize(bedroom)-1);

    return true;
}

//test #5 - clear badroom set from it's contents checking, and than destroy it.
static bool clearAndDestroyBadroomCheck(AmountSet badroom){
    asClear(badroom);
    ASSERT_TEST(asGetSize(badroom)==0);
    ASSERT_TEST(asGetFirst(badroom)==NULL);
    ASSERT_TEST(asContains(badroom, "king-size bed")==false);
    ASSERT_TEST(asChangeAmount(badroom, "queen-size bed", (double)VERY_BIG_NEGATIVE) == AS_ITEM_DOES_NOT_EXIST);
    ASSERT_TEST(asChangeAmount(badroom, asGetNext(badroom), (double)VERY_BIG_NEGATIVE) == AS_NULL_ARGUMENT);
    asDestroy(badroom);

    return true;
}

//test #5 - office set initial insertions
static void insertToOffice(AmountSet office){
    asRegister(office, "WOODEN desk");
    asRegister(office, "PLASTIC desk");
    asRegister(office, "ALUMINUM desk");
    asRegister(office, "office chair with 5 wheels");
    asRegister(office, "office chair with 3 wheels");
    asRegister(office, "static office chair");
    asRegister(office, "Gaming chair - pro");
    asRegister(office, "Gaming chair - comfort");
    asRegister(office, "Gaming chair - kids");
    asRegister(office, "Gaming chair - expert pro super duper max with an espresso machine in right handle");
    asRegister(office, "laptop holder #3986542");
    asRegister(office, "debugging duck");
}

//test #5 - office set amount change check
// in first loop - for odd should be always 0 (reducing same amount as is there),
// for even should work correctly and double the amount!
// in second loop - should be 0 for all the items (delete all the amount!)!
static bool changeAmountInOfficeCheck(AmountSet office,double current_amount, char* current_item, double add_amount){
    add_amount=1;
    current_item=asGetFirst(office);
    for (int i=0; i< asGetSize(office); i++){
        asChangeAmount(office, current_item, add_amount);
        asGetAmount(office, current_item, &current_amount);
        if (i%2 == 0){
            ASSERT_TEST(current_amount==add_amount);
        }
        else {
            ASSERT_TEST(current_amount==0);
        }
        add_amount*=(-1);
        current_item=asGetNext(office);
    }

    current_item=asGetFirst(office);
    for (int i=0; i< asGetSize(office); i++){
        asGetAmount(office, current_item, &add_amount);
        asChangeAmount(office, current_item, -add_amount);
        asGetAmount(office, current_item, &current_amount);
        ASSERT_TEST(current_amount==0);
        current_item=asGetNext(office);
    }

    return true;
}
//test #5 - FULL
bool allAroundTest() {
    AmountSet badroom = asCreate();
    AmountSet office = asCreate();
    insertToBadroom(badroom);
    checkBadroomSort(badroom);
    badroomErrorReturningCheck(badroom);
    double current_amount=random();
    char* current_item=asGetFirst(badroom);
    double add_amount=(double)random();
    badroomAmountCheck(badroom, current_amount, current_item, add_amount);
    AmountSet bedroom= asCopy(badroom);
    copyBadroomToBedroomCheck(badroom, bedroom);
    clearAndDestroyBadroomCheck(badroom);
    insertToOffice(office);
    changeAmountInOfficeCheck(office, current_amount, current_item, add_amount);

    asDestroy(bedroom);
    asDestroy(office);

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//test #6 - FULL
bool testNoMemory() {
    AmountSet set = asCreate();
    char str[] = { [41] = '\1' };
    rand_str(str, sizeof str - 1);
    AmountSetResult register_res=AS_SUCCESS;
    while (register_res!=AS_OUT_OF_MEMORY){
        register_res=asRegister(set, str);
        rand_str(str, sizeof str - 1);
    }

    asDestroy(set);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// I have to mention that the demand of 50 lines maximum for each function also in the test doesn't make any sense to
// me. It just made me separate my main test to mini-functions, and, in my opinion, makes it harder to read and follow.
// But, We sure want every bonus point we can earn, so there you have it.

//created by Noam Marco & Dolev Gefen.
