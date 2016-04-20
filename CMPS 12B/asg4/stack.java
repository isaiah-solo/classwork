//
// Isaiah Solomon
// icsolomo
// CMPS 12B
// stack.java
// 
//

import java.io.*;
import java.util.*;

public class stack
{
	private class room
	{
		String name;
		room under;
	}
	private room bottom;
	private room top;

	public void push (String room)
	{
		room temp = new room();
		if (bottom == null)
		{
			bottom = temp;
		}
		temp.name = room;
		temp.under = top;
		top = temp;
	}

	public String pop ()
	{
		String temp = top.name;
		top = top.under;
		return temp;
	}

	public void empty ()
	{
		while (top != null)
		{
			top = top.under;
		}
	}

	public boolean isEmpty ()
	{
		if (top == null)
		{
			return true;
		}
		return false;
	}
}
