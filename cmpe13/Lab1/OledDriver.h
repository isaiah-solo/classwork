// Include standard C libraries.
#ifndef OLED_DRIVER_H
#define OLED_DRIVER_H

#include <xc.h>
#include <plib.h>

// Set the SPI channel to use to communicate with the OLED.
#define OLED_DRIVER_SPI_CHANNEL SPI_CHANNEL2

// The number of pixel columns in the OLED display.
#define OLED_DRIVER_PIXEL_COLUMNS                                                          128

// The number of pixel rows in the OLED display.
#define OLED_DRIVER_PIXEL_ROWS                                                              32

// Store how high each column is for the OLED in bits in terms of data structure storage.
#define OLED_DRIVER_BUFFER_LINE_HEIGHT                                                       8

// The number of bytes required to store all the data for the whole display. 1 bit / pixel.
#define OLED_DRIVER_BUFFER_SIZE     ((OLED_DRIVER_PIXEL_COLUMNS * OLED_DRIVER_PIXEL_ROWS) / 8)

/**
 * This array is the off-screen frame buffer used for rendering. It isn't possible to read back from
 * the OLED display device, so display data is rendered into this off-screen buffer and then copied
 * to the display. The high-order bits equate to the lower pixel rows.
 * @note Any time this is updated, An `OledDriverUpdateDisplay()` call must be performed.
 */
extern uint8_t rgbOledBmp[OLED_DRIVER_BUFFER_SIZE];

void OledHostInit(void);

int PORTPORTClearBits();
int pbClkDiv ;

void OledDriverInitDisplay(void);

int ClearBits();
int _Spi2Put();

void OledDriverDisableDisplay(void);

void OledDriverUpdateDisplay(void);

void OledDriverSetDisplayInverted(void);

void OledDriverSetDisplayNormal(void);

#endif // OLED_DRIVER_H