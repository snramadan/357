#include <stdio.h>

int main()
{
   double a, b, c, volume, sa;
   
   printf("Enter the length of the cuboid: ");
   scanf("%lf", &a);
   printf("Enter the  width of the cuboid: ");
   scanf("%lf", &b);
   printf("Enter the height of the cuboid: ");
   scanf("%lf", &c);
   
   volume = a*b*c;
   sa = 2 * ((a*b) + (a*c) + (b*c));
   
   printf("V=%.2f SA=%.2f", volume, sa); 
   printf(" (Cuboid: L=%.2f W=%.2f H=%.2f)\n" ,a, b, c);
   
   return 0;
}
