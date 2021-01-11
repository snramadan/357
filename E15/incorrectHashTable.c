#include <limits.h>
#include "hashTable.h"

void* htCreate(
   HTFunctions *functions,
   unsigned sizes[],
   int numSizes,
   float rehashLoadFactor)
{
   /* Wrong answer */
   return NULL;
}

void htDestroy(void *hashTable, int destroyData)
{
   /* void function, can't return a wrong answer but can do nothing! */
}

unsigned htAdd(void *hashTable, void *data)
{
   /* Best "wrong" answer I can think of */
   return 0;
}

HTEntry htLookUp(void *hashTable, void *data)
{
   HTEntry entry;

   /* Excellent "wrong" answer - combination should never happen! */
   entry.data = "This is a test";
   entry.frequency = 0;
   
   return entry;
}

HTEntry* htToArray(void *hashTable, unsigned *size)
{
   /* Excellent "wrong" answer - should never happen!*/
   *size = UINT_MAX;
   return NULL;
}

unsigned htCapacity(void *hashTable)
{
   /* Best "wrong" answer I can think of */
   return UINT_MAX;
}

unsigned htUniqueEntries(void *hashTable)
{
   /* Best "wrong" answer I can think of */
   return UINT_MAX;
}

unsigned htTotalEntries(void *hashTable)
{
   /* Best "wrong" answer I can think of */
   return UINT_MAX;
}

HTMetrics htMetrics(void *hashTable)
{
   HTMetrics metrics;

   /* Best "wrong" answers I can think of */
   metrics.numberOfChains = UINT_MAX;
   metrics.maxChainLength = UINT_MAX;
   metrics.avgChainLength = UINT_MAX;

   return metrics;
}
