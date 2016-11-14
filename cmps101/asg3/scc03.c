// Isaiah Solomon
// icsolomo

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "intList.h"
#include "dfsTrace1.h"
#include "dfsPhase2.h"
#include "loadGraph.h"


int main (int argc, char** argv)
{
   if (argc == 1)
   {
      printf("Usage: graph01 input.data\n");
      return 0;
   }

   /* CREATE ADJACENCY LISTS */
   int n = 0, m = 0, result;
   IntList* adj_list; IntList* transpose_list;
   adj_list = loadGraph(&n, &m, argc, argv);
   transpose_list = fix_graph(transposeGraph(adj_list, n), n);

   int discoverTime1[n], finishTime1[n], finishStk[n], parent1[n], counter = 1, stackspot = n;
   memset(discoverTime1, 0, n * sizeof(int));
   memset(finishTime1, 0, n * sizeof(int));
   memset(finishStk, 0, n * sizeof(int));
   memset(parent1, -1, n * sizeof(int));

   int discoverTime2[n], finishTime2[n], dfstRoot[n], parent2[n];
   memset(discoverTime2, 0, n * sizeof(int));
   memset(finishTime2, 0, n * sizeof(int));
   memset(parent2, -1, n * sizeof(int));
   memset(dfstRoot, 0, n * sizeof(int));

   for (int i = 1; i <= n; i++)
      result = dfsTrace1(adj_list, discoverTime1, finishTime1, finishStk, parent1, &counter, i, &stackspot);
   counter = 1;
   for (int i = 0; i < n; i++)
      result = dfsTrace2(transpose_list, discoverTime2, finishTime2, finishStk, parent2, &counter, finishStk[i], dfstRoot, finishStk[i]);
   result = result;


   /* ADJACENCY LIST */
   printf("\n__Adjacency List__\n");

   // print list
   print_list(adj_list, n, m);

   // display stack
   printf("stack: [");
   for (int i = 0; i < n; i++)
   {
      if (i != 0)
         printf(", ");
      printf("%d", finishStk[i]);
   }
   printf("]\n\n");

   // table
   printf("vertex |dtime |ftime |parent\n");
   printf("-------|------|------|------\n");
   for (int i = 0; i < n; i++)
      printf("%7d|%6d|%6d|%6d\n", i + 1, discoverTime1[i], finishTime1[i], parent1[i]);
   printf("\n\n");


   /* TRANSPOSE LIST */
   printf("__Transpose List__\n");

   // print transpose list
   print_list(transpose_list, n, m);
   printf("\n");

   // table
   printf("vertex |dtime |ftime |parent |root\n");
   printf("-------|------|------|-------|-----\n");
   for (int i = 0; i < n; i++)
      printf("%7d|%6d|%6d|%7d|%5d\n", i + 1, discoverTime2[i], finishTime2[i], parent2[i], dfstRoot[i]);
   printf("\n");


   /* FREE MEMORY */
   for (int i = 1; i <= n; i++)
   {
      for (IntList ptr = adj_list[i]; ptr != intNil; ptr = intRest(ptr));
      for (IntList ptr = transpose_list[i]; ptr != intNil; ptr = intRest(ptr));
   }
   free(adj_list);
   free(transpose_list);
}