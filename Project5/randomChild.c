/*
 * Provided file - DO NOT MODIFY!!!
 *
 * Purpose:
 *    This program is intended to be a simple child process for you to launch
 *    arbitrarily long pipelines from a parent program that you will write.
 *
 * Description:
 *    A simple program that generates and displays random integer values. By
 *    default, it generates 10 random integer values and prints them to the
 *    standard output. It can also generate different numbers of values and
 *    read values from a specified input source and write to a specified
 *    output destination.
 *
 *    Usage: randomChild [(-c count [-i 0] | [-i fd]) [-o fd]]
 *
 *    The arguments may occur in any order, here's a description:
 *
 *       -c: Defaults to 10. This flag allows the caller to specify then number
 *           of values to generate or, when the input is not the standard input,
 *           how many values to attempt to read. 
 *
 *           NOTE: This flag can only be specified when the input is
 *                 STDIN_FILENO (0).
 *
 *       -i: Defaults to STDIN_FILENO (0). This flag allows the caller to
 *           specify the fd to read from. When it is the default value the
 *           program will generate the specified number of random values,
 *           otherwise it will read values until EOF or until encountering
 *           a "Broken Pipe" when writing.
 *
 *           NOTE: This flag may not exceed MAX_FD.
 *
 *       -o: Defaults to STDOUT_FILENO (1). This flag allows the caller to
 *           specify the fd to write to. The program writes all of the input
 *           generated or read to the specified fd.
 *
 *           NOTE: This flag may not exceed MAX_FD.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

#define MAX_ARGS 7
#define MAX_FD 5
#define DEFAULT_SEED 781992343 /* WILL be different when evaluated! */
#define DEFAULT_COUNT 10 

typedef struct
{
   int count;
   int in;
   int out;
} RandomChild;

static void generateValues(RandomChild rc)
{
   int i, r, ret;

   for (i = 0; i < rc.count; i++)
   {
      r = rand();

      if (rc.out == STDOUT_FILENO)
      {
         /* Human readable */
         if (0 > fprintf(stdout, "%d\n", r))
         { 
            /* Definitly NOT okay! */
            fprintf(stderr, "randomChild: fprintf(stdout, ...)");
            exit(EXIT_FAILURE);
         }
      }
      else
      {
         /* Machine readable */
         if (sizeof(int) != (ret = write(rc.out, &r, sizeof(int))))
         {
            /* Early close of read end of pipe okay by me, other errors NOT! */
            if (errno == EPIPE)
            {
               exit(EXIT_FAILURE); /* Semi-okay - assume error is not mine */
            }
            else
            {
               perror("randomChild: write"); /* Definitly NOT okay! */
               exit(EXIT_FAILURE);
            }
         }
      }
   }
}

/* Reads from rc.in until EOF, adds a new random number to each value, and
 * writes the sum to rc.out.
 *
 * Note: Overflow happens!
 */
static void readValues(RandomChild rc)
{
   int value, ret;

   while (sizeof(int) == (ret = read(rc.in, &value, sizeof(int))))
   {
      value += rand();

      if (rc.out != STDOUT_FILENO)
      {
         /* Machine readable */
         if (sizeof(int) != (ret = write(rc.out, &value, sizeof(int))))
         {
            /* Early close of read end of pipe okay by me, other errors NOT! */
            if (errno == EPIPE)
            {
               exit(EXIT_FAILURE); /* Semi-okay - assume error is not mine */
            }
            else
            {
               perror("randomChild: write"); /* Definitly NOT okay! */
               exit(EXIT_FAILURE);
            }
         }
      }
      else
      {
         /* Human readable */
         if (0 > fprintf(stdout, "%d\n", value))
         {
            /* Early close of read end of pipe okay by me, other errors NOT! */
            if (errno == EPIPE)
            {
               exit(EXIT_FAILURE); /* Semi-okay - assume error is not mine */
            }
            else
            {
               perror("randomChild: fprintf"); /* Definitly NOT okay! */
               exit(EXIT_FAILURE);
            }
         }
      }
   }

   if (ret == -1)
   {
      perror("randomChild: read");
      exit(EXIT_FAILURE);
   }
}

static void showUsageAndExit()
{
   fprintf(stderr, "Usage: randomChild [(-c count [-i 0] | [-i fd]) ");
   fprintf(stderr, "[-o fd]]\n");
   fprintf(stderr, "Where: count > 0 and -i and -o fd <= %d\n", MAX_FD);
   exit(EXIT_FAILURE);
}

static int getArg(char *argv[], int index)
{
   int arg;

   if (1 != sscanf(argv[index], "%d", &arg))
      showUsageAndExit();
      
   /* All should, at least be zero or positive values */
   if (arg < 0)
      showUsageAndExit();
      
   return arg;
}

/* Returns true when encountering a -c flag so caller can check that it
 * is only specified when -i is stdin.
 */
static int checkArg(char *argv[], int i, RandomChild *rc)
{
   int hasCount = 0;

   if (0 == strcmp(argv[i], "-c")) {
      rc->count = getArg(argv, i + 1);

      if (rc->count < 1)
         showUsageAndExit();

      hasCount = 1;
   }
   else if (0 == strcmp(argv[i], "-i"))
   {
      rc->in = getArg(argv, i + 1);
      
      if (rc->in > MAX_FD)
         showUsageAndExit();
   }
   else if (0 == strcmp(argv[i], "-o"))
   {
      rc->out = getArg(argv, i + 1);
      
      if (rc->out > MAX_FD)
         showUsageAndExit();
   }
   else
      showUsageAndExit();

   return hasCount;
}

static RandomChild checkArgs(int argc, char *argv[])
{
   int i, hasCount = 0;
   RandomChild rc;

   if (argc > MAX_ARGS)
      showUsageAndExit();

   rc.count = DEFAULT_COUNT;
   rc.in = STDIN_FILENO;
   rc.out = STDOUT_FILENO;

   /* Check and init */
   for (i = 1; i < argc; i += 2)
   {
      /* Check for a possible flag (-c, -i, -o with no value) */
      if (i >= argc - 1)
         showUsageAndExit();

      hasCount = checkArg(argv, i, &rc);
   }

   if (hasCount && rc.in != STDIN_FILENO)
      showUsageAndExit();

   return rc;
}

int main(int argc, char *argv[])
{
   RandomChild rc = checkArgs(argc, argv);

   /* Cause failed write to pipe to return "Broken Pipe" error, not signal */
   signal(SIGPIPE, SIG_IGN);

   /* Use a different values for grading! */
   srand(DEFAULT_SEED);

   if (rc.in == STDIN_FILENO)
      generateValues(rc);
   else
      readValues(rc);

   /* Done, manage the pipe ends! */
   if (rc.in != STDIN_FILENO)
      close(rc.in);

   if (rc.out != STDOUT_FILENO)
      close(rc.out);

   return 0;
}
