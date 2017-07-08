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

#ifndef _CGRE_MATH_VECTOR2_H_
#define _CGRE_MATH_VECTOR2_H_

#include <cgre/math/common.h>

// Get the angle between 2 vector2
cgre_angular_t cgre_vec2_angle_between(
        struct cgre_vector2* v1,
        struct cgre_vector2* v2);

// Get the oriented angle between 2 vector2
cgre_angular_t cgre_vec2_oriented_angle_between(
        struct cgre_vector2* v1,
        struct cgre_vector2* v2);

// Get the 2 dimensional cross product
cgre_real_t cgre_vec2_cross_product(
        struct cgre_vector2* v1,
        struct cgre_vector2* v2);

// Get the distance to another vector
cgre_real_t cgre_vec2_distance(
        struct cgre_vector2* v1,
        struct cgre_vector2* v2);

// Returns dot product of 2 vector2
cgre_real_t cgre_vec2_dot_product(
        struct cgre_vector2* v1,
        struct cgre_vector2* v2);

// Get the length of a vector
cgre_real_t cgre_vec2_length(
        struct cgre_vector2* v);

// Normalize the vector
cgre_real_t cgre_vec2_normalize(
        struct cgre_vector2* v);

// Store sum of v1 and v2 in res
void cgre_vec2_add(
        struct cgre_vector2* v1,
        struct cgre_vector2* v2,
        struct cgre_vector2* res);

// Store difference of v1 and v2 in res
void cgre_vec2_subtract(
        struct cgre_vector2* v1,
        struct cgre_vector2* v2,
        struct cgre_vector2* res);

#endif /* ifndef _CGRE_MATH_VECTOR2_H_ */
