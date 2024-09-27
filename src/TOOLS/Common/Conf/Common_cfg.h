/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      Common_cfg.h
 *
 * @author    Jean DEBAINS
 * @date      14/06/2023
 *
 * @version   0.0.0
 *
 * @brief     Common file for common features
 *
 * @remark    Coding Language: C
 *
 * @copyright Copyright (c) 2024 This software is used for education proposal
 *
 ***********************************************************************************************************************
 */
#ifndef COMMON_CFG_H_
#define COMMON_CFG_H_


/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/
/**
 * @brief Macro used to enable or disable the debug mode in the project
 * @details Enabling the debug mode allows to
 *            - Replace an assertion by a software breakpoint
 */
#define CMN_ENABLE_DEBUG_MODE                               false


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Macro used to define whether the use of FreeRTOS is enabled or not
 */
#define CMN_CONFIG_USE_FREERTOS                             false


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Macro used to define whether the use of printf is enabled or not
 */
#define CMN_ENABLE_PRINTF                                   true


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Macro used to define whether the use of a LED error is enabled or not
 */
#define CMN_ENABLE_ERROR_LED                                true


/*--------------------------------------------------------------------------------------------------------------------*/
#endif // COMMON_CFG_H_
/*--------------------------------------------------------------------------------------------------------------------*/
