/*
 * Isaiah Solomon
 * icsolomo
 * CMPS 12M
 * 10/10/2014
 * BusinessSearch.java
 * This file reads the first number in the database, allocates an array of
 * the appropriate size, then allows the user to use a binary search to search
 * the database for the inputed query.
 */

import java.util.*;
import java.io.*;

/**
 * Business Class
 **/
class Business {

    public String name;
    public String number;
    
    Business (String infoString) {
        String[] info = infoString.split(",");
        this.name = info[0];
        this.number = info[1];
    }
}

/**
 * BusinessSearch Class
 **/
public class BusinessSearch {
    
    public static void main (String[] args) throws IOException {
        Scanner scan = new Scanner(System.in);
        List<Business> database = new ArrayList<Business>();

        // Grab data from file
        try {
            BufferedReader in = new BufferedReader(new FileReader(args[0]));
            database = ReadFile(in);
            database = MergeSort(database);
        } catch (ArrayIndexOutOfBoundsException aioobe) {
            System.out.println("Usage: BusinessSearch <file>");
            return;
        }

        int queryCount = 0;
        int failedQueryCount = 0;

        // Infinitely scan for input until manually end
        while (true) {
            System.out.print("> ");
            String query = scan.nextLine();

            if (query.equals("")) break; // End if no input

            String result = BinarySearch(database, query);

            queryCount += 1;
            failedQueryCount += result == null ? 1 : 0;

            System.out.println(result != null ? result : "Not Found");
        }

        // Display amount of total queries & queries not found
        String results = queryCount + " total queries, " + failedQueryCount + " not found.";
        System.out.println(results); 
    }

    public static List<Business> ReadFile (BufferedReader in) throws IOException {
        List<Business> businesses = new ArrayList<Business>();

        String amount = in.readLine();

        while (true) {
            String line = in.readLine();
            if (line == null) break;
            businesses.add(new Business(line));
        }

        return businesses;
    }
    
    public static String BinarySearch (List<Business> database, String query) {
        int lower = 0;
        int upper = database.size() - 1;

        String result = null;

        while (true && database.size() > 0) {
            int current = (lower + upper) / 2;

            Business business = database.get(current);
            String name = business.name;

            if (lower > upper) break;

            if (query.compareTo(name) < 0) upper = current - 1;
            else if (query.compareTo(name) > 0) lower = current + 1;
            else {
                result = business.number;
                break;
            }
        }

        return result;
    }
    
    public static List<Business> MergeSort (List<Business> database) {
        return SplitMerge(database, 0, database.size() - 1);
    }

    public static List<Business> SplitMerge (List<Business> database, int lower, int upper) {
        if (lower == upper) {
            List<Business> leaf = new ArrayList<Business>();
            leaf.add(database.get(lower));
            return leaf;
        }

        int half = (upper + lower) / 2;

        List<Business> firstHalf = SplitMerge(database, lower,  half);
        List<Business> secondHalf = SplitMerge(database, half + 1, upper);

        return Merge(firstHalf, secondHalf);
    }

    public static List<Business> Merge (List<Business> firstHalf, List<Business> secondHalf) {
        List<Business> workspace = new ArrayList<Business>();

        int first = 0;
        int second = 0;

        while (first < firstHalf.size() && second < secondHalf.size()) {
            String firstName = firstHalf.get(first).name;
            String secondName = secondHalf.get(second).name;

            if (firstName.compareTo(secondName) < 0) {
                workspace.add(firstHalf.get(first++));
            } else {
                workspace.add(secondHalf.get(second++));
            }
        }

        while (first < firstHalf.size()) {
            workspace.add(firstHalf.get(first++));
        }

        while (second < secondHalf.size()) {
            workspace.add(secondHalf.get(second++));
        }

        return workspace;
    }
}
