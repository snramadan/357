#include <stdio.h>
#include "cuboid.h"

double getLength()
{
   double l;

   printf("Enter the length of the cuboid: ");
   scanf("%lf", &l);

   return l;
}

double getWidth()
{
   double w;

   printf("Enter the  width of the cuboid: ");
   scanf("%lf", &w);

   return w;
}

double getHeight()
{
   double h;

   printf("Enter the height of the cuboid: ");
   scanf("%lf", &h);

   return h;
}

void showResults(Cuboid c)
{
   printf("V=%.2f SA=%.2f", getVolume(c), getSurArea(&c));
   printf(" (Cuboid: L=%.2f W=%.2f H=%.2f)\n", c.l, c.w, c.h);
}
