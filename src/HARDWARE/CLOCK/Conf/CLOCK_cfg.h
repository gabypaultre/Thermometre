/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      CLOCK_cfg.h
 *
 * @author    Jean DEBAINS
 * @date      Wednesday, January 31, 2024.
 *
 * @version   0.0.0
 *
 * @brief     CLOCK Hardware conf part
 * @details   Module in charge of the management of the system clock
 *
 * @remark    Coding Language: C
 *
 * @copyright Copyright (c) 2024 This software is used for education proposal
 *
 ***********************************************************************************************************************
 */
#ifndef CLOCK_CFG_H_
#define CLOCK_CFG_H_


/**********************************************************************************************************************/
/* INCLUDE FILES                                                                                                      */
/**********************************************************************************************************************/
#include "Common.h"


/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/
#define CLOCK_FOSC_FREQUENCY_01MHZ                          1   //!< @attention Don't change this macro definition >
#define CLOCK_FOSC_FREQUENCY_02MHZ                          2   //!< @attention Don't change this macro definition >
#define CLOCK_FOSC_FREQUENCY_04MHZ                          4   //!< @attention Don't change this macro definition >
#define CLOCK_FOSC_FREQUENCY_08MHZ                          8   //!< @attention Don't change this macro definition >
#define CLOCK_FOSC_FREQUENCY_16MHZ                          16  //!< @attention Don't change this macro definition >
#define CLOCK_FOSC_FREQUENCY_32MHZ                          32  //!< @attention Don't change this macro definition >
#define CLOCK_FOSC_FREQUENCY_64MHZ                          64  //!< @attention Don't change this macro definition >


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief This macro is used to set the clock frequency (Fosc)
 * @details Use the allowed frequencies "CLOCK_FOSC_FREQUENCY_xxMHZ" defined above
 */
#define CLOCK_CONFIG_FOSC_FREQUENCY_MHZ                     CLOCK_FOSC_FREQUENCY_32MHZ


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Set the frequency of the internal oscillator (HFINTOSC) which can have either 1 MHz or 64 MHz
 */
#define CLOCK_CONFIG_HFINTOSC_MHZ                           64


/*--------------------------------------------------------------------------------------------------------------------*/
#endif /* CLOCK_CFG_H_ */
/*--------------------------------------------------------------------------------------------------------------------*/
