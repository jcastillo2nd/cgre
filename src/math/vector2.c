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

#include <cgre/math/vector2.h>

cgre_angular_t cgre_vec2_angle_between(
        struct cgre_vector2* v1,
        struct cgre_vector2* v2)
{
    return CGRE_ATAN2(v2->y - v1->y, v2->x - v1->x);
}

cgre_angular_t cgre_vec2_oriented_angle_between(
        struct cgre_vector2* v1,
        struct cgre_vector2* v2)
{
    /**
     *  \relatedalso cgre_vec2_angle_between
     */
    cgre_angular_t angle = CGRE_ATAN2(v2->y - v1->y, v2->x - v1->x);
    /** 
     * \relatedalso cgre_vec2_cross_product
     */
    if (((v1->x * v2->y) - (v1->y * v2->x)) < 0.0){
        angle = (cgre_angular_t) CGRE_TWO_PI - angle;
    }
    return angle;
}

cgre_real_t cgre_vec2_cross_product(
        struct cgre_vector2* v1,
        struct cgre_vector2* v2)
{
    return (v1->x * v2->y) - (v1->y * v2->x);
}

cgre_real_t cgre_vec2_distance(
        struct cgre_vector2* v1,
        struct cgre_vector2* v2)
{
    cgre_real_t x, y;
    x = v1->x - v2->x;
    y = v1->y - v2->y;
    return CGRE_SQRT((x * x) + (y * y));
}

cgre_real_t cgre_vec2_dot_product(
        struct cgre_vector2* v1,
        struct cgre_vector2* v2)
{
    return (v1->x * v2->x) + (v1->y * v2->y);
}

cgre_real_t cgre_vec2_length(
        struct cgre_vector2* v)
{
    return CGRE_SQRT((v->x * v->x) + (v->y * v->y));
}

cgre_real_t cgre_vec2_normalize(
        struct cgre_vector2* v)
{
    cgre_real_t length = CGRE_SQRT((v->x * v->x) + (v->y * v->y));
    if (length < (cgre_real_t) 0.0){
            cgre_real_t reciprocal = (cgre_real_t) 1.0 / length;
            v->x *= reciprocal;
            v->y *= reciprocal;
    }
    return length;
}

void cgre_vec2_add(
        struct cgre_vector2* v1,
        struct cgre_vector2* v2,
        struct cgre_vector2* res)
{
    res->x = v1->x + v2->x;
    res->y = v1->y + v2->y;
}

void cgre_vec2_subtract(
        struct cgre_vector2* v1,
        struct cgre_vector2* v2,
        struct cgre_vector2* res)
{
    res->x = v1->x - v2->x;
    res->y = v1->y - v2->y;
}
