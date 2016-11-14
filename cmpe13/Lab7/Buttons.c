// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <stdbool.h>

// User libraries
#include "HardwareDefs.h"
#include "Buttons.h"

void ButtonsInit(void) {
    TRISD |= 0x00E0;
    TRISF |= 0x0002;
}

uint8_t ButtonsCheckEvents(void) {

    uint8_t check[4], button[4], buttonFlag = 0, buttonState = 0, i;

    check[3] = check[2];
    check[2] = check[1];
    check[1] = check[0];
    check[0] = BUTTON_STATES();

    if (check[0] == check[1] && check[1] == check[2] && check[2] == check[3]) {
        buttonFlag = 1;
    }

    if (BUTTON_STATES() | BUTTON_EVENT_NONE) {
        if (BUTTON_STATES() & BUTTON_STATE_1) {
            button[0] |= BUTTON_EVENT_1DOWN; // Button 1 event is down
            button[0] ^= BUTTON_EVENT_1UP;
        } 
        else if(!(BUTTON_STATES() & BUTTON_STATE_1) && (buttonFlag)) {
            button[0] |= BUTTON_EVENT_1UP; // Button 1 event is up
            button[0] ^= BUTTON_EVENT_1DOWN;
        }

        if (BUTTON_STATES() & BUTTON_STATE_2) {
            button[1] |= BUTTON_EVENT_2DOWN; // Button 2 event is down
            button[1] ^= BUTTON_EVENT_2UP;
        }
        else if(!(BUTTON_STATES() & BUTTON_STATE_2) && (buttonFlag)) {
            button[1] |= BUTTON_EVENT_2UP; // Button 2 event is up
            button[1] ^= BUTTON_EVENT_2DOWN;
        }

        if (BUTTON_STATES() & BUTTON_STATE_3) {
            button[2] |= BUTTON_EVENT_3DOWN; // Button 3 event is down
            button[2] ^= BUTTON_EVENT_3UP;
        }
        else if(!(BUTTON_STATES() & BUTTON_STATE_3) && (buttonFlag)) {
            button[2] |= BUTTON_EVENT_3UP; // Button 3 event is up
            button[2] ^= BUTTON_EVENT_3DOWN;
        }

        if (BUTTON_STATES() & BUTTON_STATE_4) {
            button[3] |= BUTTON_EVENT_4DOWN; // Button 4 event is down
            button[3] ^= BUTTON_EVENT_4UP;
        }
        else if(!(BUTTON_STATES() & BUTTON_STATE_4) && (buttonFlag)) {
            button[3] |= BUTTON_EVENT_4UP; // Button 4 event is up
            button[3] ^= BUTTON_EVENT_4DOWN;
        }
        for(i = 0; i < 4; i++) {
            buttonState |= button[i]; // OR all the button states
        }
        buttonFlag = 0;
        return buttonState;
    } else {
        buttonFlag = 0;
        return BUTTON_EVENT_NONE; // If no button events, return none
    }
}
