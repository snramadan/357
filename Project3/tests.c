#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
unsigned hashString(const void *data)
{
   unsigned hash;
   const char *str = data;

   for (hash = 0; *str;  str++)
      hash = *str + 31 * hash;

   /* Return the raw hash value - the modulo must be done by caller */
   return hash;
}

int compareString(const void *a, const void *b)
{
   return strcmp(a, b);
}

/* Good practice suggests:
 *    1. Choosing descriptive test function names.
 *    2. Including a brief description of each test.
 *
 * Test creating an empty hash table.
 */

static void test8()
{
   void *ht;
   unsigned sizes[] = {7};
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 1, 0.73);
   printf("line 61\n");
   TEST_UNSIGNED(htCapacity(ht), 7);
   TEST_UNSIGNED(htUniqueEntries(ht), 0);
   TEST_UNSIGNED(htTotalEntries(ht), 0);

   htDestroy(ht, 0);
}

static void testAddSingleEntry()
{
   void *ht;
   unsigned sizes[] = {11};
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 1, 0.73);
   printf("line 76\n");
   htAdd(ht, "Random Test String");
 
   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 1);
   TEST_UNSIGNED(htTotalEntries(ht), 1);
 
   htDestroy(ht, 0);
}

static void test10()
{
   void *ht;
   void *np;
   unsigned sizes[] = {11};
   HTFunctions functions = {hashString, compareString, NULL};
   printf("line 92\n");
   np = NULL;

   ht = htCreate(&functions, sizes, 1, 0.73);

   TEST_ERROR(htAdd(ht, np));
}

static void testAddThree()
{
   void *ht;
   unsigned sizes[] = {9, 11};
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 2, 1.00);

   htAdd(ht, "first thing");
   htAdd(ht, "second thing");
   htAdd(ht, "first thing");
   printf("line 111\n");
   TEST_UNSIGNED(htCapacity(ht), 9);
   TEST_UNSIGNED(htUniqueEntries(ht), 2);
   TEST_UNSIGNED(htTotalEntries(ht), 3);

   htDestroy(ht, 0);
}

static void testAddChars()
{
   void *ht;
   unsigned sizes[] = {3};
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 1, 0.73);

   htAdd(ht, "j");
   htAdd(ht, "h");
   htAdd(ht, "p");
   printf("line 130\n");
   TEST_UNSIGNED(htCapacity(ht), 3);
   TEST_UNSIGNED(htUniqueEntries(ht), 3);
   TEST_UNSIGNED(htTotalEntries(ht), 3);

   htDestroy(ht, 0);
}

static void test11()
{
   void *ht;
   unsigned sizes[] = {3};
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 1, 0.73);
   printf("line 145\n");
   TEST_UNSIGNED(htAdd(ht, "j"), 1);

   TEST_UNSIGNED(htCapacity(ht), 3);
   TEST_UNSIGNED(htUniqueEntries(ht), 1);
   TEST_UNSIGNED(htTotalEntries(ht), 1);

   htDestroy(ht, 0);
}
static void test1()
{
   unsigned sizes[] = {7};
   HTFunctions functions = {hashString, compareString, NULL};
   printf("line 158\n");
   TEST_ERROR(htCreate(&functions, sizes, 0, 0.73));

}

static void test2()
{
   unsigned sizes[] = {7};
   HTFunctions functions = {hashString, compareString, NULL};
   printf("line 167\n");
   TEST_ERROR(htCreate(&functions, sizes, -5, 0.73));

}

static void test2a()
{
   unsigned sizes[] = {-3};
   HTFunctions functions = {hashString, compareString, NULL};
   printf("line 176\n");
   TEST_ERROR(htCreate(&functions, sizes, -3, 0.73));

}

static void test3()
{
   unsigned sizes[4] = {5, 10, 0, 20};
   HTFunctions functions = {hashString, compareString, NULL};
   printf("line 185\n");
   TEST_ERROR(htCreate(&functions, sizes, 4, 0.73));

}

static void test4()
{
   unsigned sizes[4] = {2, 3, 3, 6};
   HTFunctions functions = {hashString, compareString, NULL};
   printf("line 194\n");
   TEST_ERROR(htCreate(&functions, sizes, 4, 0.73));

}

static void test5()
{
   unsigned sizes[4] = {1, 7, 2, 9};
   HTFunctions functions = {hashString, compareString, NULL};
   printf("line 203\n");
   TEST_ERROR(htCreate(&functions, sizes, 4, 0.73));

}

static void test13()
{
   void* ht;
   unsigned sizes[4] = {1, 7, 2, 9};
   HTFunctions functions = {hashString, compareString, NULL};
   printf("line 213\n");
   ht = htCreate(&functions, sizes, 1, 0.73);
   TEST_ERROR(htLookUp(ht, NULL));

}
static void test6()
{
   unsigned sizes[] = {13};
   HTFunctions functions = {hashString, compareString, NULL};
   printf("line 222\n");
   TEST_ERROR(htCreate(&functions, sizes, 4, 0));

}

static void test7()
{
   unsigned sizes[] = {13};
   HTFunctions functions = {hashString, compareString, NULL};
   printf("line 231\n");
   TEST_ERROR(htCreate(&functions, sizes, 4, 2.3));

}

static void test9()
{
   void *ht;
   unsigned sizes[3] = {9, 18, 20};
   HTFunctions functions = {hashString, compareString, NULL};
   printf("line 241\n");
   ht = htCreate(&functions, sizes, 3, 0.73);
   sizes[0] = 6;
   TEST_UNSIGNED(htCapacity(ht), 9);

   htDestroy(ht, 0);
}

static void testErrorZeroSize()
{
   void *ht;
   unsigned sizes[] = {0};
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 1, 0.73);
   printf("line 256\n");
   TEST_ERROR(htAdd(ht, "jp"));

}

static void testAddInts()
{
   void *ht;
   unsigned sizes[] = {20};
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 1, 0.73);
   htAdd(ht, "12");
   htAdd(ht, "15");
   printf("line 270\n");
   TEST_UNSIGNED(htCapacity(ht), 20);
   TEST_UNSIGNED(htUniqueEntries(ht), 2);
   TEST_UNSIGNED(htTotalEntries(ht), 2);

   htDestroy(ht, 0);
}

static void test12()
{
   void *ht;
   unsigned sizes[2] = {5,11};
   HTFunctions functions = {hashString, compareString, NULL};

   ht = htCreate(&functions, sizes, 1, 0.73);
   htAdd(ht, "12");
   htAdd(ht, "15");
   htAdd(ht, "12");
   htAdd(ht, "16");
   htAdd(ht, "13");
   htAdd(ht, "15");
   htAdd(ht, "14");
   printf("line 292\n");
   TEST_UNSIGNED(htCapacity(ht), 5);
   TEST_UNSIGNED(htUniqueEntries(ht), 5);
   TEST_UNSIGNED(htTotalEntries(ht), 7);

   htDestroy(ht, 0);
}

static void test14()
{
   void *ht;
   unsigned sizes[2] = {5,11};
   char ch[3];
   char* v;
   HTFunctions functions = {hashString, compareString, NULL};
   ch[0] = '1';
   ch[1] = '3';
   ch[2] = '\0';
   printf("line 310\n");
   ht = htCreate(&functions, sizes, 2, 0.73);
   htAdd(ht, "12");
   TEST_UNSIGNED(htLookUp(ht, ch).frequency, 0);
   htAdd(ht, ch);
   TEST_UNSIGNED(htLookUp(ht, ch).frequency, 1);
   v = (char*)htLookUp(ht, ch).data;
   TEST_CHAR(*v, *ch);
   htAdd(ht, ch);
   TEST_UNSIGNED(htLookUp(ht, ch).frequency, 2);
   v = (char*)htLookUp(ht, ch).data;
   TEST_CHAR(*v, *ch);

   htDestroy(ht, 0);
}

static void test15()
{
   void *ht;
   unsigned sizes[3] = {2, 3, 5};
   HTFunctions functions = {hashString, compareString, NULL};
   printf("line 333\n");

   ht = htCreate(&functions, sizes, 3, 0.73);
   htAdd(ht, "12");
   htAdd(ht, "15");
   htAdd(ht, "12");
   TEST_UNSIGNED(htCapacity(ht), 3);
   htAdd(ht, "16");
   htAdd(ht, "13");
   htAdd(ht, "15");
   htAdd(ht, "14");

   TEST_UNSIGNED(htCapacity(ht), 5);
   TEST_UNSIGNED(htUniqueEntries(ht), 5);
   TEST_UNSIGNED(htTotalEntries(ht), 7);
   TEST_UNSIGNED(htLookUp(ht, "12").frequency, 2);
   TEST_UNSIGNED(htLookUp(ht, "15").frequency, 2);
   TEST_UNSIGNED(htLookUp(ht, "13").frequency, 1);

   htDestroy(ht, 0);
}

static void test16()
{
   void *ht;
   unsigned sizes[3] = {2, 3, 5};
   HTFunctions functions = {hashString, compareString, NULL};
   printf("line 361\n");
   ht = htCreate(&functions, sizes, 3, 1.0);
   htAdd(ht, "12");
   htAdd(ht, "15");
   htAdd(ht, "12");
   TEST_UNSIGNED(htCapacity(ht), 2);
   htAdd(ht, "16");
   htAdd(ht, "13");
   htAdd(ht, "15");
   htAdd(ht, "14");

   TEST_UNSIGNED(htCapacity(ht), 2);
   TEST_UNSIGNED(htUniqueEntries(ht), 5);
   TEST_UNSIGNED(htTotalEntries(ht), 7);
   TEST_UNSIGNED(htLookUp(ht, "12").frequency, 2);
   TEST_UNSIGNED(htLookUp(ht, "15").frequency, 2);
   TEST_UNSIGNED(htLookUp(ht, "13").frequency, 1);

   htDestroy(ht, 0);
}
/*
static void test26()
{
   void *ht;
   unsigned sizes[3] = {2, 3, 5};
   HTFunctions functions = {hashString, compareString, NULL};

   printf("line 388\n");
   ht = htCreate(&functions, sizes, 3, 0.6);
   htAdd(ht, "12");
   htAdd(ht, "12");	
   htAdd(ht, "15");
   TEST_UNSIGNED(htCapacity(ht), 3);
   htAdd(ht, "16");
   htAdd(ht, "13");
   htAdd(ht, "15");
   htAdd(ht, "14");

   TEST_UNSIGNED(htCapacity(ht), 5);
   TEST_UNSIGNED(htUniqueEntries(ht), 5);
   TEST_UNSIGNED(htTotalEntries(ht), 7);
   TEST_UNSIGNED(htLookUp(ht, "12").frequency, 2);
   TEST_UNSIGNED(htLookUp(ht, "15").frequency, 2);
   TEST_UNSIGNED(htLookUp(ht, "13").frequency, 1);

   htDestroy(ht, 0);
}
*/
static void test17()
{
   void *ht;
   unsigned sizes[3] = {1, 2, 3};
   HTFunctions functions = {hashString, compareString, NULL};


   ht = htCreate(&functions, sizes, 3, 1.0);
   htAdd(ht, "12");
   htAdd(ht, "15");
   htAdd(ht, "12");
   TEST_UNSIGNED(htCapacity(ht), 1);
   htAdd(ht, "16");
   htAdd(ht, "13");
   htAdd(ht, "15");
   htAdd(ht, "14");
   printf("line 425\n");
   TEST_UNSIGNED(htCapacity(ht), 1);
   TEST_UNSIGNED(htUniqueEntries(ht), 5);
   TEST_UNSIGNED(htTotalEntries(ht), 7);
   TEST_UNSIGNED(htLookUp(ht, "12").frequency, 2);
   TEST_UNSIGNED(htLookUp(ht, "15").frequency, 2);
   TEST_UNSIGNED(htLookUp(ht, "13").frequency, 1);

   htDestroy(ht, 0);
}
/*
static void test18()
{
   void *ht;
   unsigned sizes[2] = {5,11};
   unsigned p;
   HTEntry* t;
   HTFunctions functions = {hashString, compareString, NULL};
   printf("line 442\n");
   ht = htCreate(&functions, sizes, 2, 0.73);
   t = htToArray(ht, &p);
   TEST_UNSIGNED(t->frequency, 1);
   TEST_UNSIGNED(p, 0);

   htDestroy(ht, 0);
}
*/
static void test19()
{
   void *ht;
   unsigned sizes[2] = {10, 15};
   unsigned p;
   HTEntry* t;

   HTFunctions functions = {hashString, compareString, NULL};
   printf("line 461\n");
   ht = htCreate(&functions, sizes, 2, 0.73);
   htAdd(ht, "12");
   htAdd(ht, "15");

   t = htToArray(ht, &p);
   TEST_UNSIGNED(p, 2);
   TEST_UNSIGNED(t->frequency, 1);
   TEST_UNSIGNED(htCapacity(ht), 10);
   TEST_UNSIGNED(htUniqueEntries(ht), 2);
   TEST_UNSIGNED(htTotalEntries(ht), 2);

   free(t);
   TEST_UNSIGNED(htCapacity(ht), 10);
   TEST_UNSIGNED(htUniqueEntries(ht), 2);
   TEST_UNSIGNED(htTotalEntries(ht), 2);
   htDestroy(ht, 0);
}

static void test20()
{
   void *ht;
   unsigned sizes[3] = {2, 3, 5};
   HTFunctions functions = {hashString, compareString, NULL};
   HTEntry *t;
   unsigned p;
   ht = htCreate(&functions, sizes, 3, 0.73);
   htAdd(ht, "12");
   htAdd(ht, "15");
   htAdd(ht, "12");
   TEST_UNSIGNED(htCapacity(ht), 3);
   htAdd(ht, "16");
   htAdd(ht, "13");
   htAdd(ht, "15");
   htAdd(ht, "14");
   printf("line 496\n");
   t = htToArray(ht, &p);
   TEST_UNSIGNED(p, 5);
   TEST_UNSIGNED(htCapacity(ht), 5);
   TEST_UNSIGNED(htUniqueEntries(ht), 5);
   TEST_UNSIGNED(htTotalEntries(ht), 7);
   TEST_UNSIGNED(htLookUp(ht, "12").frequency, 2);
   TEST_UNSIGNED(htLookUp(ht, "15").frequency, 2);
   TEST_UNSIGNED(htLookUp(ht, "13").frequency, 1);
   free(t);
   TEST_UNSIGNED(htCapacity(ht), 5);
   TEST_UNSIGNED(htUniqueEntries(ht), 5);
   TEST_UNSIGNED(htTotalEntries(ht), 7);
   TEST_UNSIGNED(htLookUp(ht, "12").frequency, 2);
   TEST_UNSIGNED(htLookUp(ht, "15").frequency, 2);
   TEST_UNSIGNED(htLookUp(ht, "13").frequency, 1);

   htDestroy(ht, 0);
}

static void test21()
{
   void *ht;
   unsigned sizes[2] = {3, 5};
   unsigned a, b;
   float c;
   HTFunctions functions = {hashString, compareString, NULL};
   ht = htCreate(&functions, sizes, 2, 0.73);
   a = htMetrics(ht).numberOfChains;
   b = htMetrics(ht).maxChainLength;
   c = htMetrics(ht).avgChainLength;
   printf("line 527\n");
   TEST_UNSIGNED(a, 0);
   TEST_UNSIGNED(b, 0);
   TEST_REAL(c, 0.0, 0.4);
   htDestroy(ht, 0);

}

static void test22()
{
   void *ht;
   unsigned sizes[2] = {7, 11};
   unsigned a, b;
   float c;
   HTFunctions functions = {hashString, compareString, NULL};
   ht = htCreate(&functions, sizes, 2, 0.73);
   htAdd(ht, "J");
   htAdd(ht, "J");
   htAdd(ht, "P");
   htAdd(ht, "W");
   htAdd(ht, "K");
   htAdd(ht, "P");
   printf("line 549\n");
   a = htMetrics(ht).numberOfChains;
   b = htMetrics(ht).maxChainLength;
   c = htMetrics(ht).avgChainLength;
   TEST_UNSIGNED(a, 3);
   TEST_UNSIGNED(b, 2);
   TEST_REAL(c, 1.33, .02);

   htDestroy(ht, 0);
}

static void test23()
{
   void *ht;
   unsigned sizes[2] = {1, 2};
   unsigned a, b;
   float c;
   HTFunctions functions = {hashString, compareString, NULL};
   printf("line 567\n");
   ht = htCreate(&functions, sizes, 2, 0.73);
   htAdd(ht, "12");
   htAdd(ht, "15");
   htAdd(ht, "12");
   TEST_UNSIGNED(htCapacity(ht), 2);
   htAdd(ht, "16");
   htAdd(ht, "13");
   htAdd(ht, "15");
   htAdd(ht, "14");

   TEST_UNSIGNED(htCapacity(ht), 2);
   a = htMetrics(ht).numberOfChains;
   b = htMetrics(ht).maxChainLength;
   c = htMetrics(ht).avgChainLength;
   TEST_UNSIGNED(a, 2);
   TEST_UNSIGNED(b, 3);
   TEST_REAL(c, 2.5, 0.1);

   htDestroy(ht, 0);
}

typedef struct {
   char *str;
} structure;

unsigned strfunc(const void *data)
{
   unsigned hash;
   const char *str = data;

   for (hash = 0; *str;  str++)
      hash = *(((structure*)data)->str) + 31 * hash;

   return hash;
}

int structcmp(const void *a, const void *b)
{
   return strcmp(((structure*)a)->str,((structure*)b)->str);
}

void strdestroy(const void *data) {
   free(((structure*)data)->str);
}
/*
static void test40() {
   void *ht;
   char *a;
   int i, j;
   unsigned sizes[5] = {100, 400, 2000, 8000, 10000};
   const char alpha[] =
      "ABCDEFGHJIKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz123456789";
   HTFunctions functions = {strfunc, structcmp, NULL};
   ht = htCreate(&functions, sizes, 5, 1.00);
   srand(5);
   for (i = 0; i < 1000; i++) {
      a = (char*)calloc(10, sizeof(char));
      for (j = 0; j < 9; j++) {
         a[j] = alpha[rand() % (sizeof(alpha)-1)];
      }
      a[j] = '\0';
      htAdd(ht, a);
   }
   TEST_UNSIGNED(htCapacity(ht), 100);
   TEST_UNSIGNED(htUniqueEntries(ht), 1000);
   TEST_UNSIGNED(htUniqueEntries(ht), 1000);
   htDestroy(ht, 1);
}*/

static void test42() {
   void *ht;
   int i, j;
   unsigned p;
   HTEntry *t;
   char *a;
   unsigned sizes[5] = {100, 400, 2000, 8000, 10000};
   const char alpha[] =
      "ABCDEFGHJIKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz123456789";
   HTFunctions functions = {hashString, compareString, NULL};
   ht = htCreate(&functions, sizes, 5, 1.00);
   printf("line 648\n");
   srand(5);
   for (i = 0; i < 1000; i++) {
      a = (char*)calloc(10, sizeof(char));
      for (j = 0; j < 9; j++) {
         a[j] = alpha[rand() % (sizeof(alpha)-1)];
      }
      a[j] = '\0';
      htAdd(ht, a);
   }
   t = htToArray(ht, &p);
   TEST_UNSIGNED(htCapacity(ht), 100);
   TEST_UNSIGNED(htUniqueEntries(ht), 1000);
   TEST_UNSIGNED(htUniqueEntries(ht), 1000);
   TEST_UNSIGNED(p, 1000);
   free(t);
   TEST_UNSIGNED(htCapacity(ht), 100);
   TEST_UNSIGNED(htUniqueEntries(ht), 1000);
   TEST_UNSIGNED(htUniqueEntries(ht), 1000);
   htDestroy(ht, 1);
}

static void test41() {
   void *ht;
   structure *a;
   int i, j;
   unsigned p;
   HTEntry *t;
   unsigned sizes[3] = {100, 500, 800};
   const char alpha[] =
      "ABCDEFGHJIKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz123456789";
   HTFunctions functions = {strfunc, structcmp, strdestroy};
   ht = htCreate(&functions, sizes, 3, 0.2);
   srand(5);
   printf("line 682\n");
   for (i = 0; i < 1000; i++) {
      a = (structure*)calloc(1, sizeof(structure));
      a->str = (char*)calloc(10, sizeof(char));
      for (j = 0; j < 9; j++) {
         a->str[j] = alpha[rand() % (sizeof(alpha)-1)];
      }
      a->str[j] = '\0';
      htAdd(ht, a);
   }
   t = htToArray(ht, &p);
   free(t);
   TEST_UNSIGNED(p, 1000);
   TEST_UNSIGNED(htCapacity(ht), 800);
   TEST_UNSIGNED(htUniqueEntries(ht), 1000);
   TEST_UNSIGNED(htUniqueEntries(ht), 1000);
   htDestroy(ht, 1);
}

static void test25() {
   void *ht;
   structure *a = (structure*)malloc(sizeof(structure));
   structure *b = (structure*)malloc(sizeof(structure));
   structure *c = (structure*)malloc(sizeof(structure));
   structure *d = (structure*)malloc(sizeof(structure));
   unsigned sizes[5] = {1, 5, 7, 10, 11};
   HTFunctions functions = {strfunc, structcmp, strdestroy};
   ht = htCreate(&functions, sizes, 5, .2);
   a->str = (char*)calloc(6, sizeof(char));
   b->str = (char*)calloc(6, sizeof(char));
   c->str = (char*)calloc(3, sizeof(char));
   d->str = (char*)calloc(4, sizeof(char));
   printf("line 714\n");
   a->str[0] = 'h';
   a->str[1] = 'e';
   a->str[2] = 'l';
   a->str[3] = 'l';
   a->str[4] = 'o';
   a->str[5] = '\0';
   b->str[0] = 'w';
   b->str[1] = 'o';
   b->str[2] = 'r';
   b->str[3] = 'l';
   b->str[4] = 'd';
   b->str[5] = '\0';
   c->str[0] = 'b';
   c->str[1] = 'o';
   c->str[2] = '\0';
   d->str[0] = 'b';
   d->str[1] = 'o';
   d->str[2] = 'b';
   d->str[3] = '\0';
   htAdd(ht, a);
   htAdd(ht, b);
   htAdd(ht, b);
   htAdd(ht, c);
   htAdd(ht, c);
   htAdd(ht, c);
   htAdd(ht, d);
   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 4);
   TEST_UNSIGNED(htTotalEntries(ht), 7);
   htDestroy(ht, 1);
}

static void test24()
{
   void *ht;
   unsigned sizes[5] = {1, 3, 4, 6, 7};
   char *a = "BB6VenC";
   char *b = "1CMpLbt";
   char *c = "OseeR6h";
   char *d = "eswDzK6";
   char *e = "ThfSgZo";
   char *f = "BAo0Dzg";
   char *g = "MONa7s5";
   char *h = "OmaqTMt";
   char *i = "IUna1Is";
   char *j = "WfcdsVg9L";
   char *k = "BB6VenC";
   char *l = "1CMpcsdbt";
   char *m = "OseeR6h";
   char *n = "eswDecs6";
   char *o = "ThfSgZo";
   char *p = "BAo0Dzg";
   char *q = "MONcwea7s5";
   char *r = "OmaqTMt";
   char *s = "IUncawIs";
   char *t = "WfWVg9L";
   HTFunctions functions = {hashString, compareString, NULL};
   ht = htCreate(&functions, sizes, 5, 0.2);
   printf("line 773\n");
   htAdd(ht, a);
   htAdd(ht, b);
   htAdd(ht, c);
   htAdd(ht, d);
   htAdd(ht, e);
   htAdd(ht, f);
   htAdd(ht, g);
   htAdd(ht, h);
   htAdd(ht, i);
   htAdd(ht, j);
   htAdd(ht, k);
   htAdd(ht, l);
   htAdd(ht, m);
   htAdd(ht, n);
   htAdd(ht, o);
   htAdd(ht, p);
   htAdd(ht, q);
   htAdd(ht, r);
   htAdd(ht, s);
   htAdd(ht, t);
   TEST_UNSIGNED(htCapacity(ht), 7);
   TEST_UNSIGNED(htUniqueEntries(ht), 15);
   TEST_UNSIGNED(htTotalEntries(ht), 20);
   htDestroy(ht, 0);
}

typedef unsigned char Byte;

typedef struct
{
   Byte *bytes;
   unsigned length;
} Word;

int wordcmp(const void *a, const void *b)
{
   return strcmp((char*)(((Word*)a)->bytes),(char*)(((Word*)b)->bytes));
}

void worddestroy(const void *data) {
   free(((Word*)data)->bytes);
}

static void test43() {
   void *ht;
   Word *a;
   int i, j;
   unsigned p;
   HTEntry *t;
   unsigned sizes[3] = {10, 50, 80};
   const char alpha[] =
      "ABCDEFGHJIKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz123456789";
   HTFunctions functions = {hashString, wordcmp, worddestroy};
   ht = htCreate(&functions, sizes, 3, 0.2);
   printf("line 828\n");
   srand(5);
   for (i = 0; i < 10; i++) {
      a = (Word*)calloc(1, sizeof(Word));
      a->bytes = (Byte*)calloc(10, sizeof(Byte));
      for (j = 0; j < 9; j++) {
         a->bytes[j] = alpha[rand() % (sizeof(alpha)-1)];
      }
      a->bytes[j] = '\0';
      htAdd(ht, a);
   }
   t = htToArray(ht, &p);
   free(t);
   TEST_UNSIGNED(p, 10);
   TEST_UNSIGNED(htCapacity(ht), 50);
   TEST_UNSIGNED(htUniqueEntries(ht), 10);
   TEST_UNSIGNED(htUniqueEntries(ht), 10);
   htDestroy(ht, 1);
}

/*static void test18()
{
   void *ht;
   unsigned sizes[2] = {5,11};
   unsigned p;
   HTEntry* t;

   HTFunctions functions = {hashString, compareString, NULL};
   ht = htCreate(&functions, sizes, 2, 0.73);
   t = htToArray(ht, &p);
   printf("TEST 18 / Line 845\n");
   TEST_UNSIGNED(p, 0);
   TEST_NULL(t);
   TEST_NULL(htToArray(ht, &p));

   htDestroy(ht, 0);
}*/

/*static void test19()
{
   void *ht;
   unsigned sizes[2] = {10, 15};
   unsigned p;
   HTEntry* t;

   HTFunctions functions = {hashString, compareString, NULL};
   printf("Test 19 / Line 860\n");
   ht = htCreate(&functions, sizes, 2, 0.73);
   htAdd(ht, "12");
   htAdd(ht, "15");

   t = htToArray(ht, &p);
   TEST_UNSIGNED(p, 2);
   TEST_UNSIGNED(t->frequency, 1);
   TEST_UNSIGNED(htCapacity(ht), 10);
   TEST_UNSIGNED(htUniqueEntries(ht), 2);
   TEST_UNSIGNED(htTotalEntries(ht), 2);

   free(t);
   TEST_UNSIGNED(htCapacity(ht), 10);
   TEST_UNSIGNED(htUniqueEntries(ht), 2);
   TEST_UNSIGNED(htTotalEntries(ht), 2);
   htDestroy(ht, 0);
}*/
/*
static void test20()
{
   void *ht;
   unsigned sizes[3] = {2, 3, 5};
   HTFunctions functions = {hashString, compareString, NULL};
   HTEntry *t;
   unsigned p;
   printf("Test 12 / Line 895\n");
   ht = htCreate(&functions, sizes, 3, 0.73);
   htAdd(ht, "12");
   htAdd(ht, "15");
   htAdd(ht, "12");
   TEST_UNSIGNED(htCapacity(ht), 3);
   htAdd(ht, "16");
   htAdd(ht, "13");
   htAdd(ht, "15");
   htAdd(ht, "14");
   t = htToArray(ht, &p);
   TEST_UNSIGNED(htCapacity(ht), 5);
   TEST_UNSIGNED(htUniqueEntries(ht), 5);
   TEST_UNSIGNED(htTotalEntries(ht), 7);
   TEST_UNSIGNED(htLookUp(ht, "12").frequency, 2);
   TEST_UNSIGNED(htLookUp(ht, "15").frequency, 2);
   TEST_UNSIGNED(htLookUp(ht, "13").frequency, 1);
   TEST_UNSIGNED(p, 5);
   free(t);
   TEST_UNSIGNED(htCapacity(ht), 5);
   TEST_UNSIGNED(htUniqueEntries(ht), 5);
   TEST_UNSIGNED(htTotalEntries(ht), 7);
   TEST_UNSIGNED(htLookUp(ht, "12").frequency, 2);
   TEST_UNSIGNED(htLookUp(ht, "15").frequency, 2);
   TEST_UNSIGNED(htLookUp(ht, "13").frequency, 1);

   htDestroy(ht, 0);
}*/

/*typedef struct {
   char *str;
} structure;

unsigned strfunc(const void *data)
{
   unsigned hash;
   const char *str = data;

   for (hash = 0; *str;  str++)
      hash = *(((structure*)data)->str) + 31 * hash;

   return hash;
}

int structcmp(const void *a, const void *b)
{
   return strcmp(((structure*)a)->str,((structure*)b)->str);
}

void strdestroy(const void *data) {
   free(((structure*)data)->str);
}*/

/*static void test25() {
   void *ht;
   int i, j;
   unsigned p;
   HTEntry *t;
   char *a;
   unsigned sizes[5] = {100, 400, 2000, 8000, 10000};
   const char alpha[] =
      "ABCDEFGHJIKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz123456789";
   HTFunctions functions = {hashString, compareString, NULL};
   ht = htCreate(&functions, sizes, 5, 1.00);
   printf("Test 25 / Line 958\n");
   srand(5);
   for (i = 0; i < 1000; i++) {
      a = (char*)calloc(10, sizeof(char));
      for (j = 0; j < 9; j++) {
         a[j] = alpha[rand() % (sizeof(alpha)-1)];
      }
      a[j] = '\0';
      htAdd(ht, a);
   }
   t = htToArray(ht, &p);
   TEST_UNSIGNED(p, 1000);
   TEST_UNSIGNED(htCapacity(ht), 100);
   TEST_UNSIGNED(htUniqueEntries(ht), 1000);
   TEST_UNSIGNED(htUniqueEntries(ht), 1000);
   TEST_UNSIGNED(p, 1000);
   free(t);
   htDestroy(ht, 1);
}*/

static void test252() {
   void *ht;
   structure *a;
   int i, j;
   unsigned p;
   HTEntry *t;
   unsigned sizes[3] = {100, 500, 800};
   const char alpha[] =
      "ABCDEFGHJIKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz123456789";
   HTFunctions functions = {strfunc, structcmp, strdestroy};
   ht = htCreate(&functions, sizes, 3, 0.2);
   printf("line 995\n");
   srand(5);
   for (i = 0; i < 1000; i++) {
      a = (structure*)calloc(1, sizeof(structure));
      a->str = (char*)calloc(10, sizeof(char));
      for (j = 0; j < 9; j++) {
         a->str[j] = alpha[rand() % (sizeof(alpha)-1)];
      }
      a->str[j] = '\0';
      htAdd(ht, a);
   }
   t = htToArray(ht, &p);
   free(t);
   TEST_UNSIGNED(p, 1000);
   TEST_UNSIGNED(htCapacity(ht), 800);
   TEST_UNSIGNED(htUniqueEntries(ht), 1000);
   TEST_UNSIGNED(htUniqueEntries(ht), 1000);
   htDestroy(ht, 1);
}

static void testAll(TEST_FUNC* testFuncs)
{
   int i;

   for (i = 0; testFuncs[i] != NULL; i++)
   {
      printf("Running normal test function %d\n", i);

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

   /* The array of "normal" test functions, all of these run by default */
   TEST_FUNC normalFuncs[] = {
      testAddSingleEntry,
      testAddThree,
      testAddChars,
      testAddInts,
      test8,
      test9,
      test11,
      test12,
      test14,
      test15,
      test16,
      test17,
      /*test18,*/
      test19,
      test20,
      test21,
      test22,
      test23,
      /*test26,*/
      test24,
      test25,
      /*test40,*/
      test41,
      test42,
      test43,
      test25,
      test252,
      test43,
      testCoreTest19,
      testCoreTest26,
      testCoreTest27,
      NULL
   };

   /* The array of "special" test functions - must be run one at a time */
   TEST_FUNC specialFuncs[] = {
      testErrorZeroSize,
      test1,
      test2,
      test2a,
      test3,
      test4,
      test5,
      test6,
      test7,
      test10,
      test13,
      NULL
   };

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
