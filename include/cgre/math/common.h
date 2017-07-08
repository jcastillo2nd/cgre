/*
===============================================================================

This source file is part of CGRE
    (C Graphics Rendering Engine)
CGRE is made available under the MIT License.

Copyright (c) 2016-2017 Javier Castillo II

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

===============================================================================
*/

#ifndef _CGRE_MATH_COMMON_H_
#define _CGRE_MATH_COMMON_H_

#include <float.h>
#include <limits.h>
#include <math.h>
#include <stddef.h>

#define CGRE_PI 3.1415926535897932384626433832795
#define CGRE_TWO_PI CGRE_PI * 2.0

#if HAVE_LONG_LONG_INT == 1

#define CGRE_INT_MAX LLONG_MAX
#define CGRE_UINT_MAX ULLONG_MAX
typedef long long int cgre_int_t;
typedef unsigned long long int cgre_uint_t;

#elif HAVE_LONG_INT == 1

#define CGRE_INT_MAX LONG_MAX
#define CGRE_UINT_MAX ULONG_MAX
typedef long int cgre_int_t;
typedef unsigned long int cgre_uint_t;

#else

#define CGRE_INT_MAX INT_MAX
#define CGRE_UINT_MAX UINT_MAX
typedef int cgre_int_t;
typedef unsigned int cgre_uint_t;

#endif /* ifdef HAVE_LONG_LONG_INT */

#if HAVE_LONG_DOUBLE == 1

typedef long double cgre_real_t;
#define CGRE_REAL_CAST (long double)
#define CGRE_REAL_EPSILON LDBL_MIN
#define CGRE_REAL_MAX LDBL_MAX
#define CGRE_COS (cgre_real_t) cosl
#define CGRE_COSH (cgre_real_t) coshl
#define CGRE_ACOS (cgre_real_t) acosl
#define CGRE_ACOSH (cgre_real_t) acoshl
#define CGRE_SIN (cgre_real_t) sinl
#define CGRE_SINH (cgre_real_t) sinhl
#define CGRE_ASIN (cgre_real_t) asinl
#define CGRE_ASINH (cgre_real_t) asinhl
#define CGRE_TAN (cgre_real_t) tanl
#define CGRE_TANH (cgre_real_t) tanhl
#define CGRE_ATAN (cgre_real_t) atanl
#define CGRE_ATAN2 (cgre_real_t) atan2l
#define CGRE_ATANH (cgre_real_t) atanhl
#define CGRE_MIN (cgre_real_t) fminl
#define CGRE_MAX (cgre_real_t) fmaxl
#define CGRE_SQRT (cgre_real_t) sqrtl
#define CGRE_POW (cgre_real_t) powl

#elif HAVE_DOUBLE == 1

typedef double cgre_real_t;
#define CGRE_REAL_CAST (double)
#define CGRE_REAL_EPSILON DBL_MIN 
#define CGRE_REAL_MAX DBL_MAX
#define CGRE_COS (cgre_real_t) cos
#define CGRE_COSH (cgre_real_t) cosh
#define CGRE_ACOS (cgre_real_t) acos
#define CGRE_ACOSH (cgre_real_t) acosh
#define CGRE_SIN (cgre_real_t) sin
#define CGRE_SINH (cgre_real_t) sinh
#define CGRE_ASIN (cgre_real_t) asin
#define CGRE_ASINH (cgre_real_t) asinh
#define CGRE_TAN (cgre_real_t) tan
#define CGRE_TANH (cgre_real_t) tanh
#define CGRE_ATAN (cgre_real_t) atan
#define CGRE_ATAN2 (cgre_real_t) atan2
#define CGRE_ATANH (cgre_real_t) atanh
#define CGRE_MIN (cgre_real_t) fmin
#define CGRE_MAX (cgre_real_t) fmax
#define CGRE_SQRT (cgre_real_t) sqrt
#define CGRE_POW (cgre_real_t) pow

#else

typedef float cgre_real_t;
#define CGRE_REAL_CAST (float)
#define CGRE_REAL_EPSILON FLT_MIN
#define CGRE_REAL_MAX FLT_MAX
#define CGRE_COS (cgre_real_t) cosf
#define CGRE_COSH (cgre_real_t) coshf
#define CGRE_ACOS (cgre_real_t) acosf
#define CGRE_ACOSH (cgre_real_t) acoshf
#define CGRE_SIN (cgre_real_t) sinf
#define CGRE_SINH (cgre_real_t) sinhf
#define CGRE_ASIN (cgre_real_t) asinf
#define CGRE_ASINH (cgre_real_t) asinhf
#define CGRE_TAN (cgre_real_t) tanf
#define CGRE_TANH (cgre_real_t) tanhf
#define CGRE_ATAN (cgre_real_t) atanf
#define CGRE_ATAN2 (cgre_real_t) atan2f
#define CGRE_ATANH (cgre_real_t) atanhf
#define CGRE_MIN (cgre_real_t) fminf
#define CGRE_MAX (cgre_real_t) fmaxf
#define CGRE_SQRT (cgre_real_t) sqrtf
#define CGRE_POW (cgre_real_t) powf

#endif /* CGRE_LONG_DOUBLE_PRECISION == 1 */

#define CGRE_CLAMP(V, MIN, MAX) (MIN < MAX ? CGRE_MAX(CGRE_MIN(V, MAX), MIN) : CGRE_MAX(CGRE_MIN(V, CGRE_REAL_MAX), -CGRE_REAL_MAX))

typedef cgre_real_t cgre_angular_t;

cgre_angular_t cgre_rad2deg(cgre_angular_t);

struct cgre_vector2 {
    cgre_real_t x, y;
};

struct cgre_vector3 {
    cgre_real_t x, y, z;
};

struct cgre_vector4 {
    cgre_real_t w, x, y, z;
};

struct cgre_quaternion {
    cgre_real_t w, x, y, z;
};

#define CGRE_NULL_QUATERNION NULL

#define CGRE_NULL_VECTOR2 NULL

#define CGRE_NULL_VECTOR3 NULL

#define CGRE_NULL_VECTOR4 NULL

#endif /* ifndef _CGRE_MATH_COMMON_H_ */
