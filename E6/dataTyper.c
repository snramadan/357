#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100

void INTandDOUB(double x, int y)
{
   int t;
   if (x != (int)x)
   {
      printf("   Argument %d is a double, its value is %f\n", y, x);
   }
   else
   {
      t = (int)x;
      printf("   Argument %d is an int, its value is %d\n", y, t);
   }
}

void CHARandSTR(char arr[], int z)
{  
   if (strlen(arr) == 1)
   {
      printf("   Argument %d is a char, its value is %c\n", z, *arr);
   }
   else
   {
      printf("   Argument %d is a string, its value is %s\n", z, arr);
   }
}

void getInput(int size, char *args[])
{
   double temp;
   char str[MAX];
   int j;

   for (j = 1; j < size; j++)
   {
      if (sscanf(args[j], "%lf", &temp) == 1)
      {
         INTandDOUB(temp, j);
      }
      else if (sscanf(args[j], "%s", str) == 1)
      {
         CHARandSTR(str, j);
      }
   }
}

int main(int argc, char *argv[])
{
   if (argc < 2)
   {
      fprintf(stderr, "Usage: dataTyper argument [argument ...]\n");
      exit(EXIT_FAILURE);
   }

   getInput(argc, argv);

   return EXIT_SUCCESS;
}
