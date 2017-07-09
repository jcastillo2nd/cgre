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

#ifndef _CGRE_CORE_TREE_H_
#define _CGRE_CORE_TREE_H_

#include <cgre/core/common.h>

#ifndef CGRE_RBTREE_MAX_HEIGHT
#define CGRE_RBTREE_MAX_HEIGHT 18
#endif /* ifndef CGRE_RBTREE_MAX_HEIGHT */

#define CGRE_TREE_RED 0
#define CGRE_TREE_BLACK 1

#define CGRE_TREE_UNINITIALIZED 0
#define CGRE_TREE_INITIALIZED 1
#define CGRE_TREE_NOROOT 1
#define CGRE_TREE_ROOT 3

struct cgre_node_tree {
    struct cgre_node* root;
    cgre_uint_t state;
    cgre_int_t count;
    pthread_mutex_t lock;
};

struct cgre_node* cgre_tree_delete(
        struct cgre_node_tree* tree,
        cgre_uint_t key);

struct cgre_node* cgre_tree_insert(
        struct cgre_node_tree* tree,
        struct cgre_node* node);

struct cgre_node* cgre_tree_replace(
        struct cgre_node_tree* tree,
        struct cgre_node* node);

struct cgre_node* cgre_tree_search(
        struct cgre_node_tree* tree,
        cgre_uint_t key);

struct cgre_node_tree* cgre_node_tree_initialize(
        struct cgre_node_tree* tree,
        struct cgre_node* root);

struct cgre_node_tree* cgre_node_tree_uninitialize(
        struct cgre_node_tree* tree);

#endif /* ifndef _CGRE_CORE_TREE_H_ */
