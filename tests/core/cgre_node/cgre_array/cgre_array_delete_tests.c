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

int cgre_array_delete_tests();

int main(int argc, char** argv)
{
    return (
        cgre_array_delete_tests()
    );
}

int cgre_array_delete_tests()
{
    struct cgre_node_set array;
    struct cgre_node item1;
    struct cgre_node item2;
    struct cgre_node item3;
    struct cgre_node item4;
    cgre_node_set_initialize(&array);
    cgre_node_initialize(&item1, 0, NULL);
    cgre_array_add(&array, &item1);
    cgre_node_initialize(&item2, 1, NULL);
    cgre_array_add(&array, &item2);
    cgre_node_initialize(&item3, 2, NULL);
    cgre_array_add(&array, &item3);
    cgre_node_initialize(&item4, 3, NULL);
    cgre_array_add(&array, &item4);
    // Check deleted item is returned
    if (cgre_array_delete(&array, 2) != &item3) {
        return 1;
    }
    // Check deleted item is not tail of previous
    if (item2.link[CGRE_NODE_TAIL] == &item3) {
        return 2;
    }
    // Check that deleted item is not head of next
    if (item4.link[CGRE_NODE_HEAD] == &item3) {
        return 4;
    }
    // Check that deleted previous and next are linked
    if (item2.link[CGRE_NODE_TAIL] != &item4 ||
            item4.link[CGRE_NODE_HEAD] != &item2) {
        return 8;
    }
    // Check that middle is correctly updated
    if (array.link[CGRE_NODE_MIDDLE] != &item1 ||
            array.link[CGRE_NODE_MIDDLE] != &item1) {
        return 32;
    }
    return 0;
}
