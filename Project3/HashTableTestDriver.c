#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include "unitTest.h"
#include "hashTable.h"

#define TEST_ALL -1
#define NORMAL -2 
#define SPECIAL -3

typedef struct 
{
   char *str;
}strStruct;

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

static unsigned hashStruct(const void *data)
{
   strStruct *s = (strStruct*)data;
   return hashString(((void*)s->str));
}

static strStruct* makeStruct()
{
   strStruct *s = malloc(sizeof(strStruct));
   s->str = randomString();
   return s;
}

static int compareStruct(const void *a, const void *b)
{
   strStruct *c = (strStruct*)a;
   strStruct *d = (strStruct*)b;
   return compareString(c->str, d->str);
}

static void destrotStruct(const void *data)
{
   strStruct *s = (strStruct*)data;
   free(s->str);
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

static void testAddMultSame()
{
   /* Set up the test */
   void *ht;
   void *str = randomString();
   void *str2 = str;
   int i;
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {11};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 1, 1.0);

   /* Call the function being tested */
   for (i=1; i<31; i++)
   {
      htAdd(ht, str);
      TEST_UNSIGNED(htCapacity(ht), 11);
      TEST_UNSIGNED(htUniqueEntries(ht), 1);
      TEST_UNSIGNED(htTotalEntries(ht), i);
   }

   htAdd(ht, str2);

   /* Clean up */
   htDestroy(ht, 1); /* destroyData true: There is data and it's dynamic */ 
}

static void testAddMultNoRehash()
{
   /* Set up the test */
   void *ht;
   int i;
   void *str[105];
   HTEntry entry;
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {31};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 1, 1.0);

   for (i=0; i<105; i++)
      str[i] = randomString();

   /* Call the function being tested */
   htAdd(ht, str[0]);
   for (i=1; i<105; i++)
   {
      htAdd(ht, str[i]);
      TEST_UNSIGNED(htCapacity(ht), 31);
      TEST_UNSIGNED(htUniqueEntries(ht), i+1);
      TEST_UNSIGNED(htTotalEntries(ht), i+1);
   }

   for (i=0; i<105; i++)
   {
      entry = htLookUp(ht, str[i]);

      TEST_UNSIGNED(entry.frequency, 1);
      TEST_BOOLEAN((compareString(entry.data, str[i]) == 0), 1);

   }

   /* Clean up */
   htDestroy(ht, 1); /* destroyData true: There is data and it's dynamic */ 
}

static void testAddMultRehash()
{
   /* Set up the test */
   void *ht;
   int i;
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {31, 59};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 2, .73);

   /* Call the function being tested */
   for (i=1; i<24; i++)
   {
      htAdd(ht, randomString());
      TEST_UNSIGNED(htCapacity(ht), 31);
      TEST_UNSIGNED(htUniqueEntries(ht), i);
      TEST_UNSIGNED(htTotalEntries(ht), i);
   }

   for (i=24; i<110; i++)
   {
      htAdd(ht, randomString());
      TEST_UNSIGNED(htCapacity(ht), 59);
      TEST_UNSIGNED(htUniqueEntries(ht), i);
      TEST_UNSIGNED(htTotalEntries(ht), i);
   }

   /* Clean up */
   htDestroy(ht, 1); /* destroyData true: There is data and it's dynamic */ 
}

static void testMultRehash()
{
   /* Set up the test */
   void *ht;
   int i;
   void *str[105];
   HTEntry entry;
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {23, 31, 53, 67, 103};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 5, .6);

   for (i=0; i<105; i++)
      str[i] = randomString();

   /* Call the function being tested */
   htAdd(ht, str[0]);
   for (i=1; i<105; i++)
   {
      htAdd(ht, str[i]);
      /*TEST_UNSIGNED(htCapacity(ht), 31);*/
      TEST_UNSIGNED(htUniqueEntries(ht), i+1);
      TEST_UNSIGNED(htTotalEntries(ht), i+1);
   }

   for (i=0; i<105; i++)
   {
      entry = htLookUp(ht, str[i]);

      TEST_UNSIGNED(entry.frequency, 1);
      TEST_BOOLEAN((compareString(entry.data, str[i]) == 0), 1);

   }

   /* Clean up */
   htDestroy(ht, 1); /* destroyData true: There is data and it's dynamic */ 
}

static void testAddMultEntries()
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
   htAdd(ht, randomString());

   /* Do the test(s) */
   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 2);
   TEST_UNSIGNED(htTotalEntries(ht), 2);

   /* Clean up */
   htDestroy(ht, 1); /* destroyData true: There is data and it's dynamic */ 
}

static void testAddMultEntries2()
{
   /* Set up the test */
   void *ht;
   int i;
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {11, 23};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 2, 0.73);

   /* Call the function being tested */
   for (i=0; i<12; i++)
      htAdd(ht, randomString());

   /* Do the test(s) */
   TEST_UNSIGNED(htCapacity(ht), 23);
   TEST_UNSIGNED(htUniqueEntries(ht), 12);
   TEST_UNSIGNED(htTotalEntries(ht), 12);

   /* Clean up */
   htDestroy(ht, 1); /* destroyData true: There is data and it's dynamic */ 
}

static void testLookUp()
{
   void *ht;
   HTEntry entry;
   char *str = randomString();

   unsigned sizes[] = {11, 23};
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 2, 0.73);

   entry = htLookUp(ht, str);

   TEST_UNSIGNED(entry.frequency, 0);
   TEST_BOOLEAN((entry.data == NULL), 1);

   htDestroy(ht, 1);
   free(str);
}

static void testLookUp2()
{
   void *ht;
   HTEntry entry;
   void *str = randomString();

   unsigned sizes[] = {11, 23};
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 2, 0.73);

   htAdd(ht, str);
   htAdd(ht, randomString());

   entry = htLookUp(ht, str);

   TEST_UNSIGNED(entry.frequency, 1);
   TEST_BOOLEAN((compareString(entry.data, str) == 0), 1);

   htDestroy(ht, 1);
}

static void testDeepCopy()
{
   /* Set up the test */
   void *ht;
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {11, 23, 37};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 2, 0.73);

   sizes[0] = 21;

   /* Do the test(s) */
   TEST_UNSIGNED(htCapacity(ht), 11);

   /* Clean up */
   htDestroy(ht, 1); /* destroyData true: There is data and it's dynamic */ 
}

/* Test that htCreate asserts when zero sizes specified 
 */
static void testCoreTest1()
{
   /* Set up the test */
   unsigned sizes[] = {7};
   HTFunctions functions = {hashString, compareString, NULL};

   /* Call the function being tested */
   TEST_ERROR(htCreate(&functions, sizes, 0, 0.73));

   /* No clean up necessary - expect assert/program abort by htCreate */
}

static void testCoreTest2()
{
   /* Set up the test */
   unsigned sizes[] = {5};
   HTFunctions functions = {hashString, compareString, NULL};

   /* Call the function being tested */
   TEST_ERROR(htCreate(&functions, sizes, -5, 0.73));

   /* No clean up necessary - expect assert/program abort by htCreate */
}

static void testCoreTest3()
{
   /* Set up the test */
   unsigned sizes[] = {0, 5, 3, 13};
   HTFunctions functions = {hashString, compareString, NULL};

   /* Call the function being tested */
   TEST_ERROR(htCreate(&functions, sizes, 4, 0.73));

   /* No clean up necessary - expect assert/program abort by htCreate */
}

static void testCoreTest4()
{
   /* Set up the test */
   unsigned sizes[] = {5, 11, 13, 13, 23};
   HTFunctions functions = {hashString, compareString, NULL};

   /* Call the function being tested */
   TEST_ERROR(htCreate(&functions, sizes, 5, 0.73));

   /* No clean up necessary - expect assert/program abort by htCreate */
}

static void testCoreTest5()
{
   /* Set up the test */
   unsigned sizes[] = {5, 11, 7, 13, 23, 31};
   HTFunctions functions = {hashString, compareString, NULL};

   /* Call the function being tested */
   TEST_ERROR(htCreate(&functions, sizes, 6, 0.73));

   /* No clean up necessary - expect assert/program abort by htCreate */
}

static void testCoreTest6()
{
   /* Set up the test */
   unsigned sizes[] = {5, 11, 13, 23};
   HTFunctions functions = {hashString, compareString, NULL};

   /* Call the function being tested */
   TEST_ERROR(htCreate(&functions, sizes, 4, 0));

   /* No clean up necessary - expect assert/program abort by htCreate */
}

static void testCoreTest7()
{
   /* Set up the test */
   unsigned sizes[] = {5, 11, 13, 23};
   HTFunctions functions = {hashString, compareString, NULL};

   /* Call the function being tested */
   TEST_ERROR(htCreate(&functions, sizes, 4, 1.1));

   /* No clean up necessary - expect assert/program abort by htCreate */
}

static void testCoreTest8()
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

static void testCoreTest9()
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
   htDestroy(ht, 1); /* Note: destroyData true should work here too! */
}

static void testCoreTest10()
{
   /* Set up the test */
   void *ht;
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {11, 23, 37};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 3, 0.73);

   sizes[0] = 21;

   /* Do the test(s) */
   TEST_UNSIGNED(htCapacity(ht), 11);

   /* Clean up */
   htDestroy(ht, 1); /* destroyData true: There is data and it's dynamic */ 
}

static void testCoreTest11()
{
   /* Set up the test */
   void *ht;
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {11, 23};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 1, 0.73);

   /* Do the test(s) */
   TEST_ERROR(htAdd(ht, NULL));
}

static void testCoreTest12()
{
   /* Set up the test */
   void *ht;
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {11, 23};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 2, 0.73);

   /* Do the test(s) */
   TEST_UNSIGNED(htAdd(ht, randomString()), 1);
   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 1);
   TEST_UNSIGNED(htTotalEntries(ht), 1);

   /* Clean up */
   htDestroy(ht, 1); /* destroyData true: There is data and it's dynamic */ 
}

static void testCoreTest13()
{
   /* Set up the test */
   void *ht;
   int i;
   char *str = randomString();
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {11, 23};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 2, 0.73);

   /* Do the test(s) */
   for (i=1; i<4; i++)
   {
      TEST_UNSIGNED(htAdd(ht, randomString()), 1);
      TEST_UNSIGNED(htCapacity(ht), 11);
      TEST_UNSIGNED(htUniqueEntries(ht), i);
      TEST_UNSIGNED(htTotalEntries(ht), i);
   }

   TEST_UNSIGNED(htAdd(ht, (void*)str), 1);
   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 4);
   TEST_UNSIGNED(htTotalEntries(ht), 4);

   TEST_UNSIGNED(htAdd(ht, (void*)str), 2);
   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 4);
   TEST_UNSIGNED(htTotalEntries(ht), 5);

   /* Clean up */
   htDestroy(ht, 1); /* destroyData true: There is data and it's dynamic */ 
}

static void testCoreTest14()
{
   /* Set up the test */
   void *ht;
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {11, 23};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 2, 0.73);

   /* Do the test(s) */
   TEST_ERROR(htLookUp(ht, NULL));
}

static void testCoreTest15()
{
   /* Set up the test */
   void *ht;
   int i;
   char *str = randomString();
   HTEntry entry;
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {11, 23};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 2, 0.73);

   entry = htLookUp(ht, (void*)str);

   TEST_UNSIGNED(entry.frequency, 0);
   TEST_BOOLEAN((entry.data == NULL), 1);

   TEST_UNSIGNED(htAdd(ht, (void*)str), 1);
   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 1);
   TEST_UNSIGNED(htTotalEntries(ht), 1);

   entry = htLookUp(ht, (void*)str);

   TEST_UNSIGNED(entry.frequency, 1);
   TEST_BOOLEAN((entry.data != NULL), 1);

   /* Do the test(s) */
   for (i=1; i<4; i++)
   {
      TEST_UNSIGNED(htAdd(ht, randomString()), 1);
      TEST_UNSIGNED(htCapacity(ht), 11);
      TEST_UNSIGNED(htUniqueEntries(ht), i+1);
      TEST_UNSIGNED(htTotalEntries(ht), i+1);
   }

   TEST_UNSIGNED(htAdd(ht, (void*)str), 2);
   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 4);
   TEST_UNSIGNED(htTotalEntries(ht), 5);

   entry = htLookUp(ht, (void*)str);

   TEST_UNSIGNED(entry.frequency, 2);
   TEST_BOOLEAN((entry.data != NULL), 1);

   /* Clean up */
   htDestroy(ht, 1); /* destroyData true: There is data and it's dynamic */ 
}

static void testCoreTest16()
{
   void *ht;
   char *str1 = randomString();
   char *str2 = randomString();
   char *str3 = randomString();
   HTEntry entry;
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {3, 7};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 2, 0.6);

   htAdd(ht, (void*)str1);
   TEST_UNSIGNED(htCapacity(ht), 3);
   TEST_UNSIGNED(htUniqueEntries(ht), 1);
   TEST_UNSIGNED(htTotalEntries(ht), 1);

   htAdd(ht, (void*)str2);
   TEST_UNSIGNED(htCapacity(ht), 3);
   TEST_UNSIGNED(htUniqueEntries(ht), 2);
   TEST_UNSIGNED(htTotalEntries(ht), 2);

   htAdd(ht, (void*)str3);
   TEST_UNSIGNED(htCapacity(ht), 7);
   TEST_UNSIGNED(htUniqueEntries(ht), 3);
   TEST_UNSIGNED(htTotalEntries(ht), 3);

   entry = htLookUp(ht, (void*)str1);

   TEST_UNSIGNED(entry.frequency, 1);
   TEST_BOOLEAN((entry.data != NULL), 1);

   entry = htLookUp(ht, (void*)str2);

   TEST_UNSIGNED(entry.frequency, 1);
   TEST_BOOLEAN((entry.data != NULL), 1);

   entry = htLookUp(ht, (void*)str3);

   TEST_UNSIGNED(entry.frequency, 1);
   TEST_BOOLEAN((entry.data != NULL), 1);

   htDestroy(ht, 1);
}

static void testCoreTest17()
{
   void *ht;
   char *str1 = randomString();
   char *str2 = randomString();
   char *str3 = randomString();   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {3, 7};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 2, 1);

   htAdd(ht, (void*)str1);
   TEST_UNSIGNED(htCapacity(ht), 3);
   TEST_UNSIGNED(htUniqueEntries(ht), 1);
   TEST_UNSIGNED(htTotalEntries(ht), 1);

   htAdd(ht, (void*)str2);
   TEST_UNSIGNED(htCapacity(ht), 3);
   TEST_UNSIGNED(htUniqueEntries(ht), 2);
   TEST_UNSIGNED(htTotalEntries(ht), 2);

   htAdd(ht, (void*)str3);
   TEST_UNSIGNED(htCapacity(ht), 3);
   TEST_UNSIGNED(htUniqueEntries(ht), 3);
   TEST_UNSIGNED(htTotalEntries(ht), 3);

   htDestroy(ht, 1);
}

static void testCoreTest18()
{
   void *ht;
   char *str1 = randomString();
   char *str2 = randomString();
   char *str3 = randomString();
   int i; 
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {23};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 1, .6);

   htAdd(ht, (void*)str1);
   TEST_UNSIGNED(htCapacity(ht), 23);
   TEST_UNSIGNED(htUniqueEntries(ht), 1);
   TEST_UNSIGNED(htTotalEntries(ht), 1);

   htAdd(ht, (void*)str2);
   TEST_UNSIGNED(htCapacity(ht), 23);
   TEST_UNSIGNED(htUniqueEntries(ht), 2);
   TEST_UNSIGNED(htTotalEntries(ht), 2);

   htAdd(ht, (void*)str3);
   TEST_UNSIGNED(htCapacity(ht), 23);
   TEST_UNSIGNED(htUniqueEntries(ht), 3);
   TEST_UNSIGNED(htTotalEntries(ht), 3);

   for (i=0; i<30; i++)
      htAdd(ht, randomString());

   htDestroy(ht, 1);
}

static void testCoreTest19()
{
   void *ht;
   unsigned size;
   HTEntry *data;
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {3};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 1, .6);

   data = htToArray(ht, &size);

   TEST_BOOLEAN((data == NULL), 1);
   TEST_SIGNED(size, 0);

   htDestroy(ht, 1);
}

static void testCoreTest21()
{
   void *ht;
   unsigned size;
   HTEntry *dataArray;
   char *str[100];
   int i;
   int j = 0;
   int found = 0;

   unsigned sizes[] = {7, 23, 51, 73, 101};

   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 5, .65);

   for (i=0; i<100; i++)
   {
      str[i] = randomString();
      htAdd(ht, str[i]);
   }

   dataArray = htToArray(ht, &size);
   TEST_SIGNED(size, 100);

   for (i=0; i<100; i++)
   {
      j = 0;
      while (j < size)
      {
         if (compareString((void*)str[i], dataArray[j].data) == 0)
         {
            found ++;
            break;
         }
         j++;
      }
   }

   TEST_SIGNED(found, size);

   free(dataArray);
   htDestroy(ht, 1);
}

static void testCoreTest22()
{
   void *ht;
   HTMetrics metrics;
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {3};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 1, .6);

   metrics = htMetrics(ht);

   TEST_UNSIGNED(metrics.numberOfChains, 0);
   TEST_UNSIGNED(metrics.maxChainLength, 0);
   TEST_REAL(metrics.avgChainLength, 0.0, 0.0);

   htDestroy(ht, 1);
}

static void testCoreTest24()
{
   /* Set up the test */
   void *ht;
   int i;
   HTMetrics metrics;
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {23, 31, 53, 67, 103};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 5, .6);

   /* Call the function being tested */
   for (i=1; i<105; i++)
   {
      htAdd(ht, randomString());
      TEST_UNSIGNED(htUniqueEntries(ht), i);
      TEST_UNSIGNED(htTotalEntries(ht), i);
   }

   metrics = htMetrics(ht);
   TEST_UNSIGNED(metrics.numberOfChains, 62);
   TEST_UNSIGNED(metrics.maxChainLength, 4);
   TEST_REAL(metrics.avgChainLength, 1.67742, 0.000001);

   htDestroy(ht, 1);

}

static void testCoreTest26()
{
   void *ht;
   int i;
   HTMetrics metrics;
   strStruct *s = makeStruct();

   unsigned sizes[] = {23, 103, 307, 701, 907, 1107};

   HTFunctions functions = {hashStruct, compareStruct, destrotStruct};

   ht = htCreate(&functions, sizes, 6, .73);

   for (i=0; i<900; i++)
      htAdd(ht, makeStruct());

   TEST_UNSIGNED(htCapacity(ht), 1107);
   TEST_UNSIGNED(htUniqueEntries(ht), 900);
   TEST_UNSIGNED(htTotalEntries(ht), 900);

   for (i=0; i<100; i++)
      htAdd(ht, s);

   TEST_UNSIGNED(htCapacity(ht), 1107);
   TEST_UNSIGNED(htUniqueEntries(ht), 901);
   TEST_UNSIGNED(htTotalEntries(ht), 1000);

   metrics = htMetrics(ht);

   TEST_UNSIGNED(metrics.numberOfChains, 617);
   TEST_UNSIGNED(metrics.maxChainLength, 5);
   TEST_REAL(metrics.avgChainLength, 1.45089, 0.01);

   htDestroy(ht, 1);
}

static void testCoreTest27()
{
   void *ht;
   int i;
   int j = 0;
   HTMetrics metrics;
   HTEntry *data;
   unsigned size;
   strStruct *s[900];
   int found = 0;

   unsigned sizes[] = {23, 103, 307, 701, 907, 1107};

   HTFunctions functions = {hashStruct, compareStruct, destrotStruct};

   ht = htCreate(&functions, sizes, 6, .73);

   for (i=0; i<900; i++)
   {
      s[i] = makeStruct();
      htAdd(ht, s[i]);
   }

   TEST_UNSIGNED(htCapacity(ht), 1107);
   TEST_UNSIGNED(htUniqueEntries(ht), 900);
   TEST_UNSIGNED(htTotalEntries(ht), 900);

   for (i=0; i<100; i++)
      htAdd(ht, s[i]);

   TEST_UNSIGNED(htCapacity(ht), 1107);
   TEST_UNSIGNED(htUniqueEntries(ht), 900);
   TEST_UNSIGNED(htTotalEntries(ht), 1000);

   data = htToArray(ht, &size);
   TEST_UNSIGNED(size, 900);


   for (i=0; i<900; i++)
   {
      j = 0;
      while (j < size)
      {
         if (compareStruct((void*)s[i], data[j].data) == 0)
         {
            found ++;
            break;
         }
         j++;
      }
   }

   TEST_SIGNED(found, 900);


   metrics = htMetrics(ht);

   TEST_UNSIGNED(metrics.numberOfChains, 615);
   TEST_UNSIGNED(metrics.maxChainLength, 4);
   TEST_REAL(metrics.avgChainLength, 1.46341, 0.00001);

   free(data);
   htDestroy(ht, 1);
}

static void testHeapTest1()
{
   /* Set up the test */
   void *ht;
   int i;
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {23, 103, 307, 701, 907, 1107};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 6, .75);

   /* Call the function being tested */
   for (i=0; i<1000; i++)
      htAdd(ht, randomString());

   /* Clean up */
   htDestroy(ht, 1); /* destroyData true: There is data and it's dynamic */ 
}

static void testHeapTest2()
{
   /* Set up the test */
   void *ht;
   int i;
   HTEntry *valArray;
   unsigned size;
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {23, 103, 307, 701, 907, 1107};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 6, .75);

   /* Call the function being tested */
   for (i=0; i<1000; i++)
      htAdd(ht, randomString());

   valArray = htToArray(ht, &size);
   TEST_UNSIGNED(size, 1000)
   free(valArray);

   /* Clean up */
   htDestroy(ht, 1); /* destroyData true: There is data and it's dynamic */ 
}

static void testHeapTest3()
{
   /* Set up the test */
   void *ht;
   int i;
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {23, 103, 307, 701, 907, 1107, 5003, 10061};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 8, .75);

   /* Call the function being tested */
   for (i=0; i<10000; i++)
      htAdd(ht, randomString());

   /* Clean up */
   htDestroy(ht, 1); /* destroyData true: There is data and it's dynamic */ 
}

static void testHeapTest4()
{
   /* Set up the test */
   void *ht;
   int i;
   HTEntry *valArray;
   unsigned size;
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {23, 103, 307, 701, 907, 1107, 5003, 10061};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 8, .75);

   /* Call the function being tested */
   for (i=0; i<10000; i++)
      htAdd(ht, randomString());

   valArray = htToArray(ht, &size);
   TEST_UNSIGNED(size, 10000)
   free(valArray);

   /* Clean up */
   htDestroy(ht, 1); /* destroyData true: There is data and it's dynamic */ 
}

static void testCPUTest1()
{
   /* Set up the test */
   void *ht;
   int i;
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {23, 103, 307, 701, 907, 1107, 5003, 10061};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 8, .75);

   /* Call the function being tested */
   for (i=0; i<10000; i++)
      htAdd(ht, randomString());

   /* Clean up */
   htDestroy(ht, 1); /* destroyData true: There is data and it's dynamic */ 
}

static void testCPUTest2()
{
   /* Set up the test */
   void *ht;
   int i;
   HTEntry *valArray;
   unsigned size;
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {23, 103, 307, 701, 907, 1107, 5003, 10061};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 8, .75);

   /* Call the function being tested */
   for (i=0; i<10000; i++)
      htAdd(ht, randomString());

   valArray = htToArray(ht, &size);
   TEST_UNSIGNED(size, 10000)
   free(valArray);

   /* Clean up */
   htDestroy(ht, 1); /* destroyData true: There is data and it's dynamic */ 
}

static void testCPUTest3()
{
   /* Set up the test */
   void *ht;
   int i;
   char *str[100000];
   int freq;
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {23, 103, 307, 701, 907, 1107, 5003, 10061, 50237, 100391};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 10, .75);

   /* Call the function being tested */
   for (i=0; i<100000; i++)
   {
      str[i] = randomString();
      freq = htAdd(ht, str[i]);
      if (freq > 1)
         free(str[i]);
   }

   /* Clean up */
   htDestroy(ht, 1); /* destroyData true: There is data and it's dynamic */ 
}

static void testCPUTest4()
{
   /* Set up the test */
   void *ht;
   int i;
   HTEntry *valArray;
   unsigned size;
   char *str[100000];
   int freq = 0;
   
   /* Note that the arra is on the stack - htCreate must make a deep copy! */
   unsigned sizes[] = {23, 103, 307, 701, 907, 1107, 5003, 10061, 50237, 100391};

   /* Note that struct is on the stack - htCreate must make a deep copy! */
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 10, .75);

   /* Call the function being tested */
   for (i=0; i<100000; i++)
   {
      str[i] = randomString();
      freq = htAdd(ht, str[i]);
      if (freq > 1)
         free(str[i]);
   }

   valArray = htToArray(ht, &size);
   TEST_UNSIGNED(size, 99999)
   free(valArray);

   /* Clean up */
   htDestroy(ht, 1); /* destroyData true: There is data and it's dynamic */ 
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
      testDeepCopy,
      testAddSingleEntry,
      testAddMultSame,
      testAddMultNoRehash,
      testAddMultRehash,
      testMultRehash,
      testAddMultEntries,
      testAddMultEntries2,
      testLookUp,
      testLookUp2,
      testCoreTest8,
      testCoreTest9,
      testCoreTest10,
      testCoreTest12,
      testCoreTest13,
      testCoreTest15,
      testCoreTest16,
      testCoreTest17,
      testCoreTest18,
      testCoreTest19,
      testCoreTest21,
      testCoreTest22,
      testCoreTest24,
      testCoreTest26,
      testCoreTest27,
      testHeapTest1,
      testHeapTest2,
      testHeapTest3,
      testHeapTest4,
      testCPUTest1,
      testCPUTest2,
      testCPUTest3,
      testCPUTest4,
      NULL
   };

   /* The array of "special" test functions - MUST be run one at a time */
   TEST_FUNC specialFuncs[] = {
      testCoreTest1,
      testCoreTest2,
      testCoreTest3,
      testCoreTest4,
      testCoreTest5,
      testCoreTest6,
      testCoreTest7,
      testCoreTest11,
      testCoreTest14,
      testHeapTest1,
      testHeapTest2,
      testHeapTest3,
      testHeapTest4,
      testCPUTest1,
      testCPUTest2,
      testCPUTest3,
      testCPUTest4,
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
