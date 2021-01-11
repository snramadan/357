#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include "hashTable.h"

typedef struct Node
{
   HTEntry E;
   HTMetrics M;
   struct Node *next;
} Node;

typedef struct
{
   Node **table;
   unsigned *csizes;
   int *chains;
   int index;
   HTFunctions *f;
   float loadFac;
   int numS;
   unsigned unique;
   unsigned total;
} Htable;

void* htCreate(HTFunctions *functions, 
   unsigned sizes[], int numSizes, float rehashLoadFactor)
{
   Htable *h = (Htable*)malloc(sizeof(Htable));
   int i;
 
   assert(numSizes >= 1);
   assert((rehashLoadFactor > 0.0) && (rehashLoadFactor <= 1.0));
   assert(sizes[0] > 0);

   for (i = 0; i < numSizes-1; i++)
   {
      assert(sizes[i] < sizes[(i+1)]);
   } 
   
   h->loadFac = rehashLoadFactor;
   h->numS = numSizes;
   h->unique = 0;
   h->total = 0;
   h->index = 0;
   h->table = (Node**)calloc(sizes[h->index], sizeof(Node*));
   h->chains = (int*)calloc(sizes[0], sizeof(int));
   h->csizes = malloc(numSizes * sizeof(unsigned));

   h->f = malloc(sizeof(HTFunctions));
   memcpy(h->f, functions, sizeof(HTFunctions));

   for (i = 0; i < numSizes; i++)
   {
      h->csizes[i] = sizes[i];
   }

   return h;
}

void htDestroy(void *hashTable, int destroyData)
{
   Htable *h = (Htable*)hashTable;
   Node *current; 
   Node *temp;
   int i;

   for (i = 0; i < (h->csizes[h->index]); i++)
   {
      current = h->table[i];
      while (current != NULL)
      {
         temp = current;
         if ((h->f->destroy != NULL) && (destroyData == 0))
         {
            h->f->destroy(current->E.data);
         }
         else if ((h->f->destroy != NULL) && (destroyData == 1))
         {
            h->f->destroy(current->E.data);
            free(current->E.data);
         }
         else if((h->f->destroy == NULL) && (destroyData == 1))
         {
            free(current->E.data);
         }
         current = current->next;
         free(temp);
      }
   } 

   free(h->table);
   free(h->f);
   free(h->csizes);
   free(h->chains);
   free(h);
}

int needRehash(Htable *h)
{
   int rehash = 0;
   double x;
   x = (double)(h->unique);
   if ((h->loadFac != 1.0) && ((x/h->csizes[h->index]) > h->loadFac))
   {
      if ((h->index)+1 < h->numS)
      {
         rehash = 1;
      }
   }
   return rehash;
}

void addForRehash(Htable *hashTable, Node *t, int index)
{
   Htable *h = (Htable*)hashTable;
 
   if (h->table[index] == NULL)
   {
      t->next = NULL;
   }
   else
   {  
      t->next = h->table[index];
   }
   h->table[index] = t;
   h->chains[index]++;
}

void rehash(Htable *h)
{
   int j;
   unsigned i = 0;
   Node **HT = h->table;
   int *CS = h->chains; 
   Node *temp, *n;
   h->index++;
   h->table = (Node**)calloc(h->csizes[h->index], sizeof(Node*));
   h->chains = calloc(h->csizes[h->index], sizeof(int*));
   i = 0;

   for (j = 0; j < (h->csizes[(h->index)-1]); j++)
   {
      temp = HT[j];
      while (temp != NULL)
      {
         i = (h->f->hash(temp->E.data)) % h->csizes[h->index];
         n = temp->next;
         addForRehash(h, temp, i);
         temp = n;
      }
   }
   free(CS);
   free(HT);
}

void add(Htable *hashTable, int index, HTEntry e)
{
   Htable *h = (Htable*)hashTable;
   Node *tp = (Node*)malloc(sizeof(Node));

   tp->E.data = e.data;
   tp->E.frequency = e.frequency;
   tp->next = NULL;
   if (h->table[index] != NULL)
   {
      tp->next = h->table[index];
   }
   h->table[index] = tp;
}

unsigned repeats(Htable *h, Node *curr, void *data)
{
   while (curr != NULL)
   {
      if (h->f->compare(curr->E.data, data) == 0)
      {  
         curr->E.frequency = curr->E.frequency + 1;
         return curr->E.frequency;
      }
      curr = curr->next;
   }

   return 0;
}

unsigned htAdd(void *hashTable, void *data)
{
   Htable *h = (Htable*)hashTable;
   HTEntry en;
   Node *current;
   unsigned i, freq;
   assert(data != NULL);
   if (needRehash(h) == 1)
   {
      rehash(h);
   }
   i = (h->f->hash(data)) % h->csizes[h->index];
   current = h->table[i];
   freq = repeats(h, current, data);
   h->total++;
   if (freq == 0)
   {
      en.data = data;
      en.frequency = 1;
      add(h, i, en);
      h->unique++;
      h->chains[i]++;
      freq = 1;
   }
   return freq;
}

HTEntry htLookUp(void *hashTable, void *data)
{
   Htable *h = (Htable*)hashTable;
   HTEntry E;
   Node *c;
   unsigned i;
   int j = 0;
   assert(data != NULL);
   E.data = NULL;
   E.frequency = 0;
   i = (h->f->hash(data)) % h->csizes[h->index];
   c = h->table[i];
   while (c != NULL)
   {
      if (h->f->compare(c->E.data, data) == 0)
      { 
         E.data = data;
         E.frequency = c->E.frequency;
         return E;
      }
      c = c->next;
      j++;
   }  
   return E;
}

HTEntry* htToArray(void *hashTable, unsigned *size)
{
   Htable *h = (Htable*)hashTable;
   HTEntry *arr = (HTEntry*)malloc((h->unique) * sizeof(HTEntry));
   Node *c;
   int i, j;
   j = 0;
   if((h->unique) == 0)
   {
      free(arr);
      *size = 0;
      return NULL;
   }
   
   for (i = 0; i < (h->csizes[h->index]); i++)
   { 
      c = h->table[i];
      while (c != NULL)
      {
         arr[j] = c->E;
         c = c->next;
         j++;
      }
   }

   *size = (h->unique);
   return arr;
}

unsigned htCapacity(void *hashTable)
{
   Htable *h = (Htable*)hashTable;
   return h->csizes[h->index];
}

unsigned htUniqueEntries(void *hashTable)
{
   Htable *h = (Htable*)hashTable;
   return h->unique;
}

unsigned htTotalEntries(void *hashTable)
{
   Htable *h = (Htable*)hashTable;
   return h->total;
}

unsigned findMaxChain(Htable *h)
{
   unsigned m;
   int i;
   m = 0;

   for (i = 0; i < h->csizes[h->index]; i++)
   {
      if (h->chains[i] > m)
      {
         m = h->chains[i];
      }
   }

   return m;
}

HTMetrics htMetrics(void *hashTable)
{
   Htable *h = (Htable*)hashTable;
   HTMetrics M;
   int i, j;
   unsigned total, count, Max;
   float average;
   j = 0;
   total = 0;
   count = 0;
   Max = 0;

   for (i = 0; i < h->csizes[h->index]; i++)
   {
      if (h->chains[i] >= 1)
      {
         count++;
         total = total + h->chains[i];
         j++;
      }
   }      
   Max = findMaxChain(h);
   average = total/(float)j;

   M.numberOfChains = count;
   M.maxChainLength = Max;
   M.avgChainLength = average;

   return M;

}
