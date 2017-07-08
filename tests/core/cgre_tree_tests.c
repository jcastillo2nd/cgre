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

#include <cgre/cgre.h>
#include <stdio.h>

int cgre_tree_initialize_tests();
int cgre_tree_search_node_tests();
int cgre_tree_insert_tests();
int cgre_tree_delete_tests();
int cgre_tree_replace_tests();

int main(int argc, char** argv)
{
    return (
        cgre_tree_initialize_tests() & // Error 1
        cgre_tree_search_node_tests() & // Error 2
        cgre_tree_insert_tests() & // Error 4
        cgre_tree_delete_tests() & // Error 8
        cgre_tree_replace_tests() // Error 16
   );
}

int cgre_tree_initialize_tests()
{
    struct cgre_node_tree tree1;
    struct cgre_node_tree tree2;
    struct cgre_node root;
    if (cgre_node_tree_initialize(&tree1, NULL) != &tree1 ||
            tree1.count != 0 ||
            tree1.state != CGRE_NODE_TREE_INITIALIZED ||
            cgre_node_tree_initialize(&tree2, &root) != &tree2 ||
            tree2.root != &root ||
            tree2.count != 1 ||
            tree2.state != CGRE_NODE_TREE_INITIALIZED){
        return 1;
    }
    return 0;
}


int cgre_tree_search_node_tests()
{
    struct cgre_node_tree tree;
    struct cgre_node root;
    cgre_node_initialize(&root, 80, NULL);
    cgre_node_tree_initialize(&tree, &root);
    if (cgre_tree_search(&tree, 80) != &root ||
            tree.root != &root ||
            tree.count != 1 ||
            cgre_tree_search(&tree, 99) != NULL) {
        return 2;
    }
    return 0;
}

int cgre_tree_insert_tests()
{
    struct cgre_node_tree tree1;
    struct cgre_node root1;
    struct cgre_node new1;
    cgre_node_initialize(&root1, 44, NULL);
    cgre_node_initialize(&new1, 99, NULL);
    cgre_node_tree_initialize(&tree1, &root1);
    struct cgre_node_tree tree2;
    struct cgre_node root2;
    struct cgre_node new2;
    cgre_node_initialize(&root2, 44, NULL);
    cgre_node_initialize(&new2, 44, NULL);
    cgre_node_tree_initialize(&tree2, &root2);
    if (cgre_tree_insert(&tree1, &new1) != &new1 ||
            root1.link[1] != &new1 ||
            tree1.count != 2 ||
            cgre_tree_insert(&tree2, &new2) != &root2 ||
            tree2.count != 2 ||
            cgre_tree_search(&tree2, 44) != &root2) {
        return 4;
    }
    return 0;
}

int cgre_tree_delete_tests()
{
    struct cgre_node_tree tree1;
    struct cgre_node root1;
    cgre_node_tree_initialize(&tree1, &root1);
    struct cgre_node_tree tree2;
    struct cgre_node root2;
    struct cgre_node member32;
    struct cgre_node member80;
    cgre_node_tree_initialize(&tree2, NULL);
    cgre_node_initialize(&root2, 44, NULL);
    cgre_node_initialize(&member32, 32, NULL);
    cgre_node_initialize(&member80, 80, NULL);
    root2.link[0] = &member32;
    root2.link[1] = &member80;
    tree2.count = 3;
    tree2.root = &root2;
    if (cgre_tree_delete(&tree1, 80) != NULL ||
            cgre_tree_delete(&tree2, 80) != &member80 ||
            root2.link[1] != NULL ||
            tree2.count != 2 ||
            cgre_tree_search(&tree2, 80) != NULL) {
        return 8;
    }
    return 0;
}

int cgre_tree_replace_tests()
{
    struct cgre_node_tree tree1;
    struct cgre_node root1;
    struct cgre_node member1;
    struct cgre_node new1;
    cgre_node_initialize(&root1, 13, NULL);
    cgre_node_initialize(&member1, 17, NULL);
    cgre_node_initialize(&new1, 8, NULL);
    cgre_node_tree_initialize(&tree1, &root1);
    root1.link[1] = &member1;

    struct cgre_node_tree tree2;
    struct cgre_node root2;
    struct cgre_node member2;
    struct cgre_node new2;
    cgre_node_initialize(&root2, 13, NULL);
    cgre_node_initialize(&member2, 17, NULL);
    cgre_node_initialize(&new2, 13, NULL);
    cgre_node_tree_initialize(&tree2, &root2);
    root2.link[1] = &member2;
    struct cgre_node *result2 = cgre_tree_replace(&tree2, &new2);

    struct cgre_node_tree tree3;
    struct cgre_node root3;
    struct cgre_node member17;
    struct cgre_node member8;
    struct cgre_node member11;
    struct cgre_node member15;
    struct cgre_node member25;
    struct cgre_node new3;
    cgre_node_initialize(&root3, 13, NULL);
    cgre_node_initialize(&member17, 17, NULL);
    cgre_node_initialize(&member8, 8, NULL);
    cgre_node_initialize(&member11, 11, NULL);
    cgre_node_initialize(&member15, 15, NULL);
    cgre_node_initialize(&new3, 8, NULL);
    cgre_node_tree_initialize(&tree3, &root3);
    root3.link[0] = &member8;
    root3.link[1] = &member17;
    member8.link[1] = &member11;
    member17.link[0] = &member15;
    member17.link[1] = &member25;
    struct cgre_node *result3 = cgre_tree_replace(&tree3, &new3);

    if (cgre_tree_replace(&tree1, &new1) != NULL ||
            result2 == NULL ||
            result2 != &root2 ||
            tree2.root != &new2 ||
            tree2.root->link[1] != &member2 ||
            result3 == NULL ||
            result3 != &member8 ||
            result3->link[1] != &member11 ||
            result3->link[0] != NULL ||
            root3.link[0] == &member8 ||
            root3.link[0] != &new3) {
        return 16;
    }
    return 0;
}
