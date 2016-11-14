#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>

#include "LinkedList.h"
#include "Common.h"

/******************
 * Linked List New
 ******************/
ListItem *LinkedListNew(char *data){
    // Makes room for new list
    ListItem *myListItem = malloc(sizeof(ListItem));
    
    // If list is not NULL
    if(myListItem != NULL){
        myListItem->data = data; // Store data in list
        return myListItem;
    
    // If list is NULL    
    }else{
        return NULL;
    }
}

/*********************
 * Linked List Remove
 *********************/
char *LinkedListRemove(ListItem *item){
    // Temporarily stores to-be deleted item 
    char *x = item->data;
    
    // If first item on list
    if(item->previousItem == NULL){
        // Connects NULL to next item
        item->nextItem->previousItem = NULL;
    }
    
    // If last item on list
    else if(item->nextItem == NULL){
        // Connects previous item to NULL
        item->previousItem->nextItem = NULL;
    }
    
    // If in middle of list
    else if(item->previousItem != NULL && item->nextItem != NULL){
        // Connects previous item to next item
        item->previousItem->nextItem = item->nextItem;
        item->nextItem->previousItem = item->previousItem;
    }
    
    // Frees item
    free(item);
    return x;
}

/*******************
 * Linked List Size
 *******************/
int LinkedListSize(ListItem *list){
    // Checks if list is NULL
    if(list == NULL){
        return NULL;
    }
    
    // Points to first item on list
    int y = 0;
    ListItem *x = LinkedListGetFirst(list);
    
    // Until end of list
    while(x->nextItem != NULL){
        y += 1; // Counter
        x = x->nextItem; // Increment
    }
    return y;
}

/************************
 * Linked List Get First
 ************************/
ListItem *LinkedListGetFirst(ListItem *list){
    // Checks if list is NULL
    if(list == NULL){
        return NULL;
    }
    // Decrements till first item on list
    while(list->previousItem != NULL && list->previousItem->previousItem != NULL){
        list = list->previousItem;
    }
    return list;
}

/***************************
 * Linked List Create After
 ***************************/
ListItem *LinkedListCreateAfter(ListItem *item, char *data){
    // Checks if item is NULL
    if(item == NULL){
        return NULL;
    }

    // If at end of list
    ListItem *newItem = malloc(sizeof(ListItem)); // New list item
    if(item->nextItem == NULL){
        newItem->data = data; // stores data in new item
        item->nextItem = newItem; // Connects new item to next item
        newItem->previousItem = item;
    }

    // If not at end of list
    else if(item->nextItem != NULL){
        newItem->data = data;
        ListItem *x = item->nextItem; // Creates temporary pointer to pooint to next item
        item->nextItem = newItem; // Connects item to new item
        newItem->previousItem = item;
        newItem->nextItem = x; // Connects new item to next item
        x->previousItem = newItem;
    }
    return newItem;
}

/************************
 * Linked List Swap Data
 ************************/
int LinkedListSwapData(ListItem *firstItem, ListItem *secondItem){
    // Checks if either pointers are NULL
    if(firstItem == NULL || secondItem == NULL){
        return STANDARD_ERROR;

        // Makes temporary pointer and swaps
    }else{
        char *x = firstItem->data; // Stores first item in temporary pointer
        firstItem->data = secondItem->data; // Stores second item into first item
        secondItem->data = x; // Stores data in temporary pointer into second item
        return SUCCESS;
    }
}

/*******************
 * Linked List Sort
 *******************/
int LinkedListSort(ListItem *list){
    // Checks if list is null
    if(list == NULL){
        return STANDARD_ERROR;
    }

    // Creates temporary pointers
    ListItem *i = LinkedListGetFirst(list), *j; // Pointers
    int filler = 0; // Filler

    // While first pointer is not one before end of list
    while(i != NULL && i->nextItem != NULL){
        j = i->nextItem;

        // while second pointer is not at end of list
        while(j != NULL){
            if(j->data == NULL){ // If j is NULL
                LinkedListSwapData(i,j); // Swap
            }
            else if(i->data == NULL){ // If i is NULL
                filler = 0; // Do nothing
            }
            else if(strlen(i->data) > strlen(j->data)){ // If length of i > length of j
                LinkedListSwapData(i,j); // Swap
            }
            else if(strlen(i->data) == strlen(j->data)){ // If length of i = length of j
                if(strcmp(i->data, j->data) > 0){ // Compare strings
                    LinkedListSwapData(i,j); // Swap
                }
            }
            j = j->nextItem; // Increment
        }
        i = i->nextItem; // Increment
    }
    return SUCCESS;
}

/********************
 * Linked List Print
 ********************/
int LinkedListPrint(ListItem *list){

    // Checks if list is NULL
    if(list == NULL){
        return STANDARD_ERROR;
    }

    // Prints each string in list
    ListItem *x = LinkedListGetFirst(list); // Creates temporary list
    int y = LinkedListSize(x); // Creates Counter
    printf("[");
    while(x != NULL){
        y -= 1; // Counter
        printf("%s, ", x->data); // Print data
        x = x->nextItem; // Increment

        // If last item on list
        if(y == 0){
            printf("%s", x->data);
            x = NULL;
        }
    }
    printf("]\n");
    return SUCCESS;
}