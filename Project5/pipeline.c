#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "launcher.h"

int main(int argc, char *argv[])
{
   int fd[2];
   char Carr[60];
   char Inarr[60];
   char Outarr[60];
   pid_t pid;
   int c, p, i, count, cl;
   int tempFD;
   count = 0;

   arguments(argv, &c, &p, &i);

   sprintf(Carr, "%d", c);
   if (argc != 6)  
   {
      usError();
   }

   if ((pipe(fd)) < 0)
   {
      errorMsg();
   }

   if ((pid = fork()) < 0)
   {
      errorMsg();
   }
   
   else if (pid == 0)
   {
      cl = close(fd[0]);
      if (cl < 0)
      {
         errorMsg();
      }
      sprintf(Outarr, "%d", fd[1]);
      execl(argv[i], argv[i], "-c", Carr, "-o", Outarr, (char *)NULL);
      errorMsg();
   }
   
   while (count < (p-2))
   {
      tempFD = fd[0];
      cl = close(fd[1]);
      if (cl < 0)
      {
         errorMsg();
      }
      if ((pipe(fd)) < 0)
      {
         errorMsg();
      }
      if ((pid = fork()) < 0)
      {
         errorMsg();
      }
      else if (pid == 0)
      {
         cl = close(fd[0]);
         if (cl < 0)
         {
            errorMsg();
         }
         sprintf(Outarr, "%d", fd[1]);
         sprintf(Inarr, "%d", tempFD);
         execl(argv[i], argv[i], "-c", Carr, "-i", 
            Inarr ,"-o", Outarr, (char *)NULL);
         errorMsg();
      }
      close(tempFD);
      count++;
   }
   
   if ((pid = fork()) < 0)
   {
      errorMsg();
   }

   else if (pid == 0)
   {
      cl = close(fd[1]);
      if (cl < 0)
      {
         errorMsg();
      }
      sprintf(Inarr, "%d", fd[0]);
      execl(argv[i], argv[i], "-c", Carr, "-i", Inarr, (char *)NULL);
      errorMsg();
   }
   
   close(fd[0]);
   close(fd[1]);

   for (count = 0; count < p; count++)
   {
      wait(NULL);
   }

   return 0;
}
