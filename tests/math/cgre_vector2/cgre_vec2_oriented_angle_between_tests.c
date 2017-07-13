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

#include <cgre/cgre.h>

int cgre_vec2_oriented_angle_between_tests();

int main(int argc, char** argv)
{
    return (
            cgre_vec2_oriented_angle_between_tests()
   );
}

int cgre_vec2_oriented_angle_between_tests()
{
    struct cgre_vector2 unit = {1.0, 1.0};
    struct cgre_vector2 origin = {0.0, 0.0};
    struct cgre_vector2 y_positive = {0.0, 1.0};
    struct cgre_vector2 x_positive = {1.0, 0.0};
    struct cgre_vector2 y_negative = {0.0, -1.0};
    struct cgre_vector2 x_negative = {1.0, 0.0};
    if (CGRE_FABS(cgre_vec2_oriented_angle_between(&origin, &y_positive) -
                (CGRE_PI/2.0)) <= CGRE_REAL_EPSILON) {
       return 1;
    }
    if (CGRE_FABS(cgre_vec2_oriented_angle_between(&origin, &y_negative) -
                CGRE_PI / -2.0) <= CGRE_REAL_EPSILON) {
        return 2;
    }
    if (CGRE_FABS(cgre_vec2_oriented_angle_between(&unit, &y_positive) -
                CGRE_PI) <= CGRE_REAL_EPSILON) {
        return 4;
    }
    if (CGRE_FABS(cgre_vec2_oriented_angle_between(&unit, &x_positive) -
                CGRE_TWO_PI) <= CGRE_REAL_EPSILON) {
        return 8;
    }
    if (CGRE_FABS(cgre_vec2_oriented_angle_between(&y_positive, &x_negative) -
                ((-3.0 * CGRE_PI) / 4.0)) <= CGRE_REAL_EPSILON) {
        return 16;
    }
    return 0;
}
