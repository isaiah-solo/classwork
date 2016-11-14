// Isaiah Solomon
// icsolomo

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "edgeList.h"
#include "loadWgtGraph.h"
#include "minPQ.h"



void updateFringeP (MinPQ pq, EdgeList* adj_list, int v)
{
   EdgeList remAdj;
   remAdj = adj_list[v];
   while (remAdj != edgeNil)
   {
      EdgeInfo wInfo = edgeFirst(remAdj);
      int w = wInfo.to;
      double newWgt = wInfo.wgt;
      if (pq->status[w] == UNSEEN)
         insertPQ(pq, w, newWgt, v);
      else if (pq->status[w] == FRINGE)
      {
         if (newWgt < getPriority(pq, w))
            decreaseKey(pq, w, newWgt, v);
      }
      remAdj = remAdj->next;
   }
}

void updateFringeD (MinPQ pq, EdgeList* adj_list, int v)
{
   double myDist = pq->priority[v];
   EdgeList remAdj;
   remAdj = adj_list[v];
   while (remAdj != edgeNil)
   {
      EdgeInfo wInfo = edgeFirst(remAdj);
      int w = wInfo.to;
      double newDist = myDist + wInfo.wgt;
      if (pq->status[w] == UNSEEN)
         insertPQ(pq, w, newDist, v);
      else if (pq->status[w] == FRINGE)
      {
         if (newDist < getPriority(pq, w))
            decreaseKey(pq, w, newDist, v);
      }
      remAdj = remAdj->next;
   }
}

void printOutput (MinPQ pq, int n)
{
   // table
   printf("vertex |status |parent |priority\n");
   printf("-------|-------|-------|----------------\n");
   char* stat;
   for (int i = 0; i < n; i++)
   {
      if (pq->status[i] == UNSEEN) stat = "unseen";
      if (pq->status[i] == FRINGE) stat = "fringe";
      if (pq->status[i] == INTREE) stat = "intree";
      printf("%7d|%7s|%7d|%16.3f\n", i + 1, stat, pq->parent[i], pq->priority[i]);
   }
   printf("\n");
}

void greedyTree (EdgeList* adj_list, int n, int s, int* status, int* parent, double* priority)
{
   MinPQ pq = createPQ(n, status, priority, parent);
   insertPQ(pq, s, 0, -1);
   while (isEmptyPQ(pq) == false)
   {
      int v = getMin(pq);
      delMin(pq);
      if (s == 1)
         updateFringeP(pq, adj_list, v);
      else
         updateFringeD(pq, adj_list, v);
   }
   printOutput(pq, n);
}



int main (int argc, char** argv)
{
   if (argc < 4)
   {
      printf("Usage: graph04 -(P or D) starting.vertex input.data\n");
      return 1;
   }

   /* CREATE ADJACENCY LISTS */
   int n = 0, m = 0, start = atoi(argv[2]), option = 0;
   EdgeList* adj_list;
   adj_list = loadEdges(&n, &m, &option, argc, argv);
   int status[n], parent[n];
   double priority[n];
   memset(status, 0, n * sizeof (int));
   memset(parent, 0, n * sizeof (int));
   memset(priority, 0.0, n * sizeof (double));

   void print_list (adj_list, n, m);
   greedyTree (adj_list, n, start, status, parent, priority);

   /* FREE MEMORY */
   free(adj_list);
}
