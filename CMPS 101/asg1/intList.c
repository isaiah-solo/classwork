// Isaiah Solomon
// icsolomo

#include <stdio.h>
#include <stdlib.h>

#include "intList.h"

struct IntListNode
{
   IntList next;
   int value;
};

IntList temp = NULL;
const IntList intNil;

int intFirst(IntList oldL)
{
   return oldL->value;
}

IntList intRest(IntList oldL)
{
   temp = oldL;
   oldL = oldL->next;
   free(temp);
   return oldL;
}

IntList intCons(int newE, IntList oldL)
{
   temp = malloc(sizeof (struct IntListNode));
   temp->value = newE;
   temp->next = oldL;
   return temp;
}