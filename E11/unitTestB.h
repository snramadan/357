#ifndef UNITTESTB_H
#define UNITTESTB_H

void testBooleanB(long actual, long expect);
void testCharB(char actual, char expect);
void testSignedB(long actual, long expect);
void testUnsignedB(unsigned long actual, unsigned long expect);
void testRealB(double actual, double expect, double epsilon);
void testStringB(const char *actual, const char *expect);

#endif
