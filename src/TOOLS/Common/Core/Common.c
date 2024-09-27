/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      Common.c
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



/**********************************************************************************************************************/
/* INCLUDE FILES                                                                                                      */
/**********************************************************************************************************************/
#include "Common.h"


/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/
/**
 * @brief Checks the valid value for the setting @ref CMN_ENABLE_DEBUG_MODE
*/
#if(CMN_ENABLE_DEBUG_MODE == true)
#pragma message "[CMN ] Info: DebugMode -> enabled"
#elif(CMN_ENABLE_DEBUG_MODE == false)
#pragma message "[CMN ] Info: DebugMode -> disabled"
#else
#error "[CMN ] Error: Invalid value for CMN_ENABLE_DEBUG_MODE (could be either "true" or "false")
#endif //CMN_ENABLE_DEBUG_MODE


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Checks the valid value for the setting @ref CMN_CONFIG_USE_FREERTOS
*/
#if(CMN_CONFIG_USE_FREERTOS == true)
#pragma message "[CMN ] Info: FreeRTOS -> enabled"
#elif(CMN_CONFIG_USE_FREERTOS == false)
#pragma message "[CMN ] Info: FreeRTOS -> disabled"
#else
#error "[CMN ] Error: Invalid value for CMN_CONFIG_USE_FREERTOS (could be either "true" or "false")
#endif //CMN_CONFIG_USE_FREERTOS


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Checks the valid value for the setting @ref CMN_ENABLE_PRINTF
*/
#if(CMN_ENABLE_PRINTF == true)
#pragma message "[CMN ] Info: Printf -> enabled"
#elif(CMN_ENABLE_PRINTF == false)
#pragma message "[CMN ] Info: Printf -> disabled"
#else
#error "[CMN ] Error: Invalid value for CMN_ENABLE_PRINTF (could be either "true" or "false")
#endif //CMN_ENABLE_PRINTF


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Checks the valid value for the setting @ref CMN_ENABLE_PRINTF
*/
#if(CMN_ENABLE_ERROR_LED == true)
#pragma message "[CMN ] Info: ErrorLed -> enabled"
#elif(CMN_ENABLE_ERROR_LED == false)
#pragma message "[CMN ] Info: ErrorLed -> disabled"
#else
#error "[CMN ] Error: Invalid value for CMN_ENABLE_ERROR_LED (could be either "true" or "false")
#endif //CMN_ENABLE_ERROR_LED


/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* PRIVATE VARIABLES                                                                                                  */
/**********************************************************************************************************************/
#if(CMN_CONFIG_USE_FREERTOS == false)
static bool CMN_bSystemStarted                              = false;
#endif //CMN_CONFIG_USE_FREERTOS


/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS PROTOTYPES                                                                                       */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS                                                                                       */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/
void CMN_vidInitialize(void)
{
#if(CMN_ENABLE_ERROR_LED == true)
  CMN_vidErrorLedInit();
  CMN_vidErrorLedUnset();
#endif //CMN_ENABLE_ERROR_LED
}


/*--------------------------------------------------------------------------------------------------------------------*/
#if(CMN_CONFIG_USE_FREERTOS == false)
void CMN_vidEndInit(void)
{
  CMN_bSystemStarted = true;
}
#endif //CMN_CONFIG_USE_FREERTOS


/*--------------------------------------------------------------------------------------------------------------------*/
void CMN_vidManageAssert(const bool kbCondition,
                         const uint16_t ku16FileLine,
                         char const * const kpkpau8FileName,
                         char const * const kpkpau8FunctionName)
{
#if(CMN_CONFIG_USE_FREERTOS == true)
  bool bDontBlock = CMN_bIsRtosRunning();
#elif(CMN_CONFIG_USE_FREERTOS == false)
  bool bDontBlock = CMN_bSystemStarted;
#endif //CMN_CONFIG_USE_FREERTOS

  if(!(kbCondition))
  {
    if(bDontBlock)
    {
      CMN_systemPrintf("[CMN ] Warning: Assert triggered at \"%s\", line %d (function: \"%s\")\r\n",
                       kpkpau8FileName, ku16FileLine, kpkpau8FunctionName);
#if(CMN_ENABLE_ERROR_LED == true)
      CMN_vidErrorLedSet();
#endif //CMN_ENABLE_ERROR_LED
    }
    else
    {
      CMN_systemPrintf("[CMN ] Error: Assert triggered at \"%s\", line %d (function: \"%s\")\r\n",
                       kpkpau8FileName, ku16FileLine, kpkpau8FunctionName);
      CMN_disableIsr();

      while(true)
      {
#if(CMN_ENABLE_DEBUG_MODE == true)
        CMN_vidPortBreakPoint();
#elif((CMN_ENABLE_ERROR_LED == true) && (CMN_ENABLE_DEBUG_MODE == false))
        CMN_vidErrorLedSet();
        CMN_vidPortDelayMs(CMN_50_MS);
        CMN_vidErrorLedUnset();
        CMN_vidPortDelayMs(CMN_50_MS);
#endif
      }
    }
  }
}


/*--------------------------------------------------------------------------------------------------------------------*/
void CMN_vidManageAbort(const uint16_t ku16FileLine,
                        char const * const kpkpau8FileName,
                        char const * const kpkpau8FunctionName)
{
  CMN_systemPrintf("[CMN ] Error: Abort triggered at \"%s\", line %d (function: \"%s\")\r\n",
                   kpkpau8FileName, ku16FileLine, kpkpau8FunctionName);
  CMN_disableIsr();

  while(true)
  {
#if(CMN_ENABLE_DEBUG_MODE == true)
    CMN_vidPortBreakPoint();
#elif(CMN_ENABLE_DEBUG_MODE == false)
    CMN_vidErrorLedSet();
    CMN_vidPortDelayMs(CMN_50_MS);
    CMN_vidErrorLedUnset();
    CMN_vidPortDelayMs(CMN_50_MS);
#endif //CMN_ENABLE_DEBUG_MODE
  }
}


/*--------------------------------------------------------------------------------------------------------------------*/
void CMN_vidDelayMs(const uint32_t ku32DelayMs)
{
#if(CMN_CONFIG_USE_FREERTOS == true)
#error "[CMN ] Error: FreeRTOS is not still supported by this module"
#elif(CMN_CONFIG_USE_FREERTOS == false)
  CMN_vidPortDelayMs(ku32DelayMs);
#endif //CMN_CONFIG_USE_FREERTOS}
}


/*--------------------------------------------------------------------------------------------------------------------*/
