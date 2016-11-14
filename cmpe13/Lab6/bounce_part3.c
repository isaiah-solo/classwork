// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <stdbool.h>

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "HardwareDefs.h"
#include "Buttons.h"
#include "Leds.h"
#include "Oled.h"
#include "OledDriver.h"

// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****

struct ButtonResult {
    bool event;
    uint8_t value;
} ButtonEventData;

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****

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

int main(void) {
    // Configure the device for maximum performance but do not change the PBDIV
    // Given the options, this function will change the flash wait states, RAM
    // wait state and enable prefetch cache but will not change the PBDIV.
    // The PBDIV value is already set via the pragma FPBDIV option above..
    SYSTEMConfig(F_SYS, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

    // Auto-configure the PIC32 for optimum performance at the specified operating frequency.
    SYSTEMConfigPerformance(F_SYS);

    // osc source, PLL multipler value, PLL postscaler , RC divisor
    OSCConfig(OSC_POSC_PLL, OSC_PLL_MULT_20, OSC_PLL_POST_1, OSC_FRC_POST_1);

    // Configure the PB bus to run at 1/4th the CPU frequency, so 20MHz.
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

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_8, 0xFFFF);

    // Set up the timer interrupt with a priority of 4.
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    // Initialize button library, LEDs, and variables
    ButtonsInit();
    LEDS_INIT();
    uint8_t currentSwitch, currentButton, currentLED, LED1and2 = (0x01 | 0x02),
            LED3and4 = (0x04 | 0x08),
            LED5and6 = (0x10 | 0x20),
            LED7and8 = (0x40 | 0x80);

    // If interrupt flag...
    while (1) {
        if (ButtonEventData.event == true) {
            currentSwitch = SWITCH_STATES();
            currentButton = ButtonEventData.value;

            // If switch 1 is up and button 1 is up...
            if (currentSwitch & SWITCH_STATE_SW1) {
                if (currentButton & BUTTON_EVENT_1UP) {
                    currentLED = LEDS_GET();
                    if (currentLED & LED1and2) {
                        currentLED ^= LED1and2;
                        LEDS_SET(currentLED);
                    } else {
                        currentLED |= LED1and2;
                        LEDS_SET(currentLED);
                    }
                }
            }
                // Else if switch 1 is down and button 1 is down...
            else if (currentSwitch ^ SWITCH_STATE_SW1) {
                if (currentButton & BUTTON_EVENT_1DOWN) {
                    currentLED = LEDS_GET();
                    if (currentLED & LED1and2) { // If LEDs 1 and 2 are on
                        currentLED ^= LED1and2; // Turn LEDs 1 and 2 off
                        LEDS_SET(currentLED);
                    } else { // If LEDs 1 and 2 are off
                        currentLED |= LED1and2; // Turn LEDs 1 and 2 on
                        LEDS_SET(currentLED);
                    }
                }
            }

            // If switch 2 is up and button 2 is up...
            if (currentSwitch & SWITCH_STATE_SW2) {
                if (currentButton & BUTTON_EVENT_2UP) {
                    currentLED = LEDS_GET();
                    if (currentLED & LED3and4) {
                        currentLED ^= LED3and4;
                        LEDS_SET(currentLED);
                    } else {
                        currentLED |= LED3and4;
                        LEDS_SET(currentLED);
                    }
                }
            }
                // Else if switch 2 is down and button 2 is down...
            else if (currentSwitch ^ SWITCH_STATE_SW2) {
                if (currentButton & BUTTON_EVENT_2DOWN) {
                    currentLED = LEDS_GET();
                    if (currentLED & LED3and4) { // If LEDs 3 and 4 are on
                        currentLED ^= LED3and4; // Turn LEDs 3 and 4 off
                        LEDS_SET(currentLED);
                    } else { // If LEDs 3 and 4 are off
                        currentLED |= LED3and4; // Turn LEDs 3 and 4 on
                        LEDS_SET(currentLED);
                    }
                }
            }

            // If switch 3 is up and button 3 is up...
            if (currentSwitch & SWITCH_STATE_SW3) {
                if (currentButton & BUTTON_EVENT_3UP) {
                    currentLED = LEDS_GET();
                    if (currentLED & LED5and6) {
                        currentLED ^= LED5and6;
                        LEDS_SET(currentLED);
                    } else {
                        currentLED |= LED5and6;
                        LEDS_SET(currentLED);
                    }
                }
            }
                // Else if switch 3 is down and button 3 is down...
            else if (currentSwitch ^ SWITCH_STATE_SW3) {
                if (currentButton & BUTTON_EVENT_3DOWN) {
                    currentLED = LEDS_GET();
                    if (currentLED & LED5and6) { // If LEDs 5 and 6 are on
                        currentLED ^= LED5and6; // Turn LEDs 5 and 6 off
                        LEDS_SET(currentLED);
                    } else { // If LEDs 5 and 6 are off
                        currentLED |= LED5and6; // Turn LEDs 5 and 6 on
                        LEDS_SET(currentLED);
                    }
                }
            }

            // If switch 4 is up and button 4 is up...
            if (currentSwitch & SWITCH_STATE_SW4) {
                if (currentButton & BUTTON_EVENT_4UP) {
                    currentLED = LEDS_GET();
                    if (currentLED & LED7and8) {
                        currentLED ^= LED7and8;
                        LEDS_SET(currentLED);
                    } else {
                        currentLED |= LED7and8;
                        LEDS_SET(currentLED);
                    }
                }
            }
            
            // Else if switch 4 is down and button 4 is down...
            else if (currentSwitch ^ SWITCH_STATE_SW4) {
                if (currentButton & BUTTON_EVENT_4DOWN) {
                    currentLED = LEDS_GET();
                    if (currentLED & LED7and8) { // If LEDs 7 and 8 are on
                        currentLED ^= LED7and8; // Turn LEDs 7 and 8 off
                        LEDS_SET(currentLED);
                    } else { // If LEDs 7 and 8 are off
                        currentLED |= LED7and8; // Turn LEDs 7 and 8 on
                        LEDS_SET(currentLED);
                    }
                }
            }
            ButtonEventData.event = false;
        }
    }
}

/***************************************************************************************************
 * Your code goes in between this comment and the preceding one with asterisks
 **************************************************************************************************/

/**
 * This is the interrupt for the Timer1 peripheral. It checks for button events and stores them in a
 * module-level variable.
 */
void __ISR(_TIMER_1_VECTOR, ipl4) Timer1Handler(void) {
    /*
    uint8_t prevValue, check, firstCheck;

    prevValue = ButtonEventData.value;
    firstCheck = ButtonsCheckEvents();
    check = ButtonsCheckEvents();
    check |= ButtonsCheckEvents();
    check |= ButtonsCheckEvents();
    check |= ButtonsCheckEvents();

    if (check == firstCheck) {
        if (prevValue != check) {
            ButtonEventData.value = check;
            ButtonEventData.event = true;
        }
    }
    */
    ButtonEventData.value = ButtonsCheckEvents();
    ButtonEventData.event = true;

    // Clear the interrupt flag.
    IFS0CLR = 1 << 4;
}