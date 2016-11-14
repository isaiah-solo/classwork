// Isaiah Solomon

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include <stdio.h>
#include <stdlib.h>

// **** Set macros and preprocessor directives ****
#define SYS_FREQ       80000000L
#define F_PB           20000000L
#define UART_BAUD_RATE    115200
#define UART_USED          UART1

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****

// **** Set processor configuration ****
// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 20 MHz
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
#pragma config FPLLIDIV = DIV_2     // Set the PLL input divider to 2
#pragma config FPLLMUL  = MUL_20    // Set the PLL multiplier to 20
#pragma config FPLLODIV = DIV_1     // Don't modify the PLL output.
#pragma config FNOSC    = PRIPLL    // Set the primary oscillator to internal RC w/ PLL
#pragma config FSOSCEN  = OFF       // Disable the secondary oscillator
#pragma config IESO     = OFF       // Internal/External Switch O
#pragma config POSCMOD  = XT        // Primary Oscillator Configuration
#pragma config OSCIOFNC = OFF       // Disable clock signal output
#pragma config FPBDIV   = DIV_4     // Set the peripheral clock to 1/4 system clock
#pragma config FCKSM    = CSECMD    // Clock Switching and Monitor Selection
#pragma config WDTPS    = PS1       // Specify the watchdog timer interval (unused)
#pragma config FWDTEN   = OFF       // Disable the watchdog timer
#pragma config ICESEL   = ICS_PGx2  // Allow for debugging with the Uno32
#pragma config PWP      = OFF       // Keep the program flash writeable
#pragma config BWP      = OFF       // Keep the boot flash writeable
#pragma config CP       = OFF       // Disable code protect

int main(void)
{
    // Configure the device for maximum performance but do not change the PBDIV
    // Given the options, this function will change the flash wait states, RAM
    // wait state and enable prefetch cache but will not change the PBDIV.
    // The PBDIV value is already set via the pragma FPBDIV option above..
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

    // Auto-configure the PIC32 for optimum performance at the specified operating frequency.
    SYSTEMConfigPerformance(SYS_FREQ);

    // osc source, PLL multipler value, PLL postscaler , RC divisor
    OSCConfig(OSC_POSC_PLL, OSC_PLL_MULT_20, OSC_PLL_POST_1, OSC_FRC_POST_1);

    // Configure the PB bus to run at 1/4 the CPU frequency
    OSCSetPBDIV(OSC_PB_DIV_4);

    // Enable multi-vector interrupts
    INTEnableSystemMultiVectoredInt();
    INTEnableInterrupts();

    // Set up the UART peripheral so we can send serial data.
    UARTConfigure(UART_USED, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(UART_USED, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(UART_USED, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART_USED, F_PB, UART_BAUD_RATE);
    UARTEnable(UART_USED, UART_ENABLE | UART_TX);

    // And configure printf/scanf to use the correct UART.
    if (UART_USED == UART1) {
        __XC_UART = 1;
    }

/***************************************************************************************************
 * Your code goes in between this comment and the following one with asterisks.
 **************************************************************************************************/
    // Declare Variables
    float fahr,celsius,kelv;
    int lower,upper,step;

    // Initialize Variables
    lower= 0;
    upper= 300;
    step = 20;
    fahr = lower;
    kelv = lower;

    // Print out Fahr to Celcius table
    printf("    F     C\n");
    while(fahr<=upper){
        celsius=(5.0/9.0)*(fahr-32.0);
        printf("%7.1f %04.0f\n",(double)fahr,(double)celsius);
        fahr=fahr+step;
    }

    // Print out Kelvin to Fahr table
    printf("\n    K     F\n");
    while(kelv<=upper){
        fahr=(kelv-273.15)*1.8+32.0;
        printf("%03.3f %5.6f\n",(double)kelv,(double)fahr);
        kelv=kelv+step;
    }
    
/***************************************************************************************************
 * Your code goes in between this comment and the preceding one with asterisks.
 **************************************************************************************************/

    // Returning from main() is bad form in embedded environments. So we sit and spin.
    while (1);
}
