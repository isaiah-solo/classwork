//
// Isaiah Solomon
// icsolomo
// CMPS 12B
// map.java
// 
//

import java.io.*;
import java.util.*;

public class map
{
	private class room
	{
		String name;
		String description;
		choice[] choices = new choice[12];
		int x = -1;
		room left;
		room right;
	}

	private class choice
	{
		char letter;
		String option;
		String destination;
	}

	private room root;
	private room first;
	private room current;
	private String[] rooms;

	public String room ()
	{
		return current.name;
	}

	public String description ()
	{
		return current.description;
	}

	public String choice (int number)
	{
		return current.choices[number].option;
	}

	public String choices ()
	{
		int temp = 0;
		String choices = null;
		while (temp <= 12 && current.choices[temp] != null)
		{
			if (choices == null)
				choices = current.choices[temp].letter + " - " + current.choices[temp].option;
			else
				choices += "\n" + current.choices[temp].letter + " - " + current.choices[temp].option;
			temp++;
		}
		return choices;
	}

	public String destination (int number)
	{
		if (current.choices[number] == null)
			return "invalid";
		return current.choices[number].destination;
	}

	public String beginning ()
	{
		return root.name;
	}

	public void toRoom (String name)
	{
		for (room i = root; i != null;)
		{
			if (i.name.equals(name))
			{
				current = i;
				return;
			}
			if (name.compareTo(i.name) < 0)
				i = i.left;
			else
				i = i.right; 
		}
		System.out.println("Destination not found.\n");
		System.exit(1);
	}

	public void addRoom (String name)
	{
		if (root == null)
		{
			root = new room();
			root.name = name;
			current = root;
			return;
		}
		current = root;
		while (true)
		{
			if (name.compareTo(current.name) < 0)
			{
				if (current.left == null)
				{
					current.left = new room();
					current = current.left;
					current.name = name;
					return;
				}
				current = current.left;
			}
			else
			{
				if (current.right == null)
				{
					current.right = new room();
					current = current.right;
					current.name = name;
					return;
				}
				current = current.right;
			}
		}
	}

	public void addDescription (String description)
	{
		if (current.description == null)
		{
			current.description = description;
			return;
		}
		current.description += "\n\n" + description;
	}

	public void addChoice (String option)
	{
		if (current.x == 12)
			return;
		current.x++;
		current.choices[current.x] = new choice();
		current.choices[current.x].option = option;
		switch (current.x)
		{
			case 0: current.choices[current.x].letter = 'a'; break;
			case 1: current.choices[current.x].letter = 'b'; break;
			case 2: current.choices[current.x].letter = 'c'; break;
			case 3: current.choices[current.x].letter = 'd'; break;
			case 4: current.choices[current.x].letter = 'e'; break;
			case 5: current.choices[current.x].letter = 'f'; break;
			case 6: current.choices[current.x].letter = 'g'; break;
			case 7: current.choices[current.x].letter = 'h'; break;
			case 8: current.choices[current.x].letter = 'i'; break;
			case 9: current.choices[current.x].letter = 'j'; break;
			case 10: current.choices[current.x].letter = 'k'; break;
			case 11: current.choices[current.x].letter = 'l'; break;
		}
	}

	public void addDestination (String destination)
	{
		current.choices[current.x].destination = destination;
	}

	public void layout ()
	{
		traverse(root);
		System.out.println("");
	}
	
	public void traverse (room node)
	{
		if (node.left != null)
			traverse(node.left);
		System.out.print(node.name + " : ");
		for (int i = 0; node.choices[i] != null; i++)
			System.out.print(node.choices[i].destination + " ");
		System.out.println("");
		if (node.right != null)
			traverse(node.right);
	}
	
	public boolean isAtEnd ()
	{
		if (current.choices[0] == null)
			 return true;
		return false;
	}
}
