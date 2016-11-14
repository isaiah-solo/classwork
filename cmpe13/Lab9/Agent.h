#ifndef AGENT_H
#define AGENT_H

#include <stdint.h>

/**
 * Defines the various states used within the agent state machines. All states should be used
 * within a valid agent implementation. Additionally there is no need for states outside of
 * those defined here.
 */
typedef enum {
    AGENT_STATE_GENERATE_NEG_DATA,
    AGENT_STATE_SEND_CHALLENGE_DATA,
    AGENT_STATE_DETERMINE_TURN_ORDER,
    AGENT_STATE_SEND_GUESS,
    AGENT_STATE_WAIT_FOR_HIT,
    AGENT_STATE_WAIT_FOR_GUESS,
    AGENT_STATE_INVALID,
    AGENT_STATE_LOST,
    AGENT_STATE_WON
} AgentState;

/**
 * A list of events used by the agent to trigger on.
 */
typedef enum {
    AGENT_EVENT_NONE = 0x00,
    AGENT_EVENT_MESSAGE_PARSING_FAILED = 0x01,
    AGENT_EVENT_RECEIVED_COO_MESSAGE = 0x02,
    AGENT_EVENT_RECEIVED_HIT_MESSAGE = 0x04,
    AGENT_EVENT_RECEIVED_CHA_MESSAGE = 0x08,
    AGENT_EVENT_RECEIVED_DET_MESSAGE = 0x10,
} AgentEvent;


/**
 * The following constants describes strings that should be output onto the OLED
 * when errors occur:
 *   * AGENT_ERROR_STRING_NEG_DATA: Displayed when the negotiation data from the opponent didn't
 *                                     validate, as in they cheated.
 *   * AGENT_ERROR_STRING_PARSING: Displayed whenever a message fails to parse. This should happen
 *                                 very rarely, so we treat it as a game-ending fatal error.
 *   * AGENT_ERROR_STRING_ORDERING: Displayed when turn ordering results in a tie. Also a very rare
 *                                  occurance, so treated as a fatal error.
 */
#define AGENT_ERROR_STRING_NEG_DATA "Received invalid\nnegotiation data"
#define AGENT_ERROR_STRING_PARSING     "Message parsing\nfailed"
#define AGENT_ERROR_STRING_ORDERING    "Turn ordering\nfailed"

/**
 * The Init() function for an Agent sets up everything necessary for an agent before the game
 * starts. This can include things like initialization of the field, placement of the boats,
 * etc. The agent can assume that stdlib's rand() function has been seeded properly in order to
 * use it safely within.
 */
void AgentInit(void);

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
int AgentRun(char in, char *outBuffer);

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
uint8_t AgentGetStatus(void);

/**
 * This function returns the same data as `AgentCheckState()`, but for the enemy agent.
 * @return A bitfield indicating the sunk/unsunk status of each ship under the enemy agent's
 *         control.
 *
 * @see Field.h:FieldGetBoatStates()
 * @see Field.h:BoatStatus
 */
uint8_t AgentGetEnemyStatus(void);

#endif // AGENT_H
