/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      TIMER.h
 *
 * @author    Jean DEBAINS
 * @date      Wednesday, January 31, 2024.
 *
 * @version   0.0.0
 *
 * @brief     TIMER Hardware core part
 * @details   Module in charge of the management of the TIM1 peripheral
 *
 * @remark    Coding Language: C
 *
 * @copyright Copyright (c) 2024 This software is used for education proposal
 *
 ***********************************************************************************************************************
 */
#ifndef TIMER_H_
#define TIMER_H_


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
 * @brief Callback type used to register the callback called when the timer triggers
 */
typedef void (*TIM0_tpfvidRxCallback)(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Set the list of input clock used to schedule the timer
 * @details See "PIC18F47Q10 - Datasheet", P.297, 18.2.1
 */
typedef enum TIM0_tenuClkSrc
{
  TIM0_eT0CKIPPS                                            = 0,
  TIM0_eT0CKIPPS_INV,
  TIM0_eFOSC_DIV4,
  TIM0_eHFINTOSC,
  TIM0_eLFINTOSC,
  TIM0_eSOSC,
  TIM0_eMFINTOSC_500KHZ,
  TIM0_eCLC1_OUT,
  TIM0_eCLOCK_COUNT
}TIM0_tenuClkSrc;


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Set the list of the allowed prescalers used to configure the timer period
 * @details See "PIC18F47Q10 - Datasheet", P.302, 18.6.2
 */
typedef enum TIM0_tenuPrescaler
{
  TIM0_ePRESCALER_1                                         = 0,
  TIM0_ePRESCALER_2,
  TIM0_ePRESCALER_4,
  TIM0_ePRESCALER_8,
  TIM0_ePRESCALER_16,
  TIM0_ePRESCALER_32,
  TIM0_ePRESCALER_64,
  TIM0_ePRESCALER_128,
  TIM0_ePRESCALER_256,
  TIM0_ePRESCALER_512,
  TIM0_ePRESCALER_1024,
  TIM0_ePRESCALER_2048,
  TIM0_ePRESCALER_4096,
  TIM0_ePRESCALER_8192,
  TIM0_ePRESCALER_16384,
  TIM0_ePRESCALER_32768,
  TIM0_ePRESCALER_COUNT
}TIM0_tenuPrescaler;


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Set the list of the allowed postscalers used to configure the timer period
 * @details See "PIC18F47Q10 - Datasheet", P.301, 18.6.1
 */
typedef enum TIM0_tenuPostscaler
{
  TIM0_ePOSTSCALER_1                                        = 0,
  TIM0_ePOSTSCALER_2,
  TIM0_ePOSTSCALER_3,
  TIM0_ePOSTSCALER_4,
  TIM0_ePOSTSCALER_5,
  TIM0_ePOSTSCALER_6,
  TIM0_ePOSTSCALER_7,
  TIM0_ePOSTSCALER_8,
  TIM0_ePOSTSCALER_9,
  TIM0_ePOSTSCALER_10,
  TIM0_ePOSTSCALER_11,
  TIM0_ePOSTSCALER_12,
  TIM0_ePOSTSCALER_13,
  TIM0_ePOSTSCALER_14,
  TIM0_ePOSTSCALER_15,
  TIM0_ePOSTSCALER_16,
  TIM0_ePOSTSCALER_COUNT
}TIM0_tenuPostscaler;


/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
/**
 * @brief Initialize the timer TIM0
 * @details - 1) Set the timer in 8 Bits mode (see "PIC18F47Q10 - Datasheet", P.297)
 *          - 2) Set the timer in asynchronous mode (see "PIC18F47Q10 - Datasheet", P.298)
 *          - 3) Connects the module to the interruption manager
 */
void TIM0_vidInitialize(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to register the callback function when the timer will trigger
 * @details Registerting the callback will make the timer interrupt enabled
 * @param[in] kpfCallback Pointer to the callback function to be called
 * @return Return "true" if the function ran successfully, return "false" otherwise
 */
bool TIM0_bRegisterTimerCbk(const TIM0_tpfvidRxCallback kpfCallback);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to configure the timer period and to start it
 * @details To configure the wanted timer period (Tperiod[s]) apply the following formula accordingly to the simplified schematic:
 *          (more infos can be found in the datasheet "PIC18F47Q10 - Datasheet", P.296)
 *
 *                                   (T0CKPS)              (TMR0 Body)            (T0OUTPS)
 *              (T0CS)             /-----------\        /--------------\        /------------\              (TMR0)
 *          [ClockFreq[Hz]]------->| Prescaler |------->| CompareValue |------->| Postscaler |------->[x] Tperiod[s]
 *                                 \-----------/        \--------------/        \------------/
 *
 *                          /----------------------------------------------------------------------\
 *                          | Tperiod[s] = (CompareValue * Prescaler * Postscaler) / ClockFreq[Hz] |
 *                          \----------------------------------------------------------------------/
 *
 *          Example: To have a Tperiod[s] = 0.5s the needed values are the following:
 *              - ClockFreq[Hz] = 32KHz (LFINTOSC as input clock)
 *              - Prescaler     = 64
 *              - Postscaler    = 1
 *              - CompareValue  = 250
 *
 *          --> (250 * 64 * 1)/320000 = 0.5s
 *
 * @param[in] kenuClockSrc The clock source to be used to schedule the timer (see "PIC18F47Q10 - Datasheet", P.297, 18.2.1)
 * @param[in] kenuPrescaler The prescaler used to configure the timer period (see "PIC18F47Q10 - Datasheet", P.302, 18.6.2
 * @param[in] kenuPostscaler The postscaler used to configure the timer period (see "PIC18F47Q10 - Datasheet", P.301, 18.6.1)
 * @param[in] ku8CompareValue The compare value to set to configure the timer period (see "PIC18F47Q10 - Datasheet", P.303, 18.6.3)
 * @return Return "true" if the function ran successfully, return "false" otherwise
 */
bool TIM0_bStart(const TIM0_tenuClkSrc kenuClockSrc,
                 const TIM0_tenuPrescaler kenuPrescaler,
                 const TIM0_tenuPostscaler kenuPostscaler,
                 const uint8_t ku8CompareValue);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to stop the timer
 * @return Return "true" if the function ran successfully, return "false" otherwise
 */
bool TIM0_bStop(void);


/*--------------------------------------------------------------------------------------------------------------------*/
#endif /* TIMER_H_ */
/*--------------------------------------------------------------------------------------------------------------------*/
