// Isaiah Solomon
// icsolomo

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "intList.h"

int dfsTrace2(IntList* adj_list, int* discoverTime2, int* finishTime2, int* finishStk, int* parent2, int* counter, int v, int* dfstRoot, int root)
{
   if (discoverTime2[v - 1] != 0)
      return 0;
   dfstRoot[v - 1] = root;
   discoverTime2[v - 1] = (*counter)++;
   for (IntList ptr = adj_list[v]; ptr != intNil; ptr = ptr->next)
   {
      if (dfsTrace2(adj_list, discoverTime2, finishTime2, finishStk, parent2, counter, intFirst(ptr), dfstRoot, root))
         parent2[intFirst(ptr) - 1] = v;
   }
   finishTime2[v - 1] = (*counter)++;
   return 1;
}
