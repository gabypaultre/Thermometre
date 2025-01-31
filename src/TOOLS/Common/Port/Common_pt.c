/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      common_pt.c
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



/**********************************************************************************************************************/
/* INCLUDE FILES                                                                                                      */
/**********************************************************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include "ISR.h"
#include "CLOCK.h"
#include "EUSART.h"
#include "Common_cfg.h"
#include "Common_pt.h"


/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* PRIVATE VARIABLES                                                                                                  */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS PROTOTYPES                                                                                       */
/**********************************************************************************************************************/
#if(CMN_ENABLE_PRINTF == true)
extern void putch(char s8Data);
#endif //CMN_ENABLE_PRINTF


/**********************************************************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
#if(CMN_ENABLE_PRINTF == true)
extern void putch(char s8Data)
{
  EUSART_vidSendChar(s8Data, 10);
}
#endif //CMN_ENABLE_PRINTF


/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/
void CMN_vidErrorLedInit(void)
{
#if(CMN_ENABLE_ERROR_LED == true)
  TRISAbits.TRISA7 = false;
#endif //CMN_ENABLE_ERROR_LED
}


/*--------------------------------------------------------------------------------------------------------------------*/
void CMN_vidErrorLedSet(void)
{
#if(CMN_ENABLE_ERROR_LED == true)
  LATAbits.LATA7 = true;
#endif //CMN_ENABLE_ERROR_LED
}


/*--------------------------------------------------------------------------------------------------------------------*/
void CMN_vidErrorLedUnset(void)
{
#if(CMN_ENABLE_ERROR_LED == true)
  LATAbits.LATA7 = false;
#endif //CMN_ENABLE_ERROR_LED
}


/*--------------------------------------------------------------------------------------------------------------------*/
void CMN_vidPortDelayMs(const uint32_t ku32DelayMs)
{
  uint32_t u32DelayIdx = 0;

  for(u32DelayIdx = 0; u32DelayIdx < ku32DelayMs; u32DelayIdx++)
  {
    __delay_ms(1);
  }
}


/*--------------------------------------------------------------------------------------------------------------------*/
void CMN_vidPortEnableIsr(void)
{
  ISR_GlobalInterruptEnable();
  ISR_PeripheralInterruptEnable();
//  INTCONbits.GIE_GIEH  = true;
//  INTCONbits.PEIE_GIEL = true;
}


/*--------------------------------------------------------------------------------------------------------------------*/
void CMN_vidPortDisableIsr(void)
{
  ISR_GlobalInterruptDisable();
  ISR_PeripheralInterruptDisable();
//  INTCONbits.GIE_GIEH  = false;
//  INTCONbits.PEIE_GIEL = false;
}


/*--------------------------------------------------------------------------------------------------------------------*/
