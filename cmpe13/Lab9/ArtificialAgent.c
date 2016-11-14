// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Agent.h"
#include "Field.h"
#include "FieldOled.h"
#include "Protocol.h"

#include "Buttons.h"
#include "HardwareDefs.h"
#include "Leds.h"
#include "Oled.h"

Field User, Opp;
GuessData Guess;
uint8_t row, column, check, dir_value;
bool small_check = false, medium_check = false, large_check = false, huge_check = false, Gsuccess;
BoatDirection direction;
FieldOlenTurn turn;
char string[84];

//STATES

typedef enum {
    GENERATE_DATA,
    SEND_DATA,
    DETERMINE_ORDER,
    SEND_GUESS,
    WAIT_FOR_HIT,
    WAIT_FOR_GUESS,
    INVALID,
    WON,
    LOST
} AgentRunState;

struct AgentFSM {
    AgentRunState state;
    int ReturnVal;
    char ChaMessage[PROTOCOL_MAX_MESSAGE_LEN];
} AgentFSM;

NegotiationData *MyNData;
NegotiationData *OppNData;
GuessData *MyGData;
GuessData *OppGData;

/**************************************************************************************************\
 * AgentInit()                                                                                    *
\**************************************************************************************************/

/**
 * The Init() function for an Agent sets up everything necessary for an agent before the game
 * starts. This can include things like initialization of the field, placement of the boats,
 * etc. The agent can assume that stdlib's rand() function has been seeded properly in order to
 * use it safely within.
 */
void AgentInit(void) {
    FieldInit(&User, FIELD_POSITION_EMPTY);
    FieldInit(&Opp, FIELD_POSITION_UNKNOWN);
    while (check < 4) {
        row = rand() % 6; // Random ROW
        column = rand() % 10; // Random COLUMN
        dir_value = rand() % 4; // Random direction value
        if (dir_value == 0) {
            direction = FIELD_BOAT_DIRECTION_EAST; // Interprets into EAST
        } else if (dir_value == 1) {
            direction = FIELD_BOAT_DIRECTION_NORTH; // Interprets into NORTH
        } else if (dir_value == 2) {
            direction = FIELD_BOAT_DIRECTION_SOUTH; // Interprets into SOUTH
        } else if (dir_value == 3) {
            direction = FIELD_BOAT_DIRECTION_WEST; // Interprets into WEST
        }
        // Boat-making algorithm
        if (huge_check == false) { // If HUGE boat hasn't been made
            huge_check = FieldAddBoat(&User, row, column, direction, FIELD_BOAT_HUGE);
            if (huge_check == true) {
                check++;
            }
        } else if (large_check == false) { // If LARGE boat hasn't been made
            large_check = FieldAddBoat(&User, row, column, direction, FIELD_BOAT_LARGE);
            if (large_check == true) {
                check++;
            }
        } else if (medium_check == false) { // If MEDIUM boat hasn't been made
            medium_check = FieldAddBoat(&User, row, column, direction, FIELD_BOAT_MEDIUM);
            if (medium_check == true) {
                check++;
            }
        } else if (small_check == false) { // If SMALL boat hasn't been made
            small_check = FieldAddBoat(&User, row, column, direction, FIELD_BOAT_SMALL);
            if (small_check == true) {
                check++;
            }
        }
    }
    // Initialize LEDS
    LEDS_INIT();
    // Resets data
    check = 0;
    small_check = false;
    medium_check = false;
    large_check = false;
    huge_check = false;
}

/**************************************************************************************************\
 * AgentRun()                                                                                     *
\**************************************************************************************************/

/**
 * The Run() function for an Agent takes in a single character. It then waits until enough
 * data is read that it can decode it as a full sentence via the Protocol interface. This data
 * is processed with any output returned via 'outBuffer', which is guaranteed to be 255
 * characters in length to allow for any valid NMEA0183 messages. The return value should be
 * the number of characters stored into 'outBuffer': so a 0 is both a perfectly valid output and
 * means a successful run.
 * @param in The next character in the incoming message stream.
 * @param outBuffer A string that should be transmit to the other agent. NULL if there is no
 *                  data.
 * @return The length of the string pointed to by outBuffer (excludes \0 character).
 */
int AgentRun(char in, char *outBuffer) {
    switch (AgentFSM.state) {

/*****************************************GENERATE_DATA********************************************/
        case GENERATE_DATA:
            //Create NegotiationData
            ProtocolGenerateNegotiationData(MyNData);
            //Encode and Send Challenge Message
            AgentFSM.ReturnVal = ProtocolEncodeChaMessage(outBuffer, MyNData);
            AgentFSM.state = SEND_DATA;
            break;

/*******************************************SEND_DATA**********************************************/
        case SEND_DATA:
            //Receive CHA message
            if (in != NULL) {
                ProtocolDecode(in, OppNData, OppGData);
            }                //Send determine Data
            else {
                AgentFSM.ReturnVal = ProtocolEncodeDetMessage(outBuffer, MyNData);
                AgentFSM.state = DETERMINE_ORDER;
            }
            break;

/****************************************DETERMINE_ORDER*******************************************/
        case DETERMINE_ORDER:
            //Receive Determine Message
            if (in != NULL) {
                ProtocolDecode(in, OppNData, OppGData);
            } else {
                //Validate Opponent
                //If I win start order
                if (ProtocolValidateNegotiationData(OppNData) == true) {
                    if (ProtocolGetTurnOrder(MyNData, OppNData) == TURN_ORDER_START) {
                        turn = FIELD_OLED_TURN_MINE;
                        FieldOledDrawScreen(&User, &Opp, turn);
                        AgentFSM.state = SEND_GUESS;
                    }                        //If opponent wins start order
                    else if (ProtocolGetTurnOrder(MyNData, OppNData) == TURN_ORDER_DEFER) {
                        turn = FIELD_OLED_TURN_THEIRS;
                        FieldOledDrawScreen(&User, &Opp, turn);
                        AgentFSM.state = WAIT_FOR_GUESS;
                    }//If theres A tie
                    else if (ProtocolGetTurnOrder(MyNData, OppNData) == TURN_ORDER_TIE) {
                        OledClear(0);
                        sprintf(string, "                     "
                                "      ERROR          "
                                "   STRING ORDERING   "
                                "                     ");
                        OledDrawString(string);
                        OledUpdate();
                        AgentFSM.state = INVALID;
                    }
                }//If opponent is invalid
                else if (ProtocolValidateNegotiationData(OppNData) == false) {
                    OledClear(0);
                    sprintf(string, "                     "
                            "      ERROR          "
                            "      NEG DATA       "
                            "                     ");
                    OledDrawString(string);
                    OledUpdate();
                    AgentFSM.state = INVALID;
                }
            }
            break;

/******************************************SEND_GUESS**********************************************/
        case SEND_GUESS:
            //Generate Valid Coordinates
            Gsuccess = false;
            while (Gsuccess == false) {
                MyGData->row = rand() % 6;
                MyGData->col = rand() % 10;
                if (User.field[MyGData->row][MyGData->col] == FIELD_POSITION_UNKNOWN) {
                    Gsuccess = true;
                }
            }
            //Send COO Message
            AgentFSM.ReturnVal = ProtocolEncodeCooMessage(outBuffer, MyGData);
            AgentFSM.state = WAIT_FOR_HIT;
            break;

/******************************************WAIT_FOR_HIT********************************************/
        case WAIT_FOR_HIT:
            //Record HIT message && enemy boats left
            if (in != NULL) {
                ProtocolDecode(in, OppNData, OppGData);
            } else {
                if (FieldGetBoatStates(&Opp)) {
                    FieldUpdateKnowledge(&Opp, MyGData);
                    turn = FIELD_OLED_TURN_THEIRS;
                    FieldOledDrawScreen(&User, &Opp, turn);
                    AgentFSM.state = WAIT_FOR_GUESS;
                } else if (!(FieldGetBoatStates(&Opp))) {
                    turn = FIELD_OLED_TURN_NONE;
                    FieldOledDrawScreen(&User, &Opp, turn);
                    AgentFSM.state = WON;
                }
            }
            break;

/****************************************WAIT_FOR_GUESS********************************************/
        case WAIT_FOR_GUESS:
            //Record COO Message
            if (in != NULL) {
                ProtocolDecode(in, OppNData, OppGData);
            } else {
                if (!(FieldGetBoatStates(&User))) {
                    turn = FIELD_OLED_TURN_NONE;
                    AgentFSM.ReturnVal = ProtocolEncodeHitMessage(outBuffer, MyGData);
                    FieldOledDrawScreen(&User, &Opp, turn);
                    AgentFSM.state = LOST;
                } else if (FieldGetBoatStates(&User)) {
                    turn = FIELD_OLED_TURN_THEIRS;
                    AgentFSM.ReturnVal = ProtocolEncodeHitMessage(outBuffer, MyGData);
                    FieldOledDrawScreen(&User, &Opp, turn);
                    AgentFSM.state = SEND_GUESS;
                }
            }
            break;

/*******************************************STATE_WON**********************************************/
        case WON:
            AgentFSM.ReturnVal = 0;
            break;

/*******************************************STATE_LOST*********************************************/
        case LOST:
            AgentFSM.ReturnVal = 0;
            break;

/********************************************INVALID***********************************************/
        case INVALID:
            AgentFSM.ReturnVal = 0;
            break;
    }
    return AgentFSM.ReturnVal;
}

/**************************************************************************************************\
 * AgentGetStatus()                                                                               *
\**************************************************************************************************/

/**
 * StateCheck() returns a 4-bit number indicating the status of that agent's ships. The smallest
 * ship, the 3-length one, is indicated by the 0th bit, the medium-length ship (4 tiles) is the
 * 1st bit, etc. until the 3rd bit is the biggest (6-tile) ship. This function is used within
 * main() to update the LEDs displaying each agents' ship status. This function is similar to
 * Field::FieldGetBoatStates().
 * @return A bitfield indicating the sunk/unsunk status of each ship under this agent's control.
 *
 * @see Field.h:FieldGetBoatStates()
 * @see Field.h:BoatStatus
 */
uint8_t AgentGetStatus(void) {
    return FieldGetBoatStates(&User);
}

/**************************************************************************************************\
 * AgentGetEnemyStatus()                                                                          *
\**************************************************************************************************/

/**
 * This function returns the same data as `AgentCheckState()`, but for the enemy agent.
 * @return A bitfield indicating the sunk/unsunk status of each ship under the enemy agent's
 *         control.
 *
 * @see Field.h:FieldGetBoatStates()
 * @see Field.h:BoatStatus
 */
uint8_t AgentGetEnemyStatus(void) {
    return FieldGetBoatStates(&Opp);
}