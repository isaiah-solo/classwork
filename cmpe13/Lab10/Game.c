// Standard libraries
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

// User libraries
#include "Adc.h"
#include "Ascii.h"
#include "Buttons.h"
#include "Common.h"
#include "Game.h"
#include "HardwareDefs.h"
#include "Leds.h"
#include "Oled.h"
#include "OledDriver.h"
#include "Player.h"

struct ROOM {
    char title[GAME_MAX_ROOM_TITLE_LENGTH + 1];
    char description[GAME_MAX_ROOM_DESC_LENGTH + 1];
    uint8_t title_length;
    uint8_t description_length;
    int current;
    uint8_t exits;
    uint8_t north;
    uint8_t east;
    uint8_t south;
    uint8_t west;
} room;

static FILE *file;
static char buffer[11];
static uint8_t required_item;
static uint8_t contained_item;
static uint8_t required;
static uint8_t contained;
static uint8_t check;
static uint8_t counter;
static bool version;

/**
 * These function transitions between rooms. Each call should return SUCCESS if the current room has
 * an exit in the correct direction and the new room was able to be loaded, and STANDARD_ERROR
 * otherwise.
 * @return SUCCESS if the room CAN be navigated to and changing the current room to that new room
 *         succeeded.
 */
int GameGoNorth(void) {
    if (room.exits & GAME_ROOM_EXIT_NORTH_EXISTS) {
        fclose(file);
        room.current = room.north;
        sprintf(buffer, "/room%d.txt", room.current);
        fopen(buffer, "rb");
        if (file == NULL) {
            FATAL_ERROR();
        }

        /***Title**********************************************************************************/
        room.title_length = fgetc(file);
        fread(room.title, sizeof (uint8_t), room.title_length, file);
        room.title[room.title_length] = '\0';

        /***Description****************************************************************************/
        version = false;
        while (version == false) {
            check = 0;
            // Required Items
            required = fgetc(file);
            if (required > 0) {
                counter = required;
                while (counter > 0) {
                    required_item = fgetc(file);
                    if (FindInInventory(required_item) == SUCCESS) {
                        check++;
                    }
                    counter--;
                }
            }
            // Decription
            room.description_length = fgetc(file);
            fread(room.description, sizeof (uint8_t), room.description_length, file);
            room.description[room.description_length] = '\0';
            // Contained Items
            contained = fgetc(file);
            counter = contained;
            while (counter > 0) {
                contained_item = fgetc(file);
                AddToInventory(contained_item);
                counter--;
            }
            // Check If Correct Version
            if (check == required || required == 0) {
                version = true;
            } else {
                fseek(file, 4, SEEK_CUR);
            }
        }

        /***Exits**********************************************************************************/
        room.north = fgetc(file);
        room.east = fgetc(file);
        room.south = fgetc(file);
        room.west = fgetc(file);
        return SUCCESS;
    }
    return STANDARD_ERROR;
}

/**
 * @see GameGoNorth
 */
int GameGoEast(void) {
    if (room.exits & GAME_ROOM_EXIT_EAST_EXISTS) {
        fclose(file);
        room.current = room.east;
        sprintf(buffer, "/room%d.txt", room.current);
        file = fopen(buffer, "rb");
        if (file == NULL) {
            FATAL_ERROR();
        }

        /***Title**********************************************************************************/
        room.title_length = fgetc(file);
        fread(room.title, sizeof (uint8_t), room.title_length, file);
        room.title[room.title_length] = '\0';

        /***Description****************************************************************************/
        version = false;
        while (version == false) {
            check = 0;
            // Required Items
            required = fgetc(file);
            if (required > 0) {
                counter = required;
                while (counter > 0) {
                    required_item = fgetc(file);
                    if (FindInInventory(required_item) == SUCCESS) {
                        check++;
                    }
                    counter--;
                }
            }
            // Decription
            room.description_length = fgetc(file);
            fread(room.description, sizeof (uint8_t), room.description_length, file);
            room.description[room.description_length] = '\0';
            // Contained Items
            contained = fgetc(file);
            counter = contained;
            while (counter > 0) {
                contained_item = fgetc(file);
                AddToInventory(contained_item);
                counter--;
            }
            // Check If Correct Version
            if (check == required || required == 0) {
                version = true;
            } else {
                fseek(file, 4, SEEK_CUR);
            }
        }

        /***Exits**********************************************************************************/
        room.north = fgetc(file);
        room.east = fgetc(file);
        room.south = fgetc(file);
        room.west = fgetc(file);
        return SUCCESS;
    }
    return STANDARD_ERROR;
}

/**
 * @see GameGoNorth
 */
int GameGoSouth(void) {
    if (room.exits & GAME_ROOM_EXIT_SOUTH_EXISTS) {
        fclose(file);
        room.current = room.south;
        sprintf(buffer, "/room%d.txt", room.current);
        file = fopen(buffer, "rb");
        if (file == NULL) {
            FATAL_ERROR();
        }

        /***Title**********************************************************************************/
        room.title_length = fgetc(file);
        fread(room.title, sizeof (uint8_t), room.title_length, file);
        room.title[room.title_length] = '\0';

        /***Description****************************************************************************/
        version = false;
        while (version == false) {
            check = 0;
            // Required Items
            required = fgetc(file);
            if (required > 0) {
                counter = required;
                while (counter > 0) {
                    required_item = fgetc(file);
                    if (FindInInventory(required_item) == SUCCESS) {
                        check++;
                    }
                    counter--;
                }
            }
            // Decription
            room.description_length = fgetc(file);
            fread(room.description, sizeof (uint8_t), room.description_length, file);
            room.description[room.description_length] = '\0';
            // Contained Items
            contained = fgetc(file);
            counter = contained;
            while (counter > 0) {
                contained_item = fgetc(file);
                AddToInventory(contained_item);
                counter--;
            }
            // Check If Correct Version
            if (check == required || required == 0) {
                version = true;
            } else {
                fseek(file, 4, SEEK_CUR);
            }
        }

        /***Exits**********************************************************************************/
        room.north = fgetc(file);
        room.east = fgetc(file);
        room.south = fgetc(file);
        room.west = fgetc(file);
        return SUCCESS;
    }
    return STANDARD_ERROR;
}

/**
 * @see GameGoNorth
 */
int GameGoWest(void) {
    if (room.exits & GAME_ROOM_EXIT_WEST_EXISTS) {
        fclose(file);
        room.current = room.south;
        sprintf(buffer, "/room%d.txt", room.current);
        file = fopen(buffer, "rb");
        if (file == NULL) {
            FATAL_ERROR();
        }

        /***Title**********************************************************************************/
        room.title_length = fgetc(file);
        fread(room.title, sizeof (uint8_t), room.title_length, file);
        room.title[room.title_length] = '\0';

        /***Description****************************************************************************/
        version = false;
        while (version == false) {
            check = 0;
            // Required Items
            required = fgetc(file);
            if (required > 0) {
                counter = required;
                while (counter > 0) {
                    required_item = fgetc(file);
                    if (FindInInventory(required_item) == SUCCESS) {
                        check++;
                    }
                    counter--;
                }
            }
            // Decription
            room.description_length = fgetc(file);
            fread(room.description, sizeof (uint8_t), room.description_length, file);
            room.description[room.description_length] = '\0';
            // Contained Items
            contained = fgetc(file);
            counter = contained;
            while (counter > 0) {
                contained_item = fgetc(file);
                AddToInventory(contained_item);
                counter--;
            }
            // Check If Correct Version
            if (check == required || required == 0) {
                version = true;
            } else {
                fseek(file, 4, SEEK_CUR);
            }
        }

        /***Exits**********************************************************************************/
        room.north = fgetc(file);
        room.east = fgetc(file);
        room.south = fgetc(file);
        room.west = fgetc(file);
        return SUCCESS;
    }
    return STANDARD_ERROR;
}

/**
 * This function sets up anything that needs to happen at the start of the game. This is just
 * setting the current room to STARTING_ROOM and loading it. It should return SUCCESS if it succeeds
 * and STANDARD_ERROR if it doesn't.
 * @return SUCCESS or STANDARD_ERROR
 */
int GameInit(void) {
    room.current = STARTING_ROOM;
    sprintf(buffer, "/room%d.txt", room.current);
    file = fopen(buffer, "rb");
    if (file == NULL) {
        FATAL_ERROR();
    }

    /***Title**********************************************************************************/
    room.title_length = fgetc(file);
    fread(room.title, sizeof (uint8_t), room.title_length, file);
    room.title[room.title_length] = '\0';

    /***Description****************************************************************************/
    version = false;
    while (version == false) {
        check = 0;
        // Required Items
        required = fgetc(file);
        if (required > 0) {
            counter = required;
            while (counter > 0) {
                required_item = fgetc(file);
                if (FindInInventory(required_item) == SUCCESS) {
                    check++;
                }
                counter--;
            }
        }
        // Decription
        room.description_length = fgetc(file);
        fread(room.description, sizeof (uint8_t), room.description_length, file);
        room.description[room.description_length] = '\0';
        // Contained Items
        contained = fgetc(file);
        counter = contained;
        while (counter > 0) {
            contained_item = fgetc(file);
            AddToInventory(contained_item);
            counter--;
        }
        // Check If Correct Version
        if (check == required || required == 0) {
            version = true;
        } else {
            fseek(file, 4, SEEK_CUR);
        }
    }

    /***Exits**********************************************************************************/
    room.north = fgetc(file);
    room.east = fgetc(file);
    room.south = fgetc(file);
    room.west = fgetc(file);
    return SUCCESS;
}

/**
 * Copies the appropriate room title as a NULL-terminated string into the provided character array.
 * The appropriate room means the first room description that the Player has the required items to
 * see. Only a NULL-character is copied if there was an error so that the resultant output string
 * length is 0.
 * @param desc A character array to copy the room title into. Should be GAME_MAX_ROOM_TITLE_LENGTH+1
 *             in length in order to allow for all possible titles to be copied into it.
 * @return The length of the string stored into `title`. Note that the actual number of chars
 *         written into `title` will be this value + 1 to account for the NULL terminating
 *         character.
 */
int GameGetCurrentRoomTitle(char *title) {
    strcpy(title, room.title);
    return room.title_length;
}

/**
 * GetCurrentRoomDescription() copies the description of the current room into the argument desc as
 * a C-style string with a NULL-terminating character. The room description is guaranteed to be less
 * -than-or-equal to GAME_MAX_ROOM_DESC_LENGTH characters, so the provided argument must be at least
 * GAME_MAX_ROOM_DESC_LENGTH + 1 characters long. Only a NULL-character is copied if there was an
 * error so that the resultant output string length is 0.
 * @param desc A character array to copy the room description into.
 * @return The length of the string stored into `desc`. Note that the actual number of chars
 *          written into `desc` will be this value + 1 to account for the NULL terminating
 *          character.
 */
int GameGetCurrentRoomDescription(char *desc) {
    strcpy(desc, room.description);
    return room.description_length;
}

/**
 * This function returns the exits from the current room in the lowest-four bits of the returned
 * uint8 in the order of NORTH, EAST, SOUTH, and WEST such that NORTH is in the MSB and WEST is in
 * the LSB. A bit value of 1 corresponds to there being a valid exit in that direction and a bit
 * value of 0 corresponds to there being no exit in that direction. The GameRoomExitFlags enum
 * provides bit-flags for checking the return value.
 *
 * @see GameRoomExitFlags
 *
 * @return a 4-bit bitfield signifying which exits are available to this room.
 */
uint8_t GameGetCurrentRoomExits(void) {
    room.exits = 0;
    if (room.north != 0) {
        room.exits |= 0b1000;
    }
    if (room.east != 0) {
        room.exits |= 0b0100;
    }
    if (room.south != 0) {
        room.exits |= 0b0010;
    }
    if (room.west != 0) {
        room.exits |= 0b0001;
    }
    return room.exits;
}