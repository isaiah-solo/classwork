// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <math.h>

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "HardwareDefs.h"

// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****
double Round(double operand);
double Average(double operand, double operandtwo);
double AbsoluteValue(double operand);
double FahrenheitToCelcius(double operand);
double CelciusToFahrenheit(double operand);
double Tangent(double operand);
double Round(double operand);

// If this code is used for testing, rename main to something we can call from our testing code.
#ifndef LAB1_TESTING
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

int main(void)
{
    // Configure the device for maximum performance but do not change the PBDIV
    // Given the options, this function will change the flash wait states, RAM
    // wait state and enable prefetch cache but will not change the PBDIV.
    // The PBDIV value is already set via the pragma FPBDIV option above..
    SYSTEMConfig(F_SYS, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

    // Auto-configure the PIC32 for optimum performance at the specified operating frequency.
    SYSTEMConfigPerformance(F_SYS);

    // osc source, PLL multiplier value, PLL postscaler , RC divisor
    OSCConfig(OSC_POSC_PLL, OSC_PLL_MULT_20, OSC_PLL_POST_1, OSC_FRC_POST_1);

    // Configure the PB bus to run at 1/4th the CPU frequency, so 20MHz.
    OSCSetPBDIV(OSC_PB_DIV_4);

    // Enable multi-vector interrupts
    INTEnableSystemMultiVectoredInt();
    INTEnableInterrupts();

	// Configure UART1.
    UARTConfigure(UART_USED, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(UART_USED, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(UART_USED, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART_USED, F_PB, UART_BAUD_RATE);
    UARTEnable(UART_USED, UART_ENABLE | UART_TX | UART_RX);

	// And configure printf/scanf to use UART_USED if it's different from the default of UART2.
	if (UART_USED == UART1) {
		__XC_UART = 1;
	}
#else
int their_main(void)
{
#endif // LAB1_TESTING

/******************************************************************************
 * Your code goes in between this comment and the following one with asterisks.
 *****************************************************************************/
    // Variables.
    char op, c;
    int val, bin;
    double fop, sop, ans;

    // Greeting.
    printf("Welcome to the Calculator!!\n");
    while(1){  
        printf("\nEnter an Operation (*, /, +, -, v, a, c, f, t, r):\n");
      
    // Checks if Operator is valid.        
        scanf("%c", &op);
        if(op == '*' || op == '/' || op == '+' || op == '-' || op == 'v' || op == 'a' ||
                                      op == 'f' || op == 'c' || op == 't' || op == 'r'){
            val = 1; // value = 1 if valid
        }
        else{
            val = 0; // value = 0 if invalid
        }
    
    // If Operator is valid..
        if(val == 1){

    // First Operand.
            printf("\nEnter an operand:\n");
            scanf("%lf%c", &fop, &c);

    // Checks if a Binary Operator
            if(op == '*' || op == '/' || op == '+' || op == '-' || op == 'v'){
                bin = 1;
            }
            else{
                bin = 0;
            }

    // If a Binary Operator, then Second Operand
            if(bin == 1){
                printf("\nEnter a second operand:\n");
                scanf("%lf%c", &sop, &c);
            }          

    /************\
     * Addition.*
    \************/
            if(op == '+'){
                ans = fop + sop; 
            }
        
    /***************\
     * Subtraction.*
    \***************/
            else if(op == '-'){
                ans = fop - sop;               
            }
        
    /******************\
     * Multiplication.*
    \******************/
            else if(op == '*'){
                ans = fop * sop;
            }
        
    /************\
     * Division.*
    \************/
            else if(op == '/'){
                ans = fop / sop;               
            }
        
    /***********\
     * Average.*
    \***********/
            else if(op == 'v'){
                ans = Average(fop, sop);
            }

    /******************\
     * Absolute Value.*
    \******************/
            else if(op == 'a'){
                ans = AbsoluteValue(fop);
                printf("result %f --> %f", (double)fop, (double)ans);
            }
    
    /*************************\
     * Fahrenheit to Celcius.*
    \*************************/
            else if(op == 'c'){
                ans = FahrenheitToCelcius(fop);
                printf("result %f --> %f", (double)fop, (double)ans);
            }      
        
    /************************\
     * Celcius To Fahrenheit*        .*
    \************************/
            else if(op == 'f'){
                ans = CelciusToFahrenheit(fop);
                printf("result %f --> %f", (double)fop, (double)ans);
            }

    /***********\
     * Tangent.*
    \***********/
            else if(op == 't'){
                ans = Tangent(fop);
                printf("result %f --> %f", (double)fop, (double)ans);
            }

    /*********\
     * Round.*
    \*********/
            else if(op == 'r'){
                ans = Round(fop);
                printf("result %f --> %f", (double)fop, (double)ans);
            }

    // Checks if Binary Operator
            if(bin == 1){
                if(op == '+' || op == '-' || op == '*' || op == '/'){
                    printf("%f %c %f = %f", (double)fop, op, (double)sop, (double)ans);
                }
                else if(op == 'v'){
                    printf("Average of %f and %f is %f", (double)fop, (double)sop, (double)ans);
                }
            }
        
    // If Operator is invalid, error.
        else if(val == 0){
            printf("\nInvalid operator. Try again.");
        }
        }
    }
 /******************************************************************************
 * Your code goes in between this comment and the preceding one with asterisks
 *****************************************************************************/
}

/********************************************************************************
 * Define the Absolute Value function here.
 ********************************************************************************/
    double AbsoluteValue(double operand)
    {
        double answer;
        if(operand < 0)
            answer = operand - 2 * operand;
        else if(operand >= 0)
            answer = operand;
        return answer;
    }

/*********************************************************************************
 * Define the Fahrenheit to Celsius function here.
 ********************************************************************************/
    double FahrenheitToCelcius(double operand)
    {
        double answer = (5.0/9.0)*(operand-32.0);
        return answer;
    }

/*********************************************************************************
 * Define the Celsius to Fahrenheit function here.
 ********************************************************************************/
    double CelciusToFahrenheit(double operand)
    {
        double answer = 32.0 + (operand*(5.0/9.0));
        return answer;
    }

/********************************************************************************
 * Define the Average function here.
 *******************************************************************************/
    double Average(double operand, double operandtwo)
    {
        double answer = (operand + operandtwo)/2.0;
        return answer;
    }

/*********************************************************************************
 * Define the Tangent function that takes input in degrees (Note: Your tangent 
 * function relies on a call from the tangent function of math.h which uses 
 * radians).
 ********************************************************************************/
    double Tangent(double operand)
    {
        double answer = (operand * M_PI)/180;
        answer = tan(answer);
        answer = (answer * 180)/M_PI;
        return answer;
    }

/*********************************************************************************
 * Define the Round function here.
 * In order to receive the extra credit your calculator MUST ALSO CORRECTLY utilize
 * this function.
 ********************************************************************************/
    double Round(double operand)
{
	return operand;
}

/**
 * This function overrides the built-in read() function used for stream
 * processing to read a set of characters from UART2.
 * NOTE: It's a blocking call and will wait for data.
 */
int read(int handle, void *buffer, unsigned int len)
{
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
			*(char*)buffer++ = tmp;
			WriteUART1(tmp);
			i++;
		}
		return i;
	}
	return 0;
}