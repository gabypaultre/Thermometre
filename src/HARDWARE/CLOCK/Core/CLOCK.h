/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      CLOCK.h
 *
 * @author    Jean DEBAINS
 * @date      Wednesday, January 31, 2024.
 *
 * @version   0.0.0
 *
 * @brief     CLOCK Hardware core part
 * @details   Module in charge of the management of the system clock
 *
 * @remark    This file contains only the Core part of the module, all configurations are defined in the CLOCK_cfg.h
 *
 * @remark    Coding Language: C
 *
 * @copyright Copyright (c) 2024 This software is used for education proposal
 *
 ***********************************************************************************************************************
 */
#ifndef CLOCK_H_
#define CLOCK_H_


/**********************************************************************************************************************/
/* INCLUDE FILES                                                                                                      */
/**********************************************************************************************************************/
#include "CLOCK_cfg.h"


/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/
/**
 * @brief This normalized macro allows the use of the standard delay functions accordingly to the choosen Fosc
 * @attention Do not edit this macro
 */
#define _XTAL_FREQ                                          (CLOCK_CONFIG_FOSC_FREQUENCY_MHZ * 1000000UL)


/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
/**
 * @brief This function is used to initialize the clock of the microcontroler
 * @details The clock if configured accordingly to the configurations of the internal oscillator (HFINTOSC) with the settings
 *          "CLOCK_CONFIG_HFINTOSC_MHZ" and "CLOCK_CONFIG_FOSC_FREQUENCY_MHZ", the output frequency is Fosc and is finally defined
 *          by CLOCK_CONFIG_HFINTOSC_MHZ/CLOCK_CONFIG_FOSC_FREQUENCY_MHZ
 */
void CLOCK_vidInitialize(void);


/*--------------------------------------------------------------------------------------------------------------------*/
#endif /* CLOCK_H_ */
/*--------------------------------------------------------------------------------------------------------------------*/
