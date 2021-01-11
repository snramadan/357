#include <stdio.h>

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

double getVolume(double l, double w, double h)
{
   return l * w * h;
}

double getSurArea(double l, double w, double h)
{
   return 2 * ((l*w) + (l*h) + (w*h));
}

void showResults(double l, double w, double h)
{
   printf("V=%.2f SA=%.2f", getVolume(l, w, h), getSurArea(l, w, h));
   printf(" (Cuboid: L=%.2f W=%.2f H=%.2f)\n", l, w, h);
}

int main()
{
   double l, w, h;

   l = getLength();
   w = getWidth();
   h = getHeight();
   showResults(l, w, h);

   return 0;
}
