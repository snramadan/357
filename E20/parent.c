#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define MAXLINE 101

int main(int argc, char *argv[])
{
   int fd1[2];
   int fd2[2];
   pid_t pid;
   int status, isQ = 0;
   char cArg1[MAXLINE];
   char cArg2[MAXLINE];
   char *gMsg = "quit\n";
   char outMsg[MAXLINE];
   char inMsg[MAXLINE] = "";

   if (argc != 2)
   {
      fprintf(stderr, "Usage: parent child\n");
      exit(EXIT_FAILURE);
   }

   if (pipe(fd1))
   {
      perror(NULL);
      exit(EXIT_FAILURE);
   }

   if (pipe(fd2))
   {
      perror(NULL);
      exit(EXIT_FAILURE);
   }

   if ((pid = fork()) < 0)
   {
      perror(NULL);
      exit(EXIT_FAILURE);
   }

   else if (pid == 0)
   {
      if (close(fd1[1]) < 0)
      {
         perror(NULL);
         exit(EXIT_FAILURE);
      }
      if (close(fd2[0]) < 0)
      {
         perror(NULL);
         exit(EXIT_FAILURE);
      }

      sprintf(cArg1, "%d", fd1[0]);
      sprintf(cArg2, "%d", fd2[1]);

      execl(argv[1], argv[1], cArg1, cArg2, (char *)NULL);

      perror(NULL);
      exit(EXIT_FAILURE);     
   }

   else if (pid > 0)
   {
      if (close(fd1[0]) < 0)
      {
         perror(NULL);
         exit(EXIT_FAILURE);
      }
      if (close(fd2[1]) < 0)
      {
         perror(NULL);
         exit(EXIT_FAILURE);
      }
      printf("Message from child: ");
 
      if (read(fd2[0], outMsg, MAXLINE) < 0)
      {
         perror(NULL);
         exit(EXIT_FAILURE);
      }
      printf("%s", outMsg);

      while (1)
      {
         printf("\nEnter a message for the child: ");
         fgets(inMsg, MAXLINE, stdin);
  
         if (!strcmp(inMsg, gMsg))
         {
            if (close(fd1[1]) < 0)
            {
               perror(NULL);
               exit(EXIT_FAILURE);
            }
            printf("Parent closing the write-end of pipe to child\n");
            isQ = 1;
         }
         else
         {
            if (write(fd1[1], inMsg, strlen(inMsg) +1) < 0)
            {
               perror(NULL);
               exit(EXIT_FAILURE);
            }
         }
         printf("Acknowledgement from Child: ");
         if (read(fd2[0], outMsg, MAXLINE) < 0)
         {
            perror(NULL);
            exit(EXIT_FAILURE);
         }
         printf("%s", outMsg);
         if (isQ)
         {
            break;
         }
      }
      printf("\n\nParent waiting for child to terminate...\n");
      if (wait(&status) < 0)
      {
         perror(NULL);
         exit(EXIT_FAILURE);     
      }
   }
   printf("Child has terminated, parent ending...\n");
   return 0;
}
