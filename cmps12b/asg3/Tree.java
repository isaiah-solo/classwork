// Isaiah Solomon
// icsolomo
// 1344680
// CMPS 12B
// Tree.java
// Contains methods used for a binary tree

import static java.lang.System.*;

class Tree {

    private class Node {
        String key;
        Queue<Integer> value;
        Node left;
        Node right;
    }
    private Node root;
	private Node current;

    private void debugHelper(Node tree, int depth) {
		int counter = depth;
		if (tree.left != null) {
			debugHelper(tree.left, depth + 1);
		}
		while (counter > 0) {
			System.out.print("  ");
			counter--;
		}
		System.out.println(depth + " " + tree.key);
		if (tree.right != null) {
			debugHelper(tree.right, depth + 1);
		}
    }

    private void outputHelper(Node tree) {
		if (tree.left != null) {
			outputHelper(tree.left);
		}
		System.out.print(tree.key + " :");
		tree.value.queue = tree.value.iterator();
		while (tree.value.queue.hasNext()) {
			System.out.print(" " + tree.value.queue.next());
		}
		System.out.println("");
		if (tree.right != null) {
			outputHelper(tree.right);
		}
    }

    public void insert(String key, Integer linenum) {
        // Insert a word into the tree
		int counter = 0;
		if (root == null) {
			root = new Node();
			root.value = new Queue<Integer>();
			root.value.insert(linenum);
			root.key = key;
			return;
		}
		current = root;
		while (true) {
			counter = 0;
			if (current.key.equals(key)) {
				current.value.insert(linenum);
				return;
			}
			while (key.charAt(counter) == current.key.charAt(counter)) {
				if (counter == key.length() - 1 || counter == current.key.length() - 1) {
					break;
				}
				counter++;
			}
			if (key.charAt(counter) < current.key.charAt(counter)) {
				if (current.left == null) {
					current.left = new Node();
					current = current.left;
					current.value = new Queue<Integer>();
					current.value.insert(linenum);
					current.key = key;
					return;
				}
				current = current.left;
			} else {
				if (current.right == null) {
					current.right = new Node();
					current = current.right;
					current.value = new Queue<Integer>();
					current.value.insert(linenum);
					current.key = key;
					return;
				}
				current = current.right;
			}
		}
    }

    public void debug() {
        // Show debug output of tree
        debugHelper(root, 0);
    }

    public void output() {
        // Show sorted words with lines where each word appears
        outputHelper(root);
    }

}
