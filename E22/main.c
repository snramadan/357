#include <stdio.h>
#include "string.h"

int doWORK(char *s, int *curr)
{
   int n, c;
   int temp = *curr;
   unsigned int input;
   int check = 0;
   if (strcmp("&", s) == 0)
   {
      while (check < 1)
      {
         printf("Enter RHS for the & operator in hex: ");
         if (scanf("%08X", &input) <= 0)
         {
            printf("RHS must be a value in hex, please try again\n");
            while (((c = getchar()) != EOF) && c != '\n');
         }
         else
         {
            check = 1;
         }
      }
      n = temp & input;
      printf("0x%08X & 0x%08X is 0x%08X\n", temp, input, n);
      (*curr) = n;
   }
   else if(strcmp("|", s) == 0)
   {
      while (check < 1)
      {
         printf("Enter RHS for the | operator in hex: ");
         if (scanf("%08X", &input) <= 0)
         {
            printf("RHS must be a value in hex, please try again\n");
            while (((c = getchar()) != EOF) && c != '\n');
         }
         else
         {
            check = 1;
         }
      }
      n = temp | input;
      printf("0x%08X | 0x%08X is 0x%08X\n", temp, input, n);
      (*curr) = n;
   }
   else if (strcmp("^", s) == 0)
   {
      while (check < 1)
      {
         printf("Enter RHS for the ^ operator in hex: ");
         if (scanf("%08x", &input) <= 0)
         {
            printf("RHS must be a value in hex, please try again\n");
            while (((c = getchar()) != EOF) && c != '\n');
         }
         else
         {
            check = 1;
         }
      }
      n = temp ^ input;
      printf("0x%08X ^ 0x%08X is 0x%08X\n", temp, input, n);
      (*curr) = n;
   }
   else if(strcmp("<<", s) == 0)
   {
      while (check < 1)
      {
         printf("Enter RHS for the << operator in decimal: ");
         if (scanf("%u", &input) <= 0)
         {
            printf("RHS must be an unsigned value in decimal, ");
            printf("please try again\n");
            while (((c = getchar()) != EOF) && c != '\n');
         }
         else
         {
            check = 1;
         }
      }
      n = (unsigned int)temp << input;
      printf("0x%08X << %u is 0x%08X\n", temp, input, n);
      (*curr) = n;
   }
   else if (strcmp(">>", s) == 0)
   {
      while (check < 1)
      {
         printf("Enter RHS for the >> operator in decimal: ");
         if (scanf("%u", &input) <= 0)
         {
            printf("RHS must be an unsigned value in decimal, ");
            printf("please try again\n");
            while (((c = getchar()) != EOF) && c != '\n');
         }
         else
         {
            check = 1;
         }
      }
      n = (unsigned int)temp >> input;
      printf("0x%08X >> %u is 0x%08X\n", temp, input, n);
      (*curr) = n;
   }
   else if(strcmp("~", s) == 0)
   {
      n = ~temp;
      printf("~0x%08X is 0x%08X\n", temp, n);
      (*curr) = n;
   }
   else if (strcmp("quit", s) == 0)
   {
      return 0;
   }
   else
   {
      printf("Unrecognized operator 'q', please try again");
      return -1;
   }

   return 1;
}

int main()
{  
   int curr = 0;
   int i = 0;
   int c = 0;
   char s[20];
   while (i == 0)
   {    
      printf("\nCurrent value: 0x%08X", curr);
      printf("\nEnter bitwise operator (or \"quit\"): ");
      scanf(" %s", s);
      while (((c = getchar()) != EOF) && c != '\n');
      while(doWORK(s, &curr) < 0)
      {
         printf("\nEnter bitwise operator (or 'quit'): ");
         scanf(" %s", s);
         while (((c = getchar()) != EOF) && c != '\n');
      }
      if (strcmp("quit", s) == 0)
      {
         i = 1;
      }
   }

   return 0;
}
