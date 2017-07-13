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

#ifndef _CGRE_CORE_SET_H_
#define _CGRE_CORE_SET_H_

#include <cgre/core/common.h>

#ifndef CGRE_LIST_MODE_DEFAULT
#define CGRE_LIST_MODE_DEFAULT 1
#endif /* ifndef CGRE_LIST_MODE_DEFAULT */

#ifndef CGRE_TREE_MAX_HEIGHT
#define CGRE_TREE_MAX_HEIGHT 18
#endif /* ifndef CGRE_TREE_MAX_HEIGHT */

#define CGRE_TREE_RED 1
#define CGRE_TREE_BLACK 2

#define CGRE_TREE_UNINITIALIZED 0
#define CGRE_TREE_INITIALIZED 1
#define CGRE_TREE_NOROOT 1
#define CGRE_TREE_ROOT 3

struct cgre_node* cgre_array_add(
        struct cgre_node_set* array,
        struct cgre_node* node);

struct cgre_node* cgre_array_delete(
        struct cgre_node_set* array,
        cgre_uint_t index);

struct cgre_node* cgre_array_get(
        struct cgre_node_set* array,
        cgre_uint_t index);

struct cgre_node* cgre_array_set(
        struct cgre_node_set* array,
        struct cgre_node* node,
        cgre_uint_t index);

struct cgre_node* cgre_hash_list_delete(
        struct cgre_node_set* list,
        cgre_uint_t key);

struct cgre_node* cgre_hash_list_insert(
        struct cgre_node_set* list,
        struct cgre_node* node);

struct cgre_node* cgre_hash_list_replace(
        struct cgre_node_set* list,
        struct cgre_node* node);

struct cgre_node* cgre_hash_list_search(
        struct cgre_node_set* list,
        cgre_uint_t key);

struct cgre_node* cgre_queue_push(
        struct cgre_node_set* queue,
        struct cgre_node* node);

struct cgre_node* cgre_queue_pop(
        struct cgre_node_set* queue);

struct cgre_node* cgre_queue_peek(
        struct cgre_node_set* queue);

struct cgre_node* cgre_stack_push(
        struct cgre_node_set* stack,
        struct cgre_node* node);

struct cgre_node* cgre_stack_pop(
        struct cgre_node_set* stack);

struct cgre_node* cgre_stack_peek(
        struct cgre_node_set* stack);

struct cgre_node* cgre_tree_delete(
        struct cgre_node_set* tree,
        cgre_uint_t key);

struct cgre_node* cgre_tree_insert(
        struct cgre_node_set* tree,
        struct cgre_node* node);

struct cgre_node* cgre_tree_replace(
        struct cgre_node_set* tree,
        struct cgre_node* node);

struct cgre_node* cgre_tree_search(
        struct cgre_node_set* tree,
        cgre_uint_t key);

#endif /* ifndef _CGRE_CORE_SET_H_ */
