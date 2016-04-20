// Isaiah Solomon
// icsolomo
// 1344680
// CMPS 12B
// dllist.java
// Template code for doubly-linked list of strings.

import java.util.*;

public class dllist {

	public enum position {FIRST, PREVIOUS, FOLLOWING, LAST};

	private class node {
		String item;
		node prev;
		node next;
	}

	private node first = null;
	private node current = null;
	private node last = null;
	private int currentPosition = -1;
	private int lastPosition = -1;

	public void setPosition (position pos) {
		switch (pos) {
			case FIRST:
				current = first;
				currentPosition = 0;
				break;
			case PREVIOUS:
				current = current.prev;
				currentPosition--;
				break;
			case FOLLOWING:
				current = current.next;
				currentPosition++;
				break;
			case LAST:
				current = first;
				while (current.next != null) {
					current = current.next;
					currentPosition++;
				}
				break;
			default :
				throw new IllegalArgumentException();
		}
	}

	public boolean isEmpty () {
		if (currentPosition == -1)
			return true;
		else
			return false;
	}

	public String getItem () {
		if (currentPosition == -1)
			throw new NoSuchElementException();
		else
			return current.item;
	}

	public int getPosition () {
		if (currentPosition == -1)
			throw new NoSuchElementException();
		else
			return currentPosition;
	}

	public void delete () {
		if (currentPosition == -1) {
			throw new NoSuchElementException();
		} else if (current == first && current == last) {
			first = null;
			last = null;
			current = null;
			currentPosition = -1;
			lastPosition = -1;
		} else if (current == last) {
			current.prev.next = null;
			last = current.prev;
			current = current.prev;
			currentPosition--;
			lastPosition--;
		} else if (current == first) {
			current.next.prev = null;
			first = current.next;
			current = current.next;
			lastPosition--;
		} else {
			current.prev.next = current.next;
			current.next.prev = current.prev;
			current = current.next;
			lastPosition--;
		}
		//throw new UnsupportedOperationException();
	}

	public void insert (String item, position pos) {
		if (currentPosition == -1) {
			node m = new node();
			m.item = item;
			m.next = null;
			m.prev = null;
			first = m;
			last = m;
			current = m;
			currentPosition = 0;
			lastPosition = 0;
		} else {
			switch(pos) {
				case FIRST:
					node n = new node();
					n.item = item;
					n.next = first;
					n.prev = null;
					first.prev = n;
					first = n;
					current = n;
					currentPosition = 0;
					lastPosition++;
					break;
				case PREVIOUS:
					if (current.prev == null) {
						node o = new node();
						o.item = item;
						o.next = current;
						o.prev = null;
						current.prev = o;
						first = o;
						current = o;
						lastPosition++;
					} else {
						node p = new node();
						p.item = item;
						p.next = current;
						p.prev = current.prev;
						current.prev.next = p;
						current.prev = p;
						current = p;
						lastPosition++;
					}
					break;
				case FOLLOWING:
					if (current.next == null) {
						node q = new node();
						q.item = item;
						q.prev = current;
						q.next = null;
						current.next = q;
						last = q;
						current = q;
						currentPosition++;
						lastPosition++;
					} else {
						node r = new node();
						r.item = item;
						r.next = current.next;
						r.prev = current;
						current.next.prev = r;
						current.next = r;
						current = r;
						currentPosition++;
						lastPosition++;
					}
					break;
				case LAST:
					node s = new node();
					s.item = item;
					s.prev = last;
					s.next = null;
					last.next = s;
					last = s;
					current = first;
					currentPosition = 0;
					while (current.next != null) {
						current = current.next;
						currentPosition++;
					}
					lastPosition++;
					break;
				default :
					throw new IllegalArgumentException();
			}
		}
	}

	public position first () {
		return position.FIRST;
	}

	public position last () {
		return position.LAST;
	}

	public position previous () {
		return position.PREVIOUS;
	}

	public position following () {
		return position.FOLLOWING;
	}

	public boolean isAtEnd () {
		if (currentPosition == lastPosition)
			return true;
		else
			return false;
	}
}

