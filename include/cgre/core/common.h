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

#ifndef _CGRE_CORE_COMMON_H_
#define _CGRE_CORE_COMMON_H_

#include <pthread.h>

#include <cgre/math/common.h>

#define CGRE_NODE(N) (N->value)
#define CGRE_NODE_KEY_CMP(X, Y) ((X<Y)?-1:(X>Y))

#define CGRE_NODES_MODE(N) (N & 3)
#define CGRE_NODES_MODE_SET(N, M) ((N & !3) | M)
#define CGRE_NODES_MODE_SET_VALUE(N, M) N = CGRE_NODES_MODE_SET(N, M)

#define CGRE_NODES_LOCK(L) (L & 4)
#define CGRE_NODES_LOCK_SET(L, R) ((L & !4) | R)
#define CGRE_NODES_LOCK_FAIL 4
#define CGRE_NODES_LOCK_SET_FAIL(N) N = CGRE_NODES_LOCK_SET(N, CGRE_NODES_LOCK_FAIL)

struct cgre_node {
    void* value;
    struct cgre_node* link[3];
    cgre_uint_t key;
    cgre_uint_t mode;
    cgre_uint_t state;
    cgre_uint_t dir;
    pthread_mutex_t lock;
};

#define CGRE_NODE_HEAD 0
#define CGRE_NODE_MIDDLE 1
#define CGRE_NODE_TAIL 2

struct cgre_node_set {
    struct cgre_node* link[3];
    cgre_uint_t count;
    cgre_uint_t state;
    pthread_mutex_t lock;
};

cgre_uint_t cgre_hash(void* key);

struct cgre_node* cgre_node_initialize(
        struct cgre_node* node,
        cgre_uint_t key,
        void* value);

struct cgre_node_set* cgre_node_set_initialize(
        struct cgre_node_set* set);

struct cgre_node_set* cgre_node_set_uninitialize(
        struct cgre_node_set* set);

void* cgre_node_uninitialize(
        struct cgre_node* node);

#endif /* ifndef _CGRE_CORE_COMMON_H_ */
