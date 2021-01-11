#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define QUIT 'q'
#define NEW_PIPE 'n'
#define WRITE_TO_PIPE 'w'
#define READ_FROM_PIPE 'r'
#define CLOSE_PIPE_END 'c'

static int getCommand()
{
   char command;

   printf("\nCommand Menu:\n");
   printf("   n - Create a new pipe\n");
   printf("   w - Write to a pipe end\n");
   printf("   r - Read from a pipe end\n");
   printf("   c - Close a pipe end\n");
   printf("   q - Quit\n\n");
   printf("Enter a command: ");

   /* I know, this expects the user to be perfect - hey it's an exercise! */
   scanf(" %c", &command);
   return command;
}

int errormessage(int i)
{
   if (i == -1)
   {
      perror(NULL);
      exit(EXIT_FAILURE);
   }
 
   return i;
}

void closePipe(int fd[])
{
   int x;
   printf("Enter the fd of the pipe-end you wish to close: ");
   scanf("%d", &x);
   errormessage(close(x));
   printf("File descriptor %d successfully closed\n", x);
}

void readPipe(int buf[])
{
   int x;
   printf("Enter the fd of the read-end to read from: ");
   scanf("%d", &x);
   if (errormessage(read(x, buf, sizeof(int)))== 0)
   {
      printf("EOF encountered while attempting to read from fd %d\n", x);
   }
   else
   {
      printf("The value %d was read successfully from fd %d\n", buf[0], x);
   }
}

void writePipe(int fd[])
{
   int x, y;
   printf("Enter the fd of the write-end to write to: ");
   scanf("%d", &x);
   printf("Enter the integer value you wish to write: ");
   scanf("%d", &y);
   errormessage(write(x, &y, sizeof(y)));
   printf("The value %d was written successfully to fd %d\n", y, x);
}

void createPipe(int fd[])
{
   errormessage(pipe(fd));
   printf("Pipe successfully created, read-fd is %d, write-fd is %d\n",
      fd[0], fd[1]); 
}

static int doACommand()
{
   char command = getCommand();
   int fd[2];  
   int buf[sizeof(int)];

   switch (command)
   {
      case NEW_PIPE:
         /* Good design/decompostion suggest calling a helper function here!*/ 
         createPipe(fd);
      break;

      case WRITE_TO_PIPE:
         /* Good design/decompostion suggest calling a helper function here! */
         writePipe(fd);
      break;

      case READ_FROM_PIPE:
         /* Good design/decompostion suggest calling a helper function here! */
         readPipe(buf);
      break;

      case CLOSE_PIPE_END:
         /* Good design/decompostion suggest calling a helper function here! */
         closePipe(fd);
      break;

      case QUIT:
         printf("Bye Bye - see you next time!\n");
      break;

      default:
         printf("Unknown command, please try again...\n");
   }

   return command;
}

/*
 * NOTE: The SIGPIPE signal is ignored in this program so that you can see the
 *       error that occurs when you try to write to a pipe whose read-end is
 *       closed - be sure to try that scenario!
 */
int main()
{
   int command = !QUIT;

   /* See comment block preceding main */
   signal(SIGPIPE, SIG_IGN);

   while (command != QUIT)
      command = doACommand();

   return 0;
}
