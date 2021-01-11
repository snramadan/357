#include "mastermind.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

Mastermind makeMaster(int s, char l, int p, int nG)
{
   int i, j;
   char c;
   int Ascii = (int)l;
   Mastermind m;

   m.seed = s;
   m.letter = l;
   m.pos = p;
   m.numGuess = nG; 

   srand(m.seed);

   for (i = 0; i < p; i++)
   {
      j = rand() % (Ascii - 65 + 1);
      c = j + 65;
      m.word[i] = c;
   }

   return m;
}

int findMin(int a, int b)
{
   int min;
   if (a <= b)
   {
      min = a;
   }
   else 
   {
      min = b;
   }
   return min;
}

int exact(char word[], char g[], int size)
{
   int i;
   int x = 0;

   for (i = 0; i < size; i++)
   {
      if (g[i] == word[i])
      {
         x++;
      }
   }  

   return x;
}

int inexact(char word[], char g[], int size, int x)
{
   int y = 0;
   int i, j, a, b;
   int WORD[26] = {0};
   int guess[26] = {0};

   for (j = 0; j < size; j++)
   {
      a = (((int)word[j]) - 'A');
      WORD[a] = WORD[a] + 1;
      b = (((int)g[j]) - 'A');
      guess[b] = guess[b] + 1;
   }
    
   for (i = 0; i < 26; i++) 
   {
      y += findMin(WORD[i], guess[i]);
   }

   y = y - x;

   return y;
}

void Results(int temp[])
{
   if (temp[0] == 1)
   {
      printf("\nWow, you won in %d guesses - well done!\n", temp[1]);
   }
   else
   {
      printf("\nGame over, you ran out of guesses. Better luck next time!\n");
   }
}

void guesses(Mastermind m)
{
   int outcome[2]; 
   int i, p, win, x, y;
   int s = 1;
   char str[9];   

   while (s <= m.numGuess)
   {
      printf("\nEnter guess %d: ", s);
      p = 0;
      for (i = 0; i < m.pos; i++)
      {
         if ((scanf(" %c", &str[i]) != 1) || (isupper(str[i]) == 0))
         {
            printf("Invalid guess, please try again\n");
            while ((getchar()) != '\n');
            s--;
            break;
         }
         p++;
      }

      if (p == m.pos)
      {
         x = exact(m.word, str, m.pos);
         y = inexact(m.word, str, m.pos, x);
         if (x == m.pos)
         {
            win = 1;
            break;
         }
         printf("Nope, %d exact guesses and %d inexact guesses\n", x, y);
      }
      s++;
   }

   outcome[0] = win;
   outcome[1] = s;

   Results(outcome);
}
