// **** Include libraries here ****
// Standard libraries
#include "OledDriver.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries

// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****
static char string[4 * (21 + 1)] = {};
static int index = 0;
static int newData = 0; // Set by the interrupt when new UART data has been received.

// **** Declare function prototypes ****
void OledLifeStep(void);

// **** Set processor configuration ****
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

    // Configure the PB bus to run at 1/4 the CPU frequency
    OSCSetPBDIV(OSC_PB_DIV_4);

    // Enable multi-vector interrupts
    INTEnableSystemMultiVectoredInt();
    INTEnableInterrupts();

    // Initialize pins and subsystems.
    SerialInit();
    LATE = 0;
    TRISE = 0;
    OledInit();

    // Initialize variables for storing the string.

    // Print instructions
    puts("Implementation of Conway's Game of life.\nUsage: Type to enter stating test.\nSW1"
            "switches between running the sim and allowing for new text input.\nBTN1 clears the"
            "screen.");


    LATE = 0xFF; // use the LEDs to signal ready also
    while (1) {

        // If switch 1 is active, run the game of life.
        if (SW1) {
            // Run a single iteration of the Game of Life and update the OLED.
            // We do this with a brief pause so that it's easier to watch what's going on.
            unsigned int i;
            for (i = 0; i < 500000; ++i);
            OledLifeStep();
            OledUpdate();

            // Reset the string
            index = 0;
            string[0] = '\0';
        }            // The user presses BTN1, clear the screen. Only allow this when SW1 is down.
        else if (BTN1) {
            // Clear the screen to black.
            OledClear(0);
            OledUpdate();

            // Also reset the string used for writing to the screen. This overrides everything that
            // the UART RX interrupt is doing so we don't need to worry about it.
            string[0] = '\0';
            index = 0;
        }
        // Otherwise if we've received new UART data, draw it to the screen.
        else if (newData) {
            // We clear our flag here at the beginning so that we don't lose any characters received
            // while we're updating the screen.
            newData = 0;

            // Now update the screen with the string of all received characters.
            OledDrawString(string);
            OledUpdate();
        }
    }
}

/**
 * Increments the frame buffer as if it were Conway's Game of Life. Each call to this function
 * updates the board for all cells. Neighbors are counted as if the board wraps around (like a
 * torus).
 *
 * Rules for updating the cells:
 *  1. A live cell with fewer than two live neighbors dies, i.e. underpopulation.
 *  2. A live cell with exactly 2 live neighbors survives.
 *  3. A live cell with more than three live neighbors dies, i.e. overpopulation.
 *  4. A dead cell with three live neighbors becomes alive, i.e. reproduction.
 */
void OledLifeStep(void) {
    int x, y;

    // Store the neighbor counts for every pixel on the board.
    char lifeBoard[OLED_DRIVER_PIXEL_COLUMNS][OLED_DRIVER_PIXEL_ROWS] = {};

    // Add up tile counts
    for (x = 0; x < OLED_DRIVER_PIXEL_COLUMNS; x++) {
        for (y = 0; y < OLED_DRIVER_PIXEL_ROWS; y++) {
            if (OledGetPixel(x, y)) {
                int xMinus = (x - 1) & 0x007F;
                int xPlus = (x + 1) & 0x007F;
                int yMinus = (y - 1) & 0x001F;
                int yPlus = (y + 1) & 0x001F;

                lifeBoard[xMinus][yMinus]++;
                lifeBoard[x][yMinus]++;
                lifeBoard[xPlus][yMinus]++;

                lifeBoard[xMinus][y]++;
                lifeBoard[xPlus][y]++;

                lifeBoard[xMinus][yPlus]++;
                lifeBoard[x][yPlus]++;
                lifeBoard[xPlus][yPlus]++;
            }
        }
    }

    // Apply rules to the new board
    for (x = 0; x < OLED_DRIVER_PIXEL_COLUMNS; x++) {
        for (y = 0; y < OLED_DRIVER_PIXEL_ROWS; y++) {
            switch (lifeBoard[x][y]) {
                case 0:
                case 1:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                    // This cell has died, so make it black.
                    OledSetPixel(x, y, 0);
                    break;
                case 2:
                default:
                    // This cell has not changed, so ignore it.
                    break;
                case 3:
                    // This cell is now alive, so make it white.
                    OledSetPixel(x, y, 1);
                    break;
            }
        }
    }
}

/**
 * Implement the UART1 interrupt for receiving UART data asynchronously directly into our OLED
 * string.
 */
void __ISR(_UART_1_VECTOR, ipl6) Uart1Interrupt(void) {
    // Handle receive interrupt
    if (INTGetFlag(INT_SOURCE_UART_RX(UART_USED))) {
        // Keep receiving new bytes while the buffer has data and we have a place to store it.
        // If we don't have room, just throw away the data.
        while (U1STAbits.URXDA) {
            char x = (char)U1RXREG;
            if (index < sizeof(string) - 1) {
                if (x == '\r') { // Ignore carriage returns as we assume they're Windows line endings.
                    continue;
                }
                string[index++] = x;
                string[index] = '\0'; // Make sure our string is always NULL-terminated.
            }
        }

        // Clear buffer overflow bit if triggered
        if (U1STAbits.OERR) {
            U1STAbits.OERR = 0;
        }

        // Clear the interrupt flag
        INTClearFlag(INT_SOURCE_UART_RX(UART_USED));
        
        // Log that we received new data.
        newData = 1;
    }
}
