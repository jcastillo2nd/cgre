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

int cgre_vec2_dot_product_tests();

int main(int argc, char** argv)
{
    return (
            cgre_vec2_dot_product_tests()
   );
}

/**
 * TODO: These may fail with floating point arithmetic. The values tested
 * happen to be whole integers ( no mantissa ) so for most cases, this may
 * pass. Something more reliable would be preferred.
 */

int cgre_vec2_dot_product_tests()
{
    struct cgre_vector2 unit_10 = {10.0, 10.0};
    struct cgre_vector2 m6_7 = {6.0, 7.0};
    struct cgre_vector2 m2_4 = {2.0, 4.0};
    if (cgre_vec2_dot_product(&unit_10, &m6_7) !=
                (130.0)) {
       return 1;
    }
    if (cgre_vec2_dot_product(&unit_10, &m2_4) !=
                (60.0)) {
       return 2;
    }
    if (cgre_vec2_dot_product(&m6_7, &m2_4) !=
            (40.0)) {
        return 4;
    }
    return 0;
}
