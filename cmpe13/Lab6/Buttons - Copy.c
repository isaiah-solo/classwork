// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <stdbool.h>

// User libraries
#include "HardwareDefs.h"
#include "Buttons.h"

void ButtonsInit(void){
    TRISD |= 0x00E0;
    TRISF |= 0x0002;
}
/*
uint8_t ButtonsCheckEvents(void){
    // Initialize variables
    uint8_t button1, button2, button3, button4, buttonState, buttonFlag;
    buttonState = BUTTON_STATES();
    button1 = buttonState;
    button2 = button1;
    button3 = button2;
    button4 = button3;
    if(BUTTON_STATES() | BUTTON_EVENT_NONE){

    // Button 1 events
        if(BUTTON_STATES() & BUTTON_STATE_1 && !(buttonFlag & BUTTON_STATE_1)){
            buttonFlag |= BUTTON_STATE_1;
            button1 |= BUTTON_EVENT_1DOWN;
            button1 ^= BUTTON_EVENT_1UP;
        } else if(!(BUTTON_STATES() & BUTTON_STATE_1) && buttonFlag & BUTTON_STATE_1) {
            buttonFlag &= BUTTON_STATE_1;
            button1 |= BUTTON_EVENT_1UP;
            button1 ^= BUTTON_EVENT_1DOWN;
        }

    // Button 2 events
        if(BUTTON_STATES() & BUTTON_STATE_2 && !(buttonFlag & BUTTON_STATE_2)){
            buttonFlag |= BUTTON_STATE_2;
            button1 |= BUTTON_EVENT_2DOWN;
            button1 ^= BUTTON_EVENT_2UP;
        } else if(!(BUTTON_STATES() & BUTTON_STATE_2) && buttonFlag & BUTTON_STATE_2) {
            buttonFlag &= BUTTON_STATE_2;
            button1 |= BUTTON_EVENT_2UP;
            button1 ^= BUTTON_EVENT_2DOWN;
        }

    // Button 3 events
        if(BUTTON_STATES() & BUTTON_STATE_3 && !(buttonFlag & BUTTON_STATE_3)){
            buttonFlag |= BUTTON_STATE_3;
            button1 |= BUTTON_EVENT_3DOWN;
            button1 ^= BUTTON_EVENT_3UP;
        } else if(!(BUTTON_STATES() & BUTTON_STATE_3) && buttonFlag & BUTTON_STATE_3) {
            buttonFlag &= BUTTON_STATE_3;
            button1 |= BUTTON_EVENT_3UP;
            button1 ^= BUTTON_EVENT_3DOWN;
        }

    // Button 4 events
        if(BUTTON_STATES() & BUTTON_STATE_4 && !(buttonFlag & BUTTON_STATE_4)){
            buttonFlag |= BUTTON_STATE_4;
            button1 |= BUTTON_EVENT_4DOWN;
            button1 ^= BUTTON_EVENT_4UP;
        } else if(!(BUTTON_STATES() & BUTTON_STATE_4) && buttonFlag & BUTTON_STATE_4) {
            buttonFlag &= BUTTON_STATE_4;
            button1 |= BUTTON_EVENT_4UP;
            button1 ^= BUTTON_EVENT_4DOWN;
        }
        buttonState = (((button1 | button2) | button3) | button4); // Combine
        return buttonState;
    } else {
        return BUTTON_EVENT_NONE; // If no button events, return none
    }
}
*/

/*
uint8_t ButtonsCheckEvents(void) {

    uint8_t button1, button2, button3, button4, buttonState;
    if (BUTTON_STATES() | BUTTON_EVENT_NONE) {
        if (BUTTON_STATES() & BUTTON_STATE_1) {
            button1 = BUTTON_EVENT_1DOWN; // Button 1 event is down
        } else {
            button1 = BUTTON_EVENT_1UP; // Button 1 event is up
        }

        if (BUTTON_STATES() & BUTTON_STATE_2) {
            button2 = BUTTON_EVENT_2DOWN; // Button 2 event is down
        } else {
            button2 = BUTTON_EVENT_2UP; // Button 2 event is up
        }

        if (BUTTON_STATES() & BUTTON_STATE_3) {
            button3 = BUTTON_EVENT_3DOWN; // Button 3 event is down
        } else {
            button3 = BUTTON_EVENT_3UP; // Button 3 event is up
        }

        if (BUTTON_STATES() & BUTTON_STATE_4) {
            button4 = BUTTON_EVENT_4DOWN; // Button 4 event is down
        } else {
            button4 = BUTTON_EVENT_4UP; // Button 4 event is up
        }
        buttonState = (((button1 | button2) | button3) | button4); // Combine
        return buttonState;
    } else {
        return BUTTON_EVENT_NONE; // If no button events, return none
    }
}
*/