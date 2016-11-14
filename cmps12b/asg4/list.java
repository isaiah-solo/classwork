//
// Isaiah Solomon
// icsolomo
// CMPS 12B
// list.java
// 
//

import java.io.*;
import java.util.*;

public class list
{
	private class node
	{
		char command;
		String string;
		node link;
	}

	private node front;
	private node back;

	public void insert (String line)
	{
		node temp = new node();
		try
		{
			temp.command = line.charAt(0);
		}
		catch (StringIndexOutOfBoundsException sioobe)
		{
			return;
		}
		temp.string = line.substring(2, line.length());
		if (front == null && back == null)
		{
			front = temp;
			back = temp;
			return;
		}
		back.link = temp;
		back = temp;
	}

	public char getCommand ()
	{
		return front.command;
	}

	public String getString ()
	{
		return front.string;
	}

	public void remove ()
	{
		front = front.link;
	}

	public boolean areEmpty ()
	{
		if (front != null && back != null)
			return false;
		return true;
	}
}
