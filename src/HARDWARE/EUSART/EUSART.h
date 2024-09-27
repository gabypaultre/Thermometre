/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      EUSART.h
 *
 * @author    Jean DEBAINS
 * @date      Wednesday, January 31, 2024.
 *
 * @version   0.0.0
 *
 * @brief     EUSART Hardware core part
 * @details   Module in charge of the management of the EUSART2 peripheral connected of the pins RD0 (TX) and RD1 (RX)
 *
 * @remark    Coding Language: C
 *
 * @copyright Copyright (c) 2024 This software is used for education proposal
 *
 ***********************************************************************************************************************
 */
#ifndef EUSART_H_
#define EUSART_H_


/**********************************************************************************************************************/
/* INCLUDE FILES                                                                                                      */
/**********************************************************************************************************************/
#include "Common.h"


/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
/**
 * @brief Enum to set the list all error codes.
 */
typedef enum EUSART_tenuStatus
{
  EUSART_eSTATUS_OK                                         = 0,  //!< Everything is OK
  EUSART_eSTATUS_NO_OK,                                           //!< Generic/default error code
  EUSART_eSTATUS_NULL_POINTER,                                    //!< A passed pointer as an argument is NULL
  EUSART_eSTATUS_TIMEOUT,                                         //!< The time to consider a send/receive has reached the end
  EUSART_eSTATUS_RW_OVERRUN,                                      //!< The FIFO in charge of the RX data storage is full (see "PIC18F47Q10 - Datasheet", P.504 - 28.1.2.5)
  EUSART_eSTATUS_FRAMING_ERROR,                                   //!< The "stop" bit to terminate a RX reception was not reveiced in time (see "PIC18F47Q10 - Datasheet", P.503 - 28.1.2.4)
  EUSART_eSTATUS_COUNT                                            //!< The total number of statuses available
}EUSART_tenuStatus;


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Callback type definition to notify the user when a Rx data has been received
 * @param[in] kpkau8Data    : Pointer to the buffer used to store the RX data
 * @param[in] ku16DataLenght: The number of received data
 * @param[in]     kenuStatus: The status of the EUSART (if the valud is @ref EUSART_eSTATUS_OK, then the reveicad data
 *                            are valid)
 */
typedef void (*EUSART_tpfvidRxCallback)(char const * const kpkau8Data,
                                        const uint16_t ku16DataLenght,
                                        const EUSART_tenuStatus kenuStatus);


/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
/**
 * @brief Initialization of the EUSART module
 * @details The initialization will perform the following actions:
 *              - 1) Check if the baudrate is correct
 *              - 2) Check if the frequency of Fosc is in a valid range to make this peripheral working
 *              - 3) Register the peripheral to the interruption
 *              - 4) Apply the configuration of the peripheral
 */
void EUSART_vidInitialize(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to register the Rx callback
 * @details When a Rx data is received, an interruption is generated. The pointed function is called to give the
 *          received data
 * @param[in] kpfvidCallback: The user function to be called when a Rx data is received
 * @return Return "true" if the function ran successfully, return "false" otherwise
 */
EUSART_tenuStatus EUSART_enuRegisterRxCbk(const EUSART_tpfvidRxCallback kpfvidCallback);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to unregister the Rx callback
 * @return Return "true" if the function ran successfully, return "false" otherwise
 */
EUSART_tenuStatus EUSART_enuUnRegisterRxCbk(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to send a char to the EUSART in the Tx GPIO
 * @param[in] ks8Data      : The data to be send
 * @param[in] ku32TimeoutMs: The tie to wait before considering the send as a fail
 */
EUSART_tenuStatus EUSART_vidSendChar(const char ks8Data, const uint32_t ku32TimeoutMs);


/*--------------------------------------------------------------------------------------------------------------------*/
#endif /* EUSART_H_ */
/*--------------------------------------------------------------------------------------------------------------------*/
