#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "launcher.h"

void errorMsg()
{
   perror(NULL);
   exit(EXIT_FAILURE);
}

void usError()
{
   fprintf(stderr, "Usage: pipeline pathToRandomChild -c count -p processes\n");
   fprintf(stderr, "Where: count > 0 and processes is 2 to 10, inclusive\n");
   exit(EXIT_FAILURE);
}

void arguments(char *a[], int *c, int *p, int *i)
{
   int temp, C, P, I;
   int T = 0;
   char *rand;
   C = 0;
   P = 0;
   I = 0;
   for (temp = 1; temp < 6; temp++)
   {
      if (strcmp(a[temp], "-c") == 0)
      {
         if ((temp != 5) && (C == 0))
         {
            (*c) = (int)(strtol(a[temp+1], &rand, 10));
            C++;
            temp++;
         }
         else
         {
            usError();
         }
      }
      else if (strcmp(a[temp], "-p") == 0)
      {
         if ((temp != 5) && (P == 0))
         {
            (*p) = (int)(strtol(a[temp+1], &rand, 10));
            P++;
            temp++;
         }
         else
         {
            usError();
         }
      }
      else
      {
         if (I == 0)
         {
            (*i) = temp;
            I++;
         }
         T++;
      }
   }
   
   if (T > 1)
   {
      usError();
   }
}
