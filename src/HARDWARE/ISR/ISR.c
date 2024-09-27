/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      INTERRUPT.c
 *
 * @author    Jean DEBAINS
 * @date      Wednesday, January 31, 2024.
 *
 * @version   0.0.0
 *
 * @brief     INTERRUPT Hardware core part
 * @details   Module in charge of the management of the interruptions
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
#include "ISR.h"


/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/
#define ENABLE_EXCLUSIVE_ISR_HANDLE                         false


/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* PRIVATE VARIABLES                                                                                                  */
/**********************************************************************************************************************/
/**
 * @brief Array of function pointer which contains the addresses of the functions of the peripherals which has to be called when
 *        an interruption is triggering
 */
static ISR_tpfbCallback ISR_apfbCallback[ISR_ePERIPHERAL_END] = { NULL };


/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS PROTOTYPES                                                                                       */
/**********************************************************************************************************************/
/**
 * @brief This function is called once an interruption is triggered
 * @details This function is the entry point of all the hardware interruptions of the MCU. When it triggers, the module
 *          will call all the registered callback of all the involved modules, thus each module will be able to check
 *          whether the interruption came from it or not
 */
void __interrupt() vidInterruptManager(void);


/**********************************************************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
void __interrupt() vidInterruptManager(void)
{
  ISR_tenuPeripheral enuPeripheral = ISR_ePERIPHERAL_END;
  bool               bIsIsrFound   = false;

  /*
  * Parsing of the array which contains the pointed functions of each registered peripheral
  */
  for(enuPeripheral = 0, bIsIsrFound = false; ((enuPeripheral < ISR_ePERIPHERAL_END) && (bIsIsrFound == false)); enuPeripheral++)
  {
    /*
     * If the peripheral has registered its callback function then this function will be called, that will allow the current
     * peripheral to check whether the triggered interruption was from it or not
     */
    if(ISR_apfbCallback[enuPeripheral] != NULL)
    {
#if(ENABLE_EXCLUSIVE_ISR_HANDLE == true)
      bIsIsrFound = ISR_apfbCallback[enuPeripheral]();
#elif(ENABLE_EXCLUSIVE_ISR_HANDLE == false)
      (void)ISR_apfbCallback[enuPeripheral]();
#endif //ENABLE_EXCLUSIVE_ISR_HANDLE
    }
  }
}


/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/
bool ISR_bRegisterIsrCbk(const ISR_tenuPeripheral kenuPeripheralId, const ISR_tpfbCallback kpfbCallback)
{
  bool bStatus = false;

  if((ISR_ePERIPHERAL_BEGIN < kenuPeripheralId) && (kenuPeripheralId < ISR_ePERIPHERAL_END) && (kpfbCallback != NULL))
  {
    ISR_apfbCallback[kenuPeripheralId] = kpfbCallback;
    bStatus                            = true;
  }

  return bStatus;
}


/*--------------------------------------------------------------------------------------------------------------------*/
bool ISR_bUnregisterIsrCbk(const ISR_tenuPeripheral kenuPeripheralId)
{
  bool bStatus = false;

  if((ISR_ePERIPHERAL_BEGIN < kenuPeripheralId) && (kenuPeripheralId < ISR_ePERIPHERAL_END))
  {
    ISR_apfbCallback[kenuPeripheralId] = NULL;
    bStatus                            = true;
  }

  return bStatus;
}


/*--------------------------------------------------------------------------------------------------------------------*/
