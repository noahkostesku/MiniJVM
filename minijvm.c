
//Code Written by Noah Kosteslu

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include "minijvm.h"
#include "stack.h"

//Function for pushsing 0 on the operand stack
void exec_iconst_0(minijvm *jvm)
{
    stack_push(jvm->operands, 0);
}

//Function that pops the top item off of the stack
void exec_pop(minijvm *jvm) 
{
    item_type popped_val;
    stack_pop(jvm->operands, &popped_val);
}

//Function that duplicates the top item of the stack
void exec_dup(minijvm *jvm) 
{
    item_type top;
    stack_peek(jvm->operands, &top);
    stack_push(jvm->operands, top);
}

//Function that adds the top two items on the stack
void exec_iadd(minijvm *jvm) 
{
    item_type a, b;
    stack_pop(jvm->operands, &a);
    stack_pop(jvm->operands, &b);
    stack_push(jvm->operands, a + b);
}

//Function that gets the difference from the second item from the top minus the top item
void exec_isub(minijvm *jvm) 
{
    item_type a, b;
    stack_pop(jvm->operands, &a);
    stack_pop(jvm->operands, &b);
    stack_push(jvm->operands, b - a);
}

//Function that gets the product from the top two items in the stack
void exec_imul(minijvm *jvm) 
{
    item_type a, b;
    stack_pop(jvm->operands, &a);
    stack_pop(jvm->operands, &b);
    stack_push(jvm->operands, a * b);
}

//Function that computes the quotient from the top two items in the stack
void exec_idiv(minijvm *jvm) 
{
    item_type a, b;
    stack_pop(jvm->operands, &a);
    stack_pop(jvm->operands, &b);
    if (a != 0) 
    {
        stack_push(jvm->operands, b / a);
    } 
    else 
    {
        fprintf(stderr, "Division by zero\n");
        //set the return value to 1 for errors and terminate the program
        jvm->return_value = 1; 
        exit(1);
    }
}

//Function that computes the remainder of the top two items in the stack
void exec_irem(minijvm *jvm) 
{
    item_type a, b;
    stack_pop(jvm->operands, &a);
    stack_pop(jvm->operands, &b);
    if (a != 0) 
    {
        stack_push(jvm->operands, b % a);
    }
    else 
    {
        fprintf(stderr, "Division by zero\n");
        jvm->return_value = 1;
        exit(1);
    }
}

//Function that perform s an arithmetic shift to the right
void exec_ishr(minijvm *jvm) 
{
    item_type a, b;
    stack_pop(jvm->operands, &a);
    stack_pop(jvm->operands, &b);
    stack_push(jvm->operands, b >> a);
}

//Function that exits the program
void exec_return(minijvm *jvm) 
{
    jvm->return_value = 0; 
}

//Function that prints the top item from the stack
void exec_iprint(minijvm *jvm) 
{
    item_type top;
    stack_peek(jvm->operands, &top);
    printf("%d\n", top);
}

//Function that pushes a byte onto the stack
void exec_bipush(minijvm *jvm, char operand) 
{
    stack_push(jvm->operands, operand);
}

//Function that pushes a local variable at index 'n' onto the stack
void exec_iload(minijvm *jvm, int n) 
{
    // Check if the index 'n' is within the valid range of local variables
    //sizeof(jvm->locals) calculates the total size of the array jvm->locals in bytes.
    //sizeof(jvm->locals[0]) calculates the size of one element of the array
    if (n >= 0 && (size_t)n < sizeof(jvm->locals) / sizeof(jvm->locals[0])) {
        stack_push(jvm->operands, jvm->locals[n]); 
    } 
    else 
    {
        fprintf(stderr, "Invalid index %d\n", n);
        jvm->return_value = 1;
    }
}

//Function that pops the top item of the stack and stores it in a local variable
void exec_istore(minijvm *jvm, int n) 
{
    if (n >= 0 && (size_t)n < sizeof(jvm->locals) / sizeof(jvm->locals[0])) 
    {
        item_type val;
        stack_pop(jvm->operands, &val);
        jvm->locals[n] = val; 
    } 
    else 
    {
        fprintf(stderr, "Invalid index %d\n", n);
        jvm->return_value = 1; 
    }
}

//Function that increments local variable 'n' by 'd'
void exec_iinc(minijvm *jvm, int n, int d) 
{
    if (n >= 0 && (size_t)n < sizeof(jvm->locals) / sizeof(jvm->locals[0]))
    {
        jvm->locals[n] += d;
    } 
    else 
    {
        fprintf(stderr, "Invalid index %d\n", n);
        jvm->return_value = 1;
    }
}

//Function that pops the top item off the stack and branches to offset if it is equal to zero 
void exec_ifeq(minijvm *jvm, signed char offset) 
{

    item_type val;
    stack_pop(jvm->operands, &val);  
    // Load offset from bytecode
    //branch to offset if the value is zero
    if (val == 0) 
    {
        jvm->pc += offset; 
    }
    else 
    {
        // Skip over the offset
        jvm->pc += 3; 
    }
}

//Function that branches to offset
 void exec_goto(minijvm *jvm, signed char offset) 
{
    jvm->pc += offset;
}

//Function that reads bytes from a specified file into an array
char* jvm_read(const char* filename) 
{
    char full_filename[FILENAME_MAX];

    if (strstr(filename, ".mclass") != NULL) 
    {
        snprintf(full_filename, sizeof(full_filename), "%s", filename);
    } 
    else 
    {
        snprintf(full_filename, sizeof(full_filename), "%s.mclass", filename);
    }

    FILE* file = fopen(full_filename, "rb");
    if (file == NULL) 
    {
        fprintf(stderr, "File '%s' not found\n", full_filename);
        exit(1);  
    }

    char* bytecode = (char*)malloc(MAX_CLASS_SIZE);

    if (bytecode == NULL) 
    {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);  
        exit(1);  
    }

    //After executing this line, bytecode will contain the bytecode read from the file, and bytes_read will indicate the number of bytes successfully read from the file. 
    size_t bytes_read = fread(bytecode, 1, MAX_CLASS_SIZE, file);
    if (bytes_read == 0) 
    {
        fprintf(stderr, "Failed to read file '%s'\n", full_filename);
        free(bytecode);  
        fclose(file);     
        exit(1);          
    }

    fclose(file);  
    return bytecode;
}

//Function that initializes a new minijvm struct
minijvm* jvm_init(const char* filename) 
{

    minijvm* jvm = (minijvm*)malloc(sizeof(minijvm));
    if (jvm == NULL) 
    {
        fprintf(stderr, "Failed to allocate memory for MiniJVM\n");
        exit(1);
    }
    jvm->bytecode = NULL;
    jvm->bytecode = jvm_read(filename);

    if (jvm->bytecode == NULL) 
    {
        fprintf(stderr, "Failed to read bytecode from file\n");
        free(jvm); 
        exit(1);
    }

    //// Set the program counter (pc) of the MiniJVM to point to the bytecode
    jvm->pc = jvm->bytecode;
    jvm->operands = stack_create();

    if (jvm->operands == NULL) 
    {
        fprintf(stderr, "Failed to create operand stack\n");
        free(jvm->bytecode); 
        free(jvm); 
        exit(1);
    }
    //initialize the locals array in the MiniJVM by setting all elements to 0
    memset(jvm->locals, 0, sizeof(jvm->locals));

    //set the jvm return value to 0 upon successful termination
    jvm->return_value = 0;
    return jvm;
}

void jvm_free(minijvm* jvm) 
{
    if (jvm != NULL) 
    {
        //and if the memory allocated for the bytecode block was successful...
        if (jvm->bytecode != NULL) 
        {
            free(jvm->bytecode);
        }
        //if the memory allocated for the operand stack was properly allocated...
        if (jvm->operands != NULL) 
        {
            stack_free(jvm->operands);
        }
        free(jvm);
    }
}

//Function that runs the program loaded into the miniJVM
void jvm_run(minijvm* jvm) 
{
    while (1) 
    { 
        //initialize a unsigned char variable called bytecode that gets the next bytecode pointed to by the program counter
        unsigned char bytecode = *(jvm->pc);
        signed char offset;
        switch (bytecode) 
        {
            case INST_ICONST0:
                exec_iconst_0(jvm);
                break;
            case INST_POP:
                exec_pop(jvm);
                break;
            case INST_DUP:
                exec_dup(jvm);
                break;
            case INST_IADD:
                exec_iadd(jvm);
                break;
            case INST_ISUB:
                exec_isub(jvm);
                break;
            case INST_IMUL:
                exec_imul(jvm);
                break;
            case INST_IDIV:
                exec_idiv(jvm);
                break;
            case INST_IREM:
                exec_irem(jvm);
                break;
            case INST_ISHR:
                exec_ishr(jvm);
                break;
            case INST_RETURN:
                exec_return(jvm);
                return; 
            case INST_PRINT:
                exec_iprint(jvm);
                break;
            case INST_BIPUSH:
                //*(jvm->pc + 1): This expression accesses the value of the next byte after the INST_BIPUSH instruction in memory
                exec_bipush(jvm, *(jvm->pc + 1));
                jvm->pc++; 
                break;
            case INST_ILOAD:
                exec_iload(jvm, *(jvm->pc + 1));
                jvm->pc++; 
                break;
            case INST_ISTORE:
                exec_istore(jvm, *(jvm->pc + 1));
                jvm->pc++; 
                break;
            case INST_IINC:
                //exec_iinc(jvm, *(jvm->pc + 1), *(jvm->pc + 2)); is passing the minijvm object jvm, 
                exec_iinc(jvm, *(jvm->pc + 1), *(jvm->pc + 2));
                jvm->pc += 2; 
                break;
            // case INST_IFEQ:
            //     // offset = (jvm->pc[1] << 8) | jvm->pc[2];
            //     offset = (signed char)jvm->pc[1];
            //     exec_ifeq(jvm, offset); 
            //     jvm->pc += 2; 
            //     break;
            // case INST_GOTO:
            //     //bitwise operation to convert to integer
            //     offset = (jvm->pc[1] << 8) | jvm->pc[2];
            //     exec_goto(jvm, offset);  
            //     break;
            default:
                //default case: unknown, set the error value to 1 as per instructions and terminate the program
                jvm->return_value = 1; 
                return; 
        }
        jvm->pc++;
    }
}

//Function that prints the program's usage information to standard error
void jvm_usage(const char* name_of_file) 
{
    fprintf(stderr, "Usage: mjvm %s\n", name_of_file);
}

int main(int argc, char** argv) 
{
    if (argc != 2) {
        //call the usage function and print its contents to the user where argv[0] is the name of the program
        jvm_usage(argv[0]);
        return 1; 
    }

    //initialize a minijvm pointer variable called jvm that initializes the miniJVM with the filename inputted by the user
    minijvm* jvm = jvm_init(argv[1]);
    if (jvm == NULL) 
    {
        fprintf(stderr, "Error initializing MiniJVM\n");
        return 1; 
    }

    jvm_run(jvm);
    int return_val = jvm->return_value; 
    jvm_free(jvm);
    return return_val;
}