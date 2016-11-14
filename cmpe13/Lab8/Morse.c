// Standard C libraries
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "Morse.h"
#include "Buttons.h"
#include "Common.h"
#include "Tree.h"
#include "Oled.h"

Node *morse_tree;
uint8_t button_states;
bool morse_init = false;
MorseEvent current_morse_event;
Node *pointer;
char decoded_char;
int button_press_counter = 0;

/**
 * This function initializes the Morse code decoder. This is primarily the generation of the
 * Morse tree: a binary tree consisting of all of the ASCII alphanumeric characters arranged
 * according to the DOTs and DASHes that represent each character. Traversal of the tree is done by
 * taking the left-child if it is a dot and the right-child if it is a dash. If the morse tree is
 * successfully generated, SUCCESS is returned, otherwise STANDARD_ERROR is returned. This function
 * also initializes the Buttons library so that MorseCheckEvents() can work properly.
 * @return Either SUCCESS if the decoding tree was successfully created or STANDARD_ERROR if not.
 */
int MorseInit(void){
    OledInit();
    ButtonsInit();
    morse_init = true;
    char morse_data[63] = {NULL, 'E', 'I', 'S', 'H', '5', '4', 'V', NULL, '3', 'U', 'F', NULL, NULL,
    NULL, NULL, '2', 'A', 'R', 'L', NULL, NULL, NULL, NULL, NULL, 'W', 'P',NULL, NULL, 'J', NULL,
    '1', 'T', 'N', 'D', 'B', '6', NULL, 'X', NULL, NULL, 'K', 'C', NULL, NULL, 'Y', NULL, NULL,
    'M', 'G', 'Z', '7', NULL, 'Q', NULL, NULL, 'O', NULL, '8', NULL, NULL, '9', '0'};
    morse_tree = TreeCreate(6, morse_data);
    TreePrint(morse_tree);
    pointer = morse_tree;
    if (morse_tree)
        return SUCCESS;
    else
        return STANDARD_ERROR;
}

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
char MorseDecode(MorseChar in) {
    if (morse_init == false) {
        return STANDARD_ERROR;
    } else if (in == MORSE_CHAR_DOT) {
        pointer = pointer->leftChild;
        return SUCCESS;
    } else if (in == MORSE_CHAR_DASH) {
        pointer = pointer->rightChild;
        return SUCCESS;
    } else if (in == MORSE_CHAR_END_OF_CHAR) {
        decoded_char = pointer->data;
        pointer = morse_tree;
        return decoded_char;
    } else if (in == MORSE_CHAR_DECODE_RESET) {
        pointer = morse_tree;
        return SUCCESS;
    } else {
        pointer = morse_tree;
        return STANDARD_ERROR;
    }
}

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
MorseEvent MorseCheckEvents(void){
    button_states = ButtonsCheckEvents();
    if (!(BUTTON_EVENT_4DOWN & button_states) && button_press_counter < UINT8_MAX){
        button_press_counter++;
    } else if (!(BUTTON_EVENT_4UP & button_states) && button_press_counter < UINT8_MAX) {
        button_press_counter++;
    }
    if (BUTTON_EVENT_4DOWN & button_states && button_press_counter <
            MORSE_EVENT_LENGTH_UP_INTER_LETTER) {
        current_morse_event = MORSE_EVENT_NONE;
        button_press_counter = 0;
    } else if (BUTTON_EVENT_4DOWN & button_states && button_press_counter >=
            MORSE_EVENT_LENGTH_UP_INTER_LETTER && button_press_counter <
            MORSE_EVENT_LENGTH_UP_INTER_WORD){
        current_morse_event = MORSE_EVENT_INTER_LETTER;
        button_press_counter = 0;
    } else if (BUTTON_EVENT_4DOWN & button_states && button_press_counter >=
            MORSE_EVENT_LENGTH_UP_INTER_WORD && button_press_counter <
            MORSE_EVENT_LENGTH_UP_TIMEOUT) {
        current_morse_event = MORSE_EVENT_INTER_WORD;
        button_press_counter = 0;
    } else if (BUTTON_EVENT_4DOWN & button_states && button_press_counter >=
            MORSE_EVENT_LENGTH_UP_TIMEOUT) {
        current_morse_event = MORSE_EVENT_TIMEOUT;
        button_press_counter = 0;
    }
    if (BUTTON_EVENT_4UP & button_states && button_press_counter < MORSE_EVENT_LENGTH_DOWN_DOT){
        button_press_counter = 0;
        current_morse_event = MORSE_EVENT_NONE;
    } else if (BUTTON_EVENT_4UP & button_states && button_press_counter >= MORSE_EVENT_LENGTH_DOWN_DASH){
        button_press_counter = 0;
        current_morse_event = MORSE_EVENT_DASH;
    } else if (BUTTON_EVENT_4UP & button_states && button_press_counter >=
            MORSE_EVENT_LENGTH_DOWN_DOT && button_press_counter < MORSE_EVENT_LENGTH_DOWN_DASH) {
        button_press_counter = 0;
        current_morse_event = MORSE_EVENT_DOT;
    }
    return current_morse_event;
}