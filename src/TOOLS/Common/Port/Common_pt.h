/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      common_pt.h
 *
 * @author    Jean DEBAINS
 * @date      14/06/2023
 *
 * @version   0.0.0
 *
 * @brief     Common port file the MCU PIC18F47Q10
 *
 * @remark    Coding Language: C
 *
 * @copyright Copyright (c) 2024 This software is used for education proposal
 *
 ***********************************************************************************************************************
 */
#ifndef COMMON_PORT_H_
#define COMMON_PORT_H_


/**********************************************************************************************************************/
/* INCLUDE FILES                                                                                                      */
/**********************************************************************************************************************/
#include <stdint.h>
#include <xc.h>


/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/
/**
 * @brief Port break point macro
 */
#define CMN_vidPortBreakPoint                               __debug_break


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Port printf redirection macro
 */
#define CMN_portPrintf(...)                                 printf(__VA_ARGS__)


/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
/**
 * @brief Function in charge of the initialization of the error LED
 * @remark This function shall not be used directly
 */
void CMN_vidErrorLedInit(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to set the error LED On
 * @remark This function shall not be used directly
 */
void CMN_vidErrorLedSet(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to set the error LED Off
 * @remark This function shall not be used directly
 */
void CMN_vidErrorLedUnset(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to set a delay in millisecond
 * @param[in] ku32DelayMs: The delay to be applied in millisecond
 * @remark This function shall not be used directly
 */
void CMN_vidPortDelayMs(const uint32_t ku32DelayMs);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to enable the interruption of the MCU
 * @remark This function shall not be used directly
 */
void CMN_vidPortEnableIsr(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to disable the interruption of the MCU
 * @remark This function shall not be used directly
 */
void CMN_vidPortDisableIsr(void);


/*--------------------------------------------------------------------------------------------------------------------*/
#endif // COMMON_PORT_H_
/*--------------------------------------------------------------------------------------------------------------------*/
