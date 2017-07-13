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

int cgre_array_list_add_tests();

int main(int argc, char** argv)
{
    return (
        cgre_array_list_add_tests()
    );
}

int cgre_array_list_add_tests()
{
    struct cgre_node_set array;
    struct cgre_node item1;
    struct cgre_node item2;
    struct cgre_node item3;
    struct cgre_node item4;
    struct cgre_node item5;
    cgre_node_set_initialize(&array);
    cgre_node_initialize(&item1, 0, NULL);
    cgre_node_initialize(&item2, 1, NULL);
    cgre_node_initialize(&item3, 2, NULL);
    cgre_node_initialize(&item4, 3, NULL);
    cgre_node_initialize(&item5, 4, NULL);
    if (cgre_array_add(&array, &item1) != &item1 ||
            array.count != 1 ||
            array.link[CGRE_NODE_HEAD] != &item1 ||
            array.link[CGRE_NODE_MIDDLE] != &item1 ||
            array.link[CGRE_NODE_TAIL] != &item1 ||
            item1.link[CGRE_NODE_HEAD] != NULL ||
            item1.link[CGRE_NODE_TAIL] != NULL) {
        return 1;
    }
    if (cgre_array_add(&array, &item2) != &item2 ||
            array.count != 2 ||
            array.link[CGRE_NODE_HEAD] != &item1 ||
            array.link[CGRE_NODE_MIDDLE] != &item1 ||
            array.link[CGRE_NODE_TAIL] != &item2 ||
            item1.link[CGRE_NODE_TAIL] != &item2 ||
            item2.link[CGRE_NODE_HEAD] != &item1 ||
            item2.link[CGRE_NODE_TAIL] != NULL) {
        return 2;
    }
    if (cgre_array_add(&array, &item3) != &item3 ||
            array.count != 3 ||
            array.link[CGRE_NODE_HEAD] != &item1 ||
            array.link[CGRE_NODE_MIDDLE] != &item2 ||
            array.link[CGRE_NODE_TAIL] != &item3 ||
            item2.link[CGRE_NODE_TAIL] != &item3 ||
            item3.link[CGRE_NODE_HEAD] != &item2 ||
            item3.link[CGRE_NODE_TAIL] != NULL) {
        return 4;
    }
    if (cgre_array_add(&array, &item4) != &item4 ||
            array.count != 4 ||
            array.link[CGRE_NODE_HEAD] != &item1 ||
            array.link[CGRE_NODE_MIDDLE] != &item2 ||
            array.link[CGRE_NODE_TAIL] != &item4 ||
            item3.link[CGRE_NODE_TAIL] != &item4 ||
            item4.link[CGRE_NODE_HEAD] != &item3 ||
            item4.link[CGRE_NODE_TAIL] != NULL) {
        return 8;
    }
    if (cgre_array_add(&array, &item5) != &item5 ||
            array.count != 5 ||
            array.link[CGRE_NODE_HEAD] != &item1 ||
            array.link[CGRE_NODE_MIDDLE] != &item3 ||
            array.link[CGRE_NODE_TAIL] != &item5 ||
            item4.link[CGRE_NODE_TAIL] != &item5 ||
            item5.link[CGRE_NODE_HEAD] != &item4 ||
            item5.link[CGRE_NODE_TAIL] != NULL) {
        return 16;
    }
    return 0;
}
