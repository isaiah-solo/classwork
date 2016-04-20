/*
 * Isaiah Solomon
 * icsolomo
 * CMPS 12M
 * Calc.java
 * This file contains all of the methods needed for an RPN calculator, such as 
 *   push, pop, add, subtract, etc.
 */
 
 public class Calc {
	// Variables
	private double[] stack;
	private int current;
	
	// Constructors
	public Calc () {
		stack = new double[100];
		current = -1;
	}
	
	// Methods
	public void push (double x) { 	// PUSH
		stack[++current] = x;
	}
	public double pop () { 			// POP
		double y = stack[current];
		stack[current--] = 0;
		return y;
	}
	public double peek () {			// PEEK
		return stack[current];
	}
	public void add () {			// ADD
		push(pop() + pop());
	}
	public void subtract () {		// SUBTRACT
		double a = pop();
		double b = pop();
		push(b - a);
	}
	public void multiply () {		// MULTIPLY
		push(pop() * pop());
	}
	public void divide () {			// DIVIDE
		double a = pop();
		double b = pop();
		push(b / a);
	}
	public int depth () {			// DEPTH
		return (current + 1);
	}
	public void reciprocal () {		// RECIPROCAL
		push(1 / pop());
	}
 }