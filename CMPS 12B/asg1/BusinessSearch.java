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

/*****************
 * Business Class
 *****************/
class Business {
	// Variables
	private String name;
	private String phoneNumber;
	
	// Constructors
	Business (String businessInfo) {
		String[] seperatedBusinessInfo = businessInfo.split(",");
		this.name = seperatedBusinessInfo[0];
		this.phoneNumber = seperatedBusinessInfo[1];
	}
	Business (String businessName, String businessNumber) {
		this.name = businessName;
		this.phoneNumber = businessNumber;
	}
	
	// Methods
	public String Name () {
		return this.name;
	}
	public String Number () {
		return this.phoneNumber;
	}
	public void changeNameTo (String newName) {
		this.name = newName;
	}
	public void changeNumberTo (String newNumber) {
		this.phoneNumber = newNumber;
	}
}

/***********************
 * BusinessSearch Class
 ***********************/
public class BusinessSearch {
	// Variables
    static String query;
    static int amountOfBusinesses;
    static Business[] businessList;
	static int totalQueries = 0;
	static int queriesNotFound = 0;
	static boolean fileOpened = false;
    
	/******************** Main ********************/
    public static void main (String[] args) throws IOException{
		Scanner scan = new Scanner(System.in);  // Creates Scanner
		try {
			BufferedReader in = new BufferedReader(new FileReader(args[0]));  // Reads from file
			ReadFile(in);  // Separates data in file into object array
			MergeSort(businessList, amountOfBusinesses);
		} catch (ArrayIndexOutOfBoundsException aioobe) {  // If cannot find file,
			System.out.println("Usage: BusinessSearch businessDB");  // display usage message
			return;
		}
		while (true) {
			query = scan.nextLine();  // Scan input
			if (query.equals("")) {  // If only the ENTER key is inputed,
				break;  // end input loop
			} else {
				System.out.println(BinarySearch(businessList, query, amountOfBusinesses));  // print search result
			}
		}
		System.out.println(totalQueries + " total queries, " + queriesNotFound + " not found.");  // Display amount of total queries & queries not found
    }

	/******************** ReadFile ********************/
	public static void ReadFile (BufferedReader in) throws IOException{
		int counter = 0;
    	while (true) {
    		String line = in.readLine();  // Read next line in file
    		if (line == null) break;  // If end of file, break
			if (counter == 0) {
				BusinessSearch.amountOfBusinesses = Integer.parseInt(line);
				businessList = new Business[BusinessSearch.amountOfBusinesses + 1];
			} else {
				businessList[counter - 1] = new Business(line);  // Add to businessList
			}
			counter++;
    	}
	}
	
	/******************** BinarySearch ********************/
    public static String BinarySearch (Business[] database, String query, int length) {
		int lower = 0, upper = length - 1, current, counter = 0;
		while (true) {
			current = (lower + upper) / 2;  // Index to check
			if (query.equals(database[current].Name())) {  // If query is the same as the index,
				totalQueries++;
				return database[current].Number();  // return the phone number of the business.
			} else if (lower > upper) {	 // Else if query is not found,
				totalQueries++;
				queriesNotFound++;
				return "NOT FOUND";	 // return "NOT FOUND".
			} else {
				while (database[current].Name().charAt(counter) == query.charAt(counter)) {  // Loop until chars are not equal
					if (counter == query.length() - 1 || counter == database[current].Name().length() - 1) {
						break;
					}
					counter++;  // Increment counter
				}
				if ((counter == database[current].Name().length() - 1 && 
				  query.length() > database[current].Name().length()) ||
				  database[current].Name().charAt(counter) < query.charAt(counter))  // If query is after index,
					lower = current + 1;  // set lowerbound to current index + 1.
				else  // Else if query is before index,
					upper = current - 1;  // set upperbound to current index - 1.
				counter = 0;  // Reset counter
            }
		}
    }
    
	/******************** MergeSort ********************/
    public static void MergeSort (Business[] database, int length) {
    	Business[] workspace = new Business[length];  // Create workspace
        SplitMerge(workspace, 0, length - 1);
    }

	/******************** SplitMerge ********************/
    public static void SplitMerge (Business[] workspace, int lower, int upper) {
        if(lower == upper)  // If only one element
            return;
        int half = (upper + lower) / 2;  // Find midpoint
        SplitMerge(workspace, lower,  half);  // Split left side
        SplitMerge(workspace, half + 1, upper);  // Split right side
        Merge(workspace, lower, half + 1, upper);  // Merge and sort
    }

	/******************** Merge ********************/
    public static void Merge (Business[] workspace, int lowPointer, int highPointer, int upper) {
		// Variables
    	int index = 0;
        int lowerBound = lowPointer;
        int mid = highPointer - 1;
        int numberOfItems = upper - lowPointer + 1;
        int counter = 0;

        while(lowPointer <= mid && highPointer <= upper) {
        	while (businessList[lowPointer].Name().charAt(counter) == businessList[highPointer].Name().charAt(counter)) {  // Loop until chars are not equal
        		if (counter == businessList[lowPointer].Name().length() - 1 ||
				counter == businessList[highPointer].Name().length() - 1) {
					break;
				}
				counter++;
        	}
        	if((businessList[lowPointer].Name().length() == counter && 
				businessList[lowPointer].Name().length() < businessList[highPointer].Name().length()) ||
				businessList[lowPointer].Name().charAt(counter) < businessList[highPointer].Name().charAt(counter))  // If left index is before right index, 
        		workspace[index++] = businessList[lowPointer++];  // place left element into workspace
        	else
        		workspace[index++] = businessList[highPointer++];  // place right element into workspace
        	counter = 0;
        }
        while(lowPointer <= mid) {
        	workspace[index++] = businessList[lowPointer++];
        }
        while(highPointer <= upper) {
        	workspace[index++] = businessList[highPointer++];
        }
        for(index = 0; index < numberOfItems; index++)
           businessList[lowerBound + index] = workspace[index];
    }
}