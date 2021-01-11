#include <stdio.h>

typedef struct
{
   double l, w, h;
} Cuboid;

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

Cuboid makeCuboid(double length, double width, double height)
{
   Cuboid c;

   c.l = length;
   c.w = width;
   c.h = height;

   return c;
}

double getVolume(Cuboid c)
{
   return c.l * c.w * c.h;
}

double getSurArea(Cuboid *c)
{
   return 2 * ((c->l*c->w) + (c->l*c->h) + (c->w*c->h));
}

void showResults(Cuboid c)
{
   printf("V=%.2f SA=%.2f", getVolume(c), getSurArea(&c));
   printf(" (Cuboid: L=%.2f W=%.2f H=%.2f)\n", c.l, c.w, c.h);
}

int main()
{
   double l, w, h;

   l = getLength();
   w = getWidth();
   h = getHeight();
   showResults(makeCuboid(l, w, h));

   return 0;
}
