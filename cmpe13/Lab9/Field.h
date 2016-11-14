#ifndef FIELD_H
#define FIELD_H

#include <stdint.h>
#include <stdbool.h>

#include "Protocol.h"

/**
 * Define the dimensions of the game field. They can be overridden by compile-time specifications.
 * All references to the dimensions of the field should use these constants instead of hard-coding
 * a numeric value so that the field dimensions can be changed with minimal coding changes.
 */
#ifndef FIELD_COLS
#define FIELD_COLS 10
#endif
#ifndef FIELD_ROWS
#define FIELD_ROWS 6
#endif

/**
 * Set different constants used for conveying different information about the different locations
 * of the field. These values should be used for the actual storage of the field state, which is
 * almost every usage. For displaying the field using FieldPrint(), the FieldPositionDisplay enum
 * values should be used instead.
 */
typedef enum {
    /// These denote field positions useful for representing the local board
    FIELD_POSITION_EMPTY = 0,   // An empty field position.
    FIELD_POSITION_SMALL_BOAT,   // This position contains part of the small boat.
    FIELD_POSITION_MEDIUM_BOAT,  // This position contains part of the medium boat.
    FIELD_POSITION_LARGE_BOAT,   // This position contains part of the large boat.
    FIELD_POSITION_HUGE_BOAT,   // This position contains part of the huge boat.
    FIELD_POSITION_MISS,    // This position was attacked by the enemy, but was empty.

    /// These denote field positions useful for representing the enemy's board
    FIELD_POSITION_UNKNOWN, // It is unknown what is here. Useful for denoting a position on the
                            // enemy's board that hasn't been checked.
    FIELD_POSITION_HIT,     // A field position that was attacked and contained part of a boat.
    FIELD_POSITION_CURSOR   // This is used merely for use in FieldOled.c for indicating the current
                            // cursor when selecting a position to attack.
} FieldPosition;

/**
 * A struct for tracking all of the necessary data for an agent's field.
 */
typedef struct {
    FieldPosition field[FIELD_ROWS][FIELD_COLS];
    uint8_t smallBoatLives;
    uint8_t mediumBoatLives;
    uint8_t largeBoatLives;
    uint8_t hugeBoatLives;
} Field;

/**
 * Specify how many boats there exist on the field. There is 1 boat of each of the 4 types, so 4
 * total.
 */
#define FIELD_NUM_BOATS 4

/**
 * Declares direction constants for use with FieldAddShip. They're set to nice ASCII values so that
 * user input can be directly read and used with FieldAddShip.
 */
typedef enum {
    FIELD_BOAT_DIRECTION_NORTH,
    FIELD_BOAT_DIRECTION_EAST,
    FIELD_BOAT_DIRECTION_SOUTH,
    FIELD_BOAT_DIRECTION_WEST
} BoatDirection;

/**
 * Constants for specifying which boat the current operation refers to. This is independent of the
 * FieldPosition enum.
 */
typedef enum {
    FIELD_BOAT_SMALL,
    FIELD_BOAT_MEDIUM,
    FIELD_BOAT_LARGE,
    FIELD_BOAT_HUGE
} BoatType;

/**
 * Track the alive state of the boats. They are arranged as as mutually-exclusive bits so that they
 * can be ORed together. Used for checking the return value of  `FieldGetBoatStates()`.
 */
typedef enum {
    FIELD_BOAT_STATUS_SMALL  = 0x01,
    FIELD_BOAT_STATUS_MEDIUM = 0x02,
    FIELD_BOAT_STATUS_LARGE  = 0x04,
    FIELD_BOAT_STATUS_HUGE   = 0x08,
} BoatStatus;

/**
 * This enum lists the number of lives, and therefore the number of squares, each boat has/occupies.
 */
typedef enum {
    FIELD_BOAT_LIVES_SMALL  = 3,
    FIELD_BOAT_LIVES_MEDIUM = 4,
    FIELD_BOAT_LIVES_LARGE  = 5,
    FIELD_BOAT_LIVES_HUGE   = 6
} BoatLives;

/**
 * FieldInit() will fill the passed field array with the data specified in positionData. Also the
 * lives for each boat are filled according to the `BoatLives` enum.
 * @param f The field to initialize.
 * @param p The data to initialize the entire field to, should be a member of enum
 *                     FieldPosition.
 */
void FieldInit(Field *f, FieldPosition p);

/**
 * Retrieves the value at the specified field position.
 * @param f The Field being referenced
 * @param row The row-component of the location to retrieve
 * @param col The column-component of the location to retrieve
 * @return
 */
FieldPosition FieldAt(const Field *f, uint8_t row, uint8_t col);

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
FieldPosition FieldSetLocation(Field *f, uint8_t row, uint8_t col, FieldPosition p);

/**
 * FieldAddBoat() places a single ship on the player's field based on arguments 2-5. Arguments 2, 3
 * represent the x, y coordinates of the pivot point of the ship.  Argument 4 represents the
 * direction of the ship, and argument 5 is the length of the ship being placed. All spaces that
 * the boat would occupy are checked to be clear before the field is modified so that if the boat
 * can fit in the desired position, the field is modified as SUCCESS is returned. Otherwise the
 * field is unmodified and STANDARD_ERROR is returned. There is no hard-coded limit to how many
 * times a boat can be added to a field within this function.
 *
 * So this is valid test code:
 * {
 *   Field myField;
 *   FieldInit(&myField);
 *   FieldAddBoat(&myField, 0, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_SMALL);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_MEDIUM);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_HUGE);
 *   FieldAddBoat(&myField, 0, 6, FIELD_BOAT_DIRECTION_SOUTH, FIELD_BOAT_SMALL);
 * }
 *
 * should result in a field like:
 *  _ _ _ _ _ _ _ _
 * [ 3 3 3       3 ]
 * [ 4 4 4 4     3 ]
 * [             3 ]
 *  . . . . . . . .
 *
 * @param f The field to grab data from.
 * @param row The row that the boat will start from, valid range is from 0 and to FIELD_ROWS - 1.
 * @param col The column that the boat will start from, valid range is from 0 and to FIELD_COLS - 1.
 * @param dir The direction that the boat will face once places, from the BoatDirection enum.
 * @param boatType The type of boat to place. Relies on the FIELD_POSITION_*_BOAT values from the
 * FieldPosition enum.
 * @return true for success, false for failure
 */
bool FieldAddBoat(Field *f, uint8_t row, uint8_t col, BoatDirection dir, BoatType type);

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
FieldPosition FieldRegisterEnemyAttack(Field *f, GuessData *gData);

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
FieldPosition FieldUpdateKnowledge(Field *f, const GuessData *gData);

/**
 * This function returns the alive states of all 4 boats as a 4-bit bitfield (stored as a uint8).
 * The boats are ordered from smallest to largest starting at the least-significant bit. So that:
 * 0b00001010 indicates that the small boat and large boat are sunk, while the medium and huge boat
 * are still alive. See the BoatStatus enum for the bit arrangement.
 * @param f The field to grab data from.
 * @return A 4-bit value with each bit corresponding to whether each ship is alive or not.
 */
uint8_t FieldGetBoatStates(const Field *f);

#endif // FIELD_H
