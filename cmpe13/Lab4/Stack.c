#include <stdio.h>
#include <math.h>
#include <plib.h>
#include <stdbool.h>

#include "Stack.h"
#include "Common.h"

/************************
 * Stack Initialization *
 ************************/
void StackInit(struct Stack *stack){
    if(stack->initialized != true){
        stack->initialized = true;
        stack->currentItemIndex = -1;
    }
}

/**************
 * Stack Push *
 **************/
int StackPush(struct Stack *stack, float value){
    if(stack->initialized == true){
        if(StackIsFull(stack) == false){
            ++(stack->currentItemIndex);
            stack->stackItems[stack->currentItemIndex] = value;
            return true;
        }else{
            printf("ERROR: No more room on stack.\n");
            return false;
        }
    }else{
        return false;
    }
}

/*************
 * Stack Pop *
 *************/
int StackPop(struct Stack *stack, float *value){
    if(stack->initialized == true){
        if(StackIsEmpty(stack) == false){
            *value = stack->stackItems[stack->currentItemIndex];
            stack->stackItems[stack->currentItemIndex] = 0;
            --(stack->currentItemIndex);
            return true;
        }else{
            printf("ERROR: Not enough operands before operator.\n");
            return false;
        }
    }else{
        return false;
    }
}

/******************
 * Stack Is Empty *
 ******************/
int StackIsEmpty(const struct Stack *stack){
    if(stack->initialized == true){
        if(stack->currentItemIndex == -1){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

/*****************
 * Stack Is Full *
 *****************/
int StackIsFull(const struct Stack *stack){
    if(stack->initialized == true){
        if(stack->currentItemIndex == (STACK_SIZE - 1)){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

/*******************
 * Stack Gets Size *
 *******************/
int StackGetSize(const struct Stack *stack){
    if(stack->currentItemIndex == -1){
        return -1;
    }else{
        return 0;
    }
}