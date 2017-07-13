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
int cgre_array_list_delete_tests();
int cgre_array_list_get_tests();
int cgre_array_list_set_tests();

int main(int argc, char** argv)
{
    return (
        cgre_array_list_add_tests() | // Error 16
        cgre_array_list_delete_tests() | // Error 32
        cgre_array_list_get_tests() | // Error 64
        cgre_array_list_set_tests() // Error 32
    );
}

int cgre_array_list_add_tests()
{
    struct cgre_node_list list;
    struct cgre_node item1;
    struct cgre_node item2;
    struct cgre_node item3;
    struct cgre_node item4;
    struct cgre_node item5;
    cgre_node_list_initialize(&list);
    cgre_node_initialize(&item1, 0, NULL);
    cgre_node_initialize(&item2, 1, NULL);
    cgre_node_initialize(&item3, 2, NULL);
    cgre_node_initialize(&item4, 3, NULL);
    cgre_node_initialize(&item5, 4, NULL);
    if (cgre_array_list_add(&list, &item1) != &item1 ||
            list.count != 1 ||
            list.link[CGRE_LIST_HEAD] != &item1 ||
            list.link[CGRE_LIST_MIDDLE] != &item1 ||
            list.link[CGRE_LIST_TAIL] != &item1 ||
            item1.link[CGRE_LIST_HEAD] != NULL ||
            item1.link[CGRE_LIST_TAIL] != NULL) {
        return 1;
    }
    if (cgre_array_list_add(&list, &item2) != &item2 ||
            list.count != 2 ||
            list.link[CGRE_LIST_HEAD] != &item1 ||
            list.link[CGRE_LIST_MIDDLE] != &item1 ||
            list.link[CGRE_LIST_TAIL] != &item2 ||
            item1.link[CGRE_LIST_TAIL] != &item2 ||
            item2.link[CGRE_LIST_HEAD] != &item1 ||
            item2.link[CGRE_LIST_TAIL] != NULL) {
        return 2;
    }
    if (cgre_array_list_add(&list, &item3) != &item3 ||
            list.count != 3 ||
            list.link[CGRE_LIST_HEAD] != &item1 ||
            list.link[CGRE_LIST_MIDDLE] != &item2 ||
            list.link[CGRE_LIST_TAIL] != &item3 ||
            item2.link[CGRE_LIST_TAIL] != &item3 ||
            item3.link[CGRE_LIST_HEAD] != &item2 ||
            item3.link[CGRE_LIST_TAIL] != NULL) {
        return 4;
    }
    if (cgre_array_list_add(&list, &item4) != &item4 ||
            list.count != 4 ||
            list.link[CGRE_LIST_HEAD] != &item1 ||
            list.link[CGRE_LIST_MIDDLE] != &item2 ||
            list.link[CGRE_LIST_TAIL] != &item4 ||
            item3.link[CGRE_LIST_TAIL] != &item4 ||
            item4.link[CGRE_LIST_HEAD] != &item3 ||
            item4.link[CGRE_LIST_TAIL] != NULL) {
        return 8;
    }
    if (cgre_array_list_add(&list, &item5) != &item5 ||
            list.count != 5 ||
            list.link[CGRE_LIST_HEAD] != &item1 ||
            list.link[CGRE_LIST_MIDDLE] != &item3 ||
            list.link[CGRE_LIST_TAIL] != &item5 ||
            item4.link[CGRE_LIST_TAIL] != &item5 ||
            item5.link[CGRE_LIST_HEAD] != &item4 ||
            item5.link[CGRE_LIST_TAIL] != NULL) {
        return 16;
    }
    return 0;
}

int cgre_array_list_delete_tests()
{
    struct cgre_node_list list;
    struct cgre_node item1;
    struct cgre_node item2;
    struct cgre_node item3;
    struct cgre_node item4;
    cgre_node_list_initialize(&list);
    cgre_node_initialize(&item1, 0, NULL);
    cgre_array_list_add(&list, &item1);
    cgre_node_initialize(&item2, 1, NULL);
    cgre_array_list_add(&list, &item2);
    cgre_node_initialize(&item3, 2, NULL);
    cgre_array_list_add(&list, &item3);
    cgre_node_initialize(&item4, 3, NULL);
    cgre_array_list_add(&list, &item4);
    if (cgre_array_list_delete(&list, 2) != &item3 ||
            item2.link[CGRE_LIST_TAIL] != item3.link[CGRE_LIST_HEAD] ||
            item4.link[CGRE_LIST_HEAD] != item3.link[CGRE_LIST_TAIL] ||
            list.link[CGRE_LIST_MIDDLE] != &item2) {
        return 32;
    }
    return 0;
}

int cgre_array_list_get_tests()
{
    struct cgre_node_list list;
    struct cgre_node item1;
    struct cgre_node item2;
    struct cgre_node item3;
    struct cgre_node item4;
    cgre_node_list_initialize(&list);
    cgre_node_initialize(&item1, 0, NULL);
    cgre_array_list_add(&list, &item1);
    cgre_node_initialize(&item2, 1, NULL);
    cgre_array_list_add(&list, &item2);
    cgre_node_initialize(&item3, 2, NULL);
    cgre_array_list_add(&list, &item3);
    cgre_node_initialize(&item4, 3, NULL);
    if (cgre_array_list_get(&list, 0) != &item1 ||
            cgre_array_list_get(&list, 1) != &item2 ||
            cgre_array_list_get(&list, 2) != &item3 ||
            cgre_array_list_get(&list, 3) != &item4 ||
            cgre_array_list_get(&list, 4) != NULL) {
        return 64;
    }
    
    return 0;
}

int cgre_array_list_set_tests()
{
    struct cgre_node_list list;
    cgre_node_list_initialize(&list);
    struct cgre_node_list empty;
    cgre_node_list_initialize(&empty);
    struct cgre_node item1;
    cgre_node_initialize(&item1, 0, NULL);
    struct cgre_node item2;
    cgre_node_initialize(&item2, 0, NULL);
    struct cgre_node empty1;
    cgre_node_initialize(&empty1, 0, NULL);
    if (cgre_array_list_set(&list, &item2, 0) != &item1 ||
           list.link[CGRE_LIST_HEAD] != &item2 ||
           list.link[CGRE_LIST_MIDDLE] !=&item2 ||
           list.link[CGRE_LIST_TAIL] != &item2) {
        return 128;
    }
    if (cgre_array_list_set(&empty, &empty1, 0) != NULL) {
        return 256;
    }
    return 0;
}
