#include <stdio.h>
#include <fcntl.h> 
#include <unistd.h>
#include <stdlib.h>
#include "memmon.h"

int main(int argc, char *argv[])
{
   int f1, f2, i, mem, curr, peak;
   int n = (int) strtol(argv[1], (char **)NULL, 10);
   char *buf = malloc(n * sizeof(char));
   if (argc != 4)
   {
      fprintf(stderr, "Usage: cpBuffered bufsize fromfile tofile\n");
      exit(EXIT_FAILURE);
   }
   if (n <= 0)
   {
      perror(NULL);
      exit(EXIT_FAILURE);
   }
   if ((f1 = open(argv[2], O_RDONLY, 0)) < 0)
   {
      perror(NULL);
      exit(EXIT_FAILURE);
   }
   if ((f2 = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0)
   {
      perror(NULL);
      exit(EXIT_FAILURE);
   }  

   while ((i = read(f1, buf, n)) == n)
   {
      if (write(f2, buf, i) < 0)
      {
         perror(NULL);
         exit(EXIT_FAILURE);
      }
   }
  
   if (write(f2, buf, i) < 0)
   {
      perror(NULL);
      exit(EXIT_FAILURE);
   }

   if (close(f1) < 0)
   {
      perror(NULL);
      exit(EXIT_FAILURE);

   }
   if (close(f2) < 0)
   {
      perror(NULL);
      exit(EXIT_FAILURE);

   }
   free(buf);

   mem = memAllocs();
   curr = memCurrent();
   peak = memPeak();

   printf("Number of Allocations: %d\n", mem);
   printf("   Current Allocation: %d\n", curr);
   printf("      Peak Allocation: %d\n", peak); 

   return 0;
}
