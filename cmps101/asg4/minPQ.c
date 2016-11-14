#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "edgeList.h"
#include "loadWgtGraph.h"
#include "minPQ.h"

int isEmptyPQ(MinPQ pq)
{
   return (pq->numPQ == 0);
}

int getMin(MinPQ pq)
{
   if (pq->minVertex == -1)
   {
      double minWgt = pq->oo;
      for (int i = 1; i <= pq->n; i++)
      {
         if (pq->status[i] == FRINGE)
         {
            if (pq->priority[i] < minWgt)
            {
               pq->minVertex = i;
               minWgt = pq->priority[i];
            }
         }
      }
   }
   return pq->minVertex;
}

int getStatus(MinPQ pq, int id)
{
   return pq->status[id];
}

int getParent(MinPQ pq, int id)
{
   return pq->parent[id];
}

double getPriority(MinPQ pq, int id)
{
   return pq->priority[id];
}

void delMin(MinPQ pq)
{
   int oldMin = getMin(pq);
   pq->status[oldMin] = INTREE;
   pq->minVertex = -1;
   pq->numPQ -= 1;
}

void insertPQ(MinPQ pq, int id, double priority, int par)
{
   pq->parent[id] = par;
   pq->priority[id] = priority;
   pq->status[id] = FRINGE;
   pq->minVertex = -1;
   pq->numPQ += 1;
}

void decreaseKey(MinPQ pq, int id, double priority, int par)
{
   pq->parent[id] = par;
   pq->priority[id] = priority;
   pq->minVertex = -1;
}

MinPQ createPQ(int n, int status[], double priority[], int parent[])
{
   MinPQ pq = calloc (1, sizeof (struct MinPQNode));
   pq->n = n;
   pq->status = status;
   pq->priority = priority;
   pq->parent = parent;
   for (int i = 1; i <= n; i++)
      pq->status[i] = UNSEEN;
   pq->numPQ = 0;
   pq->minVertex = -1;
   pq->oo = INFINITY;
   return pq;
}