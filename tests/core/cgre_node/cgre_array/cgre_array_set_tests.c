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

int cgre_array_set_tests();

int main(int argc, char** argv)
{
    return (
        cgre_array_set_tests()
    );
}

int cgre_array_set_tests()
{
    struct cgre_node_set array;
    cgre_node_set_initialize(&array);
    struct cgre_node_set empty;
    cgre_node_set_initialize(&empty);
    struct cgre_node item1;
    cgre_node_initialize(&item1, 0, NULL);
    cgre_array_add(&array, &item1);
    struct cgre_node item2;
    cgre_node_initialize(&item2, 0, NULL);
    struct cgre_node empty1;
    cgre_node_initialize(&empty1, 0, NULL);
    // Check if set list index 0 returns item1
    if (cgre_array_set(&array, &item2, 0) != &item1) {
        return 1;
    }
    // Check list head, middle and tail updated to item2
    if (array.link[CGRE_NODE_HEAD] != &item2) {
        return 2;
    }
    if (array.link[CGRE_NODE_MIDDLE] != &item2) {
        return 4;
    }
    if (array.link[CGRE_NODE_TAIL] != &item2) {
        return 8;
    }
    // Check out of bounds set returns NULL
    if (cgre_array_set(&array, &item1, 4) != NULL) {
        return 16;
    }
    // Check if empty list set is NULL
    if (cgre_array_set(&empty, &empty1, 0) != NULL) {
        return 32;
    }
    return 0;
}
