/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      ADC.h
 *
 * @author    Jean DEBAINS
 * @date      Wednesday, January 31, 2024.
 *
 * @version   0.0.0
 *
 * @brief     ADC Hardware core part
 * @details   Module in charge of the management of the ADC peripheral
 *
 * @remark    Coding Language: C
 *
 * @copyright Copyright (c) 2024 This software is used for education proposal
 *
 ***********************************************************************************************************************
 */
#ifndef ADC_H_
#define ADC_H_


/**********************************************************************************************************************/
/* INCLUDE FILES                                                                                                      */
/**********************************************************************************************************************/
#include "Common.h"
#include "ADC_cfg.h"


/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/
#define ADC_TIMEOUT_RAW_VALUE                               0xffff


/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
/**
 * @enum ADC_tenuStatus
 * @brief Enumeration used to list all of the allowed status of the ADC module (return codes)
 * @details These returns code are returned by the public functions of this module
 */
typedef enum ADC_tenuStatus
{
  ADC_eSTATUS_OK                                            = 0,  //!< Everything is OK
  ADC_eSTATUS_NO_OK,                                              //!< Generic/default error code
  ADC_eSTATUS_NULL_POINTER,                                       //!< The passed pointer is NULL
  ADC_eSTATUS_CONVERSION_TIMEOUT,                                 //!< The ADC conversion has timed out
  ADC_eSTATUS_COUNT                                               //!< The total number of return code
}ADC_tenuStatus;


/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
/**
 * @brief Function used to initialize the hardware GPIO module
 * @attention This function must be called before the start of the main software
 */
void ADC_vidInitialize(void);


/*--------------------------------------------------------------------------------------------------------------------*/
ADC_tenuStatus ADC_enuGetRawValue(uint16_t * const kpu16AdcRawValue, const uint32_t ku32TimeoutMs);


/*--------------------------------------------------------------------------------------------------------------------*/
#endif /* ADC_H_ */
/*--------------------------------------------------------------------------------------------------------------------*/
