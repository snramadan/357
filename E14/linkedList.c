#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"
#include "memmon.h"

ListNode* addHead(ListNode *list, int value)
{
   ListNode *n = (ListNode*)malloc(sizeof(ListNode));

   if (n == NULL)
   {
      fprintf(stderr, "malloc failure attempting to add %d\n", value);
      exit(EXIT_FAILURE);
   }

   n->value = value;
   if (list != NULL)
   {
      n->next = list;
   }
   else
   {
      n->next = NULL;
   }
 
   list = n;
 
   return list;
}

ListNode* addTail(ListNode *list, int value)
{
   ListNode *n = (ListNode*)malloc(sizeof(ListNode));
   ListNode *temp = list;

   if (n == NULL)
   {
      fprintf(stderr, "malloc failure attempting to add %d\n", value);
      exit(EXIT_FAILURE);
   }

   n->value = value;
   if (list != NULL)
   {
      while(temp->next != NULL)
      {
         temp = temp->next;
      }
      n->next = NULL;
      temp->next = n;
   }
   else
   {
      list = n;
      n->next = NULL;
   }

   return list;
}

ListNode* deleteNode(ListNode *list, int index)
{
   ListNode *n = (ListNode*)malloc(sizeof(ListNode));
   ListNode *t;
   int i;

   n = list;

   if (index == 0 && n->next == NULL)
   {
      free(list); 
      return NULL;  
   }
   if (index == 0 && n->next != NULL)
   {
      t = n;
      n = n->next;
      free(t);
      list = n;
      return list;
   }
   for (i = 0; i < index - 1; i++)
   {   
      if (n == NULL)
      {
         printf("Index %d is out of bounds\n", index);
         return list;
      }
   }
   t = n->next;
   n->next = n->next->next;
   free(t);

   list = n;

   return list;    
}

void printList(ListNode *list)
{
   ListNode *n = (ListNode*)malloc(sizeof(ListNode));   

   if (list == NULL)
   {
      printf("List: Empty");
   }

   n = list;
   printf("List:");
   while(n != NULL)
   { 
      printf(" %d", n->value);
      n = n->next;
   }
   printf("\n");
}
