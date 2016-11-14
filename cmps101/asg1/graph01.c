// Isaiah Solomon
// icsolomo

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "intList.h"

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
   IntList* adjVertices;
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
   adjVertices = calloc(n + 1, sizeof (struct IntListNode*));
   for (int i = 1; i <= n; i++)
      adjVertices[i] = intNil;
   while (fgets(buffer, sizeof (buffer), the_file))
   {
      sscanf(buffer, "%d %d", &from, &to);
      if (from < n + 1 && from > 0)
      {
         m++;
         adjVertices[from] = intCons(to, adjVertices[from]);
      }
   }
   if (in == false)
      fclose(the_file);

   // Print Array
   printf("n = %d\nm = %d\n", n, m);
   for (int i = 1; i <= n; i++)
   {
      printf("%d\t", i);
      if (adjVertices[i] == intNil)
      {
         printf("null\n");
         continue;
      }
      for (bool first = true; adjVertices[i] != intNil; adjVertices[i] = intRest(adjVertices[i]))
      {
         if (first == true)
         {
            first = false;
            printf("[%d", intFirst(adjVertices[i]));
         }
         else
            printf(", %d", intFirst(adjVertices[i]));
      }
      printf("]\n");
   }
   free(adjVertices);
}
