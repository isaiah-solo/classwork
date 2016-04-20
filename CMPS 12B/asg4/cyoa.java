//
// Isaiah Solomon
// icsolomo
// CMPS 12B
// cyoa.java
// 
//

import java.io.*;
import java.util.*;

class cyoa
{
	public static void main (String[] args) throws IOException
	{
		if (args.length < 1)
		{
			System.out.println("No File Found");
			System.exit(1);
		}
		map game = new map();
		stack undo = new stack();
		Scanner in = new Scanner(System.in);
		String input;
		boolean error = false;
		createMap(args[0], game);
		
		game.toRoom(game.beginning());
		System.out.println("");
		while (true)
		{
			if (error == false)
			{
				System.out.println(game.description() + "\n");
				if (game.isAtEnd())
					System.exit(0);
				System.out.println(game.choices() + "\n");
			}
			error = false;
			System.out.print("> ");

			input = in.nextLine();
			if (input.length() > 1)
			{
				System.out.println("\nInvalid command.\n");
				error = true;
				continue;
			}
			switch (input)
			{
				case "a":
					if (game.destination(0) == "invalid")
					{
						System.out.println("\nInvalid command.\n");
						error = true;
						break;
					}
					System.out.println("[" + game.choice(0) + "]\n");
					undo.push(game.room());
					game.toRoom(game.destination(0));
					break;
				case "b":
					if (game.destination(1) == "invalid")
					{
						System.out.println("\nInvalid command.\n");
						error = true;
						break;
					}
					System.out.println("[" + game.choice(1) + "]\n");
					undo.push(game.room());
					game.toRoom(game.destination(1));
					break;
				case "c":
					if (game.destination(2) == "invalid")
					{
						System.out.println("\nInvalid command.\n");
						error = true;
						break;
					}
					System.out.println("[" + game.choice(2) + "]\n");
					undo.push(game.room());
					game.toRoom(game.destination(2));
					break;
				case "d":
					if (game.destination(3) == "invalid")
					{
						System.out.println("\nInvalid command.\n");
						error = true;
						break;
					}
					System.out.println("[" + game.choice(3) + "]\n");
					undo.push(game.room());
					game.toRoom(game.destination(3));
					break;
				case "e":
					if (game.destination(4) == "invalid")
					{
						System.out.println("\nInvalid command.\n");
						error = true;
						break;
					}
					System.out.println("[" + game.choice(4) + "]\n");
					undo.push(game.room());
					game.toRoom(game.destination(4));
					break;
				case "f":
					if (game.destination(5) == "invalid")
					{
						System.out.println("\nInvalid command.\n");
						error = true;
						break;
					}
					System.out.println("[" + game.choice(5) + "]\n");
					undo.push(game.room());
					game.toRoom(game.destination(5));
					break;
				case "g":
					if (game.destination(6) == "invalid")
					{
						System.out.println("\nInvalid command.\n");
						error = true;
						break;
					}
					System.out.println("[" + game.choice(6) + "]\n");
					undo.push(game.room());
					game.toRoom(game.destination(6));
					break;
				case "h":
					if (game.destination(7) == "invalid")
					{
						System.out.println("\nInvalid command.\n");
						error = true;
						break;
					}
					System.out.println("[" + game.choice(7) + "]\n");
					undo.push(game.room());
					game.toRoom(game.destination(7));
					break;
				case "i":
					if (game.destination(8) == "invalid")
					{
						System.out.println("\nInvalid command.\n");
						error = true;
						break;
					}
					System.out.println("[" + game.choice(8) + "]\n");
					undo.push(game.room());
					game.toRoom(game.destination(8));
					break;
				case "j":
					if (game.destination(9) == "invalid")
					{
						System.out.println("\nInvalid command.\n");
						error = true;
						break;
					}
					System.out.println("[" + game.choice(9) + "]\n");
					undo.push(game.room());
					game.toRoom(game.destination(9));
					break;
				case "k":
					if (game.destination(10) == "invalid")
					{
						System.out.println("\nInvalid command.\n");
						error = true;
						break;
					}
					System.out.println("[" + game.choice(10) + "]\n");
					undo.push(game.room());
					game.toRoom(game.destination(10));
					break;
				case "l":
					if (game.destination(11) == "invalid")
					{
						System.out.println("\nInvalid command.\n");
						error = true;
						break;
					}
					System.out.println("[" + game.choice(11) + "]\n");
					undo.push(game.room());
					game.toRoom(game.destination(11));
					break;
				case "q":
					System.out.println("[quit]\n");
					System.exit(0);
					break;
				case "r":
					System.out.println("[restart]\n");
					undo.empty();
					game.toRoom(game.beginning());
					break;
				case "y":
					System.out.println("[information]\n");
					game.layout();
					break;
				case "z":
					System.out.println("[undo]\n");
					if (undo.isEmpty())
					{
						System.out.println("Cannot Undo.\n");
						error = true;
						break;
					}
					game.toRoom(undo.pop());
					break;
				default :
					error = true;
					System.out.println("\nInvalid command.\n");
			}
		}
	}
	
	static void createMap (String file, map game) throws IOException
	{
		list commands = new list();
		readFile(file, commands);
		while (! commands.areEmpty())
		{
			switch (commands.getCommand())
			{
				case 'r':
					game.addRoom(commands.getString());
					commands.remove();
					break;
				case 'd':
					game.addDescription(commands.getString());
					commands.remove();
					break;
				case 'o':
					game.addChoice(commands.getString());
					commands.remove();
					break;
				case 't':
					game.addDestination(commands.getString());
					commands.remove();
					break;
				default :
					System.out.println("Invalid command: " + commands.getCommand());
					System.exit(1);
			}
		}
	}

	static void readFile (String file, list commands) throws IOException
	{
		BufferedReader in = new BufferedReader(new FileReader(file));
		String line = in.readLine();
		while (line != null)
		{
			commands.insert(line);
			line = in.readLine();
		}
	}
}
