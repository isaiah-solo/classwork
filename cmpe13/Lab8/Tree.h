#ifndef TREE_H
#define TREE_H

/**
 * @file
 *
 * This file implements an API for creating and printing a binary tree built of Nodes. Each node can
 * store a single character. There are no function for manipulating the tree as it is assumed that
 * the tree will be traversed and manipulated depending on the data contained within and no generic
 * solution would work.
 * 
 * @note This libraries relies on malloc() being available and usage, therefore a heap must be set.
 *
 * Example usage for creating a tree:
 * char treeData[7] = "abdecfg"; // Not a string! Missing ending '\0'
 * Node *root = TreeCreate(3, treeData);
 */

/**
 * A node in the binary tree. This is the only element used for representing the tree and there's no
 * object used to represent the tree as a whole. It has a left child, right child, and single char.
 * If either leftChild or rightChild are NULL, it implies that that child doesn't exist. Note that
 * this structure has no way to refer to the parent, therefore it's impossible to traverse up the
 * tree without having first traversed down it.
 */
typedef struct Node {
	struct Node *leftChild;
	struct Node *rightChild;
	char data;
} Node;

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
Node *TreeCreate(int level, const char *data);
void TreePrint(Node *data);

#endif // TREE_H
