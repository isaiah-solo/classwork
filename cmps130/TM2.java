/**
 * CMPS130 - Homework assignment 9
 *
 * Author: Isaiah Solomon
 */

import java.util.LinkedList;
import java.util.List;
import static java.util.Arrays.asList;

public class TM2 {
	
	// Turing machine definition (States, Symbols, Transitions, INITIAL, ACCEPTING)
	
	private enum State { Q0, Q1, Q2, Q3, Q4, Q5, Q6, Q7, Q8 }
	
	private enum Symbol { A, B, C, EMPTY, EOS }

	private enum Direction { Left, Right }
	
	private static class Transition {
		public State fromState;
		public Symbol withSymbol;
		public State toState;
		public Symbol writeSymbol;
		public Direction direction;
		
		public Transition(State fromState, Symbol withSymbol, State toState, Symbol writeSymbol, Direction direction) {
			this.fromState = fromState;
			this.withSymbol = withSymbol;
			this.toState = toState;
			this.writeSymbol = writeSymbol;
			this.direction = direction;
		}
	}
	
	private static Transition[] TRANSITIONS = {
		new Transition(State.Q0, Symbol.EOS, State.Q1, Symbol.EOS, Direction.Right),
		
		new Transition(State.Q1, Symbol.A, State.Q2, Symbol.EMPTY, Direction.Right),
		
		new Transition(State.Q2, Symbol.A, State.Q2, Symbol.A, Direction.Right),
		new Transition(State.Q2, Symbol.EMPTY, State.Q2, Symbol.EMPTY, Direction.Right),
		new Transition(State.Q2, Symbol.B, State.Q6, Symbol.EMPTY, Direction.Left),
		new Transition(State.Q2, Symbol.C, State.Q3, Symbol.EMPTY, Direction.Left),
		
		new Transition(State.Q3, Symbol.A, State.Q4, Symbol.EMPTY, Direction.Right),
		new Transition(State.Q3, Symbol.EMPTY, State.Q3, Symbol.EMPTY, Direction.Left),
		new Transition(State.Q3, Symbol.EOS, State.Q7, Symbol.EOS, Direction.Right),
		
		new Transition(State.Q4, Symbol.EMPTY, State.Q4, Symbol.EMPTY, Direction.Right),
		new Transition(State.Q4, Symbol.C, State.Q3, Symbol.EMPTY, Direction.Left),

		new Transition(State.Q5, Symbol.A, State.Q2, Symbol.EMPTY, Direction.Right),
		new Transition(State.Q5, Symbol.EMPTY, State.Q5, Symbol.EMPTY, Direction.Right),
		new Transition(State.Q5, Symbol.EOS, State.Q8, Symbol.EOS, Direction.Right),

		new Transition(State.Q6, Symbol.A, State.Q6, Symbol.A, Direction.Left),
		new Transition(State.Q6, Symbol.EMPTY, State.Q6, Symbol.EMPTY, Direction.Left),
		new Transition(State.Q6, Symbol.EOS, State.Q5, Symbol.EOS, Direction.Right),

		new Transition(State.Q7, Symbol.EMPTY, State.Q7, Symbol.EMPTY, Direction.Right),
		new Transition(State.Q7, Symbol.EOS, State.Q8, Symbol.EOS, Direction.Right)
	};
	
	private static final State INITIAL = State.Q0;
	
	private static final State[] ACCEPTING = { State.Q8 };
	
	// Command-line interface 
	
	public static void main(String[] args) {
		if (args.length != 1) {
			System.out.println("Usage: java TM [INPUT]");
			return;
		}
		List<Symbol> input = new LinkedList<>();
		input.add(Symbol.EOS);
		for (char c : args[0].toCharArray()) {
			if (c == 'a') input.add(Symbol.A);
			if (c == 'b') input.add(Symbol.B);
			if (c == 'c') input.add(Symbol.C);
		}
		input.add(Symbol.EOS);
		if (simulate(input)) {
			System.out.printf("Accepted input: %s\n", args[0]);
		} else {
			System.out.printf("Rejected input: %s\n", args[0]);
		}
	}
	
	// Turing Machine simulator
	
	public static boolean simulate(List<Symbol> input) {
		/**
		 * Copy input to tape and simulate Turing Machine.
		 * If at some point no matching transitions can be found,
		 * stop the machine and return whether accepting or not.
		 */

		/** Initialize tape */
		Symbol[] tape = new Symbol[input.size()];
		for (int i = 0; i < input.size(); i++) tape[i] = input.get(i);

		/** Set initial variables */
		int current = 0;		// Keeps track of Turing Machine
		int MIN = 0;			// Minimum tape bound
		int MAX = tape.length - 1;	// Maximum tape bound (unless finishing)
		State state = INITIAL;		// Keeps track of current state

		/** Infinitely loop through tape */
		while (true) {
			
			/** Reset transition found variable */
			boolean found = false;

			/** Check if in accept state */
			if (state == ACCEPTING[0]) break;

			/** Iterate through transitions */
			for (int i = 0; i < TRANSITIONS.length; i++) {

				/** Check if transition matches */
				if (tape[current] == TRANSITIONS[i].withSymbol &&
				    state == TRANSITIONS[i].fromState) {
					
					/** Found variable set to true */
					found = true;

					/** Update variables */
					state = TRANSITIONS[i].toState;
					tape[current] = TRANSITIONS[i].writeSymbol;

					/** If direction is left */
					if (TRANSITIONS[i].direction == Direction.Left) {

						/** If out of bounds */
						if (current == MIN) return false;
						
						/** Decrement current */
						--current;
					}

					/** If direction is right */
					if (TRANSITIONS[i].direction == Direction.Right) {

						/** If out of bounds */
						if (current == MAX && state != ACCEPTING[0])
							return false;
						
						/** Increment current */
						++current;
					}

					/** If transition found, break */
					break;
				}
			}

			/** If no matching transition */
			if (found == false) return false;
		}

		/** Input accepted */
		return true;
	}
}