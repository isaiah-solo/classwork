// Isaiah Solomon
// icsolomo

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "edgeList.h"
#include "loadWgtGraph.h"
#include "minPQ.h"

EdgeList temp = NULL;
const EdgeList edgeNil;

EdgeInfo edgeFirst(EdgeList oldL)
{
   return oldL->value;
}

EdgeList edgeRest(EdgeList oldL)
{
   temp = oldL;
   oldL = oldL->next;
   free(temp);
   return oldL;
}

EdgeList edgeCons(EdgeInfo newE, EdgeList oldL)
{
   temp = malloc(sizeof (struct EdgeListNode));
   temp->value = newE;
   temp->next = oldL;
   return temp;
}
