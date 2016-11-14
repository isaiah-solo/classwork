// Heap size 2048

// **** Include libraries here ****
// Standard C libraries
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Ascii.h"
#include "Buttons.h"
#include "Common.h"
#include "HardwareDefs.h"
#include "Morse.h"
#include "Oled.h"
#include "OledDriver.h"
#include "Tree.h"

// **** Set any macros or preprocessor directives here ****
// Specify a bit mask for setting/clearing the pin corresponding to BTN4. Should only be used when
// unit testing the Morse event checker.
#define BUTTON4_STATE_FLAG (1 << 7)

// **** Declare any data types here ****

typedef enum {
    WAITING,
    PRESSED
} MorseState;

// **** Define any module-level, global, or external variables here ****
MorseState morse_state = WAITING;
MorseEvent morse_event, morse_event_prev;
char string[84], decoded_char;
int position_top = 0, position_bot = 0, line = 1;

// **** Declare any function prototypes here ****
void TopLineClear(void);
void InputCharTop(int x, char character);
void InputCharBot(int x, int y, char character);

// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 20 MHz
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
#pragma config FPLLIDIV   = DIV_2     // Set the PLL input divider to 2
#pragma config FPLLMUL    = MUL_20    // Set the PLL multiplier to 20
#pragma config FPLLODIV   = DIV_1     // Don't modify the PLL output.
#pragma config FNOSC      = PRIPLL    // Set the primary oscillator to internal RC w/ PLL
#pragma config FSOSCEN    = OFF       // Disable the secondary oscillator
#pragma config IESO       = OFF       // Internal/External Switch O
#pragma config POSCMOD    = XT        // Primary Oscillator Configuration
#pragma config OSCIOFNC   = OFF       // Disable clock signal output
#pragma config FPBDIV     = DIV_4     // Set the peripheral clock to 1/4 system clock
#pragma config FCKSM      = CSECMD    // Clock Switching and Monitor Selection
#pragma config WDTPS      = PS1       // Specify the watchdog timer interval (unused)
#pragma config FWDTEN     = OFF       // Disable the watchdog timer
#pragma config ICESEL     = ICS_PGx2  // Allow for debugging with the Uno32
#pragma config PWP        = OFF       // Keep the program flash writeable
#pragma config BWP        = OFF       // Keep the boot flash writeable
#pragma config CP         = OFF       // Disable code protect

int main() {
    // Configure the device for maximum performance but do not change the PBDIV
    // Given the options, this function will change the flash wait states, RAM
    // wait state and enable prefetch cache but will not change the PBDIV.
    // The PBDIV value is already set via the pragma FPBDIV option above..
    SYSTEMConfig(F_SYS, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

    // Auto-configure the PIC32 for optimum performance at the specified operating frequency.
    SYSTEMConfigPerformance(F_SYS);

    // Use the primary oscillator input (8MHz), then PLL it.
    OSCConfig(OSC_POSC_PLL, OSC_PLL_MULT_20, OSC_PLL_POST_1, OSC_FRC_POST_1);

    // Configure the PB bus to run at 1/4 the CPU frequency, 20MHz.
    OSCSetPBDIV(OSC_PB_DIV_4);

    // Enable multi-vector interrupts
    INTEnableSystemMultiVectoredInt();
    INTEnableInterrupts();

    // Set up the UART peripheral so we can send serial data.
    UARTConfigure(UART_USED, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(UART_USED, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(UART_USED, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART_USED, F_PB, UART_BAUD_RATE);
    UARTEnable(UART_USED, UART_ENABLE_FLAGS(UART_TX));

    // And configure printf/scanf to use UART_USED if it's different from the default of UART2.
    if (UART_USED == UART1) {
        __XC_UART = 1;
    }


    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, F_PB / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/
    MorseInit();
    while (1) {
        switch (morse_state) {
            case WAITING:
                if (morse_event == MORSE_EVENT_NONE) {
                    morse_event_prev = morse_event;
                    morse_state = PRESSED;
                } else if (morse_event == MORSE_EVENT_INTER_LETTER) { // points to next letter
                    morse_event_prev = morse_event;
                    decoded_char = MorseDecode(MORSE_CHAR_END_OF_CHAR);
                    InputCharBot(position_bot, line, decoded_char);
                    if (position_bot >= 21) {
                        position_bot = 0;
                        line++;
                    } else {
                        position_bot++;
                    }
                    TopLineClear();
                    morse_state = PRESSED;
                } else if (morse_event == MORSE_EVENT_INTER_WORD) { // puts space between words
                    morse_event_prev = morse_event;
                    if (position_bot >= 20) {
                        position_bot = 0;
                        line++;
                    } else {
                        position_bot += 2;
                    }
                    TopLineClear();
                    morse_state = PRESSED;
                } else if (morse_event == MORSE_EVENT_TIMEOUT) {
                    morse_event_prev = morse_event;
                    morse_state = PRESSED;
                    MorseDecode(MORSE_CHAR_DECODE_RESET);
                    TopLineClear();
                }
                break;

            case PRESSED:
                if (morse_event == MORSE_EVENT_DOT) {
                    MorseDecode(MORSE_CHAR_DOT);
                    InputCharTop(position_top, '.');
                    position_top++;
                    morse_state = WAITING;
                } else if (morse_event == MORSE_EVENT_DASH) {
                    MorseDecode(MORSE_CHAR_DASH);
                    InputCharTop(position_top, '-');
                    position_top++;
                    morse_state = WAITING;
                } else if (morse_event == MORSE_EVENT_NONE) {
                    morse_state = WAITING;
                }
                break;
        }
    }
    /******************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks.
     *****************************************************************************/

    while (1);
}

void TopLineClear(void) {
    int x = 0;
    position_top = 0;
    while (x <= 20) {
        OledDrawChar(x * ASCII_FONT_WIDTH, 0, ' ');
        x++;
    }
    OledUpdate();
}

void InputCharTop(int x, char character) {
    OledDrawChar(x * ASCII_FONT_WIDTH, 0, character);
    OledUpdate();
}

void InputCharBot(int x, int y, char character) {
    OledDrawChar(x * ASCII_FONT_WIDTH, y * ASCII_FONT_HEIGHT, character);
    OledUpdate();
}

void __ISR(_TIMER_2_VECTOR, ipl4) TimerInterrupt100Hz(void) {
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    //******** Put your code here *************//
    morse_event = MorseCheckEvents();
}
