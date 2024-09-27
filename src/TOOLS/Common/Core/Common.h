/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      Common.h
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
#ifndef COMMON_H_
#define COMMON_H_


/**********************************************************************************************************************/
/* INCLUDE FILES                                                                                                      */
/**********************************************************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "Common_pt.h"
#include "Common_cfg.h"


/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/
/**
 * @brief Macro used to define an unused variable to avoid some compilation warnings
 */
#define CMN_unused(_PARAM_)                                 ((void)_PARAM_)


/*--------------------------------------------------------------------------------------------------------------------*/
// Macros to write in a 8, 16 or 32 Bits register:
#define CMN_write8BitsReg(_ADDRESS_,  _VALUE_)              ((*(volatile uint8_t *)_ADDRESS_)  = (_VALUE_))       //!< Macro to write in a 8Bits register
#define CMN_write16BitsReg(_ADDRESS_, _VALUE_)              ((*(volatile uint16_t *)_ADDRESS_) = (_VALUE_))       //!< Macro to write in a 16Bits register
#define CMN_write32BitsReg(_ADDRESS_, _VALUE_)              ((*(volatile uint32_t *)_ADDRESS_) = (_VALUE_))       //!< Macro to write in a 32Bits register


/*--------------------------------------------------------------------------------------------------------------------*/
// Macros to read from a 8, 16 or 32 Bits register:
#define CMN_read8BitsReg(_ADDRESS_)                         (*(volatile uint8_t *)_ADDRESS_)                      //!< Macro to read from a 8Bits register
#define CMN_read16BitsReg(_ADDRESS_)                        (*(volatile uint16_t *)_ADDRESS_)                     //!< Macro to read from a 16Bits register
#define CMN_read32BitsReg(_ADDRESS_)                        (*(volatile uint32_t *)_ADDRESS_)                     //!< Macro to read from a 32Bits register


/*--------------------------------------------------------------------------------------------------------------------*/
// Macros to set a Bit mask to a 8, 16 or 32 Bits register:
#define CMN_set8BitsMaskReg(_ADDRESS_,  _BIT_MASK_)         ((*(volatile uint8_t *)_ADDRESS_)  |= (_BIT_MASK_))   //!< Macro to set a Bit mask in a 8Bits register
#define CMN_set16BitsMaskReg(_ADDRESS_, _BIT_MASK_)         ((*(volatile uint16_t *)_ADDRESS_) |= (_BIT_MASK_))   //!< Macro to set a Bit mask in a 16Bits register
#define CMN_set32BitsMaskReg(_ADDRESS_, _BIT_MASK_)         ((*(volatile uint32_t *)_ADDRESS_) |= (_BIT_MASK_))   //!< Macro to set a Bit mask in a 32Bits register


/*--------------------------------------------------------------------------------------------------------------------*/
// Macros to clear a Bit mask to a 8, 16 or 32 Bits register:
#define CMN_clear8BitsMaskReg(_ADDRESS_,  _BIT_MASK_)       ((*(volatile uint8_t *)_ADDRESS_)  &= ~(_BIT_MASK_))  //!< Macro to clear a Bit mask in a 8Bits register
#define CMN_clear16BitsMaskReg(_ADDRESS_, _BIT_MASK_)       ((*(volatile uint16_t *)_ADDRESS_) &= ~(_BIT_MASK_))  //!< Macro to clear a Bit mask in a 16Bits register
#define CMN_clear32BitsMaskReg(_ADDRESS_, _BIT_MASK_)       ((*(volatile uint32_t *)_ADDRESS_) &= ~(_BIT_MASK_))  //!< Macro to clear a Bit mask in a 32Bits register


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Macro used to check if a Bit mask is contained in another data
 * @param[in]              _DATA_: The data to be checked
 * @param[in] _EXPECTED_BIT_MASK_: The expected Bit mask
 */
#define CMN_checkBitMaskValue(_DATA_, _EXPECTED_BIT_MASK_)  ((bool)(((_DATA_) & (_EXPECTED_BIT_MASK_)) == (_EXPECTED_BIT_MASK_)))


/*--------------------------------------------------------------------------------------------------------------------*/
#define CMN_1_BITS_SHIFT                                    1   //!< Bit shift macro
#define CMN_2_BITS_SHIFT                                    2   //!< Bit shift macro
#define CMN_3_BITS_SHIFT                                    3   //!< Bit shift macro
#define CMN_4_BITS_SHIFT                                    4   //!< Bit shift macro
#define CMN_5_BITS_SHIFT                                    5   //!< Bit shift macro
#define CMN_6_BITS_SHIFT                                    6   //!< Bit shift macro
#define CMN_7_BITS_SHIFT                                    7   //!< Bit shift macro
#define CMN_8_BITS_SHIFT                                    8   //!< Bit shift macro
#define CMN_9_BITS_SHIFT                                    9   //!< Bit shift macro
#define CMN_10_BITS_SHIFT                                   10  //!< Bit shift macro
#define CMN_11_BITS_SHIFT                                   11  //!< Bit shift macro
#define CMN_12_BITS_SHIFT                                   12  //!< Bit shift macro
#define CMN_13_BITS_SHIFT                                   13  //!< Bit shift macro
#define CMN_14_BITS_SHIFT                                   14  //!< Bit shift macro
#define CMN_15_BITS_SHIFT                                   15  //!< Bit shift macro
#define CMN_16_BITS_SHIFT                                   16  //!< Bit shift macro
#define CMN_17_BITS_SHIFT                                   17  //!< Bit shift macro
#define CMN_18_BITS_SHIFT                                   18  //!< Bit shift macro
#define CMN_19_BITS_SHIFT                                   19  //!< Bit shift macro
#define CMN_20_BITS_SHIFT                                   20  //!< Bit shift macro
#define CMN_21_BITS_SHIFT                                   21  //!< Bit shift macro
#define CMN_22_BITS_SHIFT                                   22  //!< Bit shift macro
#define CMN_23_BITS_SHIFT                                   23  //!< Bit shift macro
#define CMN_24_BITS_SHIFT                                   24  //!< Bit shift macro
#define CMN_25_BITS_SHIFT                                   25  //!< Bit shift macro
#define CMN_26_BITS_SHIFT                                   26  //!< Bit shift macro
#define CMN_27_BITS_SHIFT                                   27  //!< Bit shift macro
#define CMN_28_BITS_SHIFT                                   28  //!< Bit shift macro
#define CMN_29_BITS_SHIFT                                   29  //!< Bit shift macro
#define CMN_30_BITS_SHIFT                                   30  //!< Bit shift macro
#define CMN_31_BITS_SHIFT                                   31  //!< Bit shift macro
#define CMN_32_BITS_SHIFT                                   32  //!< Bit shift macro


/*--------------------------------------------------------------------------------------------------------------------*/
#define CMN_8_BITMASK_LEFT                                  0xf0        //!< Bit mask macro
#define CMN_8_BITMASK_RIGHT                                 0x0f        //!< Bit mask macro
#define CMN_8_BITMASK_FULL                                  0xff        //!< Bit mask macro
#define CMN_16_BITMASK_LEFT                                 0xff00      //!< Bit mask macro
#define CMN_16_BITMASK_RIGHT                                0x00ff      //!< Bit mask macro
#define CMN_16_BITMASK_MIDDLE                               0x0ff0      //!< Bit mask macro
#define CMN_16_BITMASK_FULL                                 0xffff      //!< Bit mask macro
#define CMN_32_BITMASK_LEFT                                 0xffff0000  //!< Bit mask macro
#define CMN_32_BITMASK_RIGHT                                0x0000ffff  //!< Bit mask macro
#define CMN_32_BITMASK_MIDDLE                               0x00ffff00  //!< Bit mask macro
#define CMN_32_BITMASK_FULL                                 0xffffffff  //!< Bit mask macro


/*--------------------------------------------------------------------------------------------------------------------*/
#define CMN_KILO_OCTET_FACTOR                               1024              //!< Octet/Kilo-Octet factor macro
#define CMN_MEGA_OCTET_FACTOR                               (1024*1024)       //!< ctet/Mega-Octet factor macro
#define CMN_GIGA_OCTET_FACTOR                               (1024*1024*1024)  //!< ctet/Giga-Octet factor macro


/*--------------------------------------------------------------------------------------------------------------------*/
#define CMN_1_MS                                            1     //!< Millisecond macro delay
#define CMN_2_MS                                            2     //!< Millisecond macro delay
#define CMN_3_MS                                            3     //!< Millisecond macro delay
#define CMN_4_MS                                            4     //!< Millisecond macro delay
#define CMN_5_MS                                            5     //!< Millisecond macro delay
#define CMN_6_MS                                            6     //!< Millisecond macro delay
#define CMN_7_MS                                            7     //!< Millisecond macro delay
#define CMN_8_MS                                            8     //!< Millisecond macro delay
#define CMN_9_MS                                            9     //!< Millisecond macro delay
#define CMN_10_MS                                           10    //!< Millisecond macro delay
#define CMN_20_MS                                           20    //!< Millisecond macro delay
#define CMN_30_MS                                           30    //!< Millisecond macro delay
#define CMN_40_MS                                           40    //!< Millisecond macro delay
#define CMN_50_MS                                           50    //!< Millisecond macro delay
#define CMN_60_MS                                           60    //!< Millisecond macro delay
#define CMN_70_MS                                           70    //!< Millisecond macro delay
#define CMN_80_MS                                           80    //!< Millisecond macro delay
#define CMN_90_MS                                           90    //!< Millisecond macro delay
#define CMN_100_MS                                          100   //!< Millisecond macro delay
#define CMN_200_MS                                          200   //!< Millisecond macro delay
#define CMN_300_MS                                          300   //!< Millisecond macro delay
#define CMN_400_MS                                          400   //!< Millisecond macro delay
#define CMN_500_MS                                          500   //!< Millisecond macro delay
#define CMN_600_MS                                          600   //!< Millisecond macro delay
#define CMN_700_MS                                          700   //!< Millisecond macro delay
#define CMN_800_MS                                          800   //!< Millisecond macro delay
#define CMN_900_MS                                          900   //!< Millisecond macro delay
#define CMN_1000_MS                                         1000  //!< Millisecond macro delay


/*--------------------------------------------------------------------------------------------------------------------*/
#if(CMN_ENABLE_PRINTF == true)
/**
 * @brief Enables the printf redirection if the @ref CMN_ENABLE_PRINTF is enabled
 */
#define CMN_systemPrintf(...)                               CMN_portPrintf(__VA_ARGS__)
#else
#define CMN_systemPrintf(...)
#endif //CMN_ENABLE_PRINTF


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Macro used to trigger an assertion
 */
#define CMN_triggerAssert(__COND__)                         CMN_assert(false)


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Macro used to define an assert check
 * @param[in] __COND__: The control to be checked, if set to "true" then the assertion is valid, otherwise the assertion
 *                     will fail
 */
#define CMN_assert(__COND__)                                CMN_vidManageAssert((bool)(__COND__),                      \
                                                                                (uint16_t)(__LINE__),                  \
                                                                                (char const * const)(__FILE__),        \
                                                                                (char const * const)(__FUNCTION__))


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Macro used to abort the software in case of critical error
 */
#define CMN_abortAll()                                      CMN_vidManageAbort((uint16_t)(__LINE__),                   \
                                                                               (char const * const)(__FILE__),         \
                                                                               (char const * const)(__FUNCTION__))


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Enables or disable the software breakpoints support depending of the value of the @ref CMN_ENABLE_DEBUG_MODE
 */
#if(CMN_ENABLE_DEBUG_MODE == true)
#define CMN_breakPoint()                                                                                               \
  do                                                                                                                   \
  {                                                                                                                    \
    CMN_portPrintf("[CMN ] Info: Software breakpoint triggered at \"%s\", line %d (in the function \"%s\")\r\n",       \
               __FILE__, __LINE__, __FUNCTION__);                                                                      \
    CMN_vidPortBreakPoint();                                                                                           \
  }                                                                                                                    \
  while(0)
#else
#define CMN_breakPoint()
#endif //CMN_ENABLE_DEBUG_MODE


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Enables the ISR of the MCU
 */
#define CMN_enableIsr()                                     CMN_vidPortEnableIsr()


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Disables the ISR of the MCU
 */
#define CMN_disableIsr()                                    CMN_vidPortDisableIsr()


/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
/**
 * @brief Initialization function of the common module
 * @details This function shall be called before the other modules
*/
void CMN_vidInitialize(void);


/*--------------------------------------------------------------------------------------------------------------------*/
#if(CMN_CONFIG_USE_FREERTOS == false)
/**
 * @brief Function used to set the assertion in non blocking mode
 * @remark This function shall be called at the end of the system initialization process
 * @remark This function can be used only of the setting @ref CMN_CONFIG_USE_FREERTOS is set to "false"
 */
void CMN_vidEndInit(void);
#endif //CMN_CONFIG_USE_FREERTOS


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to manage the assert check
 * @remark The direct use of this function is not recommended, use the @ref CMN_assert instead
 * @remark If the setting @ref CMN_CONFIG_USE_FREERTOS is set to "true", then the assertion won't block the system,
 *         use the @ref CMN_abortAll macro if the tested assertion has to abort all
 * @remark If the setting @ref CMN_CONFIG_USE_FREERTOS is set to "false" and the function @ref CMN_vidEndInit is called,
 *         then the assertion won't block the system, use the @ref CMN_abortAll macro if the tested assertion has to
 *         abort all
 * @param         kbCondition: The condition to be tested
 * @param        ku16FileLine: The file line when the assert triggered (to be set with @ref __LINE__)
 * @param     kpkpau8FileName: The file name when the assert triggered (to be set with @ref __FILE__)
 * @param kpkpau8FunctionName: The function name when the assert triggered (to be set with @ref __FUNCTION__)
 */
void CMN_vidManageAssert(const bool kbCondition,
                         const uint16_t ku16FileLine,
                         char const * const kpkpau8FileName,
                         char const * const kpkpau8FunctionName);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to manage the abort process
 * @remark The direct use of this function is not recommended, use the @ref CMN_abortAll instead
 * @param        ku16FileLine: The file line when the assert triggered (to be set with @ref __LINE__)
 * @param     kpkpau8FileName: The file name when the assert triggered (to be set with @ref __FILE__)
 * @param kpkpau8FunctionName: The function name when the assert triggered (to be set with @ref __FUNCTION__)
 */
void CMN_vidManageAbort(const uint16_t ku16FileLine,
                        char const * const kpkpau8FileName,
                        char const * const kpkpau8FunctionName);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to perform a delay in millisecond
 * @param ku32DelayMs:The delay in millisecond to be applied
 */
void CMN_vidDelayMs(const uint32_t ku32DelayMs);


/*--------------------------------------------------------------------------------------------------------------------*/
#endif // COMMON_H_
/*--------------------------------------------------------------------------------------------------------------------*/
