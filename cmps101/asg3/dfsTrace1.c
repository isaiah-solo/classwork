// Isaiah Solomon
// icsolomo

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "intList.h"

int dfsTrace1(IntList* adj_list, int* discoverTime1, int* finishTime1, int* finishStk, int* parent1, int* counter, int v, int* stackspot)
{
   if (discoverTime1[v - 1] != 0)
      return 0;
   discoverTime1[v - 1] = (*counter)++;
   for (IntList ptr = adj_list[v]; ptr != intNil; ptr = ptr->next)
   {
      if (dfsTrace1(adj_list, discoverTime1, finishTime1, finishStk, parent1, counter, intFirst(ptr), stackspot))
   	     parent1[intFirst(ptr) - 1] = v;
   }
   finishTime1[v - 1] = (*counter)++;
   finishStk[--(*stackspot)] = v;
   return 1;
}