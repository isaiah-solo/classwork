// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Field.h"
#include "FieldOled.h"
#include "Protocol.h"

/**************************************************************************************************\
 * FieldInit()                                                                                    *
\**************************************************************************************************/

/**
 * FieldInit() will fill the passed field array with the data specified in positionData. Also the
 * lives for each boat are filled according to the `BoatLives` enum.
 * @param f The field to initialize.
 * @param p The data to initialize the entire field to, should be a member of enum
 *                     FieldPosition.
 */
void FieldInit(Field *f, FieldPosition p) {
    int i, j;
    for (i = 0; i < FIELD_ROWS; i++) {
        for (j = 0; j < FIELD_COLS; j++) {
            f->field[i][j] = p;
        }
    }
    f->smallBoatLives = FIELD_BOAT_LIVES_SMALL;
    f->mediumBoatLives = FIELD_BOAT_LIVES_MEDIUM;
    f->largeBoatLives = FIELD_BOAT_LIVES_LARGE;
    f->hugeBoatLives = FIELD_BOAT_LIVES_HUGE;
}

/**************************************************************************************************\
 * FieldAt()                                                                                      *
\**************************************************************************************************/

/**
 * Retrieves the value at the specified field position.
 * @param f The Field being referenced
 * @param row The row-component of the location to retrieve
 * @param col The column-component of the location to retrieve
 * @return
 */
FieldPosition FieldAt(const Field *f, uint8_t row, uint8_t col) {
    return f->field[row][col];
}

/**************************************************************************************************\
 * FieldSetLocation()                                                                             *
\**************************************************************************************************/

/**
 * This function provides an interface for setting individual locations within a Field struct. This
 * is useful when FieldAddBoat() doesn't do exactly what you need. For example, if you'd like to use
 * FIELD_POSITION_CURSOR, this is the function to use.
 *
 * @param f [[The Field to modify.
 * @param row The row-component of the location to modify
 * @param col The column-component of the location to modify
 * @param p The new value of the field location
 * @return The old value at that field location
 */
FieldPosition FieldSetLocation(Field *f, uint8_t row, uint8_t col, FieldPosition p) {
    FieldPosition old = f->field[row][col];
    f->field[row][col] = p;
    return old;
}

/**************************************************************************************************\
 * FieldAddBoat()                                                                                 *
\**************************************************************************************************/

/**
 * FieldAddBoat() places a single ship on the player's field based on arguments 2-5. Arguments 2, 3
 * represent the x, y coordinates of the pivot point of the ship.  Argument 4 represents the
 * direction of the ship, and argument 5 is the length of the ship being placed. All spaces that
 * the boat would occupy are checked to be clear before the field is modified so that if the boat
 * can fit in the desired position, the field is modified as SUCCESS is returned. Otherwise the
 * field is unmodified and STANDARD_ERROR is returned. There is no hard-coded limit to how many
 * times a boat can be added to a field within this function.
 *
 * @param f The field to grab data from.
 * @param row The row that the boat will start from, valid range is from 0 and to FIELD_ROWS - 1.
 * @param col The column that the boat will start from, valid range is from 0 and to FIELD_COLS - 1.
 * @param dir The direction that the boat will face once places, from the BoatDirection enum.
 * @param boatType The type of boat to place. Relies on the FIELD_POSITION_*_BOAT values from the
 * FieldPosition enum.
 * @return true for success, false for failure
 */
bool FieldAddBoat(Field *f, uint8_t row, uint8_t col, BoatDirection dir, BoatType type) {
    int i;
    if (row > (FIELD_ROWS - 1) || row < 0 || col > (FIELD_COLS - 1) || col < 0) {
        return false;
    } else {
        switch (type) {
/*********************************FIELD_BOAT_DIRECTION_SMALL***************************************/
            case FIELD_BOAT_SMALL:
                switch (dir) {
                    case FIELD_BOAT_DIRECTION_NORTH:
                        if (f->field[row][col] == FIELD_POSITION_EMPTY &&
                                f->field[row - 1][col] == FIELD_POSITION_EMPTY &&
                                f->field[row - 2][col] == FIELD_POSITION_EMPTY &&
                                !((row) > (FIELD_ROWS - 1) || (row - 2) < 0 ||
                                (col) > (FIELD_COLS - 1) || (col) < 0)) {
                            for (i = 0; i < 3; i++) {
                                f->field[row - i][col] = FIELD_POSITION_SMALL_BOAT;
                            }
                            return true;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_EAST:
                        if (f->field[row][col] == FIELD_POSITION_EMPTY &&
                                f->field[row][col + 1] == FIELD_POSITION_EMPTY &&
                                f->field[row][col + 2] == FIELD_POSITION_EMPTY &&
                                !((row) > (FIELD_ROWS - 1) || (row) < 0 ||
                                (col + 2) > (FIELD_COLS - 1) || (col) < 0)) {
                            for (i = 0; i < 3; i++) {
                                f->field[row][col + i] = FIELD_POSITION_SMALL_BOAT;
                            }
                            return true;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_SOUTH:
                        if (f->field[row][col] == FIELD_POSITION_EMPTY &&
                                f->field[row + 1][col] == FIELD_POSITION_EMPTY &&
                                f->field[row + 2][col] == FIELD_POSITION_EMPTY &&
                                !((row + 2) > (FIELD_ROWS - 1) || (row) < 0 ||
                                (col) > (FIELD_COLS - 1) || (col) < 0)) {
                            for (i = 0; i < 3; i++) {
                                f->field[row + i][col] = FIELD_POSITION_SMALL_BOAT;
                            }
                            return true;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_WEST:
                        if (f->field[row][col] == FIELD_POSITION_EMPTY &&
                                f->field[row][col - 1] == FIELD_POSITION_EMPTY &&
                                f->field[row][col - 2] == FIELD_POSITION_EMPTY &&
                                !((row) > (FIELD_ROWS - 1) || (row) < 0 ||
                                (col) > (FIELD_COLS - 1) || (col - 2) < 0)) {
                            for (i = 0; i < 3; i++) {
                                f->field[row][col - i] = FIELD_POSITION_SMALL_BOAT;
                            }
                            return true;
                        }
                        break;
                }
                break;
/*********************************FIELD_BOAT_DIRECTION_MEDIUM**************************************/
            case FIELD_BOAT_MEDIUM:
                switch (dir) {
                    case FIELD_BOAT_DIRECTION_NORTH:
                        if (f->field[row][col] == FIELD_POSITION_EMPTY &&
                                f->field[row - 1][col] == FIELD_POSITION_EMPTY &&
                                f->field[row - 2][col] == FIELD_POSITION_EMPTY &&
                                f->field[row - 3][col] == FIELD_POSITION_EMPTY &&
                                !((row) > (FIELD_ROWS - 1) || (row - 3) < 0 ||
                                (col) > (FIELD_COLS - 1) || (col) < 0)) {
                            for (i = 0; i < 4; i++) {
                                f->field[row - i][col] = FIELD_POSITION_MEDIUM_BOAT;
                            }
                            return true;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_EAST:
                        if (f->field[row][col] == FIELD_POSITION_EMPTY &&
                                f->field[row][col + 1] == FIELD_POSITION_EMPTY &&
                                f->field[row][col + 2] == FIELD_POSITION_EMPTY &&
                                f->field[row][col + 3] == FIELD_POSITION_EMPTY &&
                                !((row) > (FIELD_ROWS - 1) || (row) < 0 ||
                                (col + 3) > (FIELD_COLS - 1) || (col) < 0)) {
                            for (i = 0; i < 4; i++) {
                                f->field[row][col + i] = FIELD_POSITION_MEDIUM_BOAT;
                            }
                            return true;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_SOUTH:
                        if (f->field[row][col]== FIELD_POSITION_EMPTY  &&
                                f->field[row + 1][col] == FIELD_POSITION_EMPTY &&
                                f->field[row + 2][col] == FIELD_POSITION_EMPTY &&
                                f->field[row + 3][col] == FIELD_POSITION_EMPTY &&
                                !((row + 3) > (FIELD_ROWS - 1) || (row) < 0 ||
                                (col) > (FIELD_COLS - 1) || (col) < 0)) {
                            for (i = 0; i < 4; i++) {
                                f->field[row + i][col] = FIELD_POSITION_MEDIUM_BOAT;
                            }
                            return true;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_WEST:
                        if (f->field[row][col] == FIELD_POSITION_EMPTY &&
                                f->field[row][col - 1] == FIELD_POSITION_EMPTY &&
                                f->field[row][col - 2] == FIELD_POSITION_EMPTY &&
                                f->field[row][col - 3] == FIELD_POSITION_EMPTY &&
                                !((row) > (FIELD_ROWS - 1) || (row) < 0 ||
                                (col) > (FIELD_COLS - 1) || (col - 3) < 0)) {
                            for (i = 0; i < 4; i++) {
                                f->field[row][col - i] = FIELD_POSITION_MEDIUM_BOAT;
                            }
                            return true;
                        }
                        break;
                }
                break;
/*********************************FIELD_BOAT_DIRECTION_LARGE***************************************/
            case FIELD_BOAT_LARGE:
                switch (dir) {
                    case FIELD_BOAT_DIRECTION_NORTH:
                        if (f->field[row][col] == FIELD_POSITION_EMPTY &&
                                f->field[row - 1][col] == FIELD_POSITION_EMPTY &&
                                f->field[row - 2][col] == FIELD_POSITION_EMPTY &&
                                f->field[row - 3][col] == FIELD_POSITION_EMPTY &&
                                f->field[row - 4][col] == FIELD_POSITION_EMPTY &&
                                !((row) > (FIELD_ROWS - 1) || (row - 4) < 0 ||
                                (col) > (FIELD_COLS - 1) || (col) < 0)) {
                            for (i = 0; i < 5; i++) {
                                f->field[row - i][col] = FIELD_POSITION_LARGE_BOAT;
                            }
                            return true;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_EAST:
                        if (f->field[row][col] == FIELD_POSITION_EMPTY &&
                                f->field[row][col + 1] == FIELD_POSITION_EMPTY &&
                                f->field[row][col + 2] == FIELD_POSITION_EMPTY &&
                                f->field[row][col + 3] == FIELD_POSITION_EMPTY &&
                                f->field[row][col + 4] == FIELD_POSITION_EMPTY &&
                                !((row) > (FIELD_ROWS - 1) || (row) < 0 ||
                                (col + 4) > (FIELD_COLS - 1) || (col) < 0)) {
                            for (i = 0; i < 5; i++) {
                                f->field[row][col + i] = FIELD_POSITION_LARGE_BOAT;
                            }
                            return true;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_SOUTH:
                        if (f->field[row][col] == FIELD_POSITION_EMPTY &&
                                f->field[row + 1][col] == FIELD_POSITION_EMPTY &&
                                f->field[row + 2][col] == FIELD_POSITION_EMPTY &&
                                f->field[row + 3][col] == FIELD_POSITION_EMPTY &&
                                f->field[row + 4][col] == FIELD_POSITION_EMPTY &&
                                !((row + 4) > (FIELD_ROWS - 1) || (row) < 0 ||
                                (col) > (FIELD_COLS - 1) || (col) < 0)) {
                            for (i = 0; i < 5; i++) {
                                f->field[row + i][col] = FIELD_POSITION_LARGE_BOAT;
                            }
                            return true;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_WEST:
                        if (f->field[row][col] == FIELD_POSITION_EMPTY &&
                                f->field[row][col - 1] == FIELD_POSITION_EMPTY &&
                                f->field[row][col - 2] == FIELD_POSITION_EMPTY &&
                                f->field[row][col - 3] == FIELD_POSITION_EMPTY &&
                                f->field[row][col - 4] == FIELD_POSITION_EMPTY &&
                                !((row) > (FIELD_ROWS - 1) || (row) < 0 ||
                                (col) > (FIELD_COLS - 1) || (col - 4) < 0)) {
                            for (i = 0; i < 5; i++) {
                                f->field[row][col - i] = FIELD_POSITION_LARGE_BOAT;
                            }
                            return true;
                        }
                        break;
                }
                break;
/*********************************FIELD_BOAT_DIRECTION_HUGE****************************************/
            case FIELD_BOAT_HUGE:
                switch (dir) {
                    case FIELD_BOAT_DIRECTION_NORTH:
                        if (f->field[row][col] == FIELD_POSITION_EMPTY &&
                                f->field[row - 1][col] == FIELD_POSITION_EMPTY &&
                                f->field[row - 2][col] == FIELD_POSITION_EMPTY &&
                                f->field[row - 3][col] == FIELD_POSITION_EMPTY &&
                                f->field[row - 4][col] == FIELD_POSITION_EMPTY &&
                                f->field[row - 5][col] == FIELD_POSITION_EMPTY &&
                                !((row) > (FIELD_ROWS - 1) || (row - 5) < 0 ||
                                (col) > (FIELD_COLS - 1) || (col) < 0)) {
                            for (i = 0; i < 6; i++) {
                                f->field[row - i][col] = FIELD_POSITION_HUGE_BOAT;
                            }
                            return true;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_EAST:
                        if (f->field[row][col] == FIELD_POSITION_EMPTY &&
                                f->field[row][col + 1] == FIELD_POSITION_EMPTY &&
                                f->field[row][col + 2] == FIELD_POSITION_EMPTY &&
                                f->field[row][col + 3] == FIELD_POSITION_EMPTY &&
                                f->field[row][col + 4] == FIELD_POSITION_EMPTY &&
                                f->field[row][col + 5] == FIELD_POSITION_EMPTY &&
                                !((row) > (FIELD_ROWS - 1) || (row) < 0 ||
                                (col + 5) > (FIELD_COLS - 1) || (col) < 0)) {
                            for (i = 0; i < 6; i++) {
                                f->field[row][col + i] = FIELD_POSITION_HUGE_BOAT;
                            }
                            return true;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_SOUTH:
                        if (f->field[row][col] == FIELD_POSITION_EMPTY &&
                                f->field[row + 1][col] == FIELD_POSITION_EMPTY &&
                                f->field[row + 2][col] == FIELD_POSITION_EMPTY &&
                                f->field[row + 3][col] == FIELD_POSITION_EMPTY &&
                                f->field[row + 4][col] == FIELD_POSITION_EMPTY &&
                                f->field[row + 5][col] == FIELD_POSITION_EMPTY &&
                                !((row + 5) > (FIELD_ROWS - 1) || (row) < 0 ||
                                (col) > (FIELD_COLS - 1) || (col) < 0)) {
                            for (i = 0; i < 6; i++) {
                                f->field[row + i][col] = FIELD_POSITION_HUGE_BOAT;
                            }
                            return true;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_WEST:
                        if (f->field[row][col] == FIELD_POSITION_EMPTY &&
                                f->field[row][col - 1] == FIELD_POSITION_EMPTY &&
                                f->field[row][col - 2] == FIELD_POSITION_EMPTY &&
                                f->field[row][col - 3] == FIELD_POSITION_EMPTY &&
                                f->field[row][col - 4] == FIELD_POSITION_EMPTY &&
                                f->field[row][col - 5] == FIELD_POSITION_EMPTY &&
                                !((row) > (FIELD_ROWS - 1) || (row) < 0 ||
                                (col) > (FIELD_COLS - 1) || (col - 5) < 0)) {
                            for (i = 0; i < 6; i++) {
                                f->field[row][col - i] = FIELD_POSITION_HUGE_BOAT;
                            }
                            return true;
                        }
                        break;
                }
                break;
        }
    }
    return false;
}

/**************************************************************************************************\
 * FieldRegisterEnemyAttack()                                                                     *
\**************************************************************************************************/

/**
 * This function registers an attack at the gData coordinates on the provided field. This means that
 * 'state' is updated with a FIELD_POSITION_HIT or FIELD_POSITION_MISS depending on what was at the
 * coordinates indicated in 'gData'. 'gData' is also updated with the proper HitStatus value
 * depending on what happened. Finally this function also reduces the lives for any boat that was
 * hit from this attack.
 * @param f The field to check against and update.
 * @param gData The coordinates that were guessed. The HIT result is stored in gData->hit as an
 *               output.
 * @return The data that was stored at the field position indicated by gData.
 */
FieldPosition FieldRegisterEnemyAttack(Field *f, GuessData *gData) {
    FieldPosition old = f->field[gData->row][gData->col];
    if (f->field[gData->row][gData->col] == (FIELD_POSITION_SMALL_BOAT ||
            FIELD_POSITION_MEDIUM_BOAT || FIELD_POSITION_LARGE_BOAT ||
            FIELD_POSITION_HUGE_BOAT || FIELD_POSITION_HIT)) {
        gData->hit = HIT_HIT;
        if (f->field[gData->row][gData->col] == FIELD_POSITION_SMALL_BOAT) {
            f->smallBoatLives--;
        } else if (f->field[gData->row][gData->col] == FIELD_POSITION_MEDIUM_BOAT) {
            f->mediumBoatLives--;
        } else if (f->field[gData->row][gData->col] == FIELD_POSITION_LARGE_BOAT) {
            f->largeBoatLives--;
        } else if (f->field[gData->row][gData->col] == FIELD_POSITION_HUGE_BOAT) {
            f->hugeBoatLives--;
        }
        f->field[gData->row][gData->col] = FIELD_POSITION_HIT;
    } else {
        gData->hit = HIT_MISS;
        f->field[gData->row][gData->col] = FIELD_POSITION_MISS;
    }
    return old;
}

/**************************************************************************************************\
 * FieldUpdateKnowledge()                                                                         *
\**************************************************************************************************/

/**
 * This function updates the FieldState representing the opponent's game board with whether the
 * guess indicated within gData was a hit or not. If it was a hit, then the field is updated with a
 * FIELD_POSITION_HIT at that position. If it was a miss, display a FIELD_POSITION_EMPTY instead, as
 * it is now known that there was no boat there. The FieldState struct also contains data on how
 * many lives each ship has. Each hit only reports if it was a hit on any boat or if a specific boat
 * was sunk, this function also clears a boats lives if it detects that the hit was a
 * HIT_SUNK_*_BOAT.
 * @param f The field to grab data from.
 * @param gData The coordinates that were guessed along with their HitStatus.
 * @return The previous value of that coordinate position in the field before the hit/miss was
 * registered.
 */
FieldPosition FieldUpdateKnowledge(Field *f, const GuessData *gData) {
    FieldPosition old = f->field[gData->row][gData->col];
    if (gData->hit == HIT_HIT) {
        f->field[gData->row][gData->col] = FIELD_POSITION_HIT;
    } else if (gData->hit == HIT_MISS) {
        f->field[gData->row][gData->col] = FIELD_POSITION_MISS;
    }
    return old;
}

/**************************************************************************************************\
 * FieldGetBoatStates()                                                                           *
\**************************************************************************************************/

/**
 * This function returns the alive states of all 4 boats as a 4-bit bitfield (stored as a uint8).
 * The boats are ordered from smallest to largest starting at the least-significant bit. So that:
 * 0b00001010 indicates that the small boat and large boat are sunk, while the medium and huge boat
 * are still alive. See the BoatStatus enum for the bit arrangement.
 * @param f The field to grab data from.
 * @return A 4-bit value with each bit corresponding to whether each ship is alive or not.
 */
uint8_t FieldGetBoatStates(const Field *f) {
    uint8_t states = 0, small = 0x01, medium = 0x02, large = 0x04, huge = 0x08;
    if (f->smallBoatLives) {
        states |= small;
    }
    if (f->mediumBoatLives) {
        states |= medium;
    }
    if (f->largeBoatLives) {
        states |= large;
    }
    if (f->hugeBoatLives) {
        states |= huge;
    }
    return states;
}