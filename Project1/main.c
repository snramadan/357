#include "mastermind.h"
#include "inputs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main(int argc, char *argv[])
{
   int s, p, n, temp;
   char l;
   Mastermind m; 

   if (argc > 2 || ((argc == 2) && ((strcmp(argv[1], "-show") != 0))))
   {
      fprintf(stderr, "Usage: mastermind [-show]\n");
      exit(EXIT_FAILURE);
   }

   if ((argc == 2) && ((strcmp(argv[1], "-show") == 0)))
   {
      temp = 1;
   }

   s = getSeed();
   l = getMaxLetter();
   p = getPositions();
   n = getNumGuesses();
   m = makeMaster(s, l, p, n);
   
   if (temp == 1)
   {
      printf("Initialized Game Board: ");
      for (temp = 0; temp < m.pos; temp++)
      {
         printf("%c", m.word[temp]);
      }
      printf("\n");
   }
   guesses(m);

   return EXIT_SUCCESS;
}
