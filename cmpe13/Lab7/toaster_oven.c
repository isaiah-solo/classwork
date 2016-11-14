// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Adc.h"
#include "Ascii.h"
#include "Buttons.h"
#include "Leds.h"
#include "Oled.h"
#include "OledDriver.h"
#include "HardwareDefs.h"

// **** Set any macros or preprocessor directives here ****
// Set a macro for resetting the timers, makes the code a little clearer.
#define TIMER_2HZ_RESET() (TMR1 = 0)
#define TIMER_5HZ_RESET() (TMR3 = 0)

// **** Declare any datatypes here ****
#define TOP_OVEN_ON 0x01
#define TOP_OVEN_OFF 0x02
#define BOT_OVEN_ON 0x03
#define BOT_OVEN_OFF 0x04
#define DEGREE 0xF8
#define LONG_PRESS 5

typedef enum {
    RESET,
    START,
    COUNTDOWN,
    PENDING_RESET,
    PENDING_SELECTOR_CHANGE
} oven_state_t;

typedef enum {
    BAKE,
    TOAST,
    BROIL
} oven_select_mode_t;

typedef enum {
    TIME,
    TEMP
} oven_select_input_t;

typedef enum {
    ON,
    OFF
} oven_on_off_t;

struct tm_t {
    int sec;
    int min;
} tm;

struct oven_data_t {
    oven_on_off_t onCheck;
    char botOn_Off;
    char topOn_Off;
    bool timeEvent;
    bool buttonEvent;
    bool buttonPressEvent;
    int potentiometer;
    float cookingTimeLeft;
    float initCookingTime;
    int temperature;
    int buttonPressCounter;
    char timePointer;
    char tempPointer;
    uint8_t buttonsState;
    oven_select_input_t inputSelection;
    oven_state_t ovenState;
    oven_select_mode_t cookingMode;
} OvenData;

// **** Define any module-level, global, or external variables here ****
void UpdateDisplay();
static int x = 0;


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

    // Configure Timer 1 using PBCLK as input. We configure it using a 1:256 prescalar, so each 
    // timer tick is actually at F_PB / 256 Hz, so setting PR1 to F_PB / 256 / 2 yields a 0.5s
    // timer.
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_256, F_PB / 256 / 2);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);

    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, F_PB / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescalar, so each
    // timer tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.
    OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_256, F_PB / 256 / 5);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T3);
    INTSetVectorPriority(INT_TIMER_3_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_3_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T3, INT_ENABLED);

    /***********************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **********************************************************************************************/
    LEDS_INIT();
    OledInit();
    ButtonsInit();
    AdcInit();
    OvenData.ovenState = RESET;
    while (1) {
        switch (OvenData.ovenState) {

                /*********
                 * RESET *
                 *********/
            case RESET:

                OvenData.inputSelection = TIME;
                if (OvenData.cookingMode == BAKE) {
                    OvenData.temperature = 350;
                    OvenData.initCookingTime = 1;
                }
                OvenData.onCheck = OFF;
                UpdateDisplay();
                OvenData.ovenState = START;
                break;

                /*********
                 * START *
                 *********/
            case START:

                OvenData.potentiometer = AdcRead() >> 2;
                OvenData.initCookingTime = OvenData.potentiometer;
                // If potentiometer is used, change values
                if (AdcChanged() == true) {
                    if (OvenData.inputSelection == TIME) {
                        tm.sec = OvenData.potentiometer;
                        tm.min = 0;
                        while (tm.sec > 59) {
                            tm.sec -= 60;
                            tm.min += 1;
                        }
                    } else if (OvenData.inputSelection == TEMP) {
                        OvenData.temperature = OvenData.potentiometer + 300;
                    }
                    UpdateDisplay();
                }
                    // If button 4 is pressed, start COUNTDOWN
                else if (OvenData.buttonsState & BUTTON_EVENT_4DOWN) {
                    OvenData.cookingTimeLeft = OvenData.initCookingTime;
                    OvenData.onCheck = ON;
                    OvenData.buttonEvent = false;
                    UpdateDisplay();
                    OvenData.ovenState = COUNTDOWN;
                }
                    // If button 3 is pressed
                else if (OvenData.buttonsState & BUTTON_EVENT_3DOWN) {
                    OvenData.buttonPressCounter = 0;
                    OvenData.buttonEvent = false;
                    OvenData.ovenState = PENDING_SELECTOR_CHANGE;
                }
                break;

                /***************************
                 * PENDING SELECTOR CHANGE *
                 ***************************/
            case PENDING_SELECTOR_CHANGE:

                // If button 3 was pressed
                if (OvenData.buttonPressCounter < LONG_PRESS &&
                        OvenData.buttonsState & BUTTON_EVENT_3UP) {
                    // If in BAKE mode
                    if (OvenData.cookingMode == BAKE) {
                        OvenData.temperature = 350;
                        OvenData.initCookingTime = 1;
                        tm.min = 0;
                        tm.sec = 1;
                        OvenData.cookingMode = TOAST;
                        // If in TOAST mode
                    } else if (OvenData.cookingMode == TOAST) {
                        OvenData.initCookingTime = 1;
                        tm.min = 0;
                        tm.sec = 1;
                        OvenData.cookingMode = BROIL;
                        // If in BROIL mode
                    } else if (OvenData.cookingMode == BROIL) {
                        OvenData.temperature = 500;
                        OvenData.initCookingTime = 1;
                        tm.min = 0;
                        tm.sec = 1;
                        OvenData.cookingMode = BAKE;
                    }
                    OvenData.inputSelection = TIME;
                    OvenData.buttonPressEvent = false;
                    OvenData.buttonEvent = false;
                    UpdateDisplay();
                    OvenData.ovenState = START;
                }
                // If button 3 was long-pressed
                if (OvenData.buttonPressCounter >= LONG_PRESS) {

                    // Switches
                    if (OvenData.cookingMode == BAKE) {
                        if (OvenData.inputSelection == TIME) {
                            OvenData.inputSelection = TEMP;
                        } else if (OvenData.inputSelection == TEMP) {
                            OvenData.inputSelection = TIME;
                        }
                    }
                    OvenData.buttonPressEvent = false;
                    OvenData.buttonEvent = false;
                    UpdateDisplay();
                    OvenData.ovenState = START;
                }
                break;

                /*************
                 * COUNTDOWN *
                 *************/
            case COUNTDOWN:


                if (OvenData.onCheck == ON && OvenData.cookingTimeLeft > 0) {
                    LEDS_SET(0xFF);
                    while (OvenData.cookingTimeLeft > 0 && x == 0) {
                        while (tm.sec > 0 && x == 0) {
                            if (OvenData.timeEvent == true) {
                                tm.sec -= 1;
                                UpdateDisplay();
                                OvenData.timeEvent = false;
                            }
                            if (ButtonsCheckEvents() & BUTTON_EVENT_4DOWN) {
                                OvenData.buttonPressCounter = 0;
                                OvenData.buttonEvent = false;
                                OvenData.ovenState = PENDING_RESET;
                                x = 1;
                            }
                        }
                        if (OvenData.cookingTimeLeft > 0 && tm.sec == 0 && OvenData.timeEvent ==
                                true) {
                            tm.sec += 59;
                            tm.min -= 1;
                            UpdateDisplay();
                            OvenData.timeEvent = false;
                        }
                        if (ButtonsCheckEvents() & BUTTON_EVENT_4DOWN) {
                            OvenData.buttonPressCounter = 0;
                            OvenData.buttonEvent = false;
                            OvenData.ovenState = PENDING_RESET;
                            x = 1;
                        }
                        if (tm.sec == 0 && tm.min == 0 && x == 0) {
                            LEDS_SET(0x00);
                            OvenData.onCheck = OFF;
                            OvenData.cookingTimeLeft = 0;
                            UpdateDisplay();
                            OvenData.ovenState = RESET;
                        }
                    }
                }
                if (x == 1) {
                    x = 0;
                    OvenData.buttonPressCounter = 0;
                }
                break;

                /*****************
                 * PENDING RESET *
                 *****************/
            case PENDING_RESET:

                if (OvenData.buttonsState & BUTTON_EVENT_4UP) {
                    OvenData.buttonEvent = false;
                    OvenData.ovenState = COUNTDOWN;
                }
                if (OvenData.buttonPressCounter >= LONG_PRESS) {
                    OvenData.ovenState = RESET;
                }
                if (OvenData.onCheck == ON && OvenData.cookingTimeLeft > 0) {
                    if (OvenData.cookingTimeLeft > 0 && x == 0) {
                        if (tm.sec > 0 && x == 0) {
                            if (OvenData.timeEvent == true) {
                                tm.sec -= 1;
                                UpdateDisplay();
                                OvenData.timeEvent = false;
                            }
                            if (ButtonsCheckEvents() & BUTTON_EVENT_4DOWN) {
                                OvenData.buttonPressCounter = 0;
                                OvenData.buttonEvent = false;
                                OvenData.ovenState = PENDING_RESET;
                                x = 1;
                            }
                        }
                        if (OvenData.cookingTimeLeft > 0 && tm.sec == 0 && OvenData.timeEvent ==
                                true) {
                            tm.sec += 59;
                            tm.min -= 1;
                            UpdateDisplay();
                            OvenData.timeEvent = false;
                        }
                        if (tm.sec == 0 && tm.min == 0 && x == 0) {
                            OvenData.onCheck = OFF;
                            OvenData.cookingTimeLeft = 0;
                            UpdateDisplay();
                            OvenData.ovenState = RESET;
                        }
                    }
                }
                break;
        }
    }
    // put event-checking code inside start state(update display and time)

    /***********************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **********************************************************************************************/
    while (1);
}

void UpdateDisplay(void) {
    OledClear(0);
    char string[84];
    if(OvenData.cookingTimeLeft <= ((.125) * OvenData.initCookingTime) &&
        OvenData.cookingTimeLeft > 0){
        LEDS_SET(0x01);
    } else if(OvenData.cookingTimeLeft <=
        ((.25) * OvenData.initCookingTime) && OvenData.cookingTimeLeft >
        ((.125) * OvenData.initCookingTime)){
        LEDS_SET(0x03);
    } else if(OvenData.cookingTimeLeft <=
        ((.375) * OvenData.initCookingTime) && OvenData.cookingTimeLeft >
        ((.25) * OvenData.initCookingTime)){
        LEDS_SET(0x07);
    } else if(OvenData.cookingTimeLeft <=
        ((.5) * OvenData.initCookingTime) && OvenData.cookingTimeLeft >
        ((.375) * OvenData.initCookingTime)){
        LEDS_SET(0x0F);
    } else if(OvenData.cookingTimeLeft <=
        ((.625) * OvenData.initCookingTime) && OvenData.cookingTimeLeft >
        ((.5) * OvenData.initCookingTime)){
        LEDS_SET(0x1F);
    } else if(OvenData.cookingTimeLeft <=
        ((.75) * OvenData.initCookingTime) && OvenData.cookingTimeLeft >
        ((.625) * OvenData.initCookingTime)){
        LEDS_SET(0x3F);
    } else if(OvenData.cookingTimeLeft <=
        ((.875) * OvenData.initCookingTime) && OvenData.cookingTimeLeft >
        ((.75) * OvenData.initCookingTime)){
        LEDS_SET(0x7F);
    }
    if (OvenData.onCheck == ON) {
        if (OvenData.cookingMode == BAKE) {
            OvenData.botOn_Off = BOT_OVEN_ON;
            OvenData.topOn_Off = TOP_OVEN_ON;
        } else if (OvenData.cookingMode == TOAST) {
            OvenData.botOn_Off = BOT_OVEN_ON;
            OvenData.topOn_Off = TOP_OVEN_OFF;
        } else if (OvenData.cookingMode == BROIL) {
            OvenData.botOn_Off = BOT_OVEN_OFF;
            OvenData.topOn_Off = TOP_OVEN_ON;
        }
    } else if (OvenData.onCheck == OFF) {
        OvenData.botOn_Off = BOT_OVEN_OFF;
        OvenData.topOn_Off = TOP_OVEN_OFF;
    }
    if (OvenData.cookingMode == BAKE) {
        if (OvenData.inputSelection == TIME) {
            sprintf(string, "|%c%c%c%c%c|   Mode: Bake \n"
                    "|     |  >Time: %02d:%02d\n"
                    "|-----|   Temp: %03d%c \n"
                    "|%c%c%c%c%c|              ", OvenData.topOn_Off, OvenData.topOn_Off,
                    OvenData.topOn_Off, OvenData.topOn_Off, OvenData.topOn_Off,
                    tm.min, tm.sec, OvenData.temperature, DEGREE, OvenData.botOn_Off,
                    OvenData.botOn_Off, OvenData.botOn_Off, OvenData.botOn_Off, OvenData.botOn_Off);
        } else if (OvenData.inputSelection == TEMP) {
            sprintf(string, "|%c%c%c%c%c|   Mode: Bake \n"
                    "|     |   Time: %02d:%02d\n"
                    "|-----|  >Temp: %03d%c \n"
                    "|%c%c%c%c%c|              ", OvenData.topOn_Off, OvenData.topOn_Off,
                    OvenData.topOn_Off, OvenData.topOn_Off, OvenData.topOn_Off,
                    tm.min, tm.sec, OvenData.temperature, DEGREE, OvenData.botOn_Off,
                    OvenData.botOn_Off, OvenData.botOn_Off, OvenData.botOn_Off, OvenData.botOn_Off);
        }
    } else if (OvenData.cookingMode == TOAST) {
        sprintf(string, "|%c%c%c%c%c|   Mode: Toast\n"
                "|     |   Time: %02d:%02d\n"
                "|-----|              \n"
                "|%c%c%c%c%c|              ", OvenData.topOn_Off, OvenData.topOn_Off,
                OvenData.topOn_Off, OvenData.topOn_Off, OvenData.topOn_Off,
                tm.min, tm.sec,
                OvenData.botOn_Off, OvenData.botOn_Off, OvenData.botOn_Off, OvenData.botOn_Off,
                OvenData.botOn_Off);
    } else if (OvenData.cookingMode == BROIL) {
        sprintf(string, "|%c%c%c%c%c|   Mode: Broil\n"
                "|     |   Time: %02d:%02d\n"
                "|-----|   Temp: 500%c \n"
                "|%c%c%c%c%c|              ", OvenData.topOn_Off, OvenData.topOn_Off,
                OvenData.topOn_Off, OvenData.topOn_Off, OvenData.topOn_Off,
                tm.min, tm.sec, DEGREE,
                OvenData.botOn_Off, OvenData.botOn_Off, OvenData.botOn_Off, OvenData.botOn_Off,
                OvenData.botOn_Off);
    }
    OledDrawString(string);
    OledUpdate();
}

void __ISR(_TIMER_1_VECTOR, ipl4) TimerInterrupt2Hz(void) // counting down while cooking
{
    if (OvenData.onCheck == ON) {
        OvenData.cookingTimeLeft -= .5;
        OvenData.timeEvent = true;
    }
    // Clear the interrupt flag.
    IFS0CLR = 1 << 4;

}

void __ISR(_TIMER_3_VECTOR, ipl4) TimerInterrupt5Hz(void) // how long holding a button down
{
    OvenData.buttonPressCounter += 1;
    // Clear the interrupt flag.
    IFS0CLR = 1 << 12;

}

void __ISR(_TIMER_2_VECTOR, ipl4) TimerInterrupt100Hz(void) // calling ButtonCheckEvents
{
    if (OvenData.buttonsState & ButtonsCheckEvents()) {
        OvenData.buttonEvent = true;
    }
    OvenData.buttonsState = ButtonsCheckEvents();
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

}