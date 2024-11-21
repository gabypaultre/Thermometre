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
#include "Common.h"

/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

typedef enum
{
    SERP_STATE_IDLE = 0,        // En attente d'un octet de début
    SERP_STATE_WAIT_DATA,       // En réception des données
    SERP_STATE_WAIT_ESCAPE      // Gestion des octets spéciaux
} SERP_tenuRxState;


/**********************************************************************************************************************/
/* PRIVATE VARIABLES                                                                                                  */
/**********************************************************************************************************************/

static bool SERP_bIsInitialized = false;

static SERP_tenuRxState SERP_enuRxState = SERP_STATE_IDLE;  // État actuel
static uint8_t SERP_au8RxBuffer[SERP_MAX_MSG_DATA_SIZE];    // Buffer pour les données reçues
static uint16_t SERP_u16RxIndex = 0;                       // Index dans le buffer
static uint16_t SERP_u16MsgLength = 0;                     // Taille attendue des données
static SERP_tenuMsgId SERP_enuCurrentMsgId;                // ID du message courant

static SERP_tpfAppManagerCallback SERP_pfAppManagerCallback = NULL; // Pointeur vers le callback AppManager .


/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/

static void SERP_vidTimerCallback(void);

static void SERP_vidRxCallback(char const * const kpkau8Data, // Callback for EUSART RX
                               const uint16_t ku16DataLength,
                               const EUSART_tenuStatus kenuStatus);

static void SERP_treatReceivedMessage(void);


/**********************************************************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

static void SERP_vidTimerCallback(void)
{
    (void)SERP_enuSendMessage(SERP_MSG_ID_LIVE_SIGN, NULL, 0);
}


static void SERP_vidRxCallback(char const * const kpkau8Data,
                               const uint16_t ku16DataLength,
                               const EUSART_tenuStatus kenuStatus)
{
    if (kenuStatus != EUSART_eSTATUS_OK)
    {
        CMN_systemPrintf("Error: RX Callback received error status\r\n");
        return;
    }

    for (uint16_t i = 0; i < ku16DataLength; i++)
    {
        uint8_t u8ReceivedByte = (uint8_t)kpkau8Data[i];

        switch (SERP_enuRxState)
        {
            case SERP_STATE_IDLE:
                if (u8ReceivedByte == SERP_START_BYTE)
                {
                    SERP_enuRxState = SERP_STATE_WAIT_DATA;
                    SERP_u16RxIndex = 0;
                }
                break;

            case SERP_STATE_WAIT_DATA:
                if (u8ReceivedByte == SERP_STOP_BYTE)
                {
                    // Message terminé : traiter le message
                    SERP_enuRxState = SERP_STATE_IDLE;

                    // Décodage du message
                    SERP_treatReceivedMessage();
                }
                else if (u8ReceivedByte == SERP_ESCAPE_BYTE)
                {
                    SERP_enuRxState = SERP_STATE_WAIT_ESCAPE;
                }
                else
                {
                    // Ajouter l'octet au buffer
                    if (SERP_u16RxIndex < SERP_MAX_MSG_DATA_SIZE)
                    {
                        SERP_au8RxBuffer[SERP_u16RxIndex++] = u8ReceivedByte;
                    }
                    else
                    {
                        CMN_systemPrintf("Error: RX buffer overflow\r\n");
                        SERP_enuRxState = SERP_STATE_IDLE;
                    }
                }
                break;

            case SERP_STATE_WAIT_ESCAPE:
                // Ajouter l'octet échappé au buffer
                if (SERP_u16RxIndex < SERP_MAX_MSG_DATA_SIZE)
                {
                    SERP_au8RxBuffer[SERP_u16RxIndex++] = u8ReceivedByte;
                    SERP_enuRxState = SERP_STATE_WAIT_DATA;
                }
                else
                {
                    CMN_systemPrintf("Error: RX buffer overflow\r\n");
                    SERP_enuRxState = SERP_STATE_IDLE;
                }
                break;

            default:
                SERP_enuRxState = SERP_STATE_IDLE;
                break;
        }
    }
}
                        

static void SERP_treatReceivedMessage(void)
{
    if (SERP_u16RxIndex < 3) // MSG_ID + MSG_LENGTH (2 octets minimum)
    {
        CMN_systemPrintf("Error: Message too short\r\n");
        return;
    }

    SERP_enuCurrentMsgId = (SERP_tenuMsgId)SERP_au8RxBuffer[0];
    SERP_u16MsgLength = (uint16_t)(SERP_au8RxBuffer[1] | (SERP_au8RxBuffer[2] << 8));

    if (SERP_u16MsgLength != (SERP_u16RxIndex - 3))
    {
        CMN_systemPrintf("Error: Length mismatch\r\n");
        return;
    }

    CMN_systemPrintf("Message received: ID=%d, Length=%d\r\n",
                     SERP_enuCurrentMsgId, SERP_u16MsgLength);

    // Si un callback est enregistré, envoyer les données
    if (SERP_pfAppManagerCallback != NULL)
    {
        SERP_pfAppManagerCallback(SERP_enuCurrentMsgId, &SERP_au8RxBuffer[3], SERP_u16MsgLength);
    }
}
               

/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/

void SERP_vidInitialize(void)
{
    // Nettoyage du buffer RX pour éviter les données résiduelles
    if (RC2STAbits.OERR)
    {
        RC2STAbits.CREN = 0; // Désactiver la réception continue
        RC2STAbits.CREN = 1; // Réactiver la réception continue
    }

    EUSART_vidInitialize();

    EUSART_tenuStatus eusartStatus = EUSART_enuRegisterRxCbk(SERP_vidRxCallback);
    if (eusartStatus != EUSART_eSTATUS_OK)
    {
        CMN_systemPrintf("Error: Failed to register EUSART RX callback\r\n");
        return;
    }

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


SERP_tenuStatus SERP_enuRegisterAppManagerCallback(SERP_tpfAppManagerCallback callback)
{
    if (callback == NULL)
    {
        return SERP_STATUS_NOK; // Échec si le callback est NULL
    }

    SERP_pfAppManagerCallback = callback; // Enregistrer le callback
    return SERP_STATUS_OK;
}


/*--------------------------------------------------------------------------------------------------------------------*/