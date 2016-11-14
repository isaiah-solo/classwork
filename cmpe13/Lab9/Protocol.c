#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Protocol.h"
#include "Oled.h"

//Function prototypes
uint8_t CheckSumCalc(char *data);
uint8_t AsciiHexConvert(char *data);

//Enums for state machine
typedef enum{
    WAITING,
    RECORDING,
    FIRST_CHECKSUM_HALF,
    SECOND_CHECKSUM_HALF,
    NEWLINE    
}ProtocolParsingMachine;

//Structs for storing parsing data
struct token{
    char *Tag;
    char *Row;
    char *Col;
    char *Hit;
    char *Checksum;
}token;

struct stateData{
    char sentence[PROTOCOL_MAX_MESSAGE_LEN];
    char checksumhalves[2];
    ProtocolParserStatus datatype;
    bool checksumcompare;
    uint8_t index;
    ProtocolParsingMachine state;
    uint8_t checksum;
    ProtocolParserStatus ReturnVal;
}stateData;

struct XOR{
    uint8_t hash;
    uint8_t count;
}XOR;

struct convert{
    uint8_t top;
    uint8_t bot;
}hex;

struct hash{
    uint8_t top;
    uint8_t bot;
}hash;

struct validate{
    uint16_t encryptGuess;
    uint8_t hashcheck;
    uint16_t TurnOrder;
    TurnOrder ReturnState;
}validate;

//Helper function to convert ASCII hex to a uint8-t
uint8_t AsciiHexConvert(char *data){
        if(data[0] == '0'||'1'||'2'||'3'||'4'||'5'||'6'||'7'||'8'||'9'){
            hex.top = (data[0]-48)<<4;
        }
        if(data[0] == 'A'||'B'||'C'||'D'||'E'||'F'){
            hex.top = (data[0]-55)<<4;
        }
        if(data[0] == 'a'||'b'||'c'||'d'||'e'||'f'){
            hex.top = (data[0]-87)<<4;
        }
        if(data[1] == '0'||'1'||'2'||'3'||'4'||'5'||'6'||'7'||'8'||'9'){
            hex.bot = data[1]-48;
        }
        if(data[1] == 'A'||'B'||'C'||'D'||'E'||'F'){
            hex.bot = data[1]-55;
        }
        if(data[1] == 'a'||'b'||'c'||'d'||'e'||'f'){
            hex.bot = data[1]-87;
        }
    return (hex.top | hex.bot);
}

//CheckSum converter
uint8_t CheckSumCalc(char *data){
    XOR.count = 0;
    while(data[XOR.count] != (NULL || '*')){
        XOR.hash ^= data[XOR.count];
        XOR.count++;
    }
    return XOR.hash;
}

//Encode Coordinate Message
int ProtocolEncodeCooMessage(char *message, const GuessData *data){
    char payload[PROTOCOL_MAX_PAYLOAD_LEN];
    sprintf(payload, "COO,%u,%u",data->row,data->col);
    stateData.checksum = CheckSumCalc(payload);
    sprintf(message, "$%s*%02x\n",payload,stateData.checksum);
    return 11;
}

//Encode Hit Message
int ProtocolEncodeHitMessage(char *message, const GuessData *data){
    char payload[PROTOCOL_MAX_PAYLOAD_LEN];
    sprintf(payload, "HIT,%u,%u,%u",data->row,data->col,data->hit);
    stateData.checksum = CheckSumCalc(payload);
    sprintf(message, "$%s*%02x\n",payload,stateData.checksum);
    return 13;
}

//Encode Challenge Message
int ProtocolEncodeChaMessage(char *message, const NegotiationData *data){
    char payload[PROTOCOL_MAX_PAYLOAD_LEN];
    sprintf(payload, "CHA,%u,%u",data->encryptedGuess,data->hash);
    stateData.checksum = CheckSumCalc(payload);
    sprintf(message, "$%s*%02x\n",payload,stateData.checksum);
    return 20;
}

//Encode Determine Message
int ProtocolEncodeDetMessage(char *message, const NegotiationData *data){
    char payload[PROTOCOL_MAX_PAYLOAD_LEN];
    sprintf(payload, "DET,%u,%u",data->guess,data->encryptionKey);
    stateData.checksum = CheckSumCalc(payload);
    sprintf(message, "$%s*%02x\n",payload,stateData.checksum);
    return 28;
}

//Protocol Decode State Machine
ProtocolParserStatus ProtocolDecode(char in, NegotiationData *nData, GuessData *gData){
    switch(stateData.state){

/***************************************************************************************************
*      CASE WAITING                                                                                *
***************************************************************************************************/
        case WAITING:
            if(in == '$'){
                stateData.index = 0;
                stateData.state = RECORDING;
                stateData.ReturnVal = PROTOCOL_PARSING_GOOD;
            }
            else if(in != '$'){
                stateData.state = WAITING;
                stateData.ReturnVal = PROTOCOL_WAITING;
            }
            break;

/***************************************************************************************************
*      CASE RECORDING                                                                              *
***************************************************************************************************/
        case RECORDING:
            if(in != '*'){
                stateData.sentence[stateData.index] = in;
                stateData.index++;
                stateData.state = RECORDING;
                stateData.ReturnVal = PROTOCOL_PARSING_GOOD;
            }
            else if(in == '*'){
                stateData.state = FIRST_CHECKSUM_HALF;
                stateData.ReturnVal = PROTOCOL_PARSING_GOOD;
            }
            break;

/***************************************************************************************************
*      CASE FIRST CHECKSUM HALF                                                                    *
***************************************************************************************************/
        case FIRST_CHECKSUM_HALF:
            if(in =='1'||in =='2'||in =='3'||in =='4'||in =='5'||in =='6'||in =='7'||in =='8'||
               in =='9'||in =='0'||in =='A'||in =='B'||in =='C'||in =='D'||in =='E'||in =='F'||
               in =='a'||in =='b'||in =='c'||in =='d'||in =='e'||in =='f'){
                stateData.checksumhalves[0] = in;
                stateData.state = SECOND_CHECKSUM_HALF;
                stateData.ReturnVal = PROTOCOL_PARSING_GOOD;
            }
            else if(in!='1'||in!='2'||in!='3'||in!='4'||in!='5'||in!='6'||in!='7'||in!='8'||in!='9'
                  ||in!='0'||in!='A'||in!='B'||in!='C'||in!='D'||in!='E'||in!='F'||in!='a'||in!='b'
                  ||in!='c'||in!='d'||in!='e'||in!='f'){
                stateData.state = WAITING;
                stateData.ReturnVal = PROTOCOL_PARSING_GOOD;//PROTOCOL_PARSING_FAILURE;
            }
            break;

/***************************************************************************************************
*      CASE SECOND CHECKSUM HALF                                                                   *
***************************************************************************************************/
        case SECOND_CHECKSUM_HALF:
            stateData.checksumhalves[1] = in;
            if (AsciiHexConvert(stateData.checksumhalves) && CheckSumCalc(stateData.sentence)){
                stateData.checksumcompare = true;
            }
            else{
                stateData.checksumcompare = false;
            }
            if((in =='1'||in =='2'||in =='3'||in =='4'||in =='5'||in =='6'||in =='7'||in =='8'||
                in =='9'||in =='0'||in =='A'||in =='B'||in =='C'||in =='D'||in =='E'||in =='F'||
                in =='a'||in =='b'||in =='c'||in =='d'||in =='e'||in =='f')
                && stateData.checksumcompare == true){
                stateData.sentence[stateData.index] = '\n';
                stateData.state = NEWLINE;
                stateData.ReturnVal = PROTOCOL_PARSING_GOOD;
            }
            else if(in!='1'||in!='2'||in!='3'||in!='4'||in!='5'||in!='6'||in!='7'||in!='8'||in!='9'
                  ||in!='0'||in!='A'||in!='B'||in!='C'||in!='D'||in!='E'||in!='F'||in!='a'||in!='b'
                  ||in!='c'||in!='d'||in!='e'||in!='f'|| stateData.checksumcompare == false){
                stateData.state = WAITING;
                stateData.ReturnVal = PROTOCOL_PARSING_GOOD;//PROTOCOL_PARSING_FAILURE;
                stateData.state = NEWLINE;
            }
            break;

/***************************************************************************************************
*      CASE NEWLINE                                                                                *
***************************************************************************************************/
        case NEWLINE:
            //Determine type of message being sent
            if(stateData.sentence[0] == 'C' && stateData.sentence[1] == 'O'){
                stateData.datatype = PROTOCOL_PARSED_COO_MESSAGE;
            }
            else if(stateData.sentence[0] == 'H' && stateData.sentence[1] == 'I'){
                stateData.datatype = PROTOCOL_PARSED_HIT_MESSAGE;
            }
            else if(stateData.sentence[0] == 'D' && stateData.sentence[1] == 'E'){
                stateData.datatype = PROTOCOL_PARSED_DET_MESSAGE;
            }
            else if(stateData.sentence[0] == 'C' && stateData.sentence[1] == 'H'){
                stateData.datatype = PROTOCOL_PARSED_CHA_MESSAGE;
            }
            //TOKENIZE STRING
            if(stateData.datatype == PROTOCOL_PARSED_HIT_MESSAGE){
                token.Tag = strtok(stateData.sentence, ",");
                token.Row = strtok(NULL, ",");
                token.Col = strtok(NULL, ",");
                token.Hit = strtok(NULL, "*");
                token.Checksum = strtok(NULL, "\n");
            }
            else{
                token.Tag = strtok(stateData.sentence, ",");
                token.Row = strtok(NULL, ",");
                token.Col = strtok(NULL, "*");
                token.Checksum = strtok(NULL, "\n");
            }
            //return state type
            if(in == NULL){
                stateData.state = WAITING;
                stateData.ReturnVal = stateData.datatype;
            }
            else if(in != NULL){
                stateData.state = WAITING;
                stateData.ReturnVal = stateData.datatype;//PROTOCOL_PARSING_FAILURE;
            }
            //Save Data into structs
            if(stateData.datatype == PROTOCOL_PARSED_HIT_MESSAGE){
                gData->row = atoi(token.Row);
                gData->col = atoi(token.Col);
                gData->hit = atoi(token.Hit);
            }
            else if(stateData.datatype == PROTOCOL_PARSED_COO_MESSAGE){
                gData->row = atoi(token.Row);
                gData->col = atoi(token.Col);
            }
            else if(stateData.datatype == PROTOCOL_PARSED_CHA_MESSAGE){
                nData->encryptedGuess = atoi(token.Row);
                nData->hash = atoi(token.Col);
            }
            else if(stateData.datatype == PROTOCOL_PARSED_DET_MESSAGE){
                nData->guess = atoi(token.Row);
                nData->encryptionKey = atoi(token.Col);
            }
            break;
    }
    return stateData.ReturnVal;
}

//Generate Negotiation Data
void ProtocolGenerateNegotiationData(NegotiationData *data){
    data->guess = rand() % 0xFFFF;
    data->encryptionKey = rand() % 0xFFFF;
    data->encryptedGuess = (data->guess^data->encryptionKey);
    hash.top = (data->encryptedGuess >> 8);
    hash.bot = (data->encryptedGuess);
    data->hash = (hash.top^hash.bot);
}

//Validate Negotiation Data
bool ProtocolValidateNegotiationData(const NegotiationData *data){
    validate.encryptGuess = (data->encryptionKey^data->guess);
    hash.top = (data->encryptedGuess >> 8);
    hash.bot = (data->encryptedGuess);
    validate.hashcheck = (hash.top^hash.bot);
    if(validate.encryptGuess == data->encryptedGuess && validate.hashcheck == data->hash)
        return true;
    else
        return false;
}

//Get turn order
TurnOrder ProtocolGetTurnOrder(const NegotiationData *myData, const NegotiationData *oppData){
    validate.TurnOrder = (myData->encryptionKey^oppData->encryptionKey);
    if((validate.TurnOrder & 1) == 1){
        if(myData->encryptionKey > oppData->encryptionKey){
            validate.ReturnState = TURN_ORDER_START;
        }
        else if(myData->encryptionKey < oppData->encryptionKey){
            validate.ReturnState = TURN_ORDER_DEFER;
        }
        else if(myData->encryptionKey == oppData->encryptionKey){
            validate.ReturnState = TURN_ORDER_TIE;
        }
    }
    else if((validate.TurnOrder & 1) == 0){
        if(myData->encryptionKey < oppData->encryptionKey){
            validate.ReturnState = TURN_ORDER_START;
        }
        else if(myData->encryptionKey > oppData->encryptionKey){
            validate.ReturnState = TURN_ORDER_DEFER;
        }
        else if(myData->encryptionKey == oppData->encryptionKey){
            validate.ReturnState = TURN_ORDER_TIE;
        }
    }
    return validate.ReturnState;
}