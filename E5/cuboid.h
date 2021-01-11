#ifndef CUBOID_H
   #define CUBOID_H

   typedef struct
   {
      double l, w, h;
   } Cuboid;

   Cuboid makeCuboid(double length, double width, double height);

   double getVolume(Cuboid c);

   double getSurArea(Cuboid *c);

#endif
