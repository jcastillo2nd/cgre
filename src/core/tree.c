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

#include <cgre/core/tree.h>

/**
 * @file include/cgre/core/tree.h
 * @brief Node Tree header file
 *
 * This contains the macros, definitions, structs and public API function
 * declarations for node and node tree operations
 */

/**
 * @def CGRE_RBTREE_MAX_HEIGHT
 * @brief The maximum height for the red-black binary search tree height.
 *
 * @remark This is is modified by the `--with-max-tree-height` configure
 * option.
 *
 * This value is used in `cgre_node_tree` operations. It defaults to 18,
 * however it can be increased at compile time if needed.
 */

/**
 * @def CGRE_TREE_RED 0
 * @brief Tree color is red
 */

/**
 * @def CGRE_TREE_BLACK 1
 * @brief Tree color is black
 */

/**
 * @def CGRE_TREE_UNINITIALIZED 0
 * @brief The tree has not been initialized
 */

/**
 * @def CGRE_TREE_INITIALIZED 1
 * @brief The tree has been initialized
 */

/**
 * @def CGRE_TREE_NOROOT 1
 * @brief The tree has been initialized but has no root
 */

/**
 * @def CGRE_TREE_ROOT 3
 * @brief The tree has been initialized with a root
 */


/**
 * @struct cgre_node_tree include/cgre/core/tree.h <cgre/core/tree.h>
 * @brief Red-Black BST struct
 *
 * @remark
 * Operations with a `cgre_node_tree` are thread-safe.
 *
 * This struct provides the interface for Red-Black BSTs.
 *
 * @var struct cgre_node* root
 * The root node in the tree
 * @var cgre_int_t state
 * The state of the tree
 * @note This isn't currently used by the cgre_node_tree API, so any methods
 * leveraging a tree can use this for their own needs.
 * @var cgre_int_t count
 * Number of members within the tree
 * @var pthread_mutex_t lock
 * A mutex lock used to provide thread safe insert, search and delete.
 */

/**
 * @brief Delete a node from a tree
 *
 * @param[in] tree Tree for deletion
 * @param[in] key Key for deletion
 * @return cgre_node pointer of node removed from tree or NULL
 */
struct cgre_node* cgre_tree_delete(
        struct cgre_node_tree* tree,
        cgre_uint_t key)
{
    struct cgre_node *nodes[CGRE_RBTREE_MAX_HEIGHT];
    unsigned char direction[CGRE_RBTREE_MAX_HEIGHT];
    struct cgre_node* delete_point;
    cgre_int_t height, cmp;
    // We are going to be working on this tree
    pthread_mutex_lock(&(tree->lock));

    height = 0;
    delete_point = tree->root;
    for (cmp = -1; cmp != 0;
         cmp = CGRE_NODE_KEY_CMP(key, delete_point->key))
      {
        cgre_int_t dir = cmp > 0;

        nodes[height] = delete_point;
        direction[height++] = dir;

        delete_point = delete_point->link[dir];
        if (delete_point == NULL)
            // We are done working on this tree
            pthread_mutex_unlock(&(tree->lock));
            return NULL;
      }

    if (delete_point->link[1] == NULL) {
        nodes[height - 1]->link[direction[height - 1]] = delete_point->link[0];
    }
    else {
        cgre_int_t color;
        struct cgre_node *r = delete_point->link[1];

        if (r->link[0] == NULL) {
            r->link[0] = delete_point->link[0];
            color = r->state;
            r->state = delete_point->state;
            delete_point->state = color;
            nodes[height - 1]->link[direction[height - 1]] = r;
            direction[height] = 1;
            nodes[height++] = r;
        } else {
            struct cgre_node *s;
            cgre_int_t j = height++;

            for (;;) {
                direction[height] = 0;
                nodes[height++] = r;
                s = r->link[0];
                if (s->link[0] == NULL) {
                  break;
                }

                r = s;
            }

            direction[j] = 1;
            nodes[j] = s;
            nodes[j - 1]->link[direction[j - 1]] = s;

            s->link[0] = delete_point->link[0];
            r->link[0] = s->link[1];
            s->link[1] = delete_point->link[1];

            color = s->state;
            s->state = delete_point->state;
            delete_point->state = color;
        }
    }

    if (CGRE_NODES_MODE(delete_point->state) == CGRE_TREE_BLACK) {
        for (;;) {
            struct cgre_node *x = \
                    nodes[height - 1]->link[direction[height - 1]];
            if (x != NULL && (CGRE_NODES_MODE(x->state) == CGRE_TREE_RED)) {
                x->state = CGRE_NODES_MODE_SET(x->state, CGRE_TREE_BLACK);
                break;
            }
            if (height < 2) {
                break;
            }
            if (direction[height - 1] == 0) {
                struct cgre_node *w = nodes[height - 1]->link[1];

                if (w->state == CGRE_NODES_MODE_SET(w->state, CGRE_TREE_RED)) {
                    w->state = CGRE_NODES_MODE_SET(w->state, CGRE_TREE_BLACK);
                    nodes[height - 1]->state = \
                            CGRE_NODES_MODE_SET(nodes[height - 1]->state,
                                    CGRE_TREE_RED);

                    nodes[height - 1]->link[1] = w->link[0];
                    w->link[0] = nodes[height - 1];
                    nodes[height - 2]->link[direction[height - 2]] = w;

                    nodes[height] = nodes[height - 1];
                    direction[height] = 0;
                    nodes[height - 1] = w;
                    height++;

                    w = nodes[height - 1]->link[1];
                }
                if ((w->link[0] == NULL
                     || CGRE_NODES_MODE(w->link[0]->state) == CGRE_TREE_BLACK)
                    && (w->link[1] == NULL
                        || CGRE_NODES_MODE(w->link[1]->state) == CGRE_TREE_BLACK)) {
                    w->state = CGRE_NODES_MODE_SET(w->state, CGRE_TREE_RED);
                } else {
                    if (w->link[1] == NULL
                        || CGRE_NODES_MODE(w->link[1]->state) == CGRE_TREE_BLACK) {
                        struct cgre_node *y = w->link[0];
                        y->state = CGRE_NODES_MODE_SET(y->state, CGRE_TREE_BLACK);
                        w->state = CGRE_NODES_MODE_SET(w->state, CGRE_TREE_RED);
                        w->link[0] = y->link[1];
                        y->link[1] = w;
                        w = nodes[height - 1]->link[1] = y;
                    }

                    w->state = nodes[height - 1]->state;
                    nodes[height - 1]->state = \
                            CGRE_NODES_MODE_SET(nodes[height - 1]->state, CGRE_TREE_BLACK);
                    w->link[1]->state = \
                            CGRE_NODES_MODE_SET(w->link[1]->state, CGRE_TREE_BLACK);

                    nodes[height - 1]->link[1] = w->link[0];
                    w->link[0] = nodes[height - 1];
                    nodes[height - 2]->link[direction[height - 2]] = w;
                    break;
                }
            } else {
                struct cgre_node *w = nodes[height - 1]->link[0];

                if (CGRE_NODES_MODE(w->state) == CGRE_TREE_RED) {
                    w->state = CGRE_NODES_MODE_SET(w->state, CGRE_TREE_BLACK);
                    nodes[height - 1]->state = \
                            CGRE_NODES_MODE_SET(nodes[height - 1]->state,
                                    CGRE_TREE_RED);

                    nodes[height - 1]->link[0] = w->link[1];
                    w->link[1] = nodes[height - 1];
                    nodes[height - 2]->link[direction[height - 2]] = w;

                    nodes[height] = nodes[height - 1];
                    direction[height] = 1;
                    nodes[height - 1] = w;
                    height++;

                    w = nodes[height - 1]->link[0];
                }
                if ((w->link[0] == NULL
                     || CGRE_NODES_MODE(w->link[0]->state) == CGRE_TREE_BLACK)
                    && (w->link[1] == NULL
                        || CGRE_NODES_MODE(w->link[1]->state) == CGRE_TREE_BLACK)) {
                    w->state = CGRE_NODES_MODE_SET(w->state, CGRE_TREE_RED);
                } else {
                    if (w->link[0] == NULL
                        || CGRE_NODES_MODE(w->link[0]->state) == CGRE_TREE_BLACK) {
                        struct cgre_node *y = w->link[1];
                        y->state = CGRE_NODES_MODE_SET(y->state, CGRE_TREE_BLACK);
                        w->state = CGRE_NODES_MODE_SET(w->state, CGRE_TREE_RED);
                        w->link[1] = y->link[0];
                        y->link[0] = w;
                        w = nodes[height - 1]->link[0] = y;
                    }

                    w->state = nodes[height - 1]->state;
                    nodes[height - 1]->state = \
                            CGRE_NODES_MODE_SET(nodes[height - 1]->state, CGRE_TREE_BLACK);
                    w->link[0]->state = \
                            CGRE_NODES_MODE_SET(w->link[0]->state, CGRE_TREE_BLACK);

                    nodes[height - 1]->link[0] = w->link[1];
                    w->link[1] = nodes[height - 1];
                    nodes[height - 2]->link[direction[height - 2]] = w;
                    break;
                }
            }
            height--;
        }

      }

    tree->count--;
    // We are done working on this tree
    pthread_mutex_unlock(&(tree->lock));
    return delete_point;
}

/**
 * @brief Insert a node into a tree
 *
 * @param[in] tree Tree for insertion
 * @param[in] node Node to insert
 * @return cgre_node pointer of existing or inserted node
 * or NULL if error
 *
 * @note The red-black BST implementation supports a default maximum height of
 * 18, allowing 262144 members.
 *
 * @warning If you are observing segfaults in here,  and you are exceeding 2^18
 * members, try compiling a larger max height with the configuration
 * option `--with-tree-max-height`
 */
struct cgre_node* cgre_tree_insert(
        struct cgre_node_tree* tree,
        struct cgre_node* node)
{
    struct cgre_node *nodes[CGRE_RBTREE_MAX_HEIGHT];
    unsigned char direction[CGRE_RBTREE_MAX_HEIGHT];
    cgre_int_t height, cmp;
    struct cgre_node *insert_point;

    if (tree == NULL || node == NULL) {
        return NULL;
    }
    // We are going to be working on this tree
    pthread_mutex_lock(&(tree->lock));
    // In case node wasn't properly "initialized", set links to NULL
    node->link[0] = node->link[1] = NULL;

    nodes[0] = tree->root;
    direction[0] = 0;
    height = 1;
    for (insert_point = tree->root;
        insert_point != NULL;
        insert_point = insert_point->link[direction[height - 1]]) {
        // Does this key already exist?
        cmp = CGRE_NODE_KEY_CMP(insert_point->key, node->key);
        if (cmp == 0) {
            // We are done working on this tree
            pthread_mutex_unlock(&(tree->lock));
            // Return existing node
            return insert_point;
        }

        nodes[height] = insert_point;
        direction[height++] = cmp > 0;
    }

    nodes[height - 1]->link[direction[height - 1]] = node;
    node->state = CGRE_TREE_BLACK;
    tree->count++;

    while (height >= 3 && nodes[height - 1]->state & CGRE_TREE_RED) {
        if (nodes[height - 2] == 0) {
            struct cgre_node *y = nodes[height - 2]->link[1];

            if (y != NULL && y->state & CGRE_TREE_RED) {
                nodes[height - 1]->state = y->state = CGRE_TREE_BLACK;
                nodes[height - 2]->state = CGRE_TREE_RED;
                height -= 2;
            } else {
                struct cgre_node *x;

                if (direction[height - 1] == 0){
                    y = nodes[height - 1];
                } else {
                    x = nodes[height - 1];
                    y = x->link[1];
                    x->link[1] = y->link[0];
                    y->link[0] = x;
                    nodes[height - 2]->link[0] = y;
                }

                x = nodes[height - 2];
                x->state = CGRE_TREE_RED;
                y->state = CGRE_TREE_BLACK;

                x->link[0] = y->link[1];
                y->link[1] = x;
                nodes[height - 3]->link[direction[height - 3]] = y;
                break;
            }
        } else {
            struct cgre_node *y = nodes[height - 2]->link[0];
            if (y != NULL && y->state & CGRE_TREE_RED) {
                nodes[height - 1]->state = y->state = CGRE_TREE_BLACK;
                nodes[height - 2]->state = CGRE_TREE_RED;
                height -= 2;
            } else {
                struct cgre_node *x;

                if (direction[height - 1] == 1) {
                  y = nodes[height - 1];
                } else {
                    x = nodes[height - 1];
                    y = x->link[0];
                    x->link[0] = y->link[1];
                    y->link[1] = x;
                    nodes[height - 2]->link[1] = y;
                }

                x = nodes[height - 2];
                x->state = CGRE_TREE_RED;
                y->state = CGRE_TREE_BLACK;

                x->link[1] = y->link[0];
                y->link[0] = x;
                nodes[height - 3]->link[direction[height - 3]] = y;
                break;
            }
        }
    }
    tree->root->state = CGRE_TREE_BLACK;
    // We are done working on this tree
    pthread_mutex_unlock(&(tree->lock));

    return node;
}

/**
 * @brief Replace existing node with same key
 *
 * @param[in] tree Tree to replace node in
 * @param[in] node Node to replace by key
 * @return pointer to replaced node or NULL on error
 */
struct cgre_node* cgre_tree_replace(
        struct cgre_node_tree* tree,
        struct cgre_node* node)
{
    struct cgre_node *old;
    old = cgre_tree_search(tree, node->key);
    // We will be working on this tree
    pthread_mutex_lock(&(tree->lock));
    if (old != NULL) {
        node->link[0] = old->link[0];
        node->link[1] = old->link[1];
        if (old == tree->root) {
            tree->root = node;
        } else {
            // Check if we are root node, replace end TODO
            for (struct cgre_node* parent = tree->root; parent != NULL;) {
                if (node->key < parent->key) {
                    if (parent->link[0] == old) {
                        parent->link[0] = node;
                        break;
                    }
                    parent = parent->link[0];
                } else if (node->key > parent->key) {
                    if (parent->link[1] == old) {
                        parent->link[1] = node;
                        break;
                    }
                    parent = parent->link[1];
                } else if (node->key == parent->key) {
                    // We are done working on this tree
                    old = NULL;
                    break;
                }
            }
        }
    }
    // We are done working on this tree
    pthread_mutex_unlock(&(tree->lock));
    return old;
}

/**
 * @brief Search for a node by key
 *
 * @param[in] tree Tree for search
 * @param[in] key Find node with matching key
 * @return cgre_node pointer or NULL if empty tree or not found
 */
struct cgre_node* cgre_tree_search(
        struct cgre_node_tree* tree,
        cgre_uint_t key)
{
    if (tree == NULL) {
        return NULL;
    }
    // We are going to be working on this tree
    pthread_mutex_lock(&(tree->lock));
    for (struct cgre_node* node = tree->root; node != NULL;){
        if (key < node->key) {
            node = node->link[0];
        } else if (key > node->key) {
            node = node->link[1];
        } else if (key == node->key) {
            // We are done working on this tree
            pthread_mutex_unlock(&tree->lock);
            return node;
        }
    }
    // We are done working on this tree
    pthread_mutex_unlock(&tree->lock);
    return NULL;
}

/**
 * @brief Initialize a node tree
 *
 * @param[in] tree Node tree to initialize
 * @param[in] root Node to set as root
 * @return cgre_node_tree pointer or NULL on error
 */
struct cgre_node_tree* cgre_node_tree_initialize(
        struct cgre_node_tree* tree,
        struct cgre_node* root)
{
    if (pthread_mutex_init(&(tree->lock), NULL) != 0) {
        tree->state = tree->state | CGRE_NODES_LOCK_FAIL;
        return NULL;
    }
    // We are going to be working on this tree
    pthread_mutex_lock(&(tree->lock));
    tree->root = root;
    if (root != NULL) {
        tree->count = 1;
        tree->state = CGRE_NODES_MODE_SET(tree->state, CGRE_TREE_ROOT);
    } else {
        tree->count = 0;
        tree->state = CGRE_NODES_MODE_SET(tree->state, CGRE_TREE_INITIALIZED);
    }
    // We are done working on this tree
    pthread_mutex_unlock(&(tree->lock));
    return tree;
}

/**
 * @brief Uninitialize a node tree
 * 
 * @param[in] tree Node tree to uninitialize
 * @return cgre_node_tree pointer or NULL on error
 *
 * @warning
 * This destroys the mutex on the object. There must be no current operations
 * executed on this tree, or a NULL while the tree is uninitialized.
 */
struct cgre_node_tree* cgre_node_tree_uninitialize(
        struct cgre_node_tree* tree)
{
    cgre_int_t fail = pthread_mutex_destroy(&(tree->lock));
    tree->root = NULL;
    tree->count = 0;
    tree->state = CGRE_NODES_MODE_SET(tree->state, CGRE_TREE_UNINITIALIZED)
    if ( fail ) {
        tree->state = tree->state | CGRE_NODES_LOCK_FAIL;
        return NULL;
    }
    return tree;
}
