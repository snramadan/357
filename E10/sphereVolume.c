#include <stdio.h>
#include <assert.h>
#define PI 3.141592

double sphereVolume(double radius, double pi)
{
   assert (radius > 0);
   
   return (4.0/3) * pi * (double)(radius * radius * radius);
}
