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

#include <cgre/core/list.h>

/**
 * @file include/cgre/core/list.h <cgre/core/list.h>
 * @brief Node List header file
 *
 * This contains the macros, definitions, structs and public API function
 * declarations for node and list operations
 */

/**
 * @def CGRE_LIST_MODE_DEFAULT
 * @brief Default mode for initialization
 */

/**
 * @def CGRE_LIST_KEY 0
 * @brief Key mode of CGRE_LIST_MODE
 */

/**
 * @def CGRE_LIST_HEAD 1
 * @brief Head mode of CGRE_LIST_MODE
 */

/**
 * @def CGRE_LIST_TAIL 2
 * @brief Key mode of CGRE_LIST_MODE
 */

/**
 * @def CGRE_LIST_INDEX 3
 * @brief List mode of CGRE_LIST_MODE
 */

/**
 * @struct cgre_node_list include/cgre/core/list.h <cgre/core/list.h>
 * @brief Node List struct
 *
 * This struct provides the interface for the various list operations.
 *
 * @remark
 * cgre_node items should never be duplicated on a list. To re-add a value to a
 * list, you *MUST* create a new node initialized with the value, and add the
 * new node.
 *
 * @remark
 * The list operations are thread safe for any given list, but operations
 * directly on the node values may not be.
 *
 * @var struct cgre_node* link[2]
 * The head, middle and tail of the list
 * @var cgre_uint_t count
 * The total number of members in this list
 * @var cgre_uint_t state
 * Flags for errors.
 *
 * @remark
 * This should be zero if all is well when a function returns null
 *
 * @code{.c}
 * struct cgre_node *current = cgre_hash_list_search(list, cgre_hash(name));
 * if (current == NULL) {
 *      if (list->state) {
 *           // Error handling
 *      }
 *      // not found handling
 * }
 * @endcode
 *
 * @var pthread_mutex_t
 * The lock for thread safe operation
 */

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
        parent = array->link[CGRE_LIST_MIDDLE];
        for (cgre_uint_t steps = (array->count - (array->count >> 1));
                steps > 0 && parent->link[CGRE_LIST_TAIL] != NULL;
                steps--){
            // From the middle, work to the end of the array
            parent = parent->link[CGRE_LIST_TAIL];
        }
    } else {
        // Are we a single item list?
        if (array->count == 1) {
            // Start at element 1
            parent = array->link[CGRE_LIST_HEAD];
        }
    }
    // Are we special case 0 elements?
    if (array->count == 0){
        // We are the start and the middle
        array->link[CGRE_LIST_HEAD] = node;
        array->link[CGRE_LIST_MIDDLE] = node;
        array->link[CGRE_LIST_TAIL] = node;
    } else {
        // Set prev and add node
        node->link[CGRE_LIST_HEAD] = parent;
        parent->link[CGRE_LIST_TAIL] = node;
        array->link[CGRE_LIST_TAIL] = node;
    }
    array->count++;
    // Will we get a new middle?
    if (array->count > 1 && (array->count & 1)) {
        // Next is the new middle
        array->link[CGRE_LIST_MIDDLE] =
            array->link[CGRE_LIST_MIDDLE]->link[CGRE_LIST_TAIL];
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
            removed = array->link[CGRE_LIST_MIDDLE];
            if (index != middle) {
                for (cgre_uint_t steps = (index - middle);
                        steps > 0 && removed->link[CGRE_LIST_TAIL] != NULL;
                        steps-- ) {
                    removed = removed->link[CGRE_LIST_TAIL];
                }
            }
            // Are we at the end?
            if (index == (array->count - 1)) {
                removed->link[CGRE_LIST_HEAD]->link[CGRE_LIST_TAIL] = NULL;
            } else {
                // link previous to next node
                removed->link[CGRE_LIST_HEAD]->link[CGRE_LIST_TAIL] =
                    removed->link[CGRE_LIST_TAIL];
                // link next node to previous
                removed->link[CGRE_LIST_TAIL]->link[CGRE_LIST_HEAD] =
                    removed->link[CGRE_LIST_HEAD];
            }
        // Nope, below the fold.
        } else {
            removed = array->link[CGRE_LIST_HEAD];
            if (index != 0) {
                for (cgre_uint_t steps = 0;
                        steps < index;
                        steps++){
                    removed = removed->link[CGRE_LIST_TAIL];
                }
            }
            // Are we at the beginning?
            if (index == 0) {
                removed->link[CGRE_LIST_TAIL]->link[CGRE_LIST_HEAD] = NULL;
            } else {
                // link previous to next node
                removed->link[CGRE_LIST_HEAD]->link[CGRE_LIST_TAIL] =
                    removed->link[CGRE_LIST_TAIL];
                // link next node to previous
                removed->link[CGRE_LIST_TAIL]->link[CGRE_LIST_HEAD] =
                    removed->link[CGRE_LIST_HEAD];
            }
        }
        array->count--;
        // Will we get a new middle?
        if (array->count > 0 && (array->count & 1)) {
            // Yes, set new middle
            array->link[CGRE_LIST_MIDDLE] =
                array->link[CGRE_LIST_MIDDLE]->link[CGRE_LIST_HEAD];
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
struct cgre_node* cgre_array_list_get(
        struct cgre_node_list* list,
        cgre_uint_t index)
{
    struct cgre_node* found = NULL;
    cgre_int_t fail = pthread_mutex_lock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
        return NULL;
    }
    // Are we within bounds?
    if ((index + 1) <= list->count) {
        // Compute the fold
        cgre_uint_t middle = ((list->count >> 1) - 1);
        // Are we above the fold?
        if (index >= middle){
            found = list->link[CGRE_LIST_MIDDLE];
            if (index != middle) {
                for (cgre_uint_t steps = (index - middle);
                        steps > 0 && found->link[CGRE_LIST_TAIL] != NULL;
                        steps-- ) {
                    found = found->link[CGRE_LIST_TAIL];
                }
            }
        // Nope, below the fold.
        } else {
            found = list->link[CGRE_LIST_HEAD];
            if (index != 0) {
                for (cgre_uint_t steps = 0;
                        steps < index;
                        steps++){
                    found = found->link[CGRE_LIST_TAIL];
                }
            }
        }
    }
    fail = pthread_mutex_unlock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
    }
    return found;
}

/**
 * @brief Set an array memeber
 *
 * @param[in] list The Node List to update
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
struct cgre_node* cgre_array_list_set(
        struct cgre_node_list* list,
        struct cgre_node* node,
        cgre_uint_t index)
{
    struct cgre_node* replaced = NULL;
    cgre_int_t fail = pthread_mutex_lock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
        return NULL;
    }
    // Are we within bounds?
    if ((index + 1) <= list->count) {
        // Compute the fold
        cgre_uint_t middle = ((list->count >> 1) - 1);
        // Are we above the fold?
        if (index >= middle){
            replaced = list->link[CGRE_LIST_MIDDLE];
            if (index != middle) {
                for (cgre_uint_t steps = (index - middle);
                        steps > 0 && replaced->link[CGRE_LIST_TAIL] != NULL;
                        steps-- ) {
                    replaced = replaced->link[CGRE_LIST_TAIL];
                }
            }
        // Nope, below the fold.
        } else {
            replaced = list->link[CGRE_LIST_HEAD];
            if (index != 0) {
                for (cgre_uint_t steps = 0;
                        steps < index;
                        steps++){
                    replaced = replaced->link[CGRE_LIST_TAIL];
                }
            }
        }
        // Copy the current head and tail
        node->link[CGRE_LIST_HEAD] = replaced->link[CGRE_LIST_HEAD];
        node->link[CGRE_LIST_TAIL] = replaced->link[CGRE_LIST_TAIL];
        if (index > 0) {
            // Update the heads and tails
            replaced->link[CGRE_LIST_HEAD]->link[CGRE_LIST_TAIL] = node;
            if (index < (list->count - 1)) {
                replaced->link[CGRE_LIST_TAIL]->link[CGRE_LIST_HEAD] = node;
            }
        } else {
            // We are the new head
            list->link[CGRE_LIST_HEAD] = node;
        }
        // Were we the middle?
        if (list->link[CGRE_LIST_MIDDLE] == replaced){
            // Update the middle
            list->link[CGRE_LIST_MIDDLE] = node;
        }
        // Were we the tail?
        if (list->link[CGRE_LIST_TAIL] == replaced){
            // Update the tail
            list->link[CGRE_LIST_TAIL] = node;
        }
    }
    fail = pthread_mutex_unlock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
    }
    return replaced;
}

/**
 * @brief Delete a node from the list
 *
 * @param[in] list The Node List to operate on
 * @param[in] key Multifunction key/index based on mode
 * @return node deleted or NULL on not found or error
 */
struct cgre_node* cgre_hash_list_delete(
        struct cgre_node_list* list,
        cgre_uint_t key)
{
    struct cgre_node* removed = NULL;
    cgre_int_t fail = pthread_mutex_lock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
        return NULL;
    }
    // If key in the list
    if (key < list->link[CGRE_LIST_TAIL]->key &&
            key > list->link[CGRE_LIST_HEAD]->key) {
        // Are we above the fold?
        if (key >= list->link[CGRE_LIST_MIDDLE]->key){
            removed = list->link[CGRE_LIST_MIDDLE];
            if (key != removed->key) {
                // Climb up the list
                for (;key != removed->key;) {
                    removed = removed->link[CGRE_LIST_TAIL];
                }
            }
            // Are we at the end? Without a match?
            if (removed->link[CGRE_LIST_TAIL] == NULL &&
                    key != removed->key) {
                // We are done working on this list
                fail = pthread_mutex_unlock(&(list->lock));
                if (fail) {
                    CGRE_NODES_LOCK_SET_FAIL(list->state);
                }
                return NULL;
            }
        // Nope, below the fold.
        } else {
            // Start from the beginning then
            removed = list->link[CGRE_LIST_HEAD];
            if (key > removed->key) {
                // Climb up the list until middle
                for (;(key != removed->key) && (removed != list->link[CGRE_LIST_MIDDLE]);){
                    removed = removed->link[CGRE_LIST_TAIL];
                }
            }
            // Are we up to the middle? And no match?
            if (removed->link[CGRE_LIST_TAIL] == list->link[CGRE_LIST_MIDDLE] &&
                    key != removed->key) {
                // We are done working on this list
                fail = pthread_mutex_unlock(&(list->lock));
                if (fail) {
                    CGRE_NODES_LOCK_SET_FAIL(list->state);
                }
               return NULL;
            }
        }
        // Are we at the end?
        if (removed == list->link[CGRE_LIST_TAIL]){
            // Remove ourselves from the chain
            removed->link[CGRE_LIST_HEAD]->link[CGRE_LIST_TAIL] = NULL;
            // Set new end
            list->link[CGRE_LIST_TAIL] = removed->link[CGRE_LIST_HEAD];
        } else {
            // Nope, somewhere in the middle
            // Matched. Link previous to next node
            removed->link[CGRE_LIST_HEAD]->link[CGRE_LIST_TAIL] =
                removed->link[CGRE_LIST_TAIL];
            // link next node to previous
            removed->link[CGRE_LIST_TAIL]->link[CGRE_LIST_HEAD] =
                removed->link[CGRE_LIST_HEAD];
        }
        // Will we get a new middle?
        if ((list->count & 1) == 0) {
            // Yes, set new middle
            list->link[CGRE_LIST_MIDDLE] =
                list->link[CGRE_LIST_MIDDLE]->link[CGRE_LIST_HEAD];
        }
        list->count--;
    }
    fail = pthread_mutex_unlock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
    }
    return removed;
}

/**
 * @brief Insert a node into the list
 *
 * @param[in] list The Node List to operate on
 * @param[in] node A node to insert
 * @return node pointer or NULL on error
 *
 * @remark
 * Nodes should be initialized as they rely on the link[]s set to NULL.
 */
struct cgre_node* cgre_hash_list_insert(
        struct cgre_node_list* list,
        struct cgre_node* node)
{
    // Could be dealing with a 0 member list, so start at NULL
    struct cgre_node* parent = NULL;
    cgre_int_t fail = pthread_mutex_lock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
        return NULL;
    }
    // Do we really need to search?
    if (list->count < 2) {
        // No. Are we the only one?
        if (list->count == 0) {
            // Yes. We are the head, middle and tail
            list->link[CGRE_LIST_HEAD] = node;
            list->link[CGRE_LIST_MIDDLE] = node;
            list->link[CGRE_LIST_TAIL] = node;
        } else {
            // No. Are we after the only item?
            if (node->key > list->link[CGRE_LIST_HEAD]->key) {
                // Yes. That is our head
                parent = list->link[CGRE_LIST_HEAD];
                // We are tail
                parent->link[CGRE_LIST_TAIL] = node;
                list->link[CGRE_LIST_TAIL] = node;
            } else {
                // No. We have no HEAD, but we have a tail
                node->link[CGRE_LIST_TAIL] = node->link[CGRE_LIST_HEAD];
                // We are the new head
                list->link[CGRE_LIST_HEAD] = node;
                list->link[CGRE_LIST_TAIL] = parent;
            }
        }
    // Yes. Before we work, Are we already the HEAD, MIDDLE or TAIL?
    } else if (node->key == list->link[CGRE_LIST_MIDDLE]->key ||
            node->key == list->link[CGRE_LIST_HEAD]->key ||
            node->key == list->link[CGRE_LIST_TAIL]->key) {
            // Yes. We cannot be inserted
            fail = pthread_mutex_unlock(&(list->lock));
            if (fail) {
                CGRE_NODES_LOCK_SET_FAIL(list->state);
            }
            return NULL;
    } else {
        // Are we below the middle?
        if (node->key < list->link[CGRE_LIST_MIDDLE]->key) {
            // Yes. Are we below the head?
            if (node->key < list->link[CGRE_LIST_HEAD]->key) {
                // Yes. Our tail is the current head
                node->link[CGRE_LIST_TAIL] = list->link[CGRE_LIST_HEAD];
                // We have no head, and we are the new head
                list->link[CGRE_LIST_HEAD] = node;
            } else {
                // We will start our search at parent head
                parent = list->link[CGRE_LIST_HEAD];
                // We are somewhere above parent, but before the MIDDLE or TAIL
                for (cgre_int_t steps = (list->count - (list->count >> 1));
                        steps > 0 && parent->link[CGRE_LIST_TAIL] != NULL;
                        steps--) {
                    // Are we already here?
                    if(parent->link[CGRE_LIST_TAIL]->key == node->key){
                        // Yes. We cannot be inserted
                        fail = pthread_mutex_unlock(&(list->lock));
                        if (fail) {
                            CGRE_NODES_LOCK_SET_FAIL(list->state);
                        }
                        return NULL;
                    // Are we above this, but before the next?
                    } else if (node->key > parent->key &&
                            node->key < parent->link[CGRE_LIST_TAIL]->key) {
                        // Yes. Next is our tail and we are next head
                        node->link[CGRE_LIST_TAIL] = parent->link[CGRE_LIST_TAIL];
                        parent->link[CGRE_LIST_TAIL]->link[CGRE_LIST_HEAD] = node;
                        // We are the tail of the parent
                        parent->link[CGRE_LIST_TAIL] = node;
                        break;
                    } else {
                        // Let's check the next item
                        parent = parent->link[CGRE_LIST_TAIL];
                    }
                }
            }
        // Are we above the middle?
        } else {
            // Yes. Are we above the tail?
            if (node->key > list->link[CGRE_LIST_TAIL]->key){
                // Yes. We have no tail
                // Our head is the current tail
                node->link[CGRE_LIST_HEAD] = list->link[CGRE_LIST_TAIL];
                // We are the new tail
                list->link[CGRE_LIST_TAIL] = node;
            } else {
                // We will start our search at the parent middle
                parent = list->link[CGRE_LIST_MIDDLE];
                // We are somewhere above middle but before tail
                for (cgre_int_t steps = (list->count - (list->count >> 1));
                        steps > 0 && parent->link[CGRE_LIST_TAIL] != NULL;
                        steps--) {
                    // Are we already here?
                    if(parent->link[CGRE_LIST_TAIL]->key == node->key){
                        // Yes. We cannot be inserted
                        fail = pthread_mutex_unlock(&(list->lock));
                        if (fail) {
                            CGRE_NODES_LOCK_SET_FAIL(list->state);
                        }
                        return NULL;
                    // Are we above this but before next?
                    } else if (node->key > parent->key &&
                            node->key < parent->link[CGRE_LIST_TAIL]->key) {
                        // Yes. Next is our tail and we are next head
                        node->link[CGRE_LIST_TAIL] = parent->link[CGRE_LIST_TAIL];
                        parent->link[CGRE_LIST_TAIL]->link[CGRE_LIST_HEAD] = node;
                        // We are the tail of the parent
                        parent->link[CGRE_LIST_TAIL] = node;
                        break;
                    } else {
                        // Let's check the next item
                        parent = parent->link[CGRE_LIST_TAIL];
                    }
                }
            }
        }
    }
    list->count++;
    // Do we need a new middle?
    if (list->count > 2 && (list->count & 1)) {
        list->link[CGRE_LIST_MIDDLE] = list->link[CGRE_LIST_MIDDLE]->link[CGRE_LIST_TAIL];
    }
    fail = pthread_mutex_unlock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
    }
    return node;
}

/**
 * @brief Replace a node in the list
 *
 * @param[in] list The Node List to operate on
 * @param[in] node A node to replace
 * @return found node or NULL on not found or error
 */
struct cgre_node* cgre_hash_list_replace(
        struct cgre_node_list* list,
        struct cgre_node* node)
{
    struct cgre_node* replaced = NULL;
    struct cgre_node* check = NULL;
    // We are going to be working on this list
    cgre_int_t fail = pthread_mutex_lock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
        return NULL;
    }
    // Do we need to search?
    if ( node->key >= list->link[CGRE_LIST_HEAD]->key &&
            node->key <= list->link[CGRE_LIST_TAIL]->key) {
        // Yes. Are we lucky?
        if (node->key == list->link[CGRE_LIST_HEAD]->key) {
            replaced = list->link[CGRE_LIST_HEAD];
            list->link[CGRE_LIST_HEAD] = node;
        } else if (node->key == list->link[CGRE_LIST_MIDDLE]->key) {
            replaced = list->link[CGRE_LIST_MIDDLE];
            list->link[CGRE_LIST_MIDDLE] = node;
        } else if (node->key == list->link[CGRE_LIST_TAIL]->key) {
            replaced = list->link[CGRE_LIST_TAIL];
            list->link[CGRE_LIST_TAIL] = node;
        } else {
            // No. Are we below the fold?
            if (node->key < list->link[CGRE_LIST_MIDDLE]->key) {
                // Yes. Start check from head
                check = list->link[CGRE_LIST_HEAD];
            } else {
                // No. Start check from middle
                check = list->link[CGRE_LIST_MIDDLE];
            }
            for (cgre_uint_t steps = (list->count - (list->count >> 1));
                    steps > 0 && check->link[CGRE_LIST_TAIL] != NULL;
                    steps--) {
                if (node->key == check->key) {
                    replaced = check;
                    break;
                }
                check = check->link[CGRE_LIST_TAIL];
            }
        }
    }
    // Did we find something?
    if (replaced != NULL) {
        // Node copy neighbors
        node->link[CGRE_LIST_HEAD] = replaced->link[CGRE_LIST_HEAD];
        node->link[CGRE_LIST_TAIL] = replaced->link[CGRE_LIST_TAIL];
        // Node takes over
        if (node->link[CGRE_LIST_HEAD] != NULL) {
            node->link[CGRE_LIST_HEAD]->link[CGRE_LIST_TAIL] = node;
        }
        if (node->link[CGRE_LIST_TAIL] != NULL) {
            node->link[CGRE_LIST_TAIL]->link[CGRE_LIST_HEAD] = node;
        }
    }
    // We are done working with this list
    fail = pthread_mutex_unlock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
        return NULL;
    }
    return replaced;
}

/**
 * @brief Search for a node in the list
 *
 * @param[in] list The Node List to operate on
 * @param[in] node Multifunction key/index based on mode
 * @return found node or NULL not found on error
 */
struct cgre_node* cgre_hash_list_search(
        struct cgre_node_list* list,
        cgre_uint_t key)
{
    struct cgre_node* found = NULL;
    struct cgre_node* check = NULL;
    // We are going to be working on this list
    cgre_int_t fail = pthread_mutex_lock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
        return NULL;
    }
    // Do we need to search?
    if ( key >= list->link[CGRE_LIST_HEAD]->key &&
            key <= list->link[CGRE_LIST_TAIL]->key) {
        // Yes. Are we lucky?
        if (key == list->link[CGRE_LIST_HEAD]->key) {
            found = list->link[CGRE_LIST_HEAD];
        } else if (key == list->link[CGRE_LIST_MIDDLE]->key) {
            found = list->link[CGRE_LIST_MIDDLE];
        } else if (key == list->link[CGRE_LIST_TAIL]->key) {
            found = list->link[CGRE_LIST_TAIL];
        } else {
            // No. Are we below the fold?
            if (key < list->link[CGRE_LIST_MIDDLE]->key) {
                // Yes. Start check from head
                check = list->link[CGRE_LIST_HEAD];
            } else {
                // No. Start check from middle
                check = list->link[CGRE_LIST_MIDDLE];
            }
            for (cgre_uint_t steps = (list->count - (list->count >> 1));
                    steps > 0 && check->link[CGRE_LIST_TAIL] != NULL;
                    steps--) {
                if (key == check->key) {
                    found = check;
                    break;
                }
                check = check->link[CGRE_LIST_TAIL];
            }
        }
    }
    // We are done working with this list
    fail = pthread_mutex_unlock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
        return NULL;
    }
    return found;
}

/**
 * @brief Initialize a Node List
 *
 * @param[in] list The Node List to initialize
 * @param[in] head The top node
 * @param[in] tail The bottom node
 */
struct cgre_node_list* cgre_node_list_initialize(
        struct cgre_node_list* list)
{
    if (pthread_mutex_init(&(list->lock), NULL) != 0) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
        return NULL;
    }
    // We are going to be working on this tree
    cgre_int_t fail = pthread_mutex_lock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
        return NULL;
    }
    list->link[0] = NULL;
    list->link[1] = NULL;
    list->link[2] = NULL;
    list->count = 0;
    list->state = 0;
    fail = pthread_mutex_unlock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
        return NULL;
    }
    return list;
}

/**
 * @brief Uninitialize a Node List
 *
 * @param[in] list The Node List to uninitialize
 * @return cgre_node_list point or NULL on error
 * 
 * @warning
 * This destroys the mutex on the object. There must be no current operations
 * executed on this tree, or tree is uninitialized but NULL returned.
 */
struct cgre_node_list* cgre_node_list_uninitialize(
        struct cgre_node_list* list)
{
    cgre_int_t fail = pthread_mutex_destroy(&(list->lock));
    list->link[0] = NULL;
    list->link[1] = NULL;
    list->link[2] = NULL;
    list->count = 0;
    list->state = 0;
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
        return NULL;
    }
    return list;
}

/**
 * @brief Queue list push
 *
 * @param[in] list The Node List to push to
 * @param[in] node The Node to push
 */
struct cgre_node* cgre_queue_list_push(
        struct cgre_node_list* list,
        struct cgre_node* node)
{
    cgre_int_t fail = pthread_mutex_lock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
        return NULL;
    }
    // Is the list empty?
    if (list->link[CGRE_LIST_HEAD] != NULL) {
        // No, need to push the reference down
        list->link[CGRE_LIST_HEAD]->link[CGRE_LIST_HEAD] = node;
        node->link[CGRE_LIST_TAIL] = list->link[CGRE_LIST_HEAD];
        list->link[CGRE_LIST_HEAD] = node;
    } else {
        // List empty. Just add 1 
        list->link[CGRE_LIST_TAIL] = node;
        list->link[CGRE_LIST_HEAD] = node;
    }
    list->count++;
    fail = pthread_mutex_unlock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
    }
    return node;
}

/**
 * @brief Queue list pop
 *
 * @param[in] list The Node List to pop from
 * @return node or NULL on empty or error
 *
 * @remark
 * Nodes should be initialized as they rely on the link[]s set to NULL.
 * Single item lists result in a node with no prev or next link ( NULL )
 */
struct cgre_node* cgre_queue_list_pop(
        struct cgre_node_list* list)
{
    struct cgre_node* popped = NULL;
    cgre_int_t fail = pthread_mutex_lock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
        return NULL;
    }
    // Is there a queue?
    if (list->link[CGRE_LIST_TAIL] != NULL) {
        popped = list->link[CGRE_LIST_TAIL];
        // Yes, is it the only item?
        if ((cgre_intptr_t) list->link[CGRE_LIST_HEAD] ^
                (cgre_intptr_t) list->link[CGRE_LIST_TAIL]) {
            // No. We need to patch the chain
            popped->link[CGRE_LIST_HEAD]->link[CGRE_LIST_TAIL] = NULL;
            list->link[CGRE_LIST_TAIL] = popped->link[CGRE_LIST_HEAD];
        } else {
            // Queue is now empty
            list->link[CGRE_LIST_HEAD] = NULL;
            list->link[CGRE_LIST_TAIL] = NULL;
        }
        list->count--;
    }
    fail = pthread_mutex_unlock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
    }
    return popped;
}

/**
 * @brief Get the next item on the queue with removing it
 *
 * @param[in] list The Node List to peek on
 */
struct cgre_node* cgre_queue_list_peek(
        struct cgre_node_list* list)
{
    struct cgre_node* peek = NULL;
    cgre_int_t fail = pthread_mutex_lock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
        return NULL;
    }
    // Let's check if something on the queue
    if (list->link[CGRE_LIST_TAIL] != NULL) {
        // We are in business. Grab it and go
        peek = list->link[CGRE_LIST_TAIL];
    }
    fail = pthread_mutex_unlock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
    }
    return peek;
}

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
struct cgre_node* cgre_stack_list_push(
        struct cgre_node_list* list,
        struct cgre_node* node)
{
    struct cgre_node* pushed = node;
    cgre_int_t fail = pthread_mutex_lock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
        return NULL;
    }
    // Let's check for empty queue
    if (list->link[CGRE_LIST_HEAD] != NULL) {
        // List is not empty, we need to update things
        node->link[CGRE_LIST_TAIL] = list->link[CGRE_LIST_HEAD];
        list->link[CGRE_LIST_HEAD]->link[CGRE_LIST_HEAD] = node;
        list->link[CGRE_LIST_HEAD] = node;
    } else {
        list->link[CGRE_LIST_HEAD] = node;
        list->link[CGRE_LIST_TAIL] = node;
    }
    list->count++;
    fail = pthread_mutex_unlock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
    }
    return pushed;
}

/**
 * @brief Remove a node from the stack
 *
 * @param[in] list The Node List to pop from
 * @return cgre_node or NULL on empty or error
 */
struct cgre_node* cgre_stack_list_pop(
        struct cgre_node_list* list)
{
    struct cgre_node* removed = NULL;
    cgre_int_t fail = pthread_mutex_lock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
        return NULL;
    }
    // Lets check for empty queue
    if (list->link[CGRE_LIST_HEAD] != NULL) {
        // Not empty, pop head
        removed = list->link[CGRE_LIST_HEAD];
        // Lets check if head is not tail
        if (list->link[CGRE_LIST_HEAD] != list->link[CGRE_LIST_TAIL]) {
            // There is in fact more than one item on the stack
            // Head is now next item in stack
            list->link[CGRE_LIST_HEAD] = removed->link[CGRE_LIST_TAIL];
            // New Head has nothing before it
            list->link[CGRE_LIST_HEAD]->link[CGRE_LIST_HEAD] = NULL;
        } else {
            // There was only one item on the stack
            list->link[CGRE_LIST_HEAD] = NULL;
            list->link[CGRE_LIST_TAIL] = NULL;
        }
        list->count--;
    }
    fail = pthread_mutex_unlock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
    }
    return removed;
}

/**
 *  @brief View a node top of stack
 *
 *  @param[in] list The Node List to peek at
 *  @return node or NULL on error or empty stack
 */
struct cgre_node* cgre_stack_list_peek(
        struct cgre_node_list* list)
{
    cgre_int_t fail = pthread_mutex_lock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
        return NULL;
    }
    // We just want to get the value at the list head, nothing else is done 
    struct cgre_node* node = list->link[CGRE_LIST_HEAD];
    fail = pthread_mutex_unlock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
    }
    return node;
}
