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
 * @brief Add a node to the array
 *
 * @param[in] array The Node Set to add to
 * @param[in] node The Node to add
 * @return node added or NULL on error
 *
 * @warning
 * Do not re-add an existing member to the list. To attach the same value,
 * create a new node with the value, and add the new node instead.
 */
struct cgre_node* cgre_array_add(
        struct cgre_node_set* array,
        struct cgre_node* node)
{
    // Could be dealing with a 0 member list, so start at NULL
    struct cgre_node* parent = NULL;
    cgre_int_t fail = pthread_mutex_lock(&(array->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(array->state);
        return NULL;
    }
    if (array->count >= 2) {
        // Start in the middle
        parent = array->link[CGRE_NODE_MIDDLE];
        for (cgre_uint_t steps = (array->count - (array->count >> 1));
                steps > 0 && parent->link[CGRE_NODE_TAIL] != NULL;
                steps--){
            // From the middle, work to the end of the array
            parent = parent->link[CGRE_NODE_TAIL];
        }
    } else {
        // Are we a single item list?
        if (array->count == 1) {
            // Start at element 1
            parent = array->link[CGRE_NODE_HEAD];
        }
    }
    // Are we special case 0 elements?
    if (array->count == 0){
        // We are the start and the middle
        array->link[CGRE_NODE_HEAD] = node;
        array->link[CGRE_NODE_MIDDLE] = node;
        array->link[CGRE_NODE_TAIL] = node;
    } else {
        // Set prev and add node
        node->link[CGRE_NODE_HEAD] = parent;
        parent->link[CGRE_NODE_TAIL] = node;
        array->link[CGRE_NODE_TAIL] = node;
    }
    array->count++;
    // Will we get a new middle?
    if (array->count > 1 && (array->count & 1)) {
        // Next is the new middle
        array->link[CGRE_NODE_MIDDLE] =
            array->link[CGRE_NODE_MIDDLE]->link[CGRE_NODE_TAIL];
    }
    fail = pthread_mutex_unlock(&(array->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(array->state);
    }
    return node;
}

/**
 * @brief Remove a node from the array
 *
 * @param[in] list The Node List to add to
 * @param[in] index The position in the array to remove
 * @return node removed or NULL on invalid index or error
 */
struct cgre_node* cgre_array_delete(
        struct cgre_node_set* array,
        cgre_uint_t index)
{
    struct cgre_node* removed = NULL;
    cgre_int_t fail = pthread_mutex_lock(&(array->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(array->state);
        return NULL;
    }
    if ((index + 1) <= array->count) {
        // Compute the fold
        cgre_uint_t middle = ((array->count >> 1) - 1);
        // Are we above the fold?
        if (index >= middle){
            removed = array->link[CGRE_NODE_MIDDLE];
            if (index != middle) {
                for (cgre_uint_t steps = (index - middle);
                        steps > 0 && removed->link[CGRE_NODE_TAIL] != NULL;
                        steps-- ) {
                    removed = removed->link[CGRE_NODE_TAIL];
                }
            }
            // Are we at the end?
            if (index == (array->count - 1)) {
                removed->link[CGRE_NODE_HEAD]->link[CGRE_NODE_TAIL] = NULL;
            } else {
                // link previous to next node
                removed->link[CGRE_NODE_HEAD]->link[CGRE_NODE_TAIL] =
                    removed->link[CGRE_NODE_TAIL];
                // link next node to previous
                removed->link[CGRE_NODE_TAIL]->link[CGRE_NODE_HEAD] =
                    removed->link[CGRE_NODE_HEAD];
            }
        // Nope, below the fold.
        } else {
            removed = array->link[CGRE_NODE_HEAD];
            if (index != 0) {
                for (cgre_uint_t steps = 0;
                        steps < index;
                        steps++){
                    removed = removed->link[CGRE_NODE_TAIL];
                }
            }
            // Are we at the beginning?
            if (index == 0) {
                removed->link[CGRE_NODE_TAIL]->link[CGRE_NODE_HEAD] = NULL;
            } else {
                // link previous to next node
                removed->link[CGRE_NODE_HEAD]->link[CGRE_NODE_TAIL] =
                    removed->link[CGRE_NODE_TAIL];
                // link next node to previous
                removed->link[CGRE_NODE_TAIL]->link[CGRE_NODE_HEAD] =
                    removed->link[CGRE_NODE_HEAD];
            }
        }
        array->count--;
        // Will we get a new middle?
        if (array->count > 0 && (array->count & 1)) {
            // Yes, set new middle
            array->link[CGRE_NODE_MIDDLE] =
                array->link[CGRE_NODE_MIDDLE]->link[CGRE_NODE_HEAD];
        }
    }
    fail = pthread_mutex_unlock(&(array->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(array->state);
    }
    return removed;
}

/**
 * @brief Get an array member
 *
 * @param[in] list The Node List to read
 * @param[in] index The Node to retreive
 * @return node at index or NULL on invalid index or error
 */
struct cgre_node* cgre_array_get(
        struct cgre_node_set* array,
        cgre_uint_t index)
{
    struct cgre_node* found = NULL;
    cgre_int_t fail = pthread_mutex_lock(&(array->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(array->state);
        return NULL;
    }
    // Are we within bounds?
    if ((index + 1) <= array->count) {
        // Compute the fold
        cgre_uint_t middle = ((array->count >> 1) - 1);
        // Are we above the fold?
        if (index >= middle){
            found = array->link[CGRE_NODE_MIDDLE];
            if (index != middle) {
                for (cgre_uint_t steps = (index - middle);
                        steps > 0 && found->link[CGRE_NODE_TAIL] != NULL;
                        steps-- ) {
                    found = found->link[CGRE_NODE_TAIL];
                }
            }
        // Nope, below the fold.
        } else {
            found = array->link[CGRE_NODE_HEAD];
            if (index != 0) {
                for (cgre_uint_t steps = 0;
                        steps < index;
                        steps++){
                    found = found->link[CGRE_NODE_TAIL];
                }
            }
        }
    }
    fail = pthread_mutex_unlock(&(array->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(array->state);
    }
    return found;
}

/**
 * @brief Set an array memeber
 *
 * @param[in] array The Node Set to update
 * @param[in] node The node to replace with
 * @param[in] index The index to update at
 * @return node that was replaced or NULL on invalid index or error
 *
 * @remark
 * Returns NULL on an empty set. To add an element to an empty array at index
 * 0, use `cgre_array_list_add()`.
 *
 * @remark
 * This function was implemented for completeness. The `cgre_node` node itself
 * can have node.value updated to a new reference if `cgre_array_list_get()`
 * returns a find from user code instead to re-use the node.
 *
 * @warn
 * An already existing member node must not be used. If you need to set the
 * same value at another index, use a new `cgre_node` initialized with the
 * value instead.
 */
struct cgre_node* cgre_array_set(
        struct cgre_node_set* array,
        struct cgre_node* node,
        cgre_uint_t index)
{
    struct cgre_node* replaced = NULL;
    cgre_int_t fail = pthread_mutex_lock(&(array->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(array->state);
        return NULL;
    }
    // Are we within bounds?
    if ((index + 1) <= array->count) {
        // Compute the fold
        cgre_uint_t middle = ((array->count >> 1) - 1);
        // Are we above the fold?
        if (index >= middle){
            replaced = array->link[CGRE_NODE_MIDDLE];
            if (index != middle) {
                for (cgre_uint_t steps = (index - middle);
                        steps > 0 && replaced->link[CGRE_NODE_TAIL] != NULL;
                        steps-- ) {
                    replaced = replaced->link[CGRE_NODE_TAIL];
                }
            }
        // Nope, below the fold.
        } else {
            replaced = array->link[CGRE_NODE_HEAD];
            if (index != 0) {
                for (cgre_uint_t steps = 0;
                        steps < index;
                        steps++){
                    replaced = replaced->link[CGRE_NODE_TAIL];
                }
            }
        }
        // Copy the current head and tail
        node->link[CGRE_NODE_HEAD] = replaced->link[CGRE_NODE_HEAD];
        node->link[CGRE_NODE_TAIL] = replaced->link[CGRE_NODE_TAIL];
        if (index > 0) {
            // Update the heads and tails
            replaced->link[CGRE_NODE_HEAD]->link[CGRE_NODE_TAIL] = node;
            if (index < (array->count - 1)) {
                replaced->link[CGRE_NODE_TAIL]->link[CGRE_NODE_HEAD] = node;
            }
        } else {
            // We are the new head
            array->link[CGRE_NODE_HEAD] = node;
        }
        // Were we the middle?
        if (array->link[CGRE_NODE_MIDDLE] == replaced){
            // Update the middle
            array->link[CGRE_NODE_MIDDLE] = node;
        }
        // Were we the tail?
        if (array->link[CGRE_NODE_TAIL] == replaced){
            // Update the tail
            array->link[CGRE_NODE_TAIL] = node;
        }
    }
    fail = pthread_mutex_unlock(&(array->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(array->state);
    }
    return replaced;
}
