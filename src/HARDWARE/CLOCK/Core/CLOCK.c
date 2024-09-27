/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      CLOCK.c
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



/**********************************************************************************************************************/
/* INCLUDE FILES                                                                                                      */
/**********************************************************************************************************************/
#include "CLOCK.h"


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
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/
void CLOCK_vidInitialize(void)
{
  #pragma config FEXTOSC = OFF

#if(CLOCK_CONFIG_HFINTOSC_MHZ == 1)
  #pragma config RSTOSC = HFINTOSC_1MHZ
#if(CLOCK_CONFIG_FOSC_FREQUENCY_MHZ != CLOCK_FOSC_FREQUENCY_01MHZ)
#error The config "CLOCK_CONFIG_FOSC_FREQUENCY_MHZ" can only have the value "1" in this case
#endif //CLOCK_CONFIG_FOSC_FREQUENCY_MHZ
#elif(CLOCK_CONFIG_HFINTOSC_MHZ == 64)
  #pragma config RSTOSC  = HFINTOSC_64MHZ

#if(CLOCK_CONFIG_FOSC_FREQUENCY_MHZ == CLOCK_FOSC_FREQUENCY_01MHZ)
  OSCCON1bits.NDIV = 0b0110;
#elif(CLOCK_CONFIG_FOSC_FREQUENCY_MHZ == CLOCK_FOSC_FREQUENCY_02MHZ)
  OSCCON1bits.NDIV = 0b0101;
#elif(CLOCK_CONFIG_FOSC_FREQUENCY_MHZ == CLOCK_FOSC_FREQUENCY_04MHZ)
  OSCCON1bits.NDIV = 0b0100;
#elif(CLOCK_CONFIG_FOSC_FREQUENCY_MHZ == CLOCK_FOSC_FREQUENCY_08MHZ)
  OSCCON1bits.NDIV = 0b0011;
#elif(CLOCK_CONFIG_FOSC_FREQUENCY_MHZ == CLOCK_FOSC_FREQUENCY_16MHZ)
  OSCCON1bits.NDIV = 0b0010;
#elif(CLOCK_CONFIG_FOSC_FREQUENCY_MHZ == CLOCK_FOSC_FREQUENCY_32MHZ)
  OSCCON1bits.NDIV = 0b0001;
#elif(CLOCK_CONFIG_FOSC_FREQUENCY_MHZ == CLOCK_FOSC_FREQUENCY_64MHZ)
  OSCCON1bits.NDIV = 0b0000;
#else
  #error Wrong value for the config "CLOCK_CONFIG_FOSC_FREQUENCY_MHZ"
#endif
#else
  #error The config "CLOCK_CONFIG_HFINTOSC_MHZ" could have only either value "1" (1 MHz) or value "64" (64 MHz)
#endif //CLOCK_CONFIG_HFINTOSC_MHZ
}


/*--------------------------------------------------------------------------------------------------------------------*/
