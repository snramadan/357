#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "copyFile.h"

/* A working but PARTIALLY COMPLETE program. As provided the program copies
 * everything a user types to the standard input to the standard output using
 * a provided copyFile function.
 *
 * For this exercise you will use the dup2 function to associate an actual
 * file with the standard input and another file with the standard output. As
 * a result, when the file names are provided as arguments the same provided
 * copyFile function will copy the input file to the output file.
 */

static void checkArgs(int argc, char *argv[])
{
   if (argc != 1 && argc != 3)
   {
      fprintf(stderr, "Usage: copyFile [inputFile outputFile]\n");
      exit(EXIT_FAILURE);
   }
}

int main(int argc, char *argv[])
{
   int inFD = STDIN_FILENO, outFD = STDOUT_FILENO; 

   checkArgs(argc, argv);

   /* Replace the standard input and output with files */ 
   if (argc == 3)
   {
      inFD = openFile(argv[1], "r");
      outFD = openFile(argv[2], "w");
 
      dup2(inFD, STDIN_FILENO);
      dup2(outFD, STDOUT_FILENO);
   }

   copyFile();
   return 0;
}
