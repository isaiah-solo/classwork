// **** Include libraries here ****
// Standard libraries

// Microchip libraries
#include <xc.h>
#include <plib.h>
#include <stdbool.h>

// User libraries
#include "HardwareDefs.h"
#include "Stack.h"
#include "Common.h"

// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****
#define SIZE_OF_INPUT 60
#define NUM_OF_TOKENS 30

// **** Declare function prototypes ****
/*
 * Function prototype for ProcessBackspaces() - This function should be defined
 * below main after the related comment.
 */
int ProcessBackspaces(char *rpn_sentence);

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
    UARTEnable(UART_USED, UART_ENABLE | UART_TX | UART_RX);

    // And configure printf/scanf to use UART_USED if it's different from the default of UART2.
    if (UART_USED == UART1) {
        __XC_UART = 1;
    }

    /******************************** Your custom code goes below here ****************************/
    // Declare Structures, Arrays, Pointers And Variables
    struct Stack myStack;
    char UserInput[SIZE_OF_INPUT + 1] = {}, *TokenStor[NUM_OF_TOKENS] = {};
    char *token;
    float x, y, z;
    double v;
    int u = 0;
    int inc = 0;
    
    // Greeting
    printf("Welcome to the RPN Calculator!\n");
    printf("Enter floats and + - / x in RPN format:\n");
    
    // User Input
    while(1){
    StackInit(&myStack); // Initialize Stacks
        fgets(UserInput, SIZE_OF_INPUT, stdin);

    // Tokens
        token = strtok(UserInput, " "); // Checks for first Token
        TokenStor[inc] = token;
        while(token != NULL){ // Checks for other tokens
            if(*TokenStor[inc] == '+' || *TokenStor[inc] == '-' || *TokenStor[inc] == '*' ||
                                         *TokenStor[inc] == '/'){ // If token is an operator
                StackPop(&myStack, &y);
                StackPop(&myStack, &x);
                if(*TokenStor[inc] == '+'){ // Addition
                    z = x + y;
                }
                else if(*TokenStor[inc] == '-'){ // Subtraction
                    z = x - y;
                }
                else if(*TokenStor[inc] == '*'){ // Multiplication
                    z = x * y;
                }
                else if(*TokenStor[inc] == '/'){ // Division
                    z = x / y;
                }
                inc++;
                printf("Result: %f\n", (double)z); // Print Result
                StackPush(&myStack, z); // Push Result onto Stack
                token = strtok(NULL, " "); // Checks for next Token
                TokenStor[inc] = token;
            }else{ // If token is a number
                v = atof(TokenStor[inc]);
                StackPush(&myStack, v); // Pushes Number onto Stack
                inc++;
                token = strtok(NULL, " "); // Checks for next Token
                TokenStor[inc] = token;
            }
        }
        inc = 0;
        u = 0;

    /**********************************************************************************************/

    // You can never return from main() in an embedded system (one that lacks an operating system).
    // This will result in the processor restarting, which is almost certainly not what you want!
    }
}

/**
 * Extra Credit: Define ProcessBackspaces() here - This function should read through an array of
 * characters and when a backspace character is read it should replace the preceding character with
 * the following character. It should be able to handle multiple repeated backspaces and also
 * strings with more backspaces than characters. It should be able to handle strings that are at
 * least 256 characters in length.
 * @param rpn_sentence The string that will be processed for backspaces. This string is modified in
 *                     place.
 * @return Returns the size of the resultant string in `rpn_sentence`.
 */

/**
 * This function overrides the built-in read() function used for stream
 * processing to read a set of characters from UART2.
 * NOTE: It's a blocking call and will wait for data.
 */
int read(int handle, void *buffer, unsigned int len) {
    int i;
    if (handle == 0) {
        while (!U1STAbits.URXDA) {

            
            if (U1STAbits.OERR) {
                U1STAbits.OERR = 0;
            }
            continue;
        }
        i = 0;
        while (U1STAbits.URXDA) {
            char tmp = U1RXREG;
            if (tmp == '\r') {
                tmp = '\n';
            }
            *(char*) buffer++ = tmp;
            WriteUART1(tmp);
            ++i;
        }
        return i;
    }
    return 0;
}

