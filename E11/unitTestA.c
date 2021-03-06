/*
 * Part of the test driver provided to students in CPE 357 to test their unit
 * test macros developed for an Exercise in CPE 357. These functions are
 * similar to those in unitTestB.c but in a different order so, when called
 * their __FILE__ and __LINE__ values will be different.
 *
 * Important note to students:
 *
 *    DO NOT MODIFY THIS FILE! You will be handing this file in with the header
 *    file containing the unit test macros you developed. If you change the
 *    behavior of this program you are not likely to pass the Evaluation
 *    System tests.
 *
 * Author: Kurt Mammen
 * Version:
 *    04/02/2106: Initial version.
 */
#include "unitTest.h"

void testStringA(const char *actual, const char *expect)
{
   TEST_STRING(actual, expect);
}

void testRealA(double actual, double expect, double epsilon)
{
   TEST_REAL(actual, expect, epsilon);
}

void testUnsignedA(unsigned long actual, unsigned long expect)
{
   TEST_UNSIGNED(actual, expect);
}

void testSignedA(long actual, long expect)
{
   TEST_SIGNED(actual, expect);
}

void testCharA(char actual, char expect)
{
   TEST_CHAR(actual, expect);
}

void testBooleanA(long actual, long expect)
{
   TEST_BOOLEAN(actual, expect);
}
