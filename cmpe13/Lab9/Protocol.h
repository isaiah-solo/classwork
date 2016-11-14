#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>

// The length of the largest possible payload (data between the '$' and '*') supported by Protocol.
#define PROTOCOL_MAX_PAYLOAD_LEN 32

// The length of the largest possible message. It's '$' + payload + '*' + 2 checksum bytes + newline
// + null character. This is useful for declaring static buffers for storing messages, as a protocol
// message is guaranteed not to overflow a buffer if it is at least this big.
#define PROTOCOL_MAX_MESSAGE_LEN (1 + PROTOCOL_MAX_PAYLOAD_LEN + 1 + 2 + 1 + 1)

/**
 * These are the values returned by UnpackageData() depending on the type of message is processed.
 */
typedef enum {
    PROTOCOL_PARSING_FAILURE = -1, // Parsing failed for some reason. Could signify and unknown
                                   // message was received or the checksum was incorrect.
    PROTOCOL_WAITING,              // Parsing is waiting for the starting '$' of a new message
    PROTOCOL_PARSING_GOOD,         // A success value that indicates no message received yet.
    
    PROTOCOL_PARSED_COO_MESSAGE,   // Coordinate message. This is used for exchanging guesses.
    PROTOCOL_PARSED_HIT_MESSAGE,   // Hit message. Indicates a response to a Coordinate message.
    PROTOCOL_PARSED_CHA_MESSAGE,   // Challenge message. Used in the first step of negotiating the
                                   // turn order.
    PROTOCOL_PARSED_DET_MESSAGE    // Determine message. Used in the second and final step of
                                   // negotiating the turn order.
} ProtocolParserStatus;

/**
 * Used by the HIT message to respond with what happened after that play. Also used in the GuessData
 * struct's hit member.
 */
typedef enum {
    HIT_MISS, // Indicates that this coordinate was a guess that missed.
    HIT_HIT, // Indicates that a boat was hit at this coordinate.
    HIT_SUNK_SMALL_BOAT, // Indicates that the SMALL_BOAT was sunk as a result of this hit.
    HIT_SUNK_MEDIUM_BOAT, // Indicates that the MEDIUM_BOAT was sunk as a result of this hit.
    HIT_SUNK_LARGE_BOAT, // Indicates that the LARGE_BOAT was sunk as a result of this hit.
    HIT_SUNK_HUGE_BOAT, // Indicates that the HUGE_BOAT was sunk as a result of this hit.
} HitStatus;

/**
 * Used to indicate which agent should have first-turn precedence.
 */
typedef enum {
    TURN_ORDER_TIE = -1,
    TURN_ORDER_DEFER = 0,
    TURN_ORDER_START = 1
} TurnOrder;

/**
 * NegotiationData stores all of the data required for negotiating the turn order.
 */
typedef struct {
    uint32_t guess;
    uint32_t encryptionKey;
    uint32_t encryptedGuess;
    uint32_t hash;
} NegotiationData;

/**
 * GuessData is used for exchanging coordinate data along with the HitStatus of that coordinate.
 */
typedef struct {
    uint32_t row; // Row of the coordinate that was guessed
    uint32_t col; // Column of the coordinate guessed
    uint32_t hit; // Status of this coordinate. Uses HitStatus enum constants.
} GuessData;

// Defined below are the various messages used by the protocol. Each follows the NMEA0183 syntax for
// messages, with the exclusion of the Talked ID portion.
#define PAYLOAD_TEMPLATE_HIT "HIT,%u,%u,%u" // Hit message: row, col, hit (see HitStatus)
#define PAYLOAD_TEMPLATE_COO "COO,%u,%u"    // Coordinate message: row, col
#define PAYLOAD_TEMPLATE_CHA "CHA,%u,%u"    // Challenge message: encryptedGuess, hash
#define PAYLOAD_TEMPLATE_DET "DET,%u,%u"    // Determine message: guess, encryptionKey

/* This constant defines the wrapper used for messages encoded using this protocol.
 * Note that it uses printf-style tokens so that it can be used with sprintf() with two arguments:
 * the message and its checksum.
 *   $ <- dollar sign
 *   ID <- a 3-5 character message ID
 *   , <- Separates the message ID from the data that follows it
 *   DATA <- comma separated ASCII-encoded numeric data
 *   * <- data sequence terminator
 *   CHECKSUM <- a two-digit ASCII checksum that's the XOR of all of the bytes between the $ and *
 *   \n <- The message is ended with a standard newline character
 */
#define MESSAGE_TEMPLATE "$%s*%02x\n"

/**
 * Encodes the coordinate data for a guess into the string `message`. This string must be big
 * enough to contain all of the necessary data. The format is specified in COO_PAYLOAD_TEMPLATE,
 * which is then wrapped within the message as defined by MESSAGE_TEMPLATE. The final length of this
 * message is then returned. There is no failure mode for this function as there is no checking
 * for NULL pointers.
 * @param message The character array used for storing the output. Must be long enough to store the
 *                entire string, see PROTOCOL_MAX_MESSAGE_LEN.
 * @param data The data struct that holds the data to be encoded into `message`.
 * @return The length of the string stored into `message`.
 */
int ProtocolEncodeCooMessage(char *message, const GuessData *data);

/**
 * Follows from ProtocolEncodeCooMessage above.
 */
int ProtocolEncodeHitMessage(char *message, const GuessData *data);

/**
 * Follows from ProtocolEncodeCooMessage above.
 */
int ProtocolEncodeChaMessage(char *message, const NegotiationData *data);

/**
 * Follows from ProtocolEncodeCooMessage above.
 */
int ProtocolEncodeDetMessage(char *message, const NegotiationData *data);

/**
 * This function decodes a message into either the NegotiationData or GuessData structs depending
 * on what the type of message is. This function receives the message one byte at a time, where the
 * messages are in the format defined by MESSAGE_TEMPLATE, with payloads of the format defined by
 * the PAYLOAD_TEMPLATE_* macros. It returns the type of message that was decoded and also places
 * the decoded data into either the `nData` or `gData` structs depending on what the message held.
 * The onus is on the calling function to make sure the appropriate structs are available (blame the
 * lack of function overloading in C for this ugliness).
 *
 * PROTOCOL_PARSING_FAILURE is returned if there was an error of any kind (though this excludes
 * checking for NULL pointers), while
 * 
 * @param parser The struct holding the data for this instance of the parser. Allows for multiple
 *               protocol decoding streams.
 * @param in The next character in the NMEA0183 message to be decoded.
 * @param nData A struct used for storing data if a message is decoded that stores NegotiationData.
 * @param gData A struct used for storing data if a message is decoded that stores GuessData.
 * @return A value from the UnpackageDataEnum enum.
 */
ProtocolParserStatus ProtocolDecode(char in, NegotiationData *nData, GuessData *gData);

/**
 * This function generates all of the data necessary for the negotiation process used to determine
 * the player that goes first. It relies on the pseudo-random functionality built into the standard
 * library. The output is stored in the passed NegotiationData struct. The negotiation data is
 * generated by creating two random 16-bit numbers, one for the actual guess and another for an
 * encryptionKey used for encrypting the data. The 'encryptedGuess' is generated with an
 * XOR(guess, encryptionKey). The hash is simply an 8-bit value that is the XOR() of all of the
 * bytes making up both the guess and the encryptionKey. There is no checking for NULL pointers
 * within this function.
 * @param data The struct used for both input and output of negotiation data.
 */ 
void ProtocolGenerateNegotiationData(NegotiationData *data);

/**
 * Validates that the negotiation data within 'data' is correct according to the algorithm given in
 * GenerateNegotitateData(). Used for verifying another agent's supplied negotiation data. There is
 * no checking for NULL pointers within this function. Returns true if the NegotiationData struct
 * is valid or false on failure.
 * @param data A filled NegotiationData struct that will be validated.
 * @return true if the NegotiationData struct is consistent and false otherwise.
 */
bool ProtocolValidateNegotiationData(const NegotiationData *data);

/**
 * This function returns a TurnOrder enum type representing which agent has won precedence for going
 * first. The value returned relates to the agent whose data is in the 'myData' variable. The turn
 * ordering algorithm relies on the XOR() of the 'encryptionKey' used by both agents. The least-
 * significant bit of XOR(myData.encryptionKey, oppData.encryptionKey) is checked so that if it's a
 * 1 the player with the largest 'guess' goes first otherwise if it's a 0, the agent with the
 * smallest 'guess' goes first. The return value of TURN_ORDER_START indicates that 'myData' won,
 * TURN_ORDER_DEFER indicates that 'oppData' won, otherwise a tie is indicated with TURN_ORDER_TIE.
 * There is no checking for NULL pointers within this function.
 * @param myData The negotiation data representing the current agent.
 * @param myData The negotiation data representing the opposing agent.
 * @return A value from the TurnOrdering enum representing which agent should go first.
 */
TurnOrder ProtocolGetTurnOrder(const NegotiationData *myData, const NegotiationData *oppData);

#endif // PROTOCOL_H
