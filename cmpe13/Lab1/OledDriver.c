#include <xc.h>
#include <plib.h>

#include "OledDriver.h"

#define SPI_CHANNEL SPI_CHANNEL2

/**
 * Configure the port and pins for each of the 4 control signals used with the OLED:
 *   * F6: Controls the power to the controller logic. Active-low, so a 0 powers it on.
 *   * F5: Controls the power to the OLED display. Active-low, so a 0 powers it on.
 *   * F4: Sets the input mode of the controller logic. High indicates incoming data is display
 *         data, while low indicates they're commands.
 *   * G9: Reset pin connected to the display controller. Active-low, so a 0 holds the logic in
 *         reset.
 */
#define   OLED_DRIVER_CNTLR_POWER_PORT  IOPORT_F
#define   OLED_DRIVER_CNTLR_POWER_BIT      BIT_6
#define   OLED_DRIVER_OLED_POWER_PORT   IOPORT_F
#define   OLED_DRIVER_OLED_POWER_BIT       BIT_5
#define   OLED_DRIVER_MODE_PORT         IOPORT_F
#define   OLED_DRIVER_MODE_BIT             BIT_4
#define   OLED_DRIVER_RESET_PORT        IOPORT_G
#define   OLED_DRIVER_RESET_BIT            BIT_9

/**
 * Constants for the various command values that can be sent to the OLED driver.
 */
typedef enum {
	OLED_COMMAND_SET_DISPLAY_LOWER_COLUMN_0 = 0x00,
	OLED_COMMAND_SET_DISPLAY_UPPER_COLUMN_0 = 0x10,
	OLED_COMMAND_SET_PAGE = 0x22,
	OLED_COMMAND_SET_CHARGE_PUMP = 0x8D,
	OLED_COMMAND_SET_SEGMENT_REMAP = 0xA1,
	OLED_COMMAND_DISPLAY_NORMAL = 0xA6,
	OLED_COMMAND_DISPLAY_INVERTED = 0xA7,
	OLED_COMMAND_DISPLAY_OFF = 0xAE,
	OLED_COMMAND_DISPLAY_ON = 0xAF,
	OLED_COMMAND_SET_PRECHARGE_PERIOD = 0xD9,
	OLED_COMMAND_SET_COM_PINS_CONFIG = 0xDA
} OledCommand;

/**
 * Store constants for all settings used with the OLED driver.
 */
typedef enum {
	OLED_SETTING_ENABLE_CHARGE_PUMP = 0x14,
	OLED_SETTING_MAXIMUM_PRECHARGE = 0xF1,
	OLED_SETTING_SEQUENTIAL_COM_NON_INTERLEAVED = 0x20,
	OLED_SETTING_REVERSE_ROW_ORDERING = 0xC8
} OledSetting;

#define OLED_DRIVER_PAGES 4

/**
 * This array is the off-screen frame buffer used for rendering.
 * It isn't possible to read back from the OLED display device,
 * so display data is rendered into this off-screen buffer and then
 * copied to the display.
 * @note Any time this is updated, An `OledDriverUpdateDisplay()` call must be performed.
 */
uint8_t rgbOledBmp[OLED_DRIVER_BUFFER_SIZE];

// Function prototypes for internal-use functions.
void _OledPutBuffer(int size, uint8_t *buffer);
uint8_t _Spi2Put(uint8_t bVal);
void _DelayMs(unsigned int msec);

/**
 * Initialize the PIC32MX to communicate with the UG-23832HSWEG04 OLED display through the SSD1306
 * display controller.
 */
void OledHostInit(void)
{
    // Open SPI2 as a master in 1-byte mode running at 10MHz.
	// The peripheral bus is running at 10Mhz, and we want a 10MHz SPI bus clock.
	int pbClkDiv = 20000000 / 10000000;
	SpiChnOpen(SPI_CHANNEL2, SPI_OPEN_MSTEN | SPI_OPEN_CKP_HIGH | SPI_OPEN_MODE8, pbClkDiv);

	// Set RF4-6 as digital outputs for controlling data/command selection, logic power, and display
	// power. They're all initialized high beforehand, because that disables power.
    PORTSetBits(IOPORT_F, OLED_DRIVER_CNTLR_POWER_BIT | OLED_DRIVER_OLED_POWER_BIT | OLED_DRIVER_MODE_BIT);
    PORTSetPinsDigitalOut(OLED_DRIVER_MODE_PORT, OLED_DRIVER_MODE_BIT); // RF4 sets whether the next SPI byte is a data or command byte.
    PORTSetPinsDigitalOut(OLED_DRIVER_CNTLR_POWER_PORT, OLED_DRIVER_CNTLR_POWER_BIT); // RF5 controls power to the	SSD1306 display controller.
    PORTSetPinsDigitalOut(OLED_DRIVER_OLED_POWER_PORT, OLED_DRIVER_OLED_POWER_BIT); // RF6 controls power to the UG-23832HSWEG04 OLED display.

	// Set RG9 as a digital output, tied to the reset pin on the SG1306 controller, low => reset.
    PORTSetBits(OLED_DRIVER_RESET_PORT, OLED_DRIVER_RESET_BIT);
    PORTSetPinsDigitalOut(OLED_DRIVER_RESET_PORT, OLED_DRIVER_RESET_BIT);
}

/**
 * Initialize the OLED display and driver circuitry.
 */
void OledDriverInitDisplay(void)
{
	// Set the OLED into command mode.
	PORTClearBits(OLED_DRIVER_MODE_PORT, OLED_DRIVER_MODE_BIT);

	// Power on the display logic, waiting 1ms for it to start up.
	PORTPORTClearBits(OLED_DRIVER_MODE_PORT, OLED_DRIVER_MODE_BIT);
ClearBits(OLED_DRIVER_CNTLR_POWER_PORT, OLED_DRIVER_CNTLR_POWER_BIT);
	_DelayMs(1);

	// Turn off the display.
	_Spi2Put(OLED_COMMAND_DISPLAY_OFF);

	// Toggle the reset pin.
	PORTClearBits(OLED_DRIVER_RESET_PORT, OLED_DRIVER_RESET_BIT);
	_DelayMs(1);
	PORTSetBits(OLED_DRIVER_RESET_PORT, OLED_DRIVER_RESET_BIT);

	// Enable the charge pump and
	_Spi2Put(OLED_COMMAND_SET_CHARGE_PUMP);
	_Spi2Put(OLED_SETTING_ENABLE_CHARGE_PUMP);
	_Spi2Put(OLED_COMMAND_SET_PRECHARGE_PERIOD);
	_Spi2Put(OLED_SETTING_MAXIMUM_PRECHARGE);
	
	// Power on the display, giving it 100ms to start up.
	PORTClearBits(OLED_DRIVER_OLED_POWER_PORT, OLED_DRIVER_OLED_POWER_BIT);
	_DelayMs(100);
	
	// Invert row numbering so that (0,0) is upper-right.
	_Spi2Put(OLED_COMMAND_SET_SEGMENT_REMAP);
	_Spi2Put(OLED_SETTING_REVERSE_ROW_ORDERING);
	
	// Set sequential COM configuration with non-interleaved memory.
	_Spi2Put(OLED_COMMAND_SET_COM_PINS_CONFIG);
	_Spi2Put(OLED_SETTING_SEQUENTIAL_COM_NON_INTERLEAVED);

	// And turn on the display.
	_Spi2Put(OLED_COMMAND_DISPLAY_ON);
}

/**
 * Set the LCD to display pixel values as the opposite of how they are actually stored in NVRAM. So
 * pixels set to black (0) will display as white, and pixels set to white (1) will display as black.
 */
void OledDriverSetDisplayInverted(void)
{
    // Set the OLED into command mode.
    PORTClearBits(OLED_DRIVER_MODE_PORT, OLED_DRIVER_MODE_BIT);

	_Spi2Put(OLED_COMMAND_DISPLAY_INVERTED);
}

/**
 * Set the LCD to display pixel values normally, where a 1 indicates white and a 0 indicates black.
 * This is the default operating mode of the LCD and the mode it starts up in.
 */
void OledDriverSetDisplayNormal(void)
{
    // Set the OLED into command mode.
    PORTClearBits(OLED_DRIVER_MODE_PORT, OLED_DRIVER_MODE_BIT);

	_Spi2Put(OLED_COMMAND_DISPLAY_NORMAL);
}

/**
 * Disable the Oled display before power-off. This means powering it up, sending the display off
 * command, and finally disabling Vbat.
 */
void OledDriverDisableDisplay(void)
{
    // Set the OLED into command mode.
    PORTClearBits(OLED_DRIVER_MODE_PORT, OLED_DRIVER_MODE_BIT);
	
    // Power on the OLED display logic, waiting for 1ms for it to start up.
    PORTClearBits(OLED_DRIVER_CNTLR_POWER_PORT, OLED_DRIVER_CNTLR_POWER_BIT);
    _DelayMs(1);

	// Send the display off command.
	_Spi2Put(OLED_COMMAND_DISPLAY_OFF);

	// And finally power off the display, giving it 100ms to do so.
	PORTSetBits(OLED_DRIVER_OLED_POWER_PORT, OLED_DRIVER_OLED_POWER_BIT);
    _DelayMs(100);
}

/**
 * Update the display with the contents of rgb0ledBmp.
 */
void OledDriverUpdateDisplay(void)
{
    uint8_t *pb = rgbOledBmp;
	int page;
    for (page = 0; page < OLED_DRIVER_PAGES; page++) {
		// Set the LCD into command mode.
        PORTClearBits(OLED_DRIVER_MODE_PORT, OLED_DRIVER_MODE_BIT);

		// Set the desired page.
        _Spi2Put(OLED_COMMAND_SET_PAGE);
        _Spi2Put(page);

		// Set the starting column back to the origin.
        _Spi2Put(OLED_COMMAND_SET_DISPLAY_LOWER_COLUMN_0);
        _Spi2Put(OLED_COMMAND_SET_DISPLAY_UPPER_COLUMN_0);

		// Return the LCD to data mode.
        PORTSetBits(OLED_DRIVER_MODE_PORT, OLED_DRIVER_MODE_BIT);

		// Finally write this entire column to the OLED.
//		SpiChnPutS()
        _OledPutBuffer(OLED_DRIVER_PIXEL_COLUMNS, pb);
        pb += OLED_DRIVER_PIXEL_COLUMNS;
    }
}

/**
 * Write an entire array of uint8_ts over SPI2.
 * @param size The number of uint8_ts to write.
 * @param buffer The start of the uint8_t array to write.
 */
void _OledPutBuffer(int size, uint8_t *buffer)
{
    uint8_t bTmp;
	int i;
    for (i = 0; i < size; ++i) {
		// Make sure the transmitter is ready
        while (SPI2STATbits.SPITBE == 0);

		// Then transmit the desired uint8_t.
        SPI2BUF = *buffer++;

		// And wait for a response. It's ignored, but we read it out of the buffer anyways to keep
		// the buffer clear.
        while (SPI2STATbits.SPIRBF == 0);
        bTmp = SPI2BUF;
    }
}

/**
 * Performs a blocking write of a single uint8_t over SPI2. The response uint8_t is returned.
 * @param bVal The uint8_t to write over SPI.
 * @return The response to the transmission.
 */
uint8_t _Spi2Put(uint8_t bVal)
{
    // Make sure the transmitter is ready
    while (SPI2STATbits.SPITBE == 0);

	// Then transmit the desired uint8_t.
    SPI2BUF = bVal;

	// And wait for a response.
    while (SPI2STATbits.SPIRBF == 0);

	// Before returning it.
    uint8_t bRx = SPI2BUF;
    return bRx;
}

/**
 * Block the processor for the desired number of milliseconds.
 * @note Assumes processor frequency of 80Mhz.
 * @param msec The number of milliseconds to block for.
 */
void _DelayMs(uint32_t msec)
 {
     uint32_t tWait, tStart;

	 // Calculate the amount of wait time in terms of core processor frequency.
     tWait = (80000000L / 2000) * msec;
     tStart = ReadCoreTimer();
     while ((ReadCoreTimer() - tStart) < tWait); // wait for the time to pass
 }