// Isaiah Solomon
// icsolomo

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "intList.h"
#include "loadGraph.h"

IntList* loadGraph(int* n, int* m, int argc, char** argv)
{
   char buffer [100];
   bool in = false;
   IntList* adj_list;
   int to = 0, from = 0;
   FILE* the_file;
   if (argc == 2 && strcmp(argv[1], "-") == 0)
   {
      the_file = stdin;
      in = true;
   }
   else
      the_file = fopen(argv[1], "r");
   fgets(buffer, sizeof (buffer), the_file);
   sscanf(buffer, "%d", n);
   adj_list = calloc(*n + 1, sizeof (struct IntListNode*));
   for (int i = 1; i <= *n; i++)
      adj_list[i] = intNil;
   while (fgets(buffer, sizeof (buffer), the_file))
   {
      sscanf(buffer, "%d %d", &from, &to);
      if (from < *n + 1 && from > 0)
      {
         (*m)++;
         adj_list[from] = intCons(to, adj_list[from]);
      }
   }
   if (in == false)
      fclose(the_file);
   adj_list = fix_graph(adj_list, *n);
   return adj_list;
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

IntList* fix_graph (IntList* orig_graph, int n)
{
   IntList* new_graph = calloc(n + 1, sizeof (struct IntListNode));
   for (int i = 1; i <= n; i++)
      new_graph[i] = intNil;
   orig_graph = transposeGraph(transposeGraph(orig_graph, n), n);
   for (int i = 1; i <= n; i++)
   {
      for (IntList ptr = orig_graph[i]; ptr != intNil; ptr = intRest(ptr))
      {
         if (ptr->value <= n)
            new_graph[i] = intCons(intFirst(ptr), new_graph[i]);
      }
   }
   free(orig_graph);
   return new_graph;
}

IntList* transposeGraph(IntList* orig_graph, int n)
{
   IntList* new_graph = calloc(n + 1, sizeof (struct IntListNode));
   for (int i = 1; i <= n; i++)
      new_graph[i] = intNil;
   for (int i = 1; i <= n; i++)
   {
      for (IntList ptr = orig_graph[i]; ptr != intNil; ptr = ptr->next)
      {
         if (ptr->value <= n)
            new_graph[intFirst(ptr)] = intCons(i, new_graph[intFirst(ptr)]);
      }
   }
   return new_graph;
}

int hasCycle(IntList* orig_graph, int n)
{
   for (int i = 1; i <= n; i++)
   {
      if (hasCycleLen(orig_graph, n, 0, i) == 1)
      {
         printf("\n");
         return 1;
      }
   }
   return 0;
}

int hasCycleLen(IntList* orig_graph, int n, int sofar, int v)
{
   if (sofar >= n)
   {
      printf("%d ", v);
      return 1;
   }
   for (IntList ptr = orig_graph[v]; ptr != intNil; ptr = ptr->next)
   {
      if (hasCycleLen(orig_graph, n, sofar + 1, intFirst(ptr)) == 1)
      {
         printf("%d ", v);
         return 1;
      }
   }
   return 0;
}