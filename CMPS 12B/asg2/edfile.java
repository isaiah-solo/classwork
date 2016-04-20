// Isaiah Solomon
// icsolomo
// 1344680
// CMPS 12B
// edfile.java
// Template for a line-oriented text editor inspired by ed.

import java.util.Scanner;
import java.io.*;
import static java.lang.System.*;

class edfile{

	static int newLines = 0;
	
	public static void main (String[] args) throws IOException {

		System.out.println("\n" + "~~~~ Welcome to the Doubly-Linked List! ~~~~" + "\n");
		System.out.println("#      - Comment");
		System.out.println("$      - Make last line the current line");
		System.out.println("*      - All lines are displayed, and last line is current line");
		System.out.println(".      - Display current line");
		System.out.println("0      - Make first line the current line");
		System.out.println("<      - Make previous line the current line");
		System.out.println(">      - Make next line current line");
		System.out.println("a text - Text is inserted after current line, and new line is current line");
		System.out.println("d      - Current line is deleted");
		System.out.println("i text - Text is inserted before current line, and new line is current line");
		System.out.println("r file - Contents of file are put into list, and last line is current line");
		System.out.println("w file - Lines on list are written to file, and last line is current line" + "\n");

		boolean want_echo = false;
		dllist lines = new dllist ();
		if (args.length > 0) {
			if (args[args.length - 1].length() > 4) {
				if (args[args.length - 1].substring(args[args.length - 1].length() - 4, args[args.length - 1].length()).equals(".txt")) {
					readFile(args[args.length - 1], lines);
					System.out.println("COMPLETED: " + args[args.length - 1] + " has been read." + "\n");
				} else {
					System.out.println("ERROR: File not found." + "\n");
				}
			} else {
				if (! (args[args.length - 1].length() > 1 && args[args.length - 1].equals("-e")))
					System.out.println("ERROR: File not found." + "\n");
			}
			if (args[0].equals("-e"))
				want_echo = true;
		}
		System.out.println("Enter a command:");
		Scanner stdin = new Scanner (in);
		for (;;) {
			System.out.print("> ");
			if (! stdin.hasNextLine()) 
				break;
			String inputline = stdin.nextLine();
			if (want_echo) 
				out.printf ("%s%n", inputline);
			if (inputline.matches ("^\\s*$")) 
				continue;
			char command = inputline.charAt(0);
			switch (command) {
				case '#':
					break;
				case '$':
					if (lines.isEmpty()) {
						System.out.println("ERROR: List is empty.");
					} else {
						lines.setPosition(lines.last());
						System.out.println("Current: " + lines.getItem());
					}
					break;
				case '*':
					if (lines.isEmpty()) {
						System.out.println("ERROR: List is empty.");
					} else {
						System.out.println("\n" + "Current list:");
						lines.setPosition(lines.first());
						System.out.println("  " + lines.getItem());
						while (! lines.isAtEnd()) {
							lines.setPosition(lines.following());
							System.out.println("  " + lines.getItem());
						}
					System.out.println("\n" + "Current: " + lines.getItem());
					}
					break;
				case '.':
					if (lines.isEmpty()) {
						System.out.println("ERROR: List is empty.");
					} else {
						System.out.println("Current: " + lines.getItem());
					}
					break;
				case '0':
					if (lines.isEmpty()) {
						System.out.println("ERROR: List is empty.");
					} else {
						lines.setPosition(lines.first());
						System.out.println("Current: " + lines.getItem());
					}
					break;
				case '<':
					if (lines.isEmpty()) {
						System.out.println("ERROR: List is empty.");
					} else if (lines.getPosition() == 0) {
						System.out.println("ERROR: Already at beginning of list.");
					} else {
						lines.setPosition(lines.previous());
						System.out.println("Current: " + lines.getItem());
					}
					break;
				case '>':
					if (lines.isEmpty()) {
						System.out.println("ERROR: List is empty.");
					} else if (lines.isAtEnd()) {
						System.out.println("ERROR: Already at end of list.");
					} else {
						lines.setPosition(lines.following());
						System.out.println("Current: " + lines.getItem());
					}
					break;
				case 'a':
					if (inputline.length() < 3 || inputline.charAt(1) != ' ') {
						System.out.println("ERROR: Incorrect use.");
					} else {
						lines.insert(inputline.substring(2, inputline.length()), lines.following());
						System.out.println("\n" + "~" + lines.getItem() + " has been inserted~" + "\n");
						System.out.println("Current: " + lines.getItem());
					}
					break;
				case 'd':
					if (lines.isEmpty()) {
						System.out.println("ERROR: List is empty.");
					} else {
						System.out.println("\n" + "~" + lines.getItem() + " has been deleted~");
						lines.delete();
						if (lines.isEmpty())
							System.out.println("~List is empty~" + "\n");
						else
							System.out.println("\n" + "Current: " + lines.getItem());
					}
					break;
				case 'i':
					if (inputline.length() < 3 || inputline.charAt(1) != ' ') {
						System.out.println("ERROR: Incorrect use.");
					} else {
						lines.insert(inputline.substring(2, inputline.length()), lines.previous());
						System.out.println("\n" + "~" + lines.getItem() + " has been inserted~" + "\n");
						System.out.println("Current: " + lines.getItem());	
					}
					break;
				case 'r':
					if (inputline.length() < 3 || inputline.charAt(1) != ' ') {
						System.out.println("ERROR: Incorrect use.");
					} else {
						if (inputline.substring(2, inputline.length()).length() > 4) {
							if (inputline.substring(inputline.length() - 4, inputline.length()).equals(".txt")) {
								readFile(inputline.substring(2, inputline.length()), lines);
								System.out.println("\n" + "~" + newLines + " have been added~" + "\n");
								System.out.println("Current: " + lines.getItem());
								newLines = 0;
							} else {
								System.out.println("ERROR: File not found.");
							}
						} else {
							System.out.println("ERROR: File not found.");
						}
					}
					break;
				case 'w':
					if (inputline.length() < 3 || inputline.charAt(1) != ' ') {
						System.out.println("ERROR: Incorrect use.");
					} else {
						if (inputline.substring(2, inputline.length()).length() > 4) {
							if (inputline.substring(inputline.length() - 4, inputline.length()).equals(".txt")) {
								File file = new File(inputline.substring(2, inputline.length()));
          							BufferedWriter output = new BufferedWriter(new FileWriter(file));
								lines.setPosition(lines.first());
								output.write(lines.getItem());
								while (! lines.isAtEnd()) {
									lines.setPosition(lines.following());
          								output.write("\n" + lines.getItem());
								}
          							output.close();
								System.out.println("\n" + "~List has been written to " + inputline.substring(2, inputline.length()) + "~" + "\n");
							} else {
								File file = new File(inputline.substring(2, inputline.length()) + ".txt");
          							BufferedWriter output = new BufferedWriter(new FileWriter(file));
								lines.setPosition(lines.first());
								output.write(lines.getItem());
								while (! lines.isAtEnd()) {
									lines.setPosition(lines.following());
          								output.write("\n" + lines.getItem());
								}
          							output.close();
								System.out.println("\n" + "~List has been written to " + inputline.substring(2, inputline.length()) + ".txt~" + "\n");
							}
						} else {
							File file = new File(inputline.substring(2, inputline.length()) + ".txt");
          						BufferedWriter output = new BufferedWriter(new FileWriter(file));
							lines.setPosition(lines.first());
							output.write(lines.getItem());
							while (! lines.isAtEnd()) {
								lines.setPosition(lines.following());
          							output.write("\n" + lines.getItem());
							}
          						output.close();
							System.out.println("\n" + "~List has been written to " + inputline.substring(2, inputline.length()) + ".txt~" + "\n");
						}
						System.out.println("Current: " + lines.getItem());
					}
					break;
				default :
					System.out.println("ERROR: Invalid command.");
					break;
			}
		}
		System.out.println("eof");
	}
	
	public static void readFile (String arg, dllist list) throws IOException {
		BufferedReader input = new BufferedReader(new FileReader(arg));
		while (true) {
			String line = input.readLine();
			if (line == null) break;
			if (list.isEmpty())
				list.insert(line, list.first());
			else
				list.insert(line, list.following());
			newLines++;
		}
	}
}

