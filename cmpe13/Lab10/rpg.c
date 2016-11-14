// Heap size 2048

// **** Include libraries here ****
// Standard libraries
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define _SUPPRESS_PLIB_WARNING 1

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Adc.h"
#include "Ascii.h"
#include "Buttons.h"
#include "Common.h"
#include "Game.h"
#include "HardwareDefs.h"
#include "Leds.h"
#include "Oled.h"
#include "OledDriver.h"
#include "Player.h"

// **** Set any macros or preprocessor directives here ****

// **** Declare any data types here ****

// **** Define any global or external variables here ****
static uint8_t buttons;
static char title[GAME_MAX_ROOM_TITLE_LENGTH + 1];
static char desc[GAME_MAX_ROOM_DESC_LENGTH + 1];
static int i = 0, j = 0;
static char pg1[84];
static char pg2[84];
static char pg3[84];
static char pg4[84];
static char pg1_desc[63];
static float potentiometer;

// **** Declare any function prototypes here ****

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

    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, F_PB / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    /******************************** Your custom code goes below here ********************************/
    // Initialize
    OledInit();
    AdcInit();
    GameInit();
    LEDS_INIT();

    // Starting Room
    GameGetCurrentRoomTitle(title);
    GameGetCurrentRoomDescription(desc);
    LEDS_SET(GameGetCurrentRoomExits());
    while (i < 63) {
        pg1_desc[j] = desc[i];
        i++;
        j++;
    }
    j = 0;
    while (i < 147) {
        pg2[j] = desc[i];
        i++;
        j++;
    }
    j = 0;
    while (i < 231) {
        pg3[j] = desc[i];
        i++;
        j++;
    }
    j = 0;
    while (i < 315) {
        pg4[j] = desc[i];
        i++;
        j++;
    }
    i = 0;
    j = 0;
    strcpy(pg1, title);
    strcat(pg1, "\n");
    strcat(pg1, pg1_desc);
    potentiometer = AdcRead();
    if (potentiometer <= (ADC_MAX_VALUE / 4)) {
        OledClear(0);
        OledDrawString(pg1);
        OledUpdate();
    }
    if (potentiometer <= (ADC_MAX_VALUE / 2) && potentiometer > (ADC_MAX_VALUE / 4)) {
        OledClear(0);
        OledDrawString(pg2);
        OledUpdate();
    }
    if (potentiometer <= ((3 * ADC_MAX_VALUE) / 4) && potentiometer > (ADC_MAX_VALUE / 2)) {
        OledClear(0);
        OledDrawString(pg3);
        OledUpdate();
    }
    if (potentiometer <= (ADC_MAX_VALUE) && potentiometer > ((3 * ADC_MAX_VALUE) / 4)) {
        OledClear(0);
        OledDrawString(pg4);
        OledUpdate();
    }

    // Event Loop
    while (1) {

        /***Potentiometer**************************************************************************/
        potentiometer = AdcRead();
        if (potentiometer <= (ADC_MAX_VALUE / 4)) {
            OledClear(0);
            OledDrawString(pg1);
            OledUpdate();
        }
        if (potentiometer <= (ADC_MAX_VALUE / 2) && potentiometer > (ADC_MAX_VALUE / 4)) {
            OledClear(0);
            OledDrawString(pg2);
            OledUpdate();
        }
        if (potentiometer <= ((3 * ADC_MAX_VALUE) / 4) && potentiometer > (ADC_MAX_VALUE / 2)) {
            OledClear(0);
            OledDrawString(pg3);
            OledUpdate();
        }
        if (potentiometer <= (ADC_MAX_VALUE) && potentiometer > ((3 * ADC_MAX_VALUE) / 4)) {
            OledClear(0);
            OledDrawString(pg4);
            OledUpdate();
        }

        /***Button 1*******************************************************************************/
        if (buttons & BUTTON_EVENT_1DOWN) {
            while (buttons & BUTTON_EVENT_1DOWN);
            GameGoWest();
            GameGetCurrentRoomTitle(title);
            GameGetCurrentRoomDescription(desc);
            LEDS_SET(GameGetCurrentRoomExits());
            while (i < 63) {
                pg1_desc[j] = desc[i];
                i++;
                j++;
            }
            j = 0;
            while (i < 147) {
                pg2[j] = desc[i];
                i++;
                j++;
            }
            j = 0;
            while (i < 231) {
                pg3[j] = desc[i];
                i++;
                j++;
            }
            j = 0;
            while (i < 315) {
                pg4[j] = desc[i];
                i++;
                j++;
            }
            i = 0;
            j = 0;
            strcpy(pg1, title);
            strcat(pg1, "\n");
            strcat(pg1, pg1_desc);
        }

        /***Button 2*******************************************************************************/
        if (buttons & BUTTON_EVENT_2DOWN) {
            while (buttons & BUTTON_EVENT_2DOWN);
            GameGoSouth();
            GameGetCurrentRoomTitle(title);
            GameGetCurrentRoomDescription(desc);
            LEDS_SET(GameGetCurrentRoomExits());
            while (i < 63) {
                pg1_desc[j] = desc[i];
                i++;
                j++;
            }
            j = 0;
            while (i < 147) {
                pg2[j] = desc[i];
                i++;
                j++;
            }
            j = 0;
            while (i < 231) {
                pg3[j] = desc[i];
                i++;
                j++;
            }
            j = 0;
            while (i < 315) {
                pg4[j] = desc[i];
                i++;
                j++;
            }
            i = 0;
            j = 0;
            strcpy(pg1, title);
            strcat(pg1, "\n");
            strcat(pg1, pg1_desc);
        }

        /***Button 3*******************************************************************************/
        if (buttons & BUTTON_EVENT_3DOWN) {
            while (buttons & BUTTON_EVENT_3DOWN);
            GameGoEast();
            GameGetCurrentRoomTitle(title);
            GameGetCurrentRoomDescription(desc);
            LEDS_SET(GameGetCurrentRoomExits());
            while (i < 63) {
                pg1_desc[j] = desc[i];
                i++;
                j++;
            }
            j = 0;
            while (i < 147) {
                pg2[j] = desc[i];
                i++;
                j++;
            }
            j = 0;
            while (i < 231) {
                pg3[j] = desc[i];
                i++;
                j++;
            }
            j = 0;
            while (i < 315) {
                pg4[j] = desc[i];
                i++;
                j++;
            }
            i = 0;
            j = 0;
            strcpy(pg1, title);
            strcat(pg1, "\n");
            strcat(pg1, pg1_desc);
        }

        /***Button 4*******************************************************************************/
        if (buttons & BUTTON_EVENT_4DOWN) {
            GameGoNorth();
            GameGetCurrentRoomTitle(title);
            GameGetCurrentRoomDescription(desc);
            LEDS_SET(GameGetCurrentRoomExits());
            while (i < 63) {
                pg1_desc[j] = desc[i];
                i++;
                j++;
            }
            j = 0;
            while (i < 147) {
                pg2[j] = desc[i];
                i++;
                j++;
            }
            j = 0;
            while (i < 231) {
                pg3[j] = desc[i];
                i++;
                j++;
            }
            j = 0;
            while (i < 315) {
                pg4[j] = desc[i];
                i++;
                j++;
            }
            i = 0;
            j = 0;
            strcpy(pg1, title);
            strcat(pg1, "\n");
            strcat(pg1, pg1_desc);
        }
    }


/**************************************************************************************************/
    while (1);
}

/**
 * Timer2 interrupt. Checks for button events.
 */
void __ISR(_TIMER_2_VECTOR, ipl4) TimerInterrupt100Hz(void) {
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;
    buttons = ButtonsCheckEvents();
}