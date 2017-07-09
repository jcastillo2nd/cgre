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

#ifndef _CGRE_CORE_LIST_H_
#define _CGRE_CORE_LIST_H_

#include <cgre/core/common.h>

#ifndef CGRE_LIST_MODE_DEFAULT
#define CGRE_LIST_MODE_DEFAULT 1
#endif /* ifndef CGRE_LIST_MODE_DEFAULT */

#define CGRE_LIST_HEAD 0
#define CGRE_LIST_MIDDLE 1
#define CGRE_LIST_TAIL 2

struct cgre_node_list {
    struct cgre_node* link[3];
    cgre_uint_t bound;
    cgre_uint_t count;
    cgre_uint_t state;
    pthread_mutex_t lock;
};

struct cgre_node* cgre_array_list_add(
        struct cgre_node_list* list,
        struct cgre_node* node);

struct cgre_node* cgre_array_list_delete(
        struct cgre_node_list* list,
        cgre_uint_t index);

struct cgre_node* cgre_array_list_get(
        struct cgre_node_list* list,
        cgre_uint_t index);

struct cgre_node* cgre_array_list_set(
        struct cgre_node_list* list,
        struct cgre_node* node,
        cgre_uint_t index);

struct cgre_node* cgre_hash_list_delete(
        struct cgre_node_list* list,
        cgre_uint_t key);

struct cgre_node* cgre_hash_list_insert(
        struct cgre_node_list* list,
        struct cgre_node* node);

struct cgre_node* cgre_hash_list_replace(
        struct cgre_node_list* list,
        struct cgre_node* node);

struct cgre_node* cgre_hash_list_search(
        struct cgre_node_list* list,
        cgre_uint_t key);

struct cgre_node_list* cgre_node_list_initialize(
        struct cgre_node_list* list);

struct cgre_node_list* cgre_node_list_uninitialize(
        struct cgre_node_list* list);

struct cgre_node* cgre_queue_list_push(
        struct cgre_node_list* list,
        struct cgre_node* node);

struct cgre_node* cgre_queue_list_pop(
        struct cgre_node_list* list);

struct cgre_node* cgre_queue_list_peek(
        struct cgre_node_list* list);

struct cgre_node* cgre_stack_list_push(
        struct cgre_node_list* list,
        struct cgre_node* node);

struct cgre_node* cgre_stack_list_pop(
        struct cgre_node_list* list);

struct cgre_node* cgre_stack_list_peek(
        struct cgre_node_list* list);

#endif /* ifndef _CGRE_CORE_LIST_H_ */
