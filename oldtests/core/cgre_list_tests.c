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

int cgre_node_list_initialize_tests();
int cgre_node_list_uninitialize_tests();
int cgre_array_list_add_tests();
int cgre_array_list_delete_tests();
int cgre_array_list_get_tests();
int cgre_array_list_set_tests();
int cgre_hash_list_delete_tests();
int cgre_hash_list_insert_tests();
int cgre_hash_list_replace_tests();
int cgre_hash_list_search_tests();
int cgre_queue_list_push_tests();
int cgre_queue_list_pop_tests();
int cgre_queue_list_peek_tests();
int cgre_stack_list_push_tests();
int cgre_stack_list_pop_tests();
int cgre_stack_list_peek_tests();

int main(int argc, char** argv)
{
    return (
        cgre_node_list_initialize_tests() | // Error 1
        cgre_node_list_uninitialize_tests() | // Error 2
        cgre_array_list_add_tests() | // Error 4
        cgre_array_list_delete_tests() | // Error 8
        cgre_array_list_get_tests() | // Error 16
        cgre_array_list_set_tests() | // Error 32
        cgre_hash_list_delete_tests() | // 64
        cgre_hash_list_insert_tests() | // 128
        cgre_hash_list_replace_tests() | // 256
        cgre_hash_list_search_tests() | // 512
        cgre_queue_list_push_tests() | // 1024
        cgre_queue_list_pop_tests() | // Error 2048
        cgre_queue_list_peek_tests() | // Error 4096
        cgre_stack_list_push_tests() | //  Error 8192
        cgre_stack_list_pop_tests() | // 16384
        cgre_stack_list_peek_tests() // 32768
    );
}

int cgre_node_list_initialize_tests()
{
    struct cgre_node_list list;
    if (cgre_node_list_initialize(&list) != &list ||
            list.link[0] != NULL ||
            list.link[1] != NULL ||
            list.link[2] != NULL ||
            list.count != 0 ||
            list.state != 0) {
        return 1;
    }
    return 0;
}

int cgre_node_list_uninitialize_tests()
{
    struct cgre_node_list list;
    cgre_node_list_initialize(&list);
    if (cgre_node_list_uninitialize(&list) != &list ||
            list.link[0] != NULL ||
            list.link[1] != NULL ||
            list.link[2] != NULL ||
            list.count != 0 ||
            list.state != 0) {
        return 2;
    }
    return 0;
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
        return 4;
    }
    if (cgre_array_list_add(&list, &item2) != &item2 ||
            list.count != 2 ||
            list.link[CGRE_LIST_HEAD] != &item1 ||
            list.link[CGRE_LIST_MIDDLE] != &item1 ||
            list.link[CGRE_LIST_TAIL] != &item2 ||
            item1.link[CGRE_LIST_TAIL] != &item2 ||
            item2.link[CGRE_LIST_HEAD] != &item1 ||
            item2.link[CGRE_LIST_TAIL] != NULL) {
        return 4;
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
        return 4;
    }
    if (cgre_array_list_add(&list, &item5) != &item5 ||
            list.count != 5 ||
            list.link[CGRE_LIST_HEAD] != &item1 ||
            list.link[CGRE_LIST_MIDDLE] != &item3 ||
            list.link[CGRE_LIST_TAIL] != &item5 ||
            item4.link[CGRE_LIST_TAIL] != &item5 ||
            item5.link[CGRE_LIST_HEAD] != &item4 ||
            item5.link[CGRE_LIST_TAIL] != NULL) {
        return 4;
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
            item2.link[CGRE_LIST_TAIL] != &item4 ||
            item4.link[CGRE_LIST_HEAD] != &item2 ||
            list.link[CGRE_LIST_MIDDLE] != &item2) {
        return 8;
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
        return 16;
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
        return 32;
    }
    if (cgre_array_list_set(&empty, &empty1, 0) != NULL) {
        return 32;
    }
    return 0;
}

int cgre_hash_list_delete_tests()
{
    struct cgre_node_list list;
    cgre_node_list_initialize(&list);
    struct cgre_node item1;
    cgre_node_initialize(&item1, 1, NULL);
    struct cgre_node item2;
    cgre_node_initialize(&item2, 2, NULL);
    struct cgre_node item3;
    cgre_node_initialize(&item3, 3, NULL);
    list.link[CGRE_LIST_HEAD] = &item1;
    list.link[CGRE_LIST_MIDDLE] = &item2;
    list.link[CGRE_LIST_TAIL] = &item3;
    item1.link[CGRE_LIST_TAIL] = &item2;
    item2.link[CGRE_LIST_HEAD] = &item1;
    item2.link[CGRE_LIST_TAIL] = &item3;
    item3.link[CGRE_LIST_HEAD] = &item2;
    if (cgre_hash_list_delete(&list, 2) != &item2) {
        return 64;
    }
    if (list.link[CGRE_LIST_HEAD] != &item1 ||
            list.link[CGRE_LIST_MIDDLE] != &item1 ||
            list.link[CGRE_LIST_TAIL] != &item3) {
        return 64;
    }
    if (item1.link[CGRE_LIST_TAIL] != &item3) {
        return 64;
    }
    if (item3.link[CGRE_LIST_HEAD] != &item1) {
        return 64;
    }
    if (cgre_hash_list_delete(&list, 7) != NULL) {
        return 64;
    }
    return 0;
}

int cgre_hash_list_insert_tests()
{
    struct cgre_node_list list;
    cgre_node_list_initialize(&list);
    struct cgre_node item1;
    cgre_node_initialize(&item1, 1, NULL);
    struct cgre_node item2;
    cgre_node_initialize(&item2, 2, NULL);
    struct cgre_node item3;
    cgre_node_initialize(&item3, 3, NULL);
    if (cgre_hash_list_insert(&list, &item1) != &item1 ||
            list.count != 1) {
        return 128;
    }
    if (list.link[CGRE_LIST_HEAD] != &item1 ||
            list.link[CGRE_LIST_MIDDLE] != &item1 ||
            list.link[CGRE_LIST_TAIL] != &item1) {
    }
    if (cgre_hash_list_insert(&list, &item2) != &item2 ||
            list.count != 2) {
        return 128;
    }
    if (list.link[CGRE_LIST_TAIL] != &item2 ||
            item1.link[CGRE_LIST_TAIL] != &item2) {
        return 128;
    }
    if (cgre_hash_list_insert(&list, &item3) != &item3 ||
            list.count != 3 ||
            list.link[CGRE_LIST_TAIL] != &item3 ||
            list.link[CGRE_LIST_MIDDLE] != &item2) {
        return 128;
    }
    return 0;
}

int cgre_hash_list_replace_tests()
{
    struct cgre_node_list list;
    cgre_node_list_initialize(&list);
    struct cgre_node item1;
    cgre_node_initialize(&item1, 1, NULL);
    struct cgre_node item2;
    cgre_node_initialize(&item2, 2, NULL);
    struct cgre_node item3;
    cgre_node_initialize(&item3, 3, NULL);
    struct cgre_node item4;
    cgre_node_initialize(&item4, 2, NULL);
    cgre_hash_list_insert(&list, &item1);
    cgre_hash_list_insert(&list, &item2);
    cgre_hash_list_insert(&list, &item3);
    if (cgre_hash_list_replace(&list, &item4) != &item2) {
        return 128;
    }
    if (item2.link[CGRE_LIST_HEAD] != item4.link[CGRE_LIST_HEAD] ||
            item2.link[CGRE_LIST_TAIL] != item4.link[CGRE_LIST_TAIL] ||
            list.link[CGRE_LIST_MIDDLE] != &item4) {
        return 128;
    }
    return 0;
}

int cgre_hash_list_search_tests()
{
    struct cgre_node_list list;
    cgre_node_list_initialize(&list);
    struct cgre_node item1;
    cgre_node_initialize(&item1, 1, NULL);
    struct cgre_node item2;
    cgre_node_initialize(&item2, 2, NULL);
    struct cgre_node item3;
    cgre_node_initialize(&item3, 3, NULL);
    cgre_hash_list_insert(&list, &item1);
    cgre_hash_list_insert(&list, &item2);
    cgre_hash_list_insert(&list, &item3);
    if (cgre_hash_list_search(&list, 2) != &item2) {
        return 256;
    }
    return 0;
}

int cgre_queue_list_push_tests()
{
    struct cgre_node_list list;
    cgre_node_list_initialize(&list);
    struct cgre_node item1;
    cgre_node_initialize(&item1, 1, NULL);
    struct cgre_node item2;
    cgre_node_initialize(&item2, 2, NULL);
    struct cgre_node item3;
    cgre_node_initialize(&item3, 3, NULL);
    if (cgre_queue_list_push(&list, &item1) != &item1 ||
            list.count != 1) {
        return 512;
    }
    if (cgre_queue_list_push(&list, &item2) != &item2 ||
            list.count != 2) {
        return 512;
    }
    if (cgre_queue_list_push(&list, &item3) != &item3 ||
            list.count != 2) {
        return 512;
    }
    return 0;
}

int cgre_queue_list_pop_tests()
{
    struct cgre_node_list list;
    cgre_node_list_initialize(&list);
    struct cgre_node item1;
    cgre_node_initialize(&item1, 1, NULL);
    struct cgre_node item2;
    cgre_node_initialize(&item2, 2, NULL);
    struct cgre_node item3;
    cgre_node_initialize(&item3, 3, NULL);
    cgre_queue_list_push(&list, &item1);
    cgre_queue_list_push(&list, &item2);
    cgre_queue_list_push(&list, &item3);
    if (cgre_queue_list_pop(&list) != &item1 ||
            list.count != 2) {
        return 1024;
    }
    if (cgre_queue_list_pop(&list) != &item2 ||
            list.count != 1) {
        return 1024;
    }
    if (cgre_queue_list_pop(&list) != &item3 ||
            list.count != 0) {
        return 1024;
    }
    if (cgre_queue_list_pop(&list) != NULL) {
        return 1024;
    }
    return 0;
}

int cgre_queue_list_peek_tests()
{
    struct cgre_node_list list;
    cgre_node_list_initialize(&list);
    struct cgre_node item1;
    cgre_node_initialize(&item1, 1, NULL);
    struct cgre_node item2;
    cgre_node_initialize(&item2, 2, NULL);
    struct cgre_node item3;
    cgre_node_initialize(&item3, 3, NULL);
    cgre_queue_list_push(&list, &item1);
    cgre_queue_list_push(&list, &item2);
    cgre_queue_list_push(&list, &item3);
    if (cgre_queue_list_peek(&list) != &item1 ||
            list.count != 3) {
        return 2048;
    }
    if (cgre_queue_list_peek(&list) != &item2 ||
            list.count != 3) {
        return 2048;
    }
    if (cgre_queue_list_peek(&list) != &item3 ||
            list.count != 3) {
        return 2048;
    }
    if (cgre_queue_list_peek(&list) != NULL) {
        return 2048;
    }
    return 0;
}

int cgre_stack_list_push_tests()
{
    struct cgre_node_list list;
    cgre_node_list_initialize(&list);
    struct cgre_node item1;
    cgre_node_initialize(&item1, 1, NULL);
    struct cgre_node item2;
    cgre_node_initialize(&item2, 2, NULL);
    struct cgre_node item3;
    cgre_node_initialize(&item3, 3, NULL);
    if (cgre_stack_list_push(&list, &item1) != &item1 ||
            list.count != 1) {
        return 4096;
    }
    if (cgre_stack_list_push(&list, &item2) != &item2 ||
            list.count != 2) {
        return 4096;
    }
    if (cgre_stack_list_push(&list, &item3) != &item3 ||
            list.count != 2) {
        return 4096;
    }
    return 0;
}

int cgre_stack_list_pop_tests()
{
    struct cgre_node_list list;
    cgre_node_list_initialize(&list);
    struct cgre_node item1;
    cgre_node_initialize(&item1, 1, NULL);
    struct cgre_node item2;
    cgre_node_initialize(&item2, 2, NULL);
    struct cgre_node item3;
    cgre_node_initialize(&item3, 3, NULL);
    cgre_stack_list_push(&list, &item1);
    cgre_stack_list_push(&list, &item2);
    cgre_stack_list_push(&list, &item3);
    if (cgre_stack_list_pop(&list) != &item3 ||
            list.count != 2) {
        return 8192;
    }
    if (cgre_stack_list_pop(&list) != &item2 ||
            list.count != 1) {
        return 8192;
    }
    if (cgre_stack_list_pop(&list) != &item1 ||
            list.count != 0) {
        return 8192;
    }
    if (cgre_stack_list_pop(&list) != NULL) {
        return 8192;
    }
    return 0;
}

int cgre_stack_list_peek_tests()
{
    struct cgre_node_list list;
    cgre_node_list_initialize(&list);
    struct cgre_node item1;
    cgre_node_initialize(&item1, 1, NULL);
    struct cgre_node item2;
    cgre_node_initialize(&item2, 2, NULL);
    struct cgre_node item3;
    cgre_node_initialize(&item3, 3, NULL);
    cgre_stack_list_push(&list, &item1);
    cgre_stack_list_push(&list, &item2);
    cgre_stack_list_push(&list, &item3);
    if (cgre_stack_list_pop(&list) != &item3 ||
            list.count != 3) {
        return 16384;
    }
    if (cgre_stack_list_pop(&list) != &item2 ||
            list.count != 3) {
        return 16384;
    }
    if (cgre_stack_list_pop(&list) != &item1 ||
            list.count != 3) {
        return 16384;
    }
    if (cgre_stack_list_pop(&list) != NULL) {
        return 16384;
    }
    return 0;
}

