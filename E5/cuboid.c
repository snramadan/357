#include "cuboid.h"

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
