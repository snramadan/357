#ifndef UNITTESTA_H
#define UNITTESTA_H

void testBooleanA(long actual, long expect);
void testCharA(char actual, char expect);
void testSignedA(long actual, long expect);
void testUnsignedA(unsigned long actual, unsigned long expect);
void testRealA(double actual, double expect, double epsilon);
void testStringA(const char *actual, const char *expect);

#endif
