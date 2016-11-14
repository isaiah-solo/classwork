// Isaiah Solomon
// icsolomo
// 1344680
// CMPS 12B
// xref.java
// Contains code that builds binary tree

import java.io.*;
import java.util.Scanner;
import static java.lang.System.*;

class xref {

    static void processFile(String filename, boolean debug) throws IOException {
        Scanner scan = new Scanner (new File(filename));
        Tree tree = new Tree();
        for (int linenr = 1; scan.hasNextLine (); ++linenr) {
            for (String word: scan.nextLine().split ("\\W+")) {
                out.printf ("%s: %d: %s%n", filename, linenr, word);
                tree.insert(word, linenr);
            }
        }
        scan.close();
        if (debug) {
			tree.debug();
        } else {
			tree.output();
        }
    }

    public static void main(String[] args) {
		boolean flag = false;
		if (args.length > 1) {
			if (args[0].equals("-d"))
				flag = true;
		}
		String filename = args[args.length - 1];
        try {
			processFile(filename, flag);
        } catch (IOException error) {
            auxlib.warn (error.getMessage());
        }
        auxlib.exit();
    }

}

