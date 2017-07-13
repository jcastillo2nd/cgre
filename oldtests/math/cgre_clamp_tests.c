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

int cgre_clamp_tests();

int main(int argc, char** argv)
{
    return (
        cgre_clamp_tests()
   );
}

int cgre_clamp_tests()
{
    cgre_real_t a = -3.0;
    cgre_real_t x = -2.0;
    cgre_real_t y = 1.5;
    cgre_real_t z = 2.0;
    cgre_real_t w = 8.0;
    if (CGRE_CLAMP(y, x, z) != y & // Within bounds, value
            CGRE_CLAMP(w, x, z) != z & // Outside bounds, max
            CGRE_CLAMP(a, x, z) != x & // Outside bounds, min
            CGRE_CLAMP(y, x, z) != y){ // Illegal bounds, value
        return 1;
    }
    return 0;
}
