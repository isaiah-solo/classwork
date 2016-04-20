// Isaiah Solomon
// icsolomo
// 1344680
// CMPS 12B
// Queue.java
// Contains methods used for queues implemented in the binary tree

import java.util.Iterator;
import java.util.NoSuchElementException;

class Queue <Item> implements Iterable <Item> {

	private class Node {
		Item item;
		Node next;
	}
	private Node head = null;
	private Node tail = null;
	public Iterator<Item> queue;
	
	public boolean isempty() {
		if (head == null && tail == null)
			return true;
		return false;
	}

	public void insert(Item newitem) {
		Node x = new Node();
		x.item = newitem;
		if (isempty()) {
			head = x;
			tail = x;
		} else {
			tail.next = x;
			tail = x;
		}
	}

	public Iterator <Item> iterator() {
		return new Itor();
	}

	class Itor implements Iterator <Item> {
		Node current = head;
		public boolean hasNext() {
			return current != null;
	}
		public Item next() {
			if (! hasNext ()) throw new NoSuchElementException();
			Item result = current.item;
			current = current.next;
			return result;
		}
		public void remove() {
			throw new UnsupportedOperationException();
		}
	}

}
