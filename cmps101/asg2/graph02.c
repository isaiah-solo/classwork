// Isaiah Solomon
// icsolomo

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "intList.h"

void print_list(IntList* adj_list, int n, int m);

int main (int argc, char** argv)
{
   if (argc == 1)
   {
      printf("Usage: graph01 input.data\n");
      return 0;
   }

   // Create Array
   char buffer [100];
   bool in = false;
   int n = 0, m = 0, to = 0, from = 0;
   IntList* adj_list;
   FILE* the_file;
   if (argc == 2 && strcmp(argv[1], "-") == 0)
   {
      the_file = stdin;
      in = true;
   }
   else
      the_file = fopen(argv[1], "r");
   fgets(buffer, sizeof (buffer), the_file);
   sscanf(buffer, "%d", &n);
   adj_list = calloc(n + 1, sizeof (struct IntListNode*));
   for (int i = 1; i <= n; i++)
      adj_list[i] = intNil;
   while (fgets(buffer, sizeof (buffer), the_file))
   {
      sscanf(buffer, "%d %d", &from, &to);
      if (from <= n && from > 0 && to <= n && to > 0)
      {
         m++;
         adj_list[from] = intCons(to, adj_list[from]);
      }
   }
   printf("Original Graph:\n");
   print_list(transposeGraph(transposeGraph(adj_list, n), n), n, m);
   if (in == false)
      fclose(the_file);
   if (hasCycle(adj_list, n) == 1)
      printf("There is a cycle\n\n");
   else
      printf("There is no cycle\n\n");
   
   printf("Transposed Graph:\n");
   print_list(transposeGraph(adj_list, n), n, m);
   if (hasCycle(transposeGraph(adj_list, n), n) == 1)
      printf("There is a cycle\n\n");
   else
      printf("There is no cycle\n\n");
   
   printf("Transposed Transposed Graph:\n");
   print_list(transposeGraph(transposeGraph(adj_list, n), n), n, m);
   if (hasCycle(transposeGraph(transposeGraph(adj_list, n), n), n) == 1)
      printf("There is a cycle\n\n");
   else
      printf("There is no cycle\n\n");
   
   printf("Transposed Transposed Transposed Graph:\n");
   print_list(transposeGraph(transposeGraph(transposeGraph(adj_list, n), n), n), n, m);
   if (hasCycle(transposeGraph(transposeGraph(transposeGraph(adj_list, n), n), n), n) == 1)
      printf("There is a cycle\n\n");
   else
      printf("There is no cycle\n\n");
   free(adj_list);
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
      for (bool first = true; adj_list[i] != intNil; adj_list[i] = intRest(adj_list[i]))
      {
         if (first == true)
         {
            first = false;
            printf("[%d", intFirst(adj_list[i]));
         }
         else
            printf(", %d", intFirst(adj_list[i]));
      }
      printf("]\n");
   }
   free(adj_list);
}
