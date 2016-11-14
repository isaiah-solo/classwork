#ifndef FIELD_OLED_H
#define FIELD_OLED_H

#include "Field.h"

/**
 * Define a tri-state variable for indicating which agent's turn it is. Before and after the game,
 * the turn should always be `*_NONE`. This means that until turn negotiation is completed by an agent,
 * it should use `*_NONE`. Once the game is completed because the agent knows someone has one, it should
 * once again be specified as `*_NONE`.
 */
typedef enum {
    FIELD_OLED_TURN_NONE,
    FIELD_OLED_TURN_MINE,
    FIELD_OLED_TURN_THEIRS
} FieldOlenTurn;

/**
 * Draw both player's fields to the screen, along with a current turn indicator.
 * @param myField The field representing this agent's field.
 * @param theirField The field representing the enemy agent's field.
 * @param playerTurn Which agent currently has the turn.
 */
void FieldOledDrawScreen(const Field *myField, const Field *theirField,
    FieldOlenTurn playerTurn);

#endif // FIELD_OLED_H
