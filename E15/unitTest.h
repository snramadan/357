#include <string.h>
#include <stdio.h>

#ifndef UNITTEST_H
   #define UNITTEST_H

   #define TEST_SIGNED(_ACTUAL,_EXPECT)\
   {\
      long _actual = _ACTUAL, _expect = _EXPECT;\
      if (_actual != _expect)\
      {\
         fprintf(stderr, "Failed test in %s at line %d:\n",\
            __FILE__, __LINE__);\
         fprintf(stderr, "   Found substitution %s, value %ld, expected %ld\n",\
            #_ACTUAL, _actual, _expect);\
      }\
   }

   #define TEST_UNSIGNED(_ACTUAL,_EXPECT)\
   {\
      unsigned long _actual = _ACTUAL, _expect = _EXPECT;\
      if (_actual != _expect)\
      {\
         fprintf(stderr, "Failed test in %s at line %d:\n",\
            __FILE__, __LINE__);\
         fprintf(stderr, "   Found substitution %s, value %lu, expected %lu\n",\
            #_ACTUAL, _actual, _expect);\
      }\
   }

   #define TEST_STRING(_ACTUAL,_EXPECT)\
   {\
      char const _actual = _ACTUAL;\
      char const _expect = _EXPECT;\
      if (strcmp(_actual, _expect) != 0)\
      {\
         fprintf(stderr, "Failed test in %s at line %d:\n",\
            __FILE__, __LINE__);\
         fprintf(stderr, "   Found substitution %s, value %s, expected %s\n",\
            #_ACTUAL, _ACTUAL, _EXPECT);\
      }\
   }

   #define TEST_CHAR(_ACTUAL,_EXPECT)\
   {\
      char _actual = _ACTUAL;\
      char _expect = _EXPECT;\
      if (_actual != _expect)\
      {\
         fprintf(stderr, "Failed test in %s at line %d:\n",\
            __FILE__, __LINE__);\
         fprintf(stderr, "   Found substitution %s, value %c, expected %c\n",\
            #_ACTUAL, _ACTUAL, _EXPECT);\
      }\
   }

   #define TEST_BOOLEAN(_ACTUAL, _EXPECT)\
   {\
      char _a[5] = {'t','r','u','e','\0'}, _e[5] = {'t','r','u','e','\0'};\
      if (_ACTUAL == 0)\
      {\
         strcpy(_a, "false");\
      }\
      if (_EXPECT == 0)\
      {\
         strcpy(_e, "false");\
      }\
      if (strcmp(_a, _e) != 0)\
      {\
         fprintf(stderr, "Failed test in %s at line %d:\n",\
            __FILE__, __LINE__);\
         fprintf(stderr, "   Found substitution %s, value %s, expected %s\n",\
            #_ACTUAL, _a, _e);\
      }\
   }

   #define TEST_REAL(_ACTUAL, _EXPECT, _EPSILON)\
   {\
      double _d = _EXPECT - _ACTUAL, _e = _EPSILON;\
      if (_d < 0 )\
      {\
         _d = -_d;\
      }\
      if (_e < 0)\
      {\
         _e = -_e;\
      }\
      if (_d > _e)\
      {\
         fprintf(stderr, "Failed test in %s at line %d:\n",\
            __FILE__, __LINE__);\
         fprintf(stderr, "   Found substitution %s, value %g, expected %g +/-%g\n",\
            #_ACTUAL, _ACTUAL, _EXPECT, _EPSILON);\
      }\
   }

   #define TEST_ERROR(_FUNCTION_CALL)\
   {\
      _FUNCTION_CALL;\
      fprintf(stderr, "Failed test in %s at line %d:\n", __FILE__, __LINE__);\
      fprintf(stderr, "   Expected error detection did not occur\n");\
   }  

#endif
