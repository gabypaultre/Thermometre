/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      TIMER.c
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



/**********************************************************************************************************************/
/* INCLUDE FILES                                                                                                      */
/**********************************************************************************************************************/
#include <limits.h>
#include "CLOCK.h"
#include "ISR.h"
#include "TIMER.h"


/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* PRIVATE VARIABLES                                                                                                  */
/**********************************************************************************************************************/
/**
 * @brief Timer callback used to informe the upper layer that the timer triggered
 */
static TIM0_tpfvidRxCallback TIM0_pfRxCallback              = NULL;


/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS PROTOTYPES                                                                                       */
/**********************************************************************************************************************/
/**
 * @brief Callback function registered to the interrupt module and called when an interruption triggers to check if this
 *        interruption came from the timer TIM0
 * @details See "PIC18F47Q10 - Datasheet", P.194, 14.9"
 */
static bool bInterruptHandler(void);


/**********************************************************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
static bool bInterruptHandler(void)
{
  bool bIsIsrFound = false;

  /*
  * If the interruption for the timer is set to "1" (enabled) and if the interrupt flag of the timer is set to "1" (interrupt
  * from the timer) then the flag has to be cleared and the callback called to notify the upper layer:
  */
  if((PIE0bits.TMR0IE == 1) && (PIR0bits.TMR0IF == 1))
  {
    // The ISR was found, then the flag is updated to inform the ISR module:
    bIsIsrFound = true;

    // Once the interrupt acked, the flag has to be clear:
    PIR0bits.TMR0IF = 0;

    // If a callback function was registered previously, then let call it:
    if(TIM0_pfRxCallback != NULL)
    {
      TIM0_pfRxCallback();
    }
  }

  // The ISR found flag is returned to the ISR module to indicate whether the ISR was found from the GPIO module of not:
  return bIsIsrFound;
}


/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/
void TIM0_vidInitialize(void)
{
  bool bStatus = false;

  // - 1) Set the timer in 8 Bits mode ("PIC18F47Q10 - Datasheet", P.297):
  T0CON0bits.T016BIT = 0;

  // - 2) Set the timer in asynchronous mode ("PIC18F47Q10 - Datasheet", P.298):
  T0CON1bits.T0ASYNC = 1;

  // - 3) Connects the module to the interruption manager, the "CMN_assert" will check if the module was correctly
  //      registered:
  bStatus = ISR_bRegisterIsrCbk(ISR_ePERIPHERAL_TIMER, bInterruptHandler);
  CMN_assert(bStatus == true);
}


/*--------------------------------------------------------------------------------------------------------------------*/
bool TIM0_bRegisterTimerCbk(const TIM0_tpfvidRxCallback kpfCallback)
{
  bool bStatus = false;

  // If the passed parameter is correct let's do the action below:
  if(kpfCallback != NULL)
  {
    // The callback is registered:
    TIM0_pfRxCallback = (TIM0_tpfvidRxCallback)kpfCallback;

    // The interruption from the timer TIM0 is enabled:
    PIE0bits.TMR0IE = 1;

    // The interruption flag is cleared to be safe:
    PIR0bits.TMR0IF = 0;

    bStatus = true;
  }

  return bStatus;
}


/*--------------------------------------------------------------------------------------------------------------------*/
bool TIM0_bStart(const TIM0_tenuClkSrc kenuClockSrc,
                 const TIM0_tenuPrescaler kenuPrescaler,
                 const TIM0_tenuPostscaler kenuPostscaler,
                 const uint8_t ku8CompareValue)
{
  bool bStatus = false;

  /*
    * Let check the following parameters:
    *   - 1) The timer shall be enabled and cannot start of there is no callback registered previously as the callback is needed
    *        to inform the user than the timer has triggered
    *   - 2) Verify if the clock source, the prescaler value and the post scaler value are in their correct range
    */
  if((PIE0bits.TMR0IE == 1)                    &&
     (TIM0_pfRxCallback != NULL)               &&
     (kenuClockSrc   < TIM0_eCLOCK_COUNT)      &&
     (kenuPrescaler  < TIM0_ePRESCALER_COUNT)  &&
     (kenuPostscaler < TIM0_ePOSTSCALER_COUNT))
  {
    bStatus            = true;
    T0CON0bits.T0EN    = 0; // Stopping the timer if it was in the running state before applying the new modifications
    T0CON1bits.T0CS    = (uint8_t)kenuClockSrc;
    T0CON1bits.T0CKPS  = (uint8_t)kenuPrescaler;
    T0CON0bits.T0OUTPS = (uint8_t)kenuPostscaler;
    TMR0H              = ku8CompareValue;
    TMR0L              = 0x00;
    T0CON0bits.T0EN    = 1; // Starting the timer
  }

  return bStatus;
}


/*--------------------------------------------------------------------------------------------------------------------*/
bool TIM0_bStop(void)
{
  bool bStatus = false;

  if(PIE0bits.TMR0IE == 1)
  {
    T0CON0bits.T0EN = 0;
    bStatus         = true;
  }

  return bStatus;
}


/*--------------------------------------------------------------------------------------------------------------------*/
