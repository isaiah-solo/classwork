#include "Oled.h"

/**
 * Define constants for available colors for the OLED: either white or black.
 */
typedef enum {
    OLED_COLOR_BLACK = 0,
    OLED_COLOR_WHITE = 1
} OledColor;

#define OLED_NUM_LINES         (OLED_DRIVER_PIXEL_ROWS / ASCII_FONT_HEIGHT)
#define OLED_CHARS_PER_LINE  (OLED_DRIVER_PIXEL_COLUMNS / ASCII_FONT_WIDTH)

/**
 * Initializes the OLED, turns it on, and clears the screen.
 */
void OledInit(void)
{
    // First initialize the PIC32 to be able to talk over SPI to the OLED.
    OledHostInit();

    // Now send initialization commands to the OLED.
    OledDriverInitDisplay();

    // Clear the frame buffer by filling it with black pixels.
    OledClear(OLED_COLOR_BLACK);

    // Finally update the screen, triggering a write of all black pixels to the screen.
    OledUpdate();
}

/**
 * Sets a specific pixel in the frame buffer, available colors are black or white.
 * @note OledUpdate() must be called before the OLED will actually display these changes.
 * @param x The X position (left is zero)
 * @param y The Y position (top is zero)
 * @param color OLED_COLOR_WHITE or OLED_COLOR_BLACK
 */
void OledSetPixel(int x, int y, int color)
{
    // Check for valid inputs before doing anything.
    if (x >= OLED_DRIVER_PIXEL_COLUMNS || y >= OLED_DRIVER_PIXEL_ROWS || x < 0 || y < 0) {
        return;
    }

    // Map the x/y coordinates into a byte/bit index.
    unsigned int index = ((y & 0xFFF8) << 4) + x;
    unsigned int shift = y & 0x0007;

    // Now set the pixel to the proper color, doing nothing if an invalid color was specified.
    if (color == OLED_COLOR_WHITE) {
        rgbOledBmp[index] = rgbOledBmp[index] | (1 << shift);
    } else if (color == OLED_COLOR_BLACK) {
        rgbOledBmp[index] = rgbOledBmp[index] & ~(1 << shift);
    } else {
        return;
    }
}

/**
 * Reads a pixel from the frame buffer.
 * @param x The X position (left is zero)
 * @param y The Y position (top is zero)
 * @return OLED_COLOR_WHITE or OLED_COLOR_BLACK
 */
int OledGetPixel(int x, int y)
{
    // Check for valid inputs before doing anything.
    if (x >= OLED_DRIVER_PIXEL_COLUMNS || y >= OLED_DRIVER_PIXEL_ROWS || x < 0 || y < 0) {
        return OLED_COLOR_BLACK;
    }

    // Map the x/y coordinates into a byte/bit index.
    unsigned int index = ((y & 0xFFF8) << 4) + x;
    unsigned int shift = y & 0x0007;

    // Now return the desired bit.
    return (rgbOledBmp[index] >> shift) & 0x01;
}

/**
 * Draws the specified character at the specified position, using Ascii.h as the font.
 * @param x The x-position to use as the left-most value for the character.
 * @param y The y-position to use as the top-most value for the character
 * @param c The character to write. Uses the character array defined in Ascii.h
 * @return True if the write succeeded. Fails on invalid inputs.
 */
bool OledDrawChar(int x, int y, char c)
{
    if (x < OLED_DRIVER_PIXEL_COLUMNS - ASCII_FONT_WIDTH && y < OLED_DRIVER_PIXEL_ROWS - ASCII_FONT_HEIGHT) {
        // Now first determine the columns and rows of the OLED bits that need to be modified
        int rowMin, rowMax, colMin, colMax;
        rowMin = y / ASCII_FONT_HEIGHT;
        int rowY = y % ASCII_FONT_HEIGHT;
        rowMax = (y + ASCII_FONT_HEIGHT) / OLED_DRIVER_BUFFER_LINE_HEIGHT;
        colMin = x;
        colMax = x + ASCII_FONT_WIDTH;
        {
            // Generate a positive mask for where in the column the new symbol will be drawn.
            int colMask = ((1 << ASCII_FONT_HEIGHT) - 1) << rowY;
            int j;
            for (j = 0; j < colMax - colMin; ++j) {
                int oledCol = colMin + j;
                uint8_t newCharCol = rgbOledBmp[rowMin * OLED_DRIVER_PIXEL_COLUMNS + oledCol] & ~colMask;
                // Make sure we always grab from the top part of the character.
                newCharCol |= (ascii[(int)c][j] & (colMask >> rowY)) << rowY;
                rgbOledBmp[rowMin * OLED_DRIVER_PIXEL_COLUMNS + oledCol] = newCharCol;
            }
        }
        if (rowMax > rowMin) {
            // Generate a positive mask for where in the column the new symbol will be drawn.
            // Since we need the lower portion of the symbol, we recalculate its height.
            int colMask = ((1 << ASCII_FONT_HEIGHT) - 1) >> (OLED_DRIVER_BUFFER_LINE_HEIGHT - rowY);
            int j;
            for (j = 0; j < colMax - colMin; ++j) {
                int oledCol = colMin + j;
                uint8_t newCharCol = rgbOledBmp[rowMax * OLED_DRIVER_PIXEL_COLUMNS + oledCol] & ~colMask;
                // Make sure we grab the proper part of the character from the font.
                newCharCol |= (ascii[(int)c][j] & (colMask << (OLED_DRIVER_BUFFER_LINE_HEIGHT - rowY))) >>
                    (OLED_DRIVER_BUFFER_LINE_HEIGHT - rowY);
                rgbOledBmp[rowMax * OLED_DRIVER_PIXEL_COLUMNS + oledCol] = newCharCol;
            }
        }
    }

    return false;
}

/**
 * Draws a string to the screen buffer, starting on the top line. 21 characters fit on the 4 lines
 * on the screen.
 * @note OledUpdate() must be called before the OLED will actually display these changes.
 * @param string A null-terminated string to print.
 */
void OledDrawString(const char *string)
{
    if (string == NULL) {
        return;
    }

    int line = 0;
    int column = 0;

    int i;
    for (i = 0; string[i] != '\0' && line < OLED_NUM_LINES && column < OLED_CHARS_PER_LINE; ++i) {
        // Move the cursor to the next line if a newline character is encountered.
        if (string[i] == '\n') {
            ++line;
            column = 0;
        } else {
            int j, charIndex, bmpIndex;
            for (j = 0; j < ASCII_FONT_WIDTH; ++j){ // copy from the font to the buffer
                charIndex = (int)string[i];
                bmpIndex = (line * OLED_DRIVER_PIXEL_COLUMNS) + (column * ASCII_FONT_WIDTH) + j;
                rgbOledBmp[bmpIndex] = ascii[charIndex][j];
            }
            ++column;
        }
    }
}

/**
 * Writes the specified color pixels to the entire frame buffer.
 * @note OledUpdate() must be called before the OLED will actually display these changes.
 * @param p The color to write all pixels in the OLED to.
 */
void OledClear(int p)
{
    int i;
    for (i = 0; i < OLED_DRIVER_BUFFER_SIZE; i++) {
        if (p == OLED_COLOR_WHITE) {
            rgbOledBmp[i] = 0xFF;
        } else {
            rgbOledBmp[i] = 0;
        }
    }
}

/**
 * Redraws the OLED display based on the contents of `rgb0ledBmp`. Use after `rgb0ledBmp` is
 * modified.
 */
void OledUpdate(void)
{
    OledDriverUpdateDisplay();
}