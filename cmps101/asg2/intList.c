// Isaiah Solomon
// icsolomo

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "intList.h"

IntList* transposeGraph(IntList* origGraph, int n);
int hasCycle(IntList* origGraph, int n);
int hasCycleLen(IntList* origGraph, int n, int sofar, int v);

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

IntList* transposeGraph(IntList* origGraph, int n)
{
   IntList* newGraph = calloc(n + 1, sizeof (struct IntListNode));
   for (int i = 1; i <= n; i++)
      newGraph[i] = intNil;
   for (int i = 1; i <= n; i++)
   {
      for (IntList ptr = origGraph[i]; ptr != intNil; ptr = ptr->next)
      {
         if (ptr->value <= n)
            newGraph[intFirst(ptr)] = intCons(i, newGraph[intFirst(ptr)]);
      }
   }
   return newGraph;
}

int hasCycle(IntList* origGraph, int n)
{
   for (int i = 1; i <= n; i++)
   {
      if (hasCycleLen(origGraph, n, 0, i) == 1)
      {
         printf("\n");
         return 1;
      }
   }
   return 0;
}

int hasCycleLen(IntList* origGraph, int n, int sofar, int v)
{
   if (sofar >= n)
   {
      printf("%d ", v);
      return 1;
   }
   for (IntList ptr = origGraph[v]; ptr != intNil; ptr = ptr->next)
   {
      if (hasCycleLen(origGraph, n, sofar + 1, intFirst(ptr)) == 1)
      {
         printf("%d ", v);
         return 1;
      }
   }
   return 0;
}
