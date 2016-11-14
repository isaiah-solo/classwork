#ifndef STACK_H
#define STACK_H

// Include Common.h so we can use the return values and boolean type declared therein.
#include "Common.h"

// Keep a fixed stack size as a constant. This variable should be used whenever possible, such as
// finding the last element of the stack should do `stack.stackItems[STACK_SIZE - 1]` instead of
// `stack.stackItems[19]`.
#define STACK_SIZE 20

/**
 * Here we declare the stack struct. While a stack works as a FILO (first-in,
 * last-out) we will be implementing this on top of a C array. The way this
 * will work is that the bottom of the stack will be array element 0. The top
 * of the stack will be the last element in the array. The index of that last
 * element (the top of the stack) will be stored in currentItemIndex.
 *
 * So:
 *  - stackItems: Contains all the floats that are in the queue in ascending
 *                order.
 *  - currentItemIndex: Contains the index in stackItems of the top of the
 *                      stack.
 *  - intialized: Stores whether or not the array was initialized. false if
 *                it isn't and true if it is.
 */
struct Stack {
	float stackItems[STACK_SIZE];
	int currentItemIndex;
	bool initialized;
};

/**
 * This function initializes the stack. For a statically allocated stack such
 * as this, this just means initializing currentItemIndex to -1 and setting
 * initialized to true.
 */
void StackInit(struct Stack *stack);

/**
 * This function pushes a value onto the stack. It returns SUCCESS for success
 * and STANDARD_ERROR for a failure. A failure occurs when trying to push onto full or
 * non-initialized stacks. (SUCCESS and STANDARD_ERROR are declared in the Common.h header file.
 */
int StackPush(struct Stack *stack, float value);

/**
 * This function returns the top of the stack into the value point location.
 * It also returns a success code: SUCCESS for success and STANDARD_ERROR for failure.
 * Failure occurs when Pop() is called on empty stacks or non-initialized
 * stacks.
 *
 * NOTE: When trying to return the output into the value variable, you'll need
 * to write it with an extra asterisk like:
 *   *value = 7.0;
 * This is because of pointers and K&R 5.1-5.2 should explain it. This is the
 * only time in your lab where you'll need to do something like that for an
 * assignment operation.
 */
int StackPop(struct Stack *stack, float *value);

/**
 * This function checks for whether the stack is empty or not. In this simple
 * case this merely checks if the currentItemIndex == -1. Returns true if the
 * stack is empty, false if not. Also needs to check if the stack is
 * initialized and return false if it isn't.
 */
int StackIsEmpty(const struct Stack *stack);

/**
 * This function checks for whether the stack is empty or not. For this simple
 * implementation that is merely if the currentItemIndex == STACK_SIZE - 1. 
 * Returns true if the stack is full and false if not. Also returns false if
 * the stack isn't initialized.
 */
int StackIsFull(const struct Stack *stack);

/**
 * Returns the current size of the stack in terms of how many active elements
 * are in it. Returns an error of SIZE_ERROR for uninitialized stacks, 0 for
 * empty and initialized stacks, and the number of elements in the stack
 * otherwise. SIZE_ERROR is declared in the Common.h header file.
 */
int StackGetSize(const struct Stack *stack);

#endif // STACK_H
