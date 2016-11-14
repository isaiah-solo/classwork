#ifndef MORSE_H
#define MORSE_H

/**
 * This library implements functions for decoding Morse code. It builds upon the Tree library to use
 * a binary tree data structure for storing the codepoints for all characters. Decoding is then done
 * through simple tree traversal. Additional functionality relies on a 100Hz clock to check the
 * button states for decoding the input of Morse characters through a physical button (BTN4).
 */

#include <stdint.h>

/**
 * This enum specifies the different possible inputs to MorseDecode().
 */
typedef enum {
    MORSE_CHAR_DOT = '.',          /// A Morse DOT.
    MORSE_CHAR_DASH = '-',         /// A Morse DASH.
    MORSE_CHAR_END_OF_CHAR = '#',  /// Signifies END_OF_CHAR: that the Morse string is over and the
                                    // character should be decoded.
    MORSE_CHAR_DECODE_RESET = '\0' /// Signifies a reset: that the internal state of `MorseDecode()`
                                    // should be reset.
} MorseChar;

/**
 * This enum declares constants for all of the possible events that this module can trigger.
 * Specifically there is an event for each Morse code character that can be found as well as a
 * null-event.
 */
typedef enum {
	MORSE_EVENT_NONE,
	MORSE_EVENT_DOT,
	MORSE_EVENT_DASH,
	MORSE_EVENT_INTER_LETTER,
	MORSE_EVENT_INTER_WORD,
	MORSE_EVENT_TIMEOUT
} MorseEvent;

/**
 * Define the length of time, in units of .01s, that either button-presses or time between button
 * presses should be.
 */
typedef enum {
	// Specify the downtime of the button press.
	MORSE_EVENT_LENGTH_DOWN_DOT = 10,
	MORSE_EVENT_LENGTH_DOWN_DASH = 30,

	// Specify the uptime between button presses.
	MORSE_EVENT_LENGTH_UP_INTER_LETTER = 30,
	MORSE_EVENT_LENGTH_UP_INTER_WORD = 70,
	MORSE_EVENT_LENGTH_UP_TIMEOUT = 150
} MorseEventLength;

/**
 * This function initializes the Morse code decoder. This is primarily the generation of the
 * Morse tree: a binary tree consisting of all of the ASCII alphanumeric characters arranged
 * according to the DOTs and DASHes that represent each character. Traversal of the tree is done by
 * taking the left-child if it is a dot and the right-child if it is a dash. If the morse tree is
 * successfully generated, SUCCESS is returned, otherwise STANDARD_ERROR is returned. This function
 * also initializes the Buttons library so that MorseCheckEvents() can work properly.
 * @return Either SUCCESS if the decoding tree was successfully created or STANDARD_ERROR if not.
 */ 
int MorseInit(void);

/**
 * MorseDecode decodes a Morse string by iteratively being passed MORSE_CHAR_DOT or MORSE_CHAR_DASH.
 * Each call that passes a DOT or DASH returns a SUCCESS if the string could still compose a
 * Morse-encoded character. Passing in the special MORSE_CHAR_END_OF_CHAR constant will terminate
 * decoding and return the decoded character. During that call to MorseDecode() the return value
 * will be the character that was decoded or STANDARD_ERROR if it couldn't be decoded. Another
 * special value exists, MORSE_CHAR_DECODE_RESET, which will clear the stored state. When a
 * MORSE_CHAR_DECODE_RESET is done, SUCCESS will be returned. If the input is not a valid MorseChar
 * then the internal state should be reset and STANDARD_ERROR should be returned.
 * 
 * @param in A value from the MorseChar enum which specifies how to traverse the Morse tree.
 * 
 * @return Either SUCCESS on DECODE_RESET or when the next traversal location is still a valid
 *         character, the decoded character on END_OF_CHAR, or STANDARD_ERROR if the Morse tree
 *         hasn't been initialized, the next traversal location doesn't exist/represent a character,
 *         or `in` isn't a valid member of the MorseChar enum.
 */
char MorseDecode(MorseChar in);

/**
 * This function calls ButtonsCheckEvents() once per call and returns which, if any,
 * of the Morse code events listed in the enum above have been encountered. It checks for BTN4
 * events in its input and should be called at 100Hz so that the timing works. The
 * length that BTN4 needs to be held down for a dot is >= 0.1s and < 0.3s with a dash being a button
 * down event for >= 0.3s. The button uptime various between dots/dashes (>= 0.1s), letters
 * (>= 0.3s), and words (>= 0.7s). If the button uptime ends up being >= 1.5s, then no event is
 * triggered on the next button press as the uptime times out.
 *
 * @note This function assumes that the buttons are all unpressed at startup, so that the first
 *       event it will see is a BUTTON_EVENT_*DOWN.
 *
 * So pressing the button for 0.1s, releasing it for 0.1s, pressing it for 0.3s, and then waiting
 * will decode the string '.-' (A). It will trigger the following order of events:
 * 9 MORSE_EVENT_NONEs, 1 MORSE_EVENT_DOT, 39 MORSE_EVENT_NONEs, a MORSE_EVENT_DASH, 69
 * MORSE_EVENT_NONEs, a MORSE_EVENT_END_CHAR, and then MORSE_EVENT_NONEs.
 * 
 * @return The MorseEvent that occurred.
 */
MorseEvent MorseCheckEvents(void);

#endif // MORSE_H
