#include <xc.h>
#include <plib.h>

#include "Serial.h"

/**
 * Initialize the UART peripheral for use by the PIC. Also sets stdin/stdout to point to the proper
 * UART and disables buffering for both.
 */
void SerialInit(void)
{
    // Configure the UART peripheral.
    UARTConfigure(UART_USED, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(UART_USED, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(UART_USED, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART_USED, F_PB, UART_BAUD_RATE);
    UARTEnable(UART_USED, UART_ENABLE_FLAGS(UART_TX | UART_RX));

    // Configure UART interrupt for just reception, transmission will remain blocking.
    INTEnable(INT_SOURCE_UART_RX(UART_USED), INT_ENABLED);
    INTSetVectorPriority(INT_VECTOR_UART(UART_USED), INT_PRIORITY_LEVEL_6);
    INTSetVectorSubPriority(INT_VECTOR_UART(UART_USED), INT_SUB_PRIORITY_LEVEL_0);

    // Set stdin/stdout to use whichever UART was indicated, assuming it's valid.
    if (UART_USED == UART1) {
        __XC_UART = 1;
    } else if (UART_USED == UART2) {
        __XC_UART = 2;
    }

    // Disable buffering for both stdin and stdout.
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
}
