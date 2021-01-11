#include "inputs.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

int getSeed()
{
   int seed, x; 
   char done = 'n'; 

   while (done == 'n')
   {
      printf("Enter the seed for random number generation: ");
      x = scanf("%d", &seed);
      if (x != 1)
      {
         printf("Seed must be an integer value, please try again\n");
         while ((getchar()) != '\n');
      }
      else
      {
         done = 'd';
      }
   }

   return seed;
}

char getMaxLetter()
{
   char ml;
   char done = 'n';
   int x;

   while (done == 'n')
   {
      printf("Enter the maximum letter for the game (A-Z): ");
      x = scanf(" %c", &ml); 
      if ((x != 1) || (isupper(ml) == 0))
      {
         printf("The letter must be an uppercase A-Z, please try again\n");
         while ((getchar()) != '\n');
      }
      else
      {
         done = 'd';
      }
   }

   return ml;
}

int getPositions()
{
   int pos, x;
   char done = 'n';

   while (done == 'n')
   {
      printf("Enter the number of positions for the game (1-8): ");
      x = scanf(" %d", &pos); 
      if (x != 1 || pos < 1 || pos > 8)
      {
         printf("The number of positions must be 1-8, please try again\n");
         while ((getchar()) != '\n');
      }
      else
      {
         done = 'd';
      }
   }

   return pos;
}

int getNumGuesses()
{
   int ng, x;
   char done = 'n';

   while (done == 'n')
   {
      printf("Enter the number of guesses allowed for the game: ");
      x = scanf("%d", &ng);
      if (x != 1 || ng < 1)
      {
         printf("The number of guesses must be a positive integer, ");
         printf("please try again\n");
         while ((getchar()) != '\n');
      }
      else
      {
         done = 'd';
      }
   }

   return ng;
}
