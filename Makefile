#Author: Noah Kostesku

#Detect the operating system
UNAME_S := $(shell uname -s)

# Set the library name based on the operating system for macOS
ifeq ($(UNAME_S),Darwin)
    LIBADT = adt-mac
else ifeq ($(UNAME_S),Linux)
    LIBADT = adt-linux
else
    $(error Unsupported OS)
endif

#macro for the gcc compiler
CC = gcc
#Extra flags for the C Compiler
# -Wall: Enable all warnings
# -Wextra: Enable extra warnings
# -g: Produce debugging information
CFLAGS = -Wall -Wextra -g
INCLUDE_DIR = include
LIB_DIR = lib
MAIN_FILE = minijvm.c
#Object files for which .c is replaced with .o
OBJS = $(MAIN_FILE:.c=.o)
EXEC = mjava
TEST_DIR = testfiles
#All of the test files
TEST_CASES = test1.mclass test2.mclass test3.mclass test4.mclass test5.mclass test6.mclass test7.mclass test8.mclass

#default target 'all' which depends on mjava executable
all: $(EXEC)

#Explains the blueprint of how to build the executable
#mjava target depends on the object files

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) -L $(LIB_DIR) $^ -l$(LIBADT) -o $@

#Converts a .c file into its corresponding .o file which depends on the minijvm.h header file
%.o: %.c $(INCLUDE_DIR)/minijvm.h
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) -c $< -o $@

#Run a for loop on the macro for test cases so all of the test cases can be run at once
test: $(EXEC)
	@for i in $(TEST_CASES); do \
    	echo "./$(EXEC) $(TEST_DIR)/$${i%%.*}"; \
    	./$(EXEC) $(TEST_DIR)/$${i%%.*} || echo "make: [Makefile:63: $${i%%.*}] Error 1 (ignored)"; \
	done

#Run ./mjava testfiles/(specific test) for every test to display the output of the respective test file
test1: $(EXEC)
	./$(EXEC) $(TEST_DIR)/test1

test2: $(EXEC)
	./$(EXEC) $(TEST_DIR)/test2

test3: $(EXEC)
	./$(EXEC) $(TEST_DIR)/test3

test4: $(EXEC)
	./$(EXEC) $(TEST_DIR)/test4

test5: $(EXEC)
	./$(EXEC) $(TEST_DIR)/test5

test6: $(EXEC)
	./$(EXEC) $(TEST_DIR)/test6

test7: $(EXEC)
	./$(EXEC) $(TEST_DIR)/test7

test8: $(EXEC)
	./$(EXEC) $(TEST_DIR)/test8

clean:
	rm -f *.o $(EXEC)
