// Heap Size 2048

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Agent.h"
#include "Ascii.h"
#include "Buttons.h"
#include "Field.h"
#include "FieldOled.h"
#include "HardwareDefs.h"
#include "Leds.h"
#include "Oled.h"
#include "OledDriver.h"
#include "Protocol.h"

// **** Set any macros or preprocessor directives here ****

// **** Declare any data types here ****
Field User;
Field Opp;
GuessData Guess;

// **** Define any module-level, global, or external variables here ****
static uint32_t counter;
static uint8_t buttonEvents;

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

int main()
{
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
    UARTEnable(UART_USED, UART_ENABLE | UART_TX | UART_RX);

	// And configure printf/scanf to use UART_USED if it's different from the default of UART2.
	if (UART_USED == UART1) {
		__XC_UART = 1;
	}

    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a 10ms timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, F_PB / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    // Disable buffering on stdout
    setbuf(stdout, NULL);

    ButtonsInit();

    OledInit();

    // Prompt the user to start the game and block until the first character press.
    OledDrawString("Press BTN4 to start.");
    OledUpdate();
    while ((buttonEvents & BUTTON_EVENT_4UP) == 0);

    // The first part of our seed is a hash of the compilation time string. The lowest-8 bits
    // are xor'd from the first-half of the string and the highest 8-bits are xor'd from the
    // second-half of the string.
    char seed1[] = __TIME__;
    int seed1_len = strlen(seed1);
    int firstHalf = seed1_len / 2;
    uint16_t seed2 = 0;
    int i;
    for (i = 0; i < seed1_len; ++i) {
        seed2 ^= seed1[i] << ((i < firstHalf) ? 0 : 8);
    }

    // Now we hash-in the time since first user input (which, as a 32-bit number, is split
    // and each half is hashed in.
    srand(seed2 ^ ((uint16_t) (counter >> 16)) ^ (uint16_t) (counter));

/******************************************************************************
 * Your code goes in between this comment and the following one with asterisks.
 *****************************************************************************/
    
    LEDS_INIT();
    FieldInit(&User, FIELD_POSITION_EMPTY);
    FieldInit(&Opp, FIELD_POSITION_UNKNOWN);
    FieldAddBoat(&User, 0, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_SMALL);
    FieldAddBoat(&User, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_MEDIUM);
    FieldAddBoat(&User, 2, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_LARGE);
    FieldAddBoat(&User, 3, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_HUGE);
    Guess.col = 0;
    Guess.row = 0;
    FieldRegisterEnemyAttack(&User, &Guess);
    Guess.col = 1;
    Guess.row = 0;
    FieldRegisterEnemyAttack(&User, &Guess);
    Guess.col = 2;
    Guess.row = 0;
    FieldRegisterEnemyAttack(&User, &Guess);
    LEDS_SET(FieldGetBoatStates(&User));
    
    FieldPosition old = FieldSetLocation(&User, 0, 0, FIELD_POSITION_CURSOR);
    User.field[0][5] = old;

    FieldOledDrawScreen(&User, &Opp, FIELD_OLED_TURN_MINE);


/******************************************************************************
 * Your code goes in between this comment and the preceeding one with asterisks
 *****************************************************************************/

    while (1);
}

/**
 * Overrides the default read() implementation to enable blocking input. Either UART1 or UART2 are
 * supported by setting the global `__XC_UART` variable to 1 or 2 respectively. This function only
 * works for reading from `stdin`.
 * @param handle
 * @param buffer
 * @param len
 * @return
 */
int read(int handle, void *buffer, unsigned int len)
{
    // Default to UART1.
    volatile unsigned int *umode = &U1MODE;
    volatile unsigned int *ustatus = &U1STA;
    volatile unsigned int *rxreg = &U1RXREG;
    volatile unsigned int *txreg = &U1TXREG;
    volatile unsigned int *brg = &U1BRG;

    // Return failure if bad inputs received.
    if ((buffer == NULL) || (len == 0))
        return 0;

    // Only support reading from stdin right now.
    if (handle == 0) {
        // If __XC_UART has been set by the user to 2, switch to UART2.
        if (__XC_UART == 2) {
            umode = &U2MODE;
            ustatus = &U2STA;
            rxreg = &U2RXREG;
            txreg = &U2TXREG;
            brg = &U2BRG;
        }

        // Wait for every byte to be received and store it in the buffer.
        while (!(*ustatus & (1 << _U1STA_URXDA_POSITION)));
        int i = 0;
        while ((*ustatus & (1 << _U1STA_URXDA_POSITION))) {
            char temp = *rxreg;
            *(char*) buffer++ = temp;
            *txreg = temp;
            ++i;
        }
        return i;
    }
    return 0;
}

/**
 * This is the interrupt for the Timer2 peripheral. It just keeps incrementing a counter used to
 * track the time until the first user input.
 */
void __ISR(_TIMER_2_VECTOR, ipl4) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    // Increment a counter to see the srand() function.
    ++counter;

    // Also check for any button events
    buttonEvents = ButtonsCheckEvents();
}