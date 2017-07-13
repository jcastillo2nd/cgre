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
 * @brief Add a node to the stack
 *
 * @param[in] list The Node List to push on
 * @param[in] node The Node to push
 * @return node inserted or NULL on error
 *
 * @remark The `cgre_node` must be initialized as the links are updated
 * optimistically as NULL.
 */
struct cgre_node* cgre_stack_push(
        struct cgre_node_set* stack,
        struct cgre_node* node)
{
    struct cgre_node* pushed = node;
    cgre_int_t fail = pthread_mutex_lock(&(stack->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(stack->state);
        return NULL;
    }
    // Let's check for empty queue
    if (stack->link[CGRE_NODE_HEAD] != NULL) {
        // List is not empty, we need to update things
        node->link[CGRE_NODE_TAIL] = stack->link[CGRE_NODE_HEAD];
        stack->link[CGRE_NODE_HEAD]->link[CGRE_NODE_HEAD] = node;
        stack->link[CGRE_NODE_HEAD] = node;
    } else {
        stack->link[CGRE_NODE_HEAD] = node;
        stack->link[CGRE_NODE_TAIL] = node;
    }
    stack->count++;
    fail = pthread_mutex_unlock(&(stack->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(stack->state);
    }
    return pushed;
}

/**
 * @brief Remove a node from the stack
 *
 * @param[in] list The Node List to pop from
 * @return cgre_node or NULL on empty or error
 */
struct cgre_node* cgre_stack_pop(
        struct cgre_node_set* stack)
{
    struct cgre_node* removed = NULL;
    cgre_int_t fail = pthread_mutex_lock(&(stack->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(stack->state);
        return NULL;
    }
    // Lets check for empty queue
    if (stack->link[CGRE_NODE_HEAD] != NULL) {
        // Not empty, pop head
        removed = stack->link[CGRE_NODE_HEAD];
        // Lets check if head is not tail
        if (stack->link[CGRE_NODE_HEAD] != stack->link[CGRE_NODE_TAIL]) {
            // There is in fact more than one item on the stack
            // Head is now next item in stack
            stack->link[CGRE_NODE_HEAD] = removed->link[CGRE_NODE_TAIL];
            // New Head has nothing before it
            stack->link[CGRE_NODE_HEAD]->link[CGRE_NODE_HEAD] = NULL;
        } else {
            // There was only one item on the stack
            stack->link[CGRE_NODE_HEAD] = NULL;
            stack->link[CGRE_NODE_TAIL] = NULL;
        }
        stack->count--;
    }
    fail = pthread_mutex_unlock(&(stack->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(stack->state);
    }
    return removed;
}

/**
 *  @brief View a node top of stack
 *
 *  @param[in] list The Node List to peek at
 *  @return node or NULL on error or empty stack
 */
struct cgre_node* cgre_stack_peek(
        struct cgre_node_set* stack)
{
    cgre_int_t fail = pthread_mutex_lock(&(stack->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(stack->state);
        return NULL;
    }
    // We just want to get the value at the list head, nothing else is done 
    struct cgre_node* node = stack->link[CGRE_NODE_HEAD];
    fail = pthread_mutex_unlock(&(stack->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(stack->state);
    }
    return node;
}
