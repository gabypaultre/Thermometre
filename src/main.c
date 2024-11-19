/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      main.c
 *
 * @author    Jean DEBAINS
 * @date      14/06/2023
 *
 * @version   0.0.0
 *
 * @brief     Main program entry point
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
#include "Common.h"


/*********************************/
/* HARDWARE INCLUDES             */
/*********************************/
#include "ISR.h"
#include "CLOCK.h"
#include "ADC.h"
#include "TIMER.h"
#include "EUSART.h"
#include "I2CM.h"

// Add the required includes for the hardware modules here...

/*********************************/
/* DRIVER INCLUDES               */
/*********************************/
#include "LCD.h"

// Add the required includes for the driver modules here...

/*********************************/
/* APPLICATION INCLUDES          */
/*********************************/
// Add the required includes for the application modules here...


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



/**********************************************************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS                                                                                       */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* START FUNCTION                                                                                                     */
/**********************************************************************************************************************/
void main(void)
{
  // WDT operating mode ? WDT Disabled:
  #pragma config WDTE = OFF

  // Low-voltage programming enabled:
  #pragma config LVP = ON

  // Enable the Global Interrupts
  ISR_GlobalInterruptEnable();

  // Enable the Peripheral Interrupts:
  ISR_PeripheralInterruptEnable();

  // Begin of the Init:
  CMN_vidInitialize();

  /*********************************/
  /* HARDWARE INITIALIZATIONS      */
  /*********************************/
  CLOCK_vidInitialize();
  EUSART_vidInitialize();
  ADC_vidInitialize();
  TIM0_vidInitialize();
  I2CM_vidInitalize();

  // Add your initialization function here for the hardware modules...

  /*********************************/
  /* DRIVER INITIALIZATIONS        */
  /*********************************/
  LCD_vidInitialize();

  // Add your initialization function here for the driver modules...

  /*********************************/
  /* APPLICATION INITIALIZATIONS   */
  /*********************************/
  // Add your initialization function here for the application modules...

  // End of the Init:
  CMN_vidEndInit();

  // Main program loop:
  while(true)
  {
    // Performs a delay of 0.5s:
    __delay_ms(500);
    
    // Send a "Hello World" message to the serial port:
    CMN_systemPrintf("Hello World ! \r\n"); // This message will be sent to the serial port on PUTTY
  }

  // We should never reach this code part:
  CMN_abortAll();
}


/*--------------------------------------------------------------------------------------------------------------------*/
