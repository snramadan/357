#ifndef MASTERMIND_H
   #define MASTERMIND_H

   typedef struct
   {
      int seed;
      char letter;
      int pos;
      int numGuess;
      char word[9];
   } Mastermind;

   Mastermind makeMaster(int s, char l, int p, int nG);

   int findMin(int a, int b);

   int exact(char word[], char g[], int size);

   int inexact(char word[], char g[], int size, int x);
    
   void Results(int temp[]);

   void guesses(Mastermind m);

#endif
