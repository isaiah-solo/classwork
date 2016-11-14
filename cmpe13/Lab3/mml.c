// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "HardwareDefs.h"
#include "MatrixMath.h"

// **** Set macros and preprocessor directives ****

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

int main()
{
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

/******************************************************************************
 * Your code goes in between this comment and the following one with asterisks.
 *****************************************************************************/
    // Declare variables and Test Matrices
    int x = 0, y = 0, z = 0, ans1, ans2, test = 0;
    int g = 7, h = -3;
    float Test1Mat1[3][3] = {{1, 1, 1}, {2, 2, 2}, {3, 3, 3}};
    float Test1Mat2[3][3] = {{1, 1, 1}, {2, 2, 2}, {3, 3, 3}};
    float Test2Mat1[3][3] = {{2000, -2000, 0}, {1/2, 1/3, 1/4}, {0, 0, 0}};
    float Test2Mat2[3][3] = {{1, 1, 1}, {2, 2, 2}, {3, 3, 3}};
    float Test2Mat3[3][3] = {{70, -70, 70}, {-60, 60, -60}, {50, -50, 50}};
    float DetMat1[3][3] = {{1, 2, 1}, {1, 2, 2}, {2, 2, 1}};
    float DetMat2[3][3] = {{-1, -2, -1}, {1, 2, 2}, {-2, -2, -1}};

    /***************************
     * Matrix/Matrix Add Tests *
     ***************************/
    {// Declares Constants
    float Add1Test1ExpOut[3][3] = {{2, 2, 2}, {4, 4, 4}, {6, 6, 6}};
    float Add1Test2ExpOut[3][3] = {{2001, -1999, 1}, {5/2, 7/3, 9/4}, {3, 3, 3}};
    
    // Calculates Result
    float Add1Test1ActOut[3][3];
    float Add1Test2ActOut[3][3];
    MatrixAdd(Test1Mat1, Test1Mat2, Add1Test1ActOut);
    MatrixAdd(Test2Mat1, Test2Mat2, Add1Test2ActOut);

    // Outputs Result
    ans1 = MatrixEquals(Add1Test1ActOut, Add1Test1ExpOut);
    ans2 = MatrixEquals(Add1Test2ActOut, Add1Test2ExpOut);
    if(ans1 == 9){
        test += 1;
    }
    if(ans2 == 9){
        test += 1;
    }
    if(test == 2){
        printf("PASSED (%d/2): MatrixAdd()\n", test);
    }else{
        printf("FAILED (%d/2): MatrixAdd()\n", test);
    }
    test = 0;
    }

    /********************************
     * Matrix/Matrix Multiply Tests *
     ********************************/
    {// Declares Constants
    float Mult1Test1ExpOut[3][3] = {{6, 6, 6}, {12, 12, 12}, {18, 18, 18}};
    float Mult1Test2ExpOut[3][3] = {{140, 140, 140}, {-120, -120, -120}, {100, 100, 100}};

    // Calculates Result
    float Mult1Test1ActOut[3][3];
    float Mult1Test2ActOut[3][3];
    MatrixMultiply(Test1Mat1, Test1Mat2, Mult1Test1ActOut);
    MatrixMultiply(Test2Mat3, Test2Mat2, Mult1Test2ActOut);

    // Outputs Result
    ans1 = MatrixEquals(Mult1Test1ActOut, Mult1Test1ExpOut);
    ans2 = MatrixEquals(Mult1Test2ActOut, Mult1Test2ExpOut);
    if(ans1 == 9){
        test += 1;
    }
    if(ans2 == 9){
        test += 1;
    }
    if(test == 2){
        printf("PASSED (%d/2): MatrixMultiply()\n", test);
    }else{
        printf("FAILED (%d/2): MatrixMultiply()\n", test);
    }
    test = 0;
    }
    

    /***************************
     * Matrix/Scalar Add Tests *
     ***************************/
    {// Declares Constants
    float Add2Test1ExpOut[3][3] = {{8, 8, 8}, {9, 9, 9}, {10, 10, 10}};
    float Add2Test2ExpOut[3][3] = {{-2, -2, -2}, {-1, -1, -1}, {0, 0, 0}};

    // Calculates Result
    float Add2Test1ActOut[3][3];
    float Add2Test2ActOut[3][3];
    MatrixScalarAdd(g, Test1Mat1, Add2Test1ActOut);
    MatrixScalarAdd(h, Test1Mat1, Add2Test2ActOut);

    // Outputs Result
    ans1 = MatrixEquals(Add2Test1ActOut, Add2Test1ExpOut);
    ans2 = MatrixEquals(Add2Test2ActOut, Add2Test2ExpOut);
    if(ans1 == 9){
        test += 1;
    }
    if(ans2 == 9){
        test += 1;
    }
    if(test == 2){
        printf("PASSED (%d/2): MatrixScalarAdd()\n", test);
    }else{
        printf("FAILED (%d/2): MatrixScalarAdd()\n", test);
    }
    test = 0;
    }

    /********************************
     * Matrix/Scalar Multiply Tests *
     ********************************/
    {// Declares Constants
    float Mult2Test1ExpOut[3][3] = {{7, 7, 7}, {14, 14, 14}, {21, 21, 21}};
    float Mult2Test2ExpOut[3][3] = {{-3, -3, -3}, {-6, -6, -6}, {-9, -9, -9}};

    // Calculates Result
    float Mult2Test1ActOut[3][3];
    float Mult2Test2ActOut[3][3];
    MatrixScalarMultiply(g, Test1Mat1, Mult2Test1ActOut);
    MatrixScalarMultiply(h, Test1Mat1, Mult2Test2ActOut);

    // Outputs Result
    ans1 = MatrixEquals(Mult2Test1ActOut, Mult2Test1ExpOut);
    ans2 = MatrixEquals(Mult2Test2ActOut, Mult2Test2ExpOut);
    if(ans1 == 9){
        test += 1;
    }
    if(ans2 == 9){
        test += 1;
    }
    if(test == 2){
        printf("PASSED (%d/2): MatrixScalarMultiply()\n", test);
    }else{
        printf("FAILED (%d/2): MatrixScalarMultiply()\n", test);
    }
    test = 0;
    }

    /*******************
     * Matrix Equality *
     *******************/
    {// Calculates Result
    int EqTest1ActOut;
    EqTest1ActOut = MatrixEquals(Test1Mat1, Test1Mat1);
    int EqTest2ActOut;
    EqTest2ActOut = MatrixEquals(Test2Mat1, Test2Mat1);
    
    // Outputs Result
    if(EqTest1ActOut >= 9){ // Test 1 Result
        x = 1;
    }else{
        x = 0;
    }
    if(EqTest2ActOut >= 9){ // Test 2 Result
        y = 1;
    }else{
        y = 0;
    }
    z = x + y;
    if(z == 2){
        printf("PASSED (%d/2): MatrixEquals()\n", z);
    }else{
        printf("FAILED (%d/2): MatrixEquals()\n", z);
    }
    }

    /**********************
     * Matrix Determinant *
     **********************/
    {// Declare Constants
    int DetTest1ExpOut = 2;
    int DetTest2ExpOut = 2;

    // Calculates Result
    int DetTest1ActOut;
    int DetTest2ActOut;
    DetTest1ActOut = MatrixDeterminant(DetMat1);
    DetTest2ActOut = MatrixDeterminant(DetMat2);
    
    // Outputs Result
    if(DetTest1ActOut == DetTest1ExpOut){ // Test 1 Result
        test += 1;
    }
    if(DetTest2ActOut == DetTest2ExpOut){ // Test 2 Result
        test += 1;
    }
    if(test == 2){
        printf("PASSED (%d/2): MatrixDeterminant()\n", test);
    }else{
        printf("FAILED (%d/2): MatrixDeterminant()\n", test);
    }
    test = 0;
    }

    /****************
     * Matrix Trace *
     ****************/
    {// Declare Constants
    int TraceTest1ExpOut = 6;
    int TraceTest2ExpOut = (6001/3);

    // Calculates Result
    int TraceTest1ActOut;
    int TraceTest2ActOut;
    TraceTest1ActOut = MatrixTrace(Test1Mat1);
    TraceTest2ActOut = MatrixTrace(Test2Mat1);

    // Outputs Result
    if(TraceTest1ActOut == TraceTest1ExpOut){ // Test 1 Result
        test += 1;
    }
    if(TraceTest2ActOut == TraceTest2ExpOut){ // Test 2 Result
        test += 1;
    }
    if(test == 2){
        printf("PASSED (%d/2): MatrixTrace()\n", test);
    }else{
        printf("FAILED (%d/2): MatrixTrace()\n", test);
    }
    test = 0;
    }

    /********************
     * Matrix Transpose *
     ********************/
    {// Declare Constants
    float TranTest1ExpOut[3][3] = {{1, 2, 3}, {1, 2, 3}, {1, 2, 3}};
    float TranTest2ExpOut[3][3] = {{2000, (1/2), 0}, {-2000, (1/3), 0}, {0, (1/4), 0}};

    // Calculates Result
    float TranTest1ActOut[3][3];
    float TranTest2ActOut[3][3];
    MatrixTranspose(Test1Mat1, TranTest1ActOut);
    MatrixTranspose(Test2Mat1, TranTest2ActOut);

    // Outputs Result
    ans1 = MatrixEquals(TranTest1ActOut, TranTest1ExpOut);
    ans2 = MatrixEquals(TranTest2ActOut, TranTest2ExpOut);
    if(ans1 == 9){
        test += 1;
    }
    if(ans2 == 9){
        test += 1;
    }
    if(test == 2){
        printf("PASSED (%d/2): MatrixTranspose()\n", test);
    }else{
        printf("FAILED (%d/2): MatrixTranspose()\n", test);
    }
    test = 0;
    }

    /******************
     * Matrix Inverse *
     ******************/
    {// Declare Constants
    float InTest1ExpOut[3][3] = {{-1, 0, 1}, {(3/2), (-1/2), (-1/2)}, {-1, 1, 0}};
    float InTest2ExpOut[3][3] = {{1, 0, -1}, {(-3/2), (-1/2), (1/2)}, {1, 1, 0}};

    // Calculates Result
    float InTest1ActOut[3][3];
    float InTest2ActOut[3][3];
    MatrixInverse(DetMat1, InTest1ActOut);
    MatrixInverse(DetMat2, InTest2ActOut);

    // Outputs Result
    ans1 = MatrixEquals(InTest1ActOut, InTest1ExpOut);
    ans2 = MatrixEquals(InTest2ActOut, InTest2ExpOut);
    if(ans1 == 9){
        test += 1;
    }
    if(ans2 == 9){
        test += 1;
    }
    if(test == 2){
        printf("PASSED (%d/2): MatrixInverse()\n", test);
    }else{
        printf("FAILED (%d/2): MatrixInverse()\n", test);
    }
    test = 0;
    }

    /****************
     * Matrix Print *
     ****************/
    {// Expected Output
    printf("Expected Output of MatrixPrint():\n");
    printf("%3.2f %3.2f %3.2f\n%3.2f %3.2f %3.2f\n%3.2f %3.2f %3.2f\n", (double)Test1Mat1[0][0],
                      (double)Test1Mat1[0][1], (double)Test1Mat1[0][2], (double)Test1Mat1[1][0],
                      (double)Test1Mat1[1][1], (double)Test1Mat1[1][2], (double)Test1Mat1[2][0],
                                              (double)Test1Mat1[2][1], (double)Test1Mat1[2][2]);

    // Actual Output
    printf("Actual Output of MatrixPrint():\n");
    MatrixPrint(Test1Mat1);
    }

/******************************************************************************
 * Your code goes in between this comment and the preceding one with asterisks
 *****************************************************************************/

	// Returning from main() is bad form in embedded environments. So we sit and spin.
	while (1);
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
