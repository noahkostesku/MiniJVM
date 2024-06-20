#include <stdbool.h>

#ifndef STACK_H
#define STACK_H

typedef int item_type;
typedef struct stack* stack;

/**
 * Initializes a new, dynamically-allocated stack.  The caller is responsible
 * for freeing the stack when it is no longer needed by calling stack_free().
 */
stack stack_create();

/**
 * Pushes a new value onto the stack.
 * Parameters:
 *    s       Stack to which the value should be pushed
 *    value   Value to push onto the stack
 */
void stack_push(stack s, item_type value);

/**
 * Retrieves the value at the top of the stack without removing it
 * Parameters:
 *    s       Stack from which to peek
 *    value   Pointer to a variable that will receive the value at the top of the stack
 * Returns:
 *    true    if the stack is not empty and the value was successfully retrieved
 *    false   otherwise
 */
bool stack_peek(stack s, item_type* value);

/**
 * Removes the value at the top of the stack and stores it in the provided variable
 * Parameters:
 *    s       Stack from which to pop
 *    value   Pointer to a variable that will receive the value at the top of the stack
 * Returns:
 *    true    if the stack is not empty and the value was successfully removed
 *    false   otherwise
 */
bool stack_pop(stack s, item_type* value);

/**
 * Returns the number of items in the stack
 * Parameters:
 *   s       Stack for which to determine the size
 * Returns:
 *    The number of items in the stack
 */
int stack_size(stack s);

/**
 * Frees the memory associated with the stack
 * Parameters:
 *    s       Stack to free
 */
void stack_free(stack s);

#endif
