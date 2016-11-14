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

struct PLAYER {
    uint8_t inventory[INVENTORY_SIZE];
    bool added;
} player;

/**
 * Adds the specified item to the player's inventory if the inventory isn't full.
 * @param item The item number to be stored: valid values are 0-255.
 * @return SUCCESS if the item was added, STANDARD_ERRROR if the item couldn't be added.
 */
int AddToInventory(uint8_t item) {
    player.added = false;
    if (!(player.inventory[0])) {
        player.inventory[0] = item;
        player.added = true;
    } else if (!(player.inventory[1]) &&
            player.inventory[0] != item) {
        player.inventory[1] = item;
        player.added = true;
    } else if (!(player.inventory[2]) &&
            player.inventory[1] != item &&
            player.inventory[0] != item) {
        player.inventory[2] = item;
        player.added = true;
    } else if (!(player.inventory[3]) &&
            player.inventory[2] != item &&
            player.inventory[1] != item &&
            player.inventory[0] != item) {
        player.inventory[3] = item;
        player.added = true;
    }
    if (player.added == true) {
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

/**
 * Check if the given item exists in the player's inventory.
 * @param item The number of the item to be searched for: valid values are 0-255.
 * @return SUCCESS if it was found or STANDARD_ERROR if it wasn't.
 */
int FindInInventory(uint8_t item) {
    if (player.inventory[0] == item ||
            player.inventory[1] == item ||
            player.inventory[2] == item ||
            player.inventory[3] == item) {
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}