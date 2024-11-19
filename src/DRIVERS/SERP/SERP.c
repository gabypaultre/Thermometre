/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      SERP.c
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

/**********************************************************************************************************************/
/* INCLUDE FILES                                                                                                      */
/**********************************************************************************************************************/

#include "SERP.h"
#include "EUSART.h"

/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE VARIABLES                                                                                                  */
/**********************************************************************************************************************/

static bool SERP_bIsInitialized = false;

/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/

static void SERP_vidTimerCallback(void);

/**********************************************************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

static void SERP_vidTimerCallback(void)
{
    (void)SERP_enuSendMessage(SERP_MSG_ID_LIVE_SIGN, NULL, 0);
}

/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/

void SERP_vidInitialize(void)
{
    EUSART_vidInitialize();

    TIM0_vidInitialize();

    (void)TIM0_bRegisterTimerCbk(SERP_vidTimerCallback);
    (void)TIM0_bStart(TIM0_eFOSC_DIV4, TIM0_ePRESCALER_64, TIM0_ePOSTSCALER_1, 250);

    SERP_bIsInitialized = true;
}

SERP_tenuStatus SERP_enuSendMessage(SERP_tenuMsgId enuMsgId, const uint8_t *pu8Data, uint16_t u16DataSize)
{
    uint8_t au8EncodedMessage[SERP_MAX_MSG_DATA_SIZE + 10]; // Buffer pour le message encodé
    uint16_t u16EncodedIndex = 0;

    // Vérifications de base
    if (!SERP_bIsInitialized) return SERP_STATUS_NOK; // Driver non initialisé
    if (u16DataSize > SERP_MAX_MSG_DATA_SIZE) return SERP_STATUS_ENCODING_ERROR; // Taille des données invalide

    // Ajout du START_BYTE
    au8EncodedMessage[u16EncodedIndex++] = SERP_START_BYTE;

    // Ajout du MSG_ID
    au8EncodedMessage[u16EncodedIndex++] = (uint8_t)enuMsgId;

    // Ajout de la longueur des données (MSG_LENGTH) en LSB first
    au8EncodedMessage[u16EncodedIndex++] = (uint8_t)(u16DataSize & 0xFF); // LSB
    au8EncodedMessage[u16EncodedIndex++] = (uint8_t)((u16DataSize >> 8) & 0xFF); // MSB

    // Encodage des données (avec gestion des octets problématiques)
    for (uint16_t i = 0; i < u16DataSize; i++)
    {
        if (pu8Data[i] == SERP_START_BYTE || pu8Data[i] == SERP_STOP_BYTE || pu8Data[i] == SERP_ESCAPE_BYTE)
        {
            au8EncodedMessage[u16EncodedIndex++] = SERP_ESCAPE_BYTE; // Ajout de l'échappement
        }
        au8EncodedMessage[u16EncodedIndex++] = pu8Data[i];
    }

    // Ajout du STOP_BYTE
    au8EncodedMessage[u16EncodedIndex++] = SERP_STOP_BYTE;

    // Envoi du message via le EUSART
    for (uint16_t i = 0; i < u16EncodedIndex; i++)
    {
        // Utilisation de la fonction EUSART_vidSendChar avec un timeout
        if (EUSART_vidSendChar((char)au8EncodedMessage[i], 100) != EUSART_eSTATUS_OK)
        {
            return SERP_STATUS_NOK; // Retourner une erreur si l'envoi échoue
        }
    }

    return SERP_STATUS_OK;
}

/*--------------------------------------------------------------------------------------------------------------------*/