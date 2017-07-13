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

int cgre_stack_push_tests();

int main(int argc, char** argv)
{
    return (
        cgre_stack_push_tests()
    );
}

int cgre_stack_push_tests()
{
    struct cgre_node_set stack;
    cgre_node_set_initialize(&stack);
    struct cgre_node item1;
    cgre_node_initialize(&item1, 1, NULL);
    struct cgre_node item2;
    cgre_node_initialize(&item2, 2, NULL);
    struct cgre_node item3;
    cgre_node_initialize(&item3, 3, NULL);
    if (cgre_stack_push(&stack, &item1) != &item1 ||
            stack.count != 1) {
        return 1;
    }
    if (cgre_stack_push(&stack, &item2) != &item2 ||
            stack.count != 2) {
        return 2;
    }
    if (cgre_stack_push(&stack, &item3) != &item3 ||
            stack.count != 3) {
        return 4;
    }
    return 0;
}
