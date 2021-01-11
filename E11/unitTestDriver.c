/*
 * Test driver provided to students in CPE 357 to test their unit test macros
 * developed for an Exercise in CPE 357.
 *
 * Important note to students:
 *
 *    DO NOT MODIFY THIS PROVIDED FILE!
 *
 * Author: Kurt Mammen
 * Version:
 *    04/02/2016: Initial version.
 *    10/05/2016: Improved flushLine to handle EOF too.
 */
#include <stdio.h>
#include <stdlib.h>
#include "unitTestA.h"
#include "unitTestB.h"
#include "unitTest.h"

/* NOTE: This limit will not be exceeded during testing
 */
#define MAX_STRING 1024

void myExit(const char *msg)
{
   fprintf(stderr, "%s\n", msg);
   exit(EXIT_FAILURE);
}

/* Flush any extra "crap" the user might have entered after the expected
 * values.
 */
void flushLine()
{
   int c;

   while ((c = getchar()) != '\n' && c != EOF);
}

void testBoolean()
{
   long expect, actual;

   /* Get actual value */
   printf("Enter   actual boolean value (0|!0): ");
   if (1 != scanf("%ld", &actual))
      myExit("Unable to read actual boolean value");
   flushLine();
   
   /* Get expected value */
   printf("Enter expected boolean value (0|!0): ");
   if (1 != scanf("%ld", &expect))
      myExit("Unable to read expected boolean value");
   flushLine();

   /* Randomize file and line numbers to verify macro is actually working */
   if (rand() % 2)
      testBooleanA(actual, expect);
   else
      testBooleanB(actual, expect);
}

void testChar()
{
   char expect, actual;

   /* Get actual value */
   printf("Enter   actual character: ");
   if (1 != scanf("%c", &actual))
      myExit("Unable to read actual character");
   flushLine();
   
   /* Get expected value */
   printf("Enter expected character: ");
   if (1 != scanf("%c", &expect))
      myExit("Unable to read expected character");
   flushLine();

   /* Randomize file and line numbers to verify macro is actually working */
   if (rand() % 2)
      testCharA(actual, expect);
   else
      testCharB(actual, expect);
}

void testSigned()
{
   long expect, actual;

   /* Get actual value */
   printf("Enter   actual signed value: ");
   if (1 != scanf("%ld", &actual))
      myExit("Unable to read actual signed value");
   flushLine();
   
   /* Get expected value */
   printf("Enter expected signed value: ");
   if (1 != scanf("%ld", &expect))
      myExit("Unable to read expected signed value");
   flushLine();

   /* Randomize file and line numbers to verify macro is actually working */
   if (rand() % 2)
      testSignedA(actual, expect);
   else
      testSignedB(actual, expect);
}

void testUnsigned()
{
   unsigned long expect, actual;

   /* Get actual value */
   printf("Enter   actual unsigned value: ");
   if (1 != scanf("%lu", &actual))
      myExit("Unable to read actual unsigned value");
   flushLine();
   
   /* Get expected value */
   printf("Enter expected unsigned value: ");
   if (1 != scanf("%lu", &expect))
      myExit("Unable to read expected unsigned value");
   flushLine();

   /* Randomize file and line numbers to verify macro is actually working */
   if (rand() % 2)
      testUnsignedA(actual, expect);
   else
      testUnsignedB(actual, expect);
}

void testReal()
{
   double expect, actual, epsilon;

   /* Get actual value */
   printf("Enter   actual real value: ");
   if (1 != scanf("%lf", &actual))
      myExit("Unable to read actual real value");
   flushLine();
   
   /* Get expected value */
   printf("Enter expected real value: ");
   if (1 != scanf("%lf", &expect))
      myExit("Unable to read expected real value");
   flushLine();
   
   /* Get epsilon value */
   printf("Enter epsilon: ");
   if (1 != scanf("%lf", &epsilon))
      myExit("Unable to read epsilon");
   flushLine();

   /* Randomize file and line numbers to verify macro is actually working */
   if (rand() % 2)
      testRealA(actual, expect, epsilon);
   else
      testRealB(actual, expect, epsilon);
}

void testString()
{
   char expect[MAX_STRING], actual[MAX_STRING];

   /* Get actual value */
   printf("Enter   actual string (<%d characters): ", MAX_STRING);
   if (1 != scanf("%s", actual))
      myExit("Unable to read actual string");
   flushLine();
   
   /* Get expected value */
   printf("Enter expected string (<%d characters): ", MAX_STRING);
   if (1 != scanf("%s", expect))
      myExit("Unable to read expected string");
   flushLine();

   /* Randomize file and line numbers to verify macro is actually working */
   if (rand() % 2)
      testStringA(actual, expect);
   else
      testStringB(actual, expect);
}

char getType()
{
   char type;

   printf("Enter the type to check (b|c|s|u|r|S|q to quit): ");

   /* Could be reading from a file so I need to check in case I'm at EOF */
   if (1 != scanf(" %c", &type))
      myExit("Unable to read type");

   flushLine();
   return type;
}

char test(char type)
{
   switch(type)
   {
      case 'b':
         testBoolean();
      break;

      case 'c':
         testChar();
      break;

      case 's':
         testSigned();
      break;

      case 'u':
         testUnsigned();
      break;
      
      case 'r':
         testReal();
      break;

      case 'S':
         testString();
      break;

      case 'q':
      break;

      default:
         printf("'%c' is an invalid type, please try again\n", type);
   }

   return type;
}

void randomize()
{
   int seed;

   printf("Enter a random seed value: ");
   
   if (1 != scanf("%d", &seed))
      myExit("Unable to read seed value");

   srand(seed);
}

int main()
{
   char type;

   randomize();

   do {
      type = test(getType());
   } while (type != 'q');

   return 0;
}
