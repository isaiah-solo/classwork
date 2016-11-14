// Standard C libraries
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>

#include "Tree.h"

/**
 * This function creates a binary tree of a given size given a serialized array of data. All nodes
 * are allocated on the heap via `malloc()` and store the input data in their data member. Note that
 * this parameter is serialized in a top-down, left-to-right order. This means it follows the
 * left-hand-side of the tree to the bottom and then backs it's way back up.
 *
 * So for a tree like:
 *           A
 *        B     C
 *      D   E F   G
 * The data variable is ordered as [A B D E C F G].
 *
 * This function uses recursion for generating the tree in this top-down, left-right order. This
 * tree, and therefore the input data set, is assumed to be full and balanced therefore each level
 * has 2^level number of nodes in it. If the `data` input is not this size, then this functions
 * behavior is undefined. Since the input data is copied directly into each node, they can
 * originally be allocated on the stack.
 *
 * @param level How many vertical levels the tree will have.
 * @param data A serialized array of the character data that will be stored in all nodes. This array
 *              should be of length `2^level - 1`.
 * @return The head of the created tree or NULL if malloc fails for any node.
 */
int element = 0;
Node *TreeCreate(int level, const char *data) {
    Node *new_tree = malloc(sizeof(Node));
    new_tree->data = data[element];
    element++;
    if (!(new_tree) || level < 1) {
        return NULL;
    }
    if (level > 1) {
        level--;
        new_tree->leftChild = TreeCreate(level, data);
        new_tree->rightChild = TreeCreate(level, data);
        level++;
    } else if (level == 1){
        new_tree->leftChild = NULL;
        new_tree->rightChild = NULL;
    }
    return new_tree;
}

void TreePrint(Node *new_tree){
    printf("%p->%c (l:%p, r:%p)\n", new_tree, new_tree->data, new_tree->leftChild, new_tree->rightChild);
    if (new_tree->leftChild != NULL)
        TreePrint(new_tree->leftChild);
    if (new_tree->rightChild != NULL)
        TreePrint(new_tree->rightChild);
}