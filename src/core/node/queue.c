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

#include <cgre/core/set.h>

/**
 * @brief Queue list push
 *
 * @param[in] queue The Node Set to push to
 * @param[in] node The Node to push
 */
struct cgre_node* cgre_queue_push(
        struct cgre_node_set* queue,
        struct cgre_node* node)
{
    cgre_int_t fail = pthread_mutex_lock(&(queue->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(queue->state);
        return NULL;
    }
    // Is the list empty?
    if (queue->link[CGRE_NODE_HEAD] != NULL) {
        // No, need to push the reference down
        queue->link[CGRE_NODE_HEAD]->link[CGRE_NODE_HEAD] = node;
        node->link[CGRE_NODE_TAIL] = queue->link[CGRE_NODE_HEAD];
        queue->link[CGRE_NODE_HEAD] = node;
    } else {
        // List empty. Just add 1 
        queue->link[CGRE_NODE_TAIL] = node;
        queue->link[CGRE_NODE_HEAD] = node;
    }
    queue->count++;
    fail = pthread_mutex_unlock(&(queue->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(queue->state);
    }
    return node;
}

/**
 * @brief Queue list pop
 *
 * @param[in] queue The Node Set to pop from
 * @return node or NULL on empty or error
 *
 * @remark
 * Nodes should be initialized as they rely on the link[]s set to NULL.
 * Single item lists result in a node with no prev or next link ( NULL )
 */
struct cgre_node* cgre_queue_pop(
        struct cgre_node_set* queue)
{
    struct cgre_node* popped = NULL;
    cgre_int_t fail = pthread_mutex_lock(&(queue->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(queue->state);
        return NULL;
    }
    // Is there a queue?
    if (queue->link[CGRE_NODE_TAIL] != NULL) {
        popped = queue->link[CGRE_NODE_TAIL];
        // Yes, is it the only item?
        if ((cgre_intptr_t) queue->link[CGRE_NODE_HEAD] ^
                (cgre_intptr_t) queue->link[CGRE_NODE_TAIL]) {
            // No. We need to patch the chain
            popped->link[CGRE_NODE_HEAD]->link[CGRE_NODE_TAIL] = NULL;
            queue->link[CGRE_NODE_TAIL] = popped->link[CGRE_NODE_HEAD];
        } else {
            // Queue is now empty
            queue->link[CGRE_NODE_HEAD] = NULL;
            queue->link[CGRE_NODE_TAIL] = NULL;
        }
        queue->count--;
    }
    fail = pthread_mutex_unlock(&(queue->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(queue->state);
    }
    return popped;
}

/**
 * @brief Get the next item on the queue with removing it
 *
 * @param[in] queue The Node Set to peek on
 */
struct cgre_node* cgre_queue_peek(
        struct cgre_node_set* queue)
{
    struct cgre_node* peek = NULL;
    cgre_int_t fail = pthread_mutex_lock(&(queue->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(queue->state);
        return NULL;
    }
    // Let's check if something on the queue
    if (queue->link[CGRE_NODE_TAIL] != NULL) {
        // We are in business. Grab it and go
        peek = queue->link[CGRE_NODE_TAIL];
    }
    fail = pthread_mutex_unlock(&(queue->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(queue->state);
    }
    return peek;
}
