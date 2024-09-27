/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      INTERRUPT.h
 *
 * @author    Jean DEBAINS
 * @date      Wednesday, January 31, 2024.
 *
 * @version   0.0.0
 *
 * @brief     INTERRUPT Hardware core part
 * @details   Module in charge of the management of the interruptions
 *
 * @remark    Coding Language: C
 *
 * @copyright Copyright (c) 2024 This software is used for education proposal
 *
 ***********************************************************************************************************************
 */
#ifndef ISR_H_
#define ISR_H_


/**********************************************************************************************************************/
/* INCLUDE FILES                                                                                                      */
/**********************************************************************************************************************/
#include "Common.h"


/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/
/**
 * @brief Enable the global interrupt (see "PIC18F47Q10 - Datasheet", P.197, 14.13.1)
 */
#define ISR_GlobalInterruptEnable()                   (INTCONbits.GIE = 1)


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Disable the global interrupt (see "PIC18F47Q10 - Datasheet", P.197, 14.13.1)
 */
#define ISR_GlobalInterruptDisable()                  (INTCONbits.GIE = 0)


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Enable the peripheral interrupt (see "PIC18F47Q10 - Datasheet", P.197, 14.13.1)
 */
#define ISR_PeripheralInterruptEnable()               (INTCONbits.PEIE = 1)


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Disable the peripheral interrupt (see "PIC18F47Q10 - Datasheet", P.197, 14.13.1)
 */
#define ISR_PeripheralInterruptDisable()              (INTCONbits.PEIE = 0)


/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
/**
 * @brief Enumeration used to list the different peripheral whose an interruption has to be handled
 * @details If a peripheral is added in the project, the enumeration below shall be added in this list
 * @remark The order of the enumeration peripheral defines also the "priority" of the ISR's peripheral
 */
typedef enum ISR_tenuPeripheral
{
  /*-----[ DO NOT EDIT THIS ]----*/
  ISR_ePERIPHERAL_BEGIN = -1, /*-*/
  /*-----------------------------*/

  ISR_ePERIPHERAL_TIMER,
  ISR_ePERIPHERAL_EUSART,
  ISR_ePERIPHERAL_INPUT_GPIO,

  /*-----[ DO NOT EDIT THIS ]----*/
  ISR_ePERIPHERAL_END /*---------*/
  /*-----------------------------*/
}ISR_tenuPeripheral;


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Callback type definition to call the pointed function of the peripheral which could have an interruption to be
 *        caught
 * @return Returns "true" if the ISR flag was found, returns "false" otherwise
 */
typedef bool (*ISR_tpfbCallback)(void);


/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
/**
 * @brief Function used to register the callback function accordingly of ID of the peripheral
 * @param[in] kenuPeripheralId: The peripheral ID
 * @param[in] kpfbCallback    : The pointer to the function to be called when an interruption is triggered
 * @return Return "true" if the function ran successfully, return "false" otherwise
 */
bool ISR_bRegisterIsrCbk(const ISR_tenuPeripheral kenuPeripheralId, const ISR_tpfbCallback kpfbCallback);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to unregister the callback function accordingly of ID of the peripheral
 * @param[in] kenuPeripheralId: The peripheral ID
 * @return Return "true" if the function ran successfully, return "false" otherwise
 */
bool ISR_bUnregisterIsrCbk(const ISR_tenuPeripheral kenuPeripheralId);


/*--------------------------------------------------------------------------------------------------------------------*/
#endif /* ISR_H_ */
/*--------------------------------------------------------------------------------------------------------------------*/
