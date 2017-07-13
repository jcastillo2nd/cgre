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

int cgre_node_initialize_tests();
int cgre_node_uninitialize_tests();

int main(int argc, char** argv)
{
    return (
            cgre_node_initialize_tests() |  // Error 1
            cgre_node_uninitialize_tests() // Error 1
   );
}

int cgre_node_initialize_tests()
{
    int i;
    struct cgre_node node1;
    struct cgre_node node2;
    if (cgre_node_initialize(&node1, 80, NULL) != &node1 ||
            node1.key != 80 || // Sets key
            node1.value != NULL || // Sets NULL value
            cgre_node_initialize(&node2, 44, &i) != &node2 ||
            node2.value != &i || // Sets value
            (node2.link[0] != NULL || node2.link[1] != NULL)) { // NULL Links
        return 1;
    }
    return 0;
}

int cgre_node_uninitialize_tests()
{
    int i;
    struct cgre_node root, l_node, r_node;
    root.value = &i;
    root.link[0] = &l_node;
    root.link[1] = &r_node;
    root.key = 55;
    l_node.value = l_node.link[0] = l_node.link[1] = NULL;
    l_node.key = 44;
    r_node.value = r_node.link[0] = r_node.link[1] = NULL;
    r_node.key = 66;
    if (cgre_node_uninitialize(&root) != &i ||
            root.value != NULL ||
            root.link[0] != NULL ||
            root.link[1] != NULL ||
            root.key != 0) { 
        return 2;
    }
    return 0;
}
