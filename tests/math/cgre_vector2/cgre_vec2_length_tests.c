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

int cgre_vec2_distance_tests();

int main(int argc, char** argv)
{
    return (
            cgre_vec2_distance_tests()
   );
}

/**
 * TODO: These may fail with floating point arithmetic. The values tested
 * happen to be whole integers ( no mantissa ) so for most cases, this may
 * pass. Something more reliable would be preferred.
 */

int cgre_vec2_distance_tests()
{
    struct cgre_vector2 m3_4 = {3.0, 4.0};
    struct cgre_vector2 m6_8 = {6.0, 8.0};
    if (cgre_vec2_length(&m3_4) !=
            (5.0)) {
       return 1;
    }
    if (cgre_vec2_length(&m6_8) !=
            (10.0)) {
        return 2;
    }
    return 0;
}
