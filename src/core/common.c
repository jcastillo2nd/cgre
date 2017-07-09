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

#include <cgre/core/common.h>

#include <string.h>

/**
 * @file include/cgre/core/common.h
 * @brief Core common header file
 *
 * This contains the macros, definitions, structs and public API function
 * declarations for core operations
 */

/**
 * @def CGRE_NODE(N)
 * @brief Reference the node value
 *
 * This macro provides the `N->value` reference. This should be type-casted
 * from `void` pointer.
 *
 * @code{.c}
 * cgre_scene *scene = (cgre_scene*) CGRE_NODE(node)
 * @endcode
 */

/**
 * @def CGRE_NODE_KEY_CMP(X, Y) (X<Y)?-1:(X>Y)
 * @brief Compare values
 *
 * This macro results in -1, 0 or 1 depending on whether the X value is less
 * than, equal to or greater than Y value. Typically used with cgre_int_t.
 */

/**
 * @def CGRE_NODES_MODE(N) (N & 3)
 * @brief Compute the mode of the collection
 *
 * Used when comparing the mode ( last 2 bits ) of a Node collection such as
 * `cgre_node_tree` or `cgre_node_list`.
 *
 * @code{.c}
 * if (CGRE_NODES_MODE(tree->state) == CGRE_TREE_RED) {
 *      // Do something with this red tree
 * }
 * @endcode
 */

/**
 * @def CGRE_NODES_MODE_SET(N, M) ((N & !3) | M)
 * @brief Compute new mode of the collection
 *
 * Used when setting the mode ( last 2 bits ) of a Node collection such as
 * `cgre_node_tree` or `cgre_node_list`.
 *
 * @code{.c}
 * list->state = CGRE_NODES_MODE_SET(list->state, CGRE_LIST_KEY);
 * @endcode
 */

/**
 * @def CGRE_NODES_MODE_SET_VALUE(N, M) N = CGRE_NODES_MODE_SET(N, M)
 * @brief Assignment of mode
 *
 * Used when assigning the mode ( last 2 bits ) of a Node collection such as
 * `cgre_node_tree` or `cgre_node_list`.
 *
 * @code{.c}
 * CGRE_NODES_MODE_SET_VALUE(tree->state, CGRE_TREE_BLACK);
 * @endcode
 */

/**
 * @def CGRE_NODES_LOCK(L, R) ((L & !4) | R)
 * @brief Compute the lock result of the collection
 *
 * Used when setting the lock state ( 3rd bit ) of a Node collection such as
 * `cgre_node_tree` or `cgre_node_list`.
 *
 * @code{.c}
 * cgre_int_t fail = pthread_mutex_unlock(&(list->lock));
 * if ( fail ) {
 *      list->state = CGRE_NODES_LOCK(list->state, CGRE_NODE_LOCK_FAIL);
 * }
 * @endcode
 */

/**
 * @def CGRE_NODES_LOCK_FAIL 4
 * @brief Indicates the lock state is uncertain
 */

/**
 * @def CGRE_NODES_LOCK_FAIL_SET(N) N = CGRE_NODES_LOCK(N, CGRE_NODES_LOCK_FAIL)
 * @brief Sets the lock fail bit on state
 *
 * @code{.c}
 * list->state = CGRE_NODES_LOCK_FAIL_SET(list->state)
 * @endcode
 */

/**
 * @struct cgre_node include/cgre/core/common.h <cgre/core/common.h>
 * @brief Node struct
 *
 * This struct provides the interface for collections. The operations on values
 * are independent of the collections, so node operations will not impact a
 * particular `cgre_node.value` across any threads.
 *
 * Nodes carry the reference to a value for use within the library across
 * collections.
 *
 * A node should only belong to and be manipulated by a single collection such
 * as `cgre_tree`,or `cgre_node_list` ( including the queue, stack, and array
 * operation ) which operate thread-safely.
 *
 * @warning
 * To ensure thread-safety with collections, do not ever modify a `cgre_node`
 * value directly. Initialize a new node with the new value, copy the key and
 * use the collection replace method instead.
 *
 * @remark
 * Nodes do contain a lock that can be used to indicate that the value is
 * currently held by a thread. While the node itself can still be modified,
 * operations on the values should retain a lock while reading/writing.
 *
 * @var void* value
 * The pointer to the value of the node.
 * @var struct cgre_node* link[2]
 * The left/right pointers to children nodes
 * @var cgre_uint_t key
 * A unique key typically generated by the `CGRE_HASH` macro.
 * @var mode
 * A placeholder for node mode
 * @var cgre_uint_t state
 * A placeholder for node state
 * @var dir
 * A placeholder for node direction
 * @var pthread_mutext_t
 * A thread mutex
 * The nodes do not have locks managed by the collection functions, but other
 * library operations may utilize the lock to holde the object referenced at
 * `node.value`.
 */

/**
 * @brief Generate a hash from a key
 *
 * Generate a unique integer evaluating the bytes of a key. There is collision
 * like MD5 sums but for keys < 128 bytes long this works well.
 *
 * @param[in] key Value to be hashed
 * @return cgre_uint_t hash
 */
cgre_uint_t cgre_hash(void* key)
{
    const char* s = (const char*) key;
    cgre_uint_t sum = 0;
    cgre_int_t interval = 0;
    for (cgre_int_t idx = 0; s[idx] != 0; idx++) {
        if (interval == 1) {
            sum <<= sizeof(char);
            interval--;
        }
        sum ^= (cgre_int_t) s[idx];
        interval++;
    }
    return sum;
}

/**
 * @brief Initialize a node with empty values
 *
 * @param[in] node Node to initialize
 * @param[in] key Key to set for Node
 * @param[in] value Node value pointer
 * @return cgre_node pointer or NULL if error
 */
struct cgre_node* cgre_node_initialize(
        struct cgre_node* node,
        cgre_uint_t key,
        void* value)
{
    node->key = key;
    node->value = value;
    node->link[0] = NULL;
    node->link[1] = NULL;
    return node;
}

/**
 * @brief Uninitialize a node
 *
 * @param[in] node Node to uninitialize
 * @return void pointer to value
 */
void* cgre_node_uninitialize(
        struct cgre_node* node)
{
    void *result = node->value;
    node->value = NULL;
    node->link[0] = NULL;
    node->link[1] = NULL;
    node->key = 0;
    return result;
}
