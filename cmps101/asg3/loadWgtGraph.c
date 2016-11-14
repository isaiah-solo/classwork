// Isaiah Solomon
// icsolomo

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "edgeList.h"
#include "loadWgtGraph.h"
#include "minPQ.h"

EdgeList* initEdges (int n)
{
   EdgeList* adj_list = calloc(n + 1, sizeof (struct EdgeListNode*));
   for (int i = 1; i <= n; i++)
      adj_list[i] = edgeNil;
   return adj_list;
}

EdgeList* loadEdges (int* n, int* m, int* o, int argc, char** argv)
{
   char buffer [100];
   bool in = false;
   int to = 0, from = 0;
   double weight= 0.0;
   FILE* the_file;
   if (strcmp(argv[1], "-P") == 0)
      *o = 1;
   if (strcmp(argv[3], "-") == 0)
   {
      the_file = stdin;
      in = true;
   }
   else
      the_file = fopen(argv[3], "r");
   fgets(buffer, sizeof (buffer), the_file);
   sscanf(buffer, "%d", n);
   EdgeList* adj_list = initEdges(*n);
   for (;fgets(buffer, sizeof (buffer), the_file);)
   {
      sscanf(buffer, "%d %d %lf", &from, &to, &weight);
      if (from < *n + 1 && from > 0 && to < *n + 1 && to > 0)
      {
         (*m)++;
         adj_list[from] = edgeCons(to, adj_list[from]);
         adj_list[from]->value.wgt = weight;
         if (*o == 1)
         {
            adj_list[to] = edgeCons(from, adj_list[to]);
            adj_list[to]->value.wgt = weight;
         }
      }
   }
   if (in == false)
      fclose(the_file);
   adj_list = fix_graph(adj_list, *n);
   return adj_list;
}

EdgeList* fix_graph (EdgeList* orig_graph, int n)
{
   EdgeList* new_graph = calloc(n + 1, sizeof (struct EdgeListNode));
   for (int i = 1; i <= n; i++)
      new_graph[i] = edgeNil;
   orig_graph = transposeGraph(transposeGraph(orig_graph, n), n);
   for (int i = 1; i <= n; i++)
   {
      for (EdgeList ptr = orig_graph[i]; ptr != edgeNil; ptr = edgeRest(ptr))
      {
         if (ptr->value.to <= n)
            new_graph[i] = edgeCons(edgeFirst(ptr).to, new_graph[i]);
      }
   }
   free(orig_graph);
   return new_graph;
}

EdgeList* transposeGraph(EdgeList* orig_graph, int n)
{
   EdgeList* new_graph = calloc(n + 1, sizeof (struct EdgeListNode));
   for (int i = 1; i <= n; i++)
      new_graph[i] = edgeNil;
   for (int i = 1; i <= n; i++)
   {
      for (EdgeList ptr = orig_graph[i]; ptr != edgeNil; ptr = ptr->next)
      {
         if (ptr->value.to <= n)
            new_graph[edgeFirst(ptr).to] = edgeCons(i, new_graph[edgeFirst(ptr).to]);
      }
   }
   return new_graph;
}

void print_list (IntList* adj_list, int n, int m)
{
   printf("n = %d\nm = %d\n", n, m);
   for (int i = 1; i <= n; i++)
   {
      printf("%d\t", i);
      if (adj_list[i] == intNil)
      {
         printf("null\n");
         continue;
      }
      bool first = true;
      for (IntList ptr = adj_list[i]; ptr != intNil; ptr = ptr->next)
      {
         if (first == true)
         {
            first = false;
            printf("[%d", intFirst(ptr));
         }
         else
            printf(", %d", intFirst(ptr));
      }
      printf("]\n");
   }
}
