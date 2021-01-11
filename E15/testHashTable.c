#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "unitTest.h"
#include "hashTable.h"

#define TEST_ALL -1
#define NORMAL -2 
#define SPECIAL -3

/* Prototype of all test functions. This allows the creation of an array of
 * function pointers which makes the testing code shorter and more clear. It
 * also makes it easier/faster to add new tests - NICE!
 */
typedef void (*TEST_FUNC)();

/* Modified hash algorithm from K&R (page 144). This version does NOT mod the
 * calculated hash value by the table size because it does not know the table
 * size - the caller does though!
 *
 * Also note the following:
 *   * Not the best/fastest/most flexible hash - you'll want something better
 *     for the word frequency project.
 *   * Assumes the data is a C string (nul-terminated sequence of char values).
 *   * You should test with more than just C strings - that implies you will
 *     need to develop a different hash function for the different types.
 *     Writing one that works on a struct (like you'll be using in the word
 *     frequency project) would be a good idea!
 */
static unsigned hashString(const void *data)
{
   unsigned hash;
   const char *str = data;

   for (hash = 0; *str;  str++)
      hash = *str + 31 * hash;

   /* Return the raw hash value - the modulo must be done by caller */
   return hash;
}

static int compareString(const void *a, const void *b)
{
   return strcmp(a, b);
}

/* Helper function to make random strings for robust testing.
 */
static char* randomString()
{
   char *str; 
   int i, length = (rand() % 70) + 3; /* 3 to 72 character random strings  */

   if (NULL == (str = malloc(length + 1)))
   {
      perror("randomString()");
      exit(EXIT_FAILURE);
   }

   /* Put random but visible char values in the string */
   for (i = 0; i < length; i++)
      str[i] = (rand() % (' ' - '~' + 1)) + ' ';

   str[length] = 0; /* nul-terminate it! */

   return str; 
}

/* Good practice suggests:
 *    1. Choosing descriptive test function names.
 *    2. Including a brief description of each test.
 * 
 * Test creating an empty hash table.
 */
static void testCreateEmpty()
{
   /* Set up the test */
   void *ht;

   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {7};
   
   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   /* Call the function being tested */
   ht = htCreate(&functions, sizes, 1, 0.73);

   /* Do the test(s) */
   TEST_UNSIGNED(htCapacity(ht), 7);
   TEST_UNSIGNED(htUniqueEntries(ht), 0);
   TEST_UNSIGNED(htTotalEntries(ht), 0);

   /* Clean up, destroyData is false because there is not data */
   htDestroy(ht, 0); /* Note: destroyData true should work here too! */
}

/* Test adding a single entry to a hash table
 */
static void testAddSingleEntry()
{
   /* Set up the test */
   void *ht;
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {11, 23};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 1, 0.73);

   /* Call the function being tested */
   htAdd(ht, randomString());

   /* Do the test(s) */
   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 1);
   TEST_UNSIGNED(htTotalEntries(ht), 1);

   /* Clean up */
   htDestroy(ht, 1); /* destroyData true: There is data and it's dynamic */ 
}

/* Test with rehash value
 */
static void testAddSameHash()
{
   void *ht;

   unsigned sizes[] = {14, 26, 45};

   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 1, 0.84);

   htAdd(ht, "Hello!!");
   htAdd(ht, "Hello!!");

   TEST_UNSIGNED(htCapacity(ht), 14);
   TEST_UNSIGNED(htUniqueEntries(ht), 1);
   TEST_UNSIGNED(htTotalEntries(ht), 2);

   htDestroy(ht, 0);
}

/* Test with rehash
 */
static void testREHASH()
{
   void *ht;

   unsigned sizes[] = {2, 26};

   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 2, 0.76);

   htAdd(ht, "1");
   htAdd(ht, "17");
   htAdd(ht, "45");
   htAdd(ht, "89");
   htAdd(ht, "7");
   htAdd(ht, "14");

   TEST_UNSIGNED(htCapacity(ht), 26);
   TEST_UNSIGNED(htUniqueEntries(ht), 6);
   TEST_UNSIGNED(htTotalEntries(ht), 6);

   htDestroy(ht, 0);
}

/* Test Multiple Entries
 */
static void testFourEntries()
{
   void *ht;

   unsigned sizes[] = {2, 26};

   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 2, 0.76);

   htAdd(ht, "Hi");
   htAdd(ht, "Hello");
   htAdd(ht, "Husky");
   htAdd(ht, "Dog");
   htAdd(ht, "Husky");

   TEST_UNSIGNED(htCapacity(ht), 26);
   TEST_UNSIGNED(htUniqueEntries(ht), 4);
   TEST_UNSIGNED(htTotalEntries(ht), 5);
   TEST_UNSIGNED(htLookUp(ht, "Hi").frequency, 1);

   htDestroy(ht, 0);
}

/* Test that htCreate asserts when zero sizes specified 
 */
static void testErrorCreateZeroSizes()
{
   /* Set up the test */
   unsigned sizes[] = {7};
   HTFunctions functions = {hashString, compareString, NULL};

   /* Call the function being tested */
   TEST_ERROR(htCreate(&functions, sizes, 0, 0.73));

   /* No clean up necessary - expect assert/program abort by htCreate */
}

/* Test Add Error
 */
static void testErrorAdd()
{
   void *ht;
   
   unsigned sizes[] = {2, 26};
   HTFunctions functions = {hashString, compareString, NULL};  

   ht = htCreate(&functions, sizes, 2, 0.76);
 
   TEST_ERROR(htAdd(ht, NULL));
}

static void testAll(TEST_FUNC* testFuncs)
{
   int i;

   for (i = 0; testFuncs[i] != NULL; i++)
   {
      printf("Running normal test function %d\n", i);

      /* Call the test function via function pointer in the array */
      testFuncs[i]();
   }
}

static void runTests(TEST_FUNC *testFuncs, int testType, int testNumber)
{
   if (testNumber == TEST_ALL)
      testAll(testFuncs);
   else
   {
      if (testType == NORMAL)
         printf("Running normal test function %d\n", testNumber);
      else
         printf("Running special test function %d\n", testNumber);
      
      /* Call the test function via function pointer in the array */
      testFuncs[testNumber]();
   }
}

static int numberOfTests(TEST_FUNC* testFuncs)
{
   int count = 0;

   while(testFuncs[count] != NULL)
      count++;

   return count;
}

static int checkArgs(int argc, char *argv[], TEST_FUNC *normalFuncs,
   TEST_FUNC *specialFuncs, int *testType)
{
   int testNumber;

   if (argc == 1)
   {
      *testType = NORMAL;      
      testNumber =  TEST_ALL;
   }
   else if (argc == 2)
   {
      if (1 != sscanf(argv[1], "%d", &testNumber))
      {
         fprintf(stderr, "Invalid test number, must be an integer\n");
         exit(EXIT_FAILURE);
      }

      if (testNumber >= numberOfTests(normalFuncs) || testNumber < 0)
      {
         fprintf(stderr, "Invalid test number (%d)\n", testNumber);
         exit(EXIT_FAILURE);
      }
      
      *testType = NORMAL;
   }
   else if (argc == 3)
   {
      if (0 != strcmp(argv[1], "-special"))
      {
         fprintf(stderr, "Invalid option '%s'\n", argv[1]);
         exit(EXIT_FAILURE);
      }
      
      if (1 != sscanf(argv[2], "%d", &testNumber))
      {
         fprintf(stderr, "Invalid test number, must be an integer\n");
         exit(EXIT_FAILURE);
      }

      if (testNumber >= numberOfTests(specialFuncs) || testNumber < 0)
      {
         fprintf(stderr, "Invalid test number (%d)\n", testNumber);
         exit(EXIT_FAILURE);
      }

      *testType = SPECIAL;
   }
   else
   {
      fprintf(stderr, "Usage: %s [N|-special N]\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   return testNumber;
}

/* Test driver for Hash Table Project.
 *
 * Usage: testHashTable [N|-special N]
 * Where: N is the zero-based index of the test to run.
 *
 * When no option is specified ALL of the tests in normalFuncs are run.
 * When N is specified the specified test in normalFuncs is run.
 * When "-special N" is specified the specified test from specialFuncs is run.
 *
 * TODO:
 *    1) Write a test function.
 *    2) Add its name to the appropriate TEST_FUNC array below.
 *    3) Compile and test your test by:
 *          1. Does it report failure when run on a function known to be
 *             incorrect?
 *          2. Does it pass when run on a function believed to be correct?
 *    4) Be sure use Valgrind, where appropriate, to check for memory errors
 *       and/or leaks!
 */
int main(int argc, char *argv[])
{
   int testNumber, testType;

   /* The array of "normal" test functions, ALL of these run by default */
   TEST_FUNC normalFuncs[] = {
      testCreateEmpty,
      testAddSingleEntry,
      testAddSameHash,
      testREHASH,
      testFourEntries,
      NULL
   };

   /* The array of "special" test functions - MUST be run one at a time */
   TEST_FUNC specialFuncs[] = {
      testErrorCreateZeroSizes,
      testErrorAdd,
      NULL
   };

   /* Random numbers used to produce "interesting" strings for testing */
   srand(182955);

   /* Make stdout unbuffered so that test output is synchronous on signals */
   setbuf(stdout, NULL);

   /* Get the test number and test type */
   testNumber = checkArgs(argc, argv, normalFuncs, specialFuncs, &testType);
 
   /* Run the test(s)... */
   if (testType == NORMAL)
      runTests(normalFuncs, NORMAL, testNumber);
   else
      runTests(specialFuncs, SPECIAL, testNumber);
   
   return 0;
}
