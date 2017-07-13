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
 * @brief Delete a node from the list
 *
 * @param[in] list The Node List to operate on
 * @param[in] key Multifunction key/index based on mode
 * @return node deleted or NULL on not found or error
 */
struct cgre_node* cgre_hash_list_delete(
        struct cgre_node_set* list,
        cgre_uint_t key)
{
    struct cgre_node* removed = NULL;
    cgre_int_t fail = pthread_mutex_lock(&(list->lock));
    if (fail) {
        CGRE_NODES_LOCK_SET_FAIL(list->state);
        return NULL;
    }
    // If key in the list
    if (key < list->link[CGRE_NODE_TAIL]->key &&
            key > list->link[CGRE_NODE_HEAD]->key) {
        // Are we above the fold?
        if (key >= list->link[CGRE_NODE_MIDDLE]->key){
            removed = list->link[CGRE_NODE_MIDDLE];
            if (key != removed->key) {
                // Climb up the list
                for (;key != removed->key;) {
                    removed = removed->link[CGRE_NODE_TAIL];
                }
            }
            // Are we at the end? Without a match?
            if (removed->link[CGRE_NODE_TAIL] == NULL &&
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
            removed = list->link[CGRE_NODE_HEAD];
            if (key > removed->key) {
                // Climb up the list until middle
                for (;(key != removed->key) && (removed != list->link[CGRE_NODE_MIDDLE]);){
                    removed = removed->link[CGRE_NODE_TAIL];
                }
            }
            // Are we up to the middle? And no match?
            if (removed->link[CGRE_NODE_TAIL] == list->link[CGRE_NODE_MIDDLE] &&
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
        if (removed == list->link[CGRE_NODE_TAIL]){
            // Remove ourselves from the chain
            removed->link[CGRE_NODE_HEAD]->link[CGRE_NODE_TAIL] = NULL;
            // Set new end
            list->link[CGRE_NODE_TAIL] = removed->link[CGRE_NODE_HEAD];
        } else {
            // Nope, somewhere in the middle
            // Matched. Link previous to next node
            removed->link[CGRE_NODE_HEAD]->link[CGRE_NODE_TAIL] =
                removed->link[CGRE_NODE_TAIL];
            // link next node to previous
            removed->link[CGRE_NODE_TAIL]->link[CGRE_NODE_HEAD] =
                removed->link[CGRE_NODE_HEAD];
        }
        // Will we get a new middle?
        if ((list->count & 1) == 0) {
            // Yes, set new middle
            list->link[CGRE_NODE_MIDDLE] =
                list->link[CGRE_NODE_MIDDLE]->link[CGRE_NODE_HEAD];
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
        struct cgre_node_set* list,
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
            list->link[CGRE_NODE_HEAD] = node;
            list->link[CGRE_NODE_MIDDLE] = node;
            list->link[CGRE_NODE_TAIL] = node;
        } else {
            // No. Are we after the only item?
            if (node->key > list->link[CGRE_NODE_HEAD]->key) {
                // Yes. That is our head
                parent = list->link[CGRE_NODE_HEAD];
                // We are tail
                parent->link[CGRE_NODE_TAIL] = node;
                list->link[CGRE_NODE_TAIL] = node;
            } else {
                // No. We have no HEAD, but we have a tail
                node->link[CGRE_NODE_TAIL] = node->link[CGRE_NODE_HEAD];
                // We are the new head
                list->link[CGRE_NODE_HEAD] = node;
                list->link[CGRE_NODE_TAIL] = parent;
            }
        }
    // Yes. Before we work, Are we already the HEAD, MIDDLE or TAIL?
    } else if (node->key == list->link[CGRE_NODE_MIDDLE]->key ||
            node->key == list->link[CGRE_NODE_HEAD]->key ||
            node->key == list->link[CGRE_NODE_TAIL]->key) {
            // Yes. We cannot be inserted
            fail = pthread_mutex_unlock(&(list->lock));
            if (fail) {
                CGRE_NODES_LOCK_SET_FAIL(list->state);
            }
            return NULL;
    } else {
        // Are we below the middle?
        if (node->key < list->link[CGRE_NODE_MIDDLE]->key) {
            // Yes. Are we below the head?
            if (node->key < list->link[CGRE_NODE_HEAD]->key) {
                // Yes. Our tail is the current head
                node->link[CGRE_NODE_TAIL] = list->link[CGRE_NODE_HEAD];
                // We have no head, and we are the new head
                list->link[CGRE_NODE_HEAD] = node;
            } else {
                // We will start our search at parent head
                parent = list->link[CGRE_NODE_HEAD];
                // We are somewhere above parent, but before the MIDDLE or TAIL
                for (cgre_int_t steps = (list->count - (list->count >> 1));
                        steps > 0 && parent->link[CGRE_NODE_TAIL] != NULL;
                        steps--) {
                    // Are we already here?
                    if(parent->link[CGRE_NODE_TAIL]->key == node->key){
                        // Yes. We cannot be inserted
                        fail = pthread_mutex_unlock(&(list->lock));
                        if (fail) {
                            CGRE_NODES_LOCK_SET_FAIL(list->state);
                        }
                        return NULL;
                    // Are we above this, but before the next?
                    } else if (node->key > parent->key &&
                            node->key < parent->link[CGRE_NODE_TAIL]->key) {
                        // Yes. Next is our tail and we are next head
                        node->link[CGRE_NODE_TAIL] = parent->link[CGRE_NODE_TAIL];
                        parent->link[CGRE_NODE_TAIL]->link[CGRE_NODE_HEAD] = node;
                        // We are the tail of the parent
                        parent->link[CGRE_NODE_TAIL] = node;
                        break;
                    } else {
                        // Let's check the next item
                        parent = parent->link[CGRE_NODE_TAIL];
                    }
                }
            }
        // Are we above the middle?
        } else {
            // Yes. Are we above the tail?
            if (node->key > list->link[CGRE_NODE_TAIL]->key){
                // Yes. We have no tail
                // Our head is the current tail
                node->link[CGRE_NODE_HEAD] = list->link[CGRE_NODE_TAIL];
                // We are the new tail
                list->link[CGRE_NODE_TAIL] = node;
            } else {
                // We will start our search at the parent middle
                parent = list->link[CGRE_NODE_MIDDLE];
                // We are somewhere above middle but before tail
                for (cgre_int_t steps = (list->count - (list->count >> 1));
                        steps > 0 && parent->link[CGRE_NODE_TAIL] != NULL;
                        steps--) {
                    // Are we already here?
                    if(parent->link[CGRE_NODE_TAIL]->key == node->key){
                        // Yes. We cannot be inserted
                        fail = pthread_mutex_unlock(&(list->lock));
                        if (fail) {
                            CGRE_NODES_LOCK_SET_FAIL(list->state);
                        }
                        return NULL;
                    // Are we above this but before next?
                    } else if (node->key > parent->key &&
                            node->key < parent->link[CGRE_NODE_TAIL]->key) {
                        // Yes. Next is our tail and we are next head
                        node->link[CGRE_NODE_TAIL] = parent->link[CGRE_NODE_TAIL];
                        parent->link[CGRE_NODE_TAIL]->link[CGRE_NODE_HEAD] = node;
                        // We are the tail of the parent
                        parent->link[CGRE_NODE_TAIL] = node;
                        break;
                    } else {
                        // Let's check the next item
                        parent = parent->link[CGRE_NODE_TAIL];
                    }
                }
            }
        }
    }
    list->count++;
    // Do we need a new middle?
    if (list->count > 2 && (list->count & 1)) {
        list->link[CGRE_NODE_MIDDLE] = list->link[CGRE_NODE_MIDDLE]->link[CGRE_NODE_TAIL];
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
        struct cgre_node_set* list,
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
    if ( node->key >= list->link[CGRE_NODE_HEAD]->key &&
            node->key <= list->link[CGRE_NODE_TAIL]->key) {
        // Yes. Are we lucky?
        if (node->key == list->link[CGRE_NODE_HEAD]->key) {
            replaced = list->link[CGRE_NODE_HEAD];
            list->link[CGRE_NODE_HEAD] = node;
        } else if (node->key == list->link[CGRE_NODE_MIDDLE]->key) {
            replaced = list->link[CGRE_NODE_MIDDLE];
            list->link[CGRE_NODE_MIDDLE] = node;
        } else if (node->key == list->link[CGRE_NODE_TAIL]->key) {
            replaced = list->link[CGRE_NODE_TAIL];
            list->link[CGRE_NODE_TAIL] = node;
        } else {
            // No. Are we below the fold?
            if (node->key < list->link[CGRE_NODE_MIDDLE]->key) {
                // Yes. Start check from head
                check = list->link[CGRE_NODE_HEAD];
            } else {
                // No. Start check from middle
                check = list->link[CGRE_NODE_MIDDLE];
            }
            for (cgre_uint_t steps = (list->count - (list->count >> 1));
                    steps > 0 && check->link[CGRE_NODE_TAIL] != NULL;
                    steps--) {
                if (node->key == check->key) {
                    replaced = check;
                    break;
                }
                check = check->link[CGRE_NODE_TAIL];
            }
        }
    }
    // Did we find something?
    if (replaced != NULL) {
        // Node copy neighbors
        node->link[CGRE_NODE_HEAD] = replaced->link[CGRE_NODE_HEAD];
        node->link[CGRE_NODE_TAIL] = replaced->link[CGRE_NODE_TAIL];
        // Node takes over
        if (node->link[CGRE_NODE_HEAD] != NULL) {
            node->link[CGRE_NODE_HEAD]->link[CGRE_NODE_TAIL] = node;
        }
        if (node->link[CGRE_NODE_TAIL] != NULL) {
            node->link[CGRE_NODE_TAIL]->link[CGRE_NODE_HEAD] = node;
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
        struct cgre_node_set* list,
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
    if ( key >= list->link[CGRE_NODE_HEAD]->key &&
            key <= list->link[CGRE_NODE_TAIL]->key) {
        // Yes. Are we lucky?
        if (key == list->link[CGRE_NODE_HEAD]->key) {
            found = list->link[CGRE_NODE_HEAD];
        } else if (key == list->link[CGRE_NODE_MIDDLE]->key) {
            found = list->link[CGRE_NODE_MIDDLE];
        } else if (key == list->link[CGRE_NODE_TAIL]->key) {
            found = list->link[CGRE_NODE_TAIL];
        } else {
            // No. Are we below the fold?
            if (key < list->link[CGRE_NODE_MIDDLE]->key) {
                // Yes. Start check from head
                check = list->link[CGRE_NODE_HEAD];
            } else {
                // No. Start check from middle
                check = list->link[CGRE_NODE_MIDDLE];
            }
            for (cgre_uint_t steps = (list->count - (list->count >> 1));
                    steps > 0 && check->link[CGRE_NODE_TAIL] != NULL;
                    steps--) {
                if (key == check->key) {
                    found = check;
                    break;
                }
                check = check->link[CGRE_NODE_TAIL];
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
