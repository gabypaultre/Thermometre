/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      GPIO.h
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

#ifndef GPIO_H_
#define GPIO_H_

/**********************************************************************************************************************/
/* INCLUDE FILES                                                                                                      */
/**********************************************************************************************************************/
#include "Common.h"
#include "ISR.h"

/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

typedef enum
{
    GPIO_OK = 0,
    GPIO_NOK
} GPIO_status;

typedef bool (*GPIO_callback)(ISR_tenuPeripheral peripheralId);

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/

GPIO_status GPIO_init(void);

GPIO_status GPIO_setGpioHigh(void);

GPIO_status GPIO_setGpioLow(void);

GPIO_status GPIO_toggleGpio(void);

GPIO_status GPIO_registerCallback(GPIO_callback callback);

/*--------------------------------------------------------------------------------------------------------------------*/
#endif /* GPIO_H_ */
/*--------------------------------------------------------------------------------------------------------------------*/
