// Heap size 1024 required!

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Common.h"
#include "HardwareDefs.h"
#include "LinkedList.h"

// **** Set any macros or preprocessor directives here ****

// **** Declare any data types here ****

// **** Define any module-level, global, or external variables here ****
static char *dog = "dog";
static char *pig1 = "pig";
static char *pig2 = "pig";
static char *cow = "cow";
static char *cat = "cat";
static char *turtle = "turtle";
static char *bird = "bird";
static char *crab = "crab";

// **** Declare any function prototypes here ****
int InitializeUnsortedWordList(ListItem **unsortedWordList);
int UnsortedWordCount(ListItem *list, int *wordCount);
int SortedWordCount(ListItem *list, int *wordCount);

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

    // osc source, PLL multiplier value, PLL postscaler , RC divisor
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

	/**
	 * ----------------------- STARTER CODE --------------------------
	 * This segment of code obtains an unsorted word list, prints it out,
	 * and displays the number of occurrences of each word in the list as
	 * described by UnsortedWordCount() below.
	 */
	ListItem *unsortedWordList = NULL;
	if (!InitializeUnsortedWordList(&unsortedWordList) || !unsortedWordList) {
		printf("ERROR: Failed to initialize word list\n");
		while (true);
	}

	LinkedListPrint(unsortedWordList);
	int g = LinkedListSize(unsortedWordList);
	int wordCount[g];
	unsortedWordList = LinkedListGetFirst(unsortedWordList);
	if (UnsortedWordCount(unsortedWordList, wordCount)) {
		printf("[%d, %d, %d, %d, %d, %d, %d, %d, %d, %d]\n",
		       wordCount[0], wordCount[1], wordCount[2],
		       wordCount[3], wordCount[4], wordCount[5],
		       wordCount[6], wordCount[7], wordCount[8],
		       wordCount[9]);
	} else {
		printf("ERROR\n");
	}
	printf("\n");

/******************************** Your custom code goes below here ********************************/
// Create new list that points to unsortedWordList
ListItem *newList = unsortedWordList;

// Sort unsortedWordList and print
LinkedListSort(unsortedWordList);
LinkedListPrint(newList);

// Displays the number of occurrences of each word in the new list
if (SortedWordCount(newList, wordCount)) {
		printf("[%d, %d, %d, %d, %d, %d, %d, %d, %d, %d]\n",
		       wordCount[0], wordCount[1], wordCount[2],
		       wordCount[3], wordCount[4], wordCount[5],
		       wordCount[6], wordCount[7], wordCount[8],
		       wordCount[9]);
	} else {
		printf("ERROR\n");
	}
	printf("\n");

/******************************** Your custom code goes above here ********************************/

// You can never return from main() in an embedded system (one that lacks an operating system).
// This will result in the processor restarting, which is almost certainly not what you want!
	while (1);
}

/**
 * This functions takes in the head of an unsorted list of words, and an array to store the number
 * of occurrences of each word.  The first time a word appears, the number of occurrences of that
 * word should be stored in the corresponding position in the array.  For repeated words the
 * negative of the number of occurrences should be stored.  For example, a list of the following
 * words should return an array with the following values:
 * {"hair", "comb", "brush", "brush"} -> [1, 1, 2, -2]
 *
 * NOTE: This function assumes that wordCount is the same length as list.
 * @param list A pointer to the head of a doubly-linked list containing unsorted words.
 * @param wordCount An array of integers. The output of this function is stored here. It must be
 *                  at least as big as the linked list pointed to be `list` is.
 * @return Either SUCCESS or STANDARD_ERROR if the head of the doubly-linked list isn't passed.
 */
int UnsortedWordCount(ListItem *list, int *wordCount)
{
	// Make sure the head of the list was given.
	if (list->previousItem != NULL) {
		return STANDARD_ERROR;
	}

	char *word1;
	char *word2;
	char *repCheck;
	ListItem *item1 = list;
	ListItem *item2;
	int temp, firstOccurencePos;
	int repetitionFlag = 0;
	int i, j;

	i = 0;
	while (item1 != NULL) {
		// Count first occurrence
		j = 1;
		// Ignore NULL words
		word1 = (char *)item1->data;
		if (word1 == NULL) {
			wordCount[i++] = 0;
			item1 = item1->nextItem;
			continue;
		}

		// Check previous occurrences
		temp = i;
		item2 = item1->previousItem;
		while (item2 != NULL) {
			--temp;
			repCheck = (char *)item2->data;
			if (repCheck != NULL && strcmp(repCheck, word1) == 0) {
				repetitionFlag = 1;
				firstOccurencePos = temp;
			}
			item2 = item2->previousItem;
		}

		// Count new occurrences
		if (repetitionFlag == 0) {
			item2 = LinkedListGetFirst(list);
			while (item2 != NULL) {
				word2 = (char *)item2->data;
				if (word2 != NULL && item1 != item2 && strcmp(word1, word2) == 0) {
					++j;
				}
				item2 = item2->nextItem;
			}
			wordCount[i++] = j;
		} else {
			wordCount[i++] = -wordCount[firstOccurencePos];
			repetitionFlag = 0;
		}
		item1 = item1->nextItem;
	}
	return SUCCESS;
}


/**
 * This function initializes a list of ListItems for use when testing the LinkedList implementation
 * and word count algorithms.
 *
 * Usage:
 * ListItem *newList;
 * InitializeUnsortedWordList(&newList);
 *
 * @param unsortedWordList[out] Where to store the pointer to the head of the list.
 * @return SUCCESS if it succeeds, STANDARD_ERROR if it fails to allocate necessary memory.
 */
int InitializeUnsortedWordList(ListItem **unsortedWordList)
{
    ListItem *tmp = LinkedListNew(crab);
    if (!tmp) {
        return STANDARD_ERROR;
    }
    if (!LinkedListCreateAfter(tmp, pig1)) {
        return STANDARD_ERROR;
    }
    if (!LinkedListCreateAfter(tmp, cow)) {
        return STANDARD_ERROR;
    }
    if (!LinkedListCreateAfter(tmp, NULL)) {
        return STANDARD_ERROR;
    }
    if (!LinkedListCreateAfter(tmp, dog)) {
        return STANDARD_ERROR;
    }
    if (!LinkedListCreateAfter(tmp, cow)) {
        return STANDARD_ERROR;
    }
    if (!LinkedListCreateAfter(tmp, bird)) {
        return STANDARD_ERROR;
    }
    if (!LinkedListCreateAfter(tmp, pig2)) {
        return STANDARD_ERROR;
    }
    if (!LinkedListCreateAfter(tmp, cat)) {
        return STANDARD_ERROR;
    }
    if (!LinkedListCreateAfter(tmp, turtle)) {
        return STANDARD_ERROR;
    }
    *unsortedWordList = tmp;
    return SUCCESS;
}

/**
 * SortedWordCount() takes in the head of a sorted list of words arranged lexicographically, and an
 * array to store the number of occurrences of each word.  The first time a word appears, the number
 * of occurrences of that word should be stored in the corresponding position in the array.  For
 * duplicate words a negative value of the number of occurrences should be stored.  For example, a
 * list of the following words should return an array with the following values:
 *     {"brush", "brush", "comb", "hair"} -> [2, -2, 1, 1]
 *
 * NOTE: This function assumes that wordCount is the same length as list.
 * @param list A pointer to the head of a doubly-linked list containing sorted words.
 * @param wordCount An array of integers. The output of this function is stored here. It must be
 *                  at least as big as the linked list pointed to be `list` is.
 * @return Either SUCCESS or STANDARD_ERROR if the head of the doubly-linked list isn't passed.
 */

int SortedWordCount(ListItem *list, int *wordCount){
    // If list is not head, ERROR
    if (list->previousItem != NULL) {
		return STANDARD_ERROR;
    }
    char *word1;
    char *word2;
    char *repCheck;
    ListItem *item1 = list;
    ListItem *item2;
    int temp, firstOccurencePos;
    int repetitionFlag = 0;
    int i, j;

    i = 0;
    while (item1 != NULL){

	// Count first occurrence
	j = 1;

	// Ignore NULL words
	word1 = (char *)item1->data;
	if (word1 == NULL){
            wordCount[i++] = 0;
            item1 = item1->nextItem;
            continue;
	}

	// Check previous occurrences
	temp = i;
	item2 = item1->previousItem;
	while (item2 != NULL){
            --temp;
            repCheck = (char *)item2->data;
            if (repCheck != NULL && strcmp(repCheck, word1) == 0){
		repetitionFlag = 1;
		firstOccurencePos = temp;
            }
            item2 = item2->previousItem;
	}

	// Count new occurrences
	if (repetitionFlag == 0){
            item2 = LinkedListGetFirst(list);
            while (item2 != NULL){
		word2 = (char *)item2->data;
		if (word2 != NULL && item1 != item2 && strcmp(word1, word2) == 0) {
                    ++j;
		}
		item2 = item2->nextItem;
            }
            wordCount[i++] = j;
        }else{
            wordCount[i++] = -wordCount[firstOccurencePos];
            repetitionFlag = 0;
	}
        item1 = item1->nextItem;
    }
    return SUCCESS;
}

