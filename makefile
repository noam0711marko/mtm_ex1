CC = gcc
MATAMIKYA_OBJS = matamikya.o matamikya_orders.o matamikya_orders_set.o matamikya_print.o matamikya_product_in_order.o matamikya_products.o matamikya_products_set.o matamikya_validation_check.o tests/matamikya_tests.o tests/matamikya_main.o
AMOUNT_SET_STR_OBJS = amount_set_str.o amount_set_str_node.o amount_set_str_tests.o amount_set_str_main.o
EXEC1 = matamikya
EXEC2 = amount_set_str
OBJ1=matamikya.o
OBJ2=amount_set_str.o
DEBUG_FLAG = # now empty, assign -g for debug
CFLAGS=-std=c99 -Wall -pedantic-errors -Werror -DNDEBUG $(DEBUG)
MATAMIKYA_FLAGS=  -L. -lmtm -las -lm

$(EXEC1) : $(OBJ1)
	$(CC) $(CFLAGS) $(MATAMIKYA_OBJS) $(MATAMIKYA_FLAGS) -o $@
$(EXEC2) : $(OBJ2)
	$(CC) $(CFLAGS) $(AMOUNT_SET_STR_OBJS) -o $@

matamikya.o: matamikya.c matamikya.h matamikya_orders_set.h matamikya_orders_set.h 
	$(CC) -c $(DEBUG_FLAG) $(CFLAGS) $(MATAMIKYA_FLAGS) $*.c
matamikya_orders_set.o: matamikya_orders_set.c matamikya_orders_set.h matamikya_orders.h
	$(CC) -c $(DEBUG_FLAG) $(CFLAGS) $(MATAMIKYA_FLAGS) $*.c
matamikya_products_set.o: matamikya_products_set.c matamikya_products_set.h set.h matamikya_products.h matamikya_validation_check.h
	$(CC) -c $(DEBUG_FLAG) $(CFLAGS) $(MATAMIKYA_FLAGS) $*.c
matamikya_products.o: matamikya_products.c matamikya_products.h matamikya.h matamikya_print.h
	$(CC) -c $(DEBUG_FLAG) $(CFLAGS) $(MATAMIKYA_FLAGS) $*.c
matamikya_orders.o : matamikya_orders.c matamikya_orders.h matamikya_products_set.h matamikya_products.h matamikya_product_in_order.h
	$(CC) -c $(DEBUG_FLAG) $(CFLAGS) $(MATAMIKYA_FLAGS) $*.c
matamikya_product_in_order.o: matamikya_product_in_order.c matamikya_product_in_order.h matamikya.h 
	$(CC) -c $(DEBUG_FLAG) $(CFLAGS) $(MATAMIKYA_FLAGS) $*.c
matamikya_validation_check.o: matamikya_validation_check.c matamikya_validation_check.h matamikya.h
	$(CC) -c $(DEBUG_FLAG) $(CFLAGS) $(MATAMIKYA_FLAGS) $*.c
matamikya_tests.o: tests/matamikya_tests.c tests/matamikya_tests.h matamikya.h tests/test_utilities.h
	$(CC) -c $(DEBUG_FLAG) $(CFLAGS) $(MATAMIKYA_FLAGS) tests/$*.c -o $*.o
matamikya_main.o: tests/matamikya_main.c tests/matamikya_tests.h tests/test_utilities.h
	$(CC) -c $(DEBUG_FLAG) $(CFLAGS) $(MATAMIKYA_FLAGS) tests/$*.c -o $*.o

amount_set_str.o: amount_set_str.c amount_set_str.h amount_set_str_node.h
	$(CC) -c $(DEBUG_FLAG) $(CFLAGS) $*.c
amount_set_str_node.o: amount_set_str_node.c amount_set_str_node.h
	$(CC) -c $(DEBUG_FLAG) $(CFLAGS) $*.c
amount_set_str_tests.o: amount_set_str_tests.c amount_set_str_tests.h set.h amount_set_str.h amount_set_str_test_utilities.h
	$(CC) -c $(DEBUG_FLAG) $(CFLAGS) $*.c
amount_set_str_main.o: amount_set_str_main.c amount_set_str_tests.h amount_set_str_test_utilities.h
	$(CC) -c $(DEBUG_FLAG) $(CFLAGS) $*.c

clean:
	rm -f $(MATAMIKYA_OBJS) $(OBJ1) $(EXEC1) $(AMOUNT_SET_STR_OBJS) $(OBJ2) $(EXEC2)