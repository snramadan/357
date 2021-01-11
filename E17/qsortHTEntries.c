#include <stdio.h>
#include <stdlib.h>
#include "qsortHTEntries.h"
#include "getWord.h"
#include "string.h"

static int strcomp(Word *X, Word *Y)
{
   unsigned i;
   int size = 0;
   int b = 0;

   if (X->length > Y->length)
   {
      i = Y->length;
   }  
   else if (X->length <= Y->length)
   {
      i = X->length;
      size = 1;
   }
   b = memcmp(X->bytes, Y->bytes, i);
   if (b == 0)
   {
      if (size == 0)
      {
         b = 1;
      }
      else if (size == 1)
      {
         b = -1;
      }
   }     
   return b;
}

static int compareHTEntries(const void *x, const void *y)
{
   HTEntry *i = (HTEntry*)x;
   HTEntry *j = (HTEntry*)y;

   if (i->frequency == j->frequency)
   { 
      return strcomp((Word*)i->data, (Word*)j->data);
   }
   return (j->frequency - i->frequency);
}

void qsortHTEntries(HTEntry *entries, int numberOfEntries)
{
   qsort(entries, numberOfEntries, sizeof(HTEntry), compareHTEntries);
}
