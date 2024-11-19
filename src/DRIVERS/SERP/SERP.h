/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      SERP.h
 *
 * @author    gpaultre
 * @date      14/11/2024
 *
 * @version   0.0.0
 *
 * @brief     <you can add a description of your module here, otherwise please delete this line>
 * @details   <you can add more details of your module here, otherwise please delete this line>
 *
 * @remark    Coding Language: C
 *
 * @copyright Copyright (c) 2024 This software is used for education proposal
 *
 ***********************************************************************************************************************
 */

#ifndef SERP_H_
#define SERP_H_

/**********************************************************************************************************************/
/* INCLUDE FILES                                                                                                      */
/**********************************************************************************************************************/

#include "EUSART.h"
#include "TIMER.h"
#include "Common.h"

/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/

#define SERP_START_BYTE 0x6F
#define SERP_STOP_BYTE 0x65
#define SERP_ESCAPE_BYTE 0x64
#define SERP_MAX_MSG_DATA_SIZE 50

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

typedef enum SERP_tenuMsgId
{
    SERP_MSG_ID_START_MEASURE = 17,
    SERP_MSG_ID_STOP_MEASURE = 18,
    SERP_MSG_ID_TEMPERATURE = 18,
    SERP_MSG_ID_LIVE_SIGN = 19,
    SERP_MSG_ID_CUSTOM = 20
} SERP_tenuMsgId;

typedef enum SERP_tenuStatus
{
    SERP_STATUS_OK = 0,
    SERP_STATUS_NOK,
    SERP_STATUS_NULL_POINTER,
    SERP_STATUS_INVALID_MSG_ID,
    SERP_STATUS_ENCODING_ERROR
} SERP_tenuStatus;

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/

void SERP_vidInitialize(void);

SERP_tenuStatus SERP_enuSendMessage(SERP_tenuMsgId enuMsgId, const uint8_t *pu8Data, uint16_t u16DataSize);

/**********************************************************************************************************************/
#endif /* SERP_H_ */
/**********************************************************************************************************************/
