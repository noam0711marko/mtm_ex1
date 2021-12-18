# The first line states the minimum CMake version to use with this project
# and is requires by CMake.
cmake_minimum_required(VERSION 2.8.12.2)

# Set <PROJNAME> to be your project name. We set C here as the target language.
project(ex1 C)

# If you want to see "under the hood" of CMake, uncomment this:
# set(CMAKE_VERBOSE_MAKEFILE ON)

# Define variables containing compiler flags for gcc
set(MTM_FLAGS_DEBUG "-std=c99 --pedantic-errors -Wall -Werror")

set(MTM_FLAGS_RELEASE "${MTM_FLAGS_DEBUG} -DNDEBUG")

# Set the flags for gcc
set(CMAKE_C_FLAGS ${MTM_FLAGS_DEBUG})

# Add the names of your source files here
set(MATAMIKYA_C_FILES matamikya.c matamikya_print.c matamikya_validation_check.c matamikya_products.c
        matamikya_products_set.c matamikya_product_in_order.c matamikya_orders.c matamikya_orders_set.c
        tests/matamikya_main.c tests/matamikya_tests.c)

set(AMOUNT_SET_STR_C_FILES amount_set_str.c amount_set_str_node.c amount_set_str_tests.c amount_set_str_main.c)

link_directories(.)
add_executable(amount_set_str ${AMOUNT_SET_STR_C_FILES})
add_executable(matamikya ${MATAMIKYA_C_FILES})
find_library(MTM_LIBRARY libmtm.a libas.a .)
target_link_libraries(matamikya LINK_PUBLIC ${MTM_LIBRARY})
