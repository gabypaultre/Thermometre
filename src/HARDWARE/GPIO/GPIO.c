/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      GPIO.c
 *
 * @author    gpaultre
 * @date      14/11/2024
 *
 * @version   0.0.0
 *
 * @brief     <you can add a description of your module here, otherwise please delete this line>
 * @details   <you can add more details of your module here, otherwise please delete this line>
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

#include "GPIO.h"
#include "ISR.h"

/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE VARIABLES                                                                                                  */
/**********************************************************************************************************************/

static bool (*AppManagerCallback)(ISR_tenuPeripheral peripheralId) = NULL;

/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/

static bool GPIO_handleGpioInterrupt(void);

/**********************************************************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

static bool GPIO_handleGpioInterrupt(void)
{
    if (IOCBFbits.IOCBF4 == 1)
    {
        IOCBFbits.IOCBF4 = 0;
        if (AppManagerCallback != NULL)
        {
            AppManagerCallback(ISR_ePERIPHERAL_INPUT_GPIO);
        }
        return true;
    }
    return false;
}

/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/

GPIO_status GPIO_init(void)
{
    TRISAbits.TRISA4 = 0;
    LATAbits.LATA4 = 0;

    TRISBbits.TRISB4 = 1;
    ANSELBbits.ANSELB4 = 0;
    // WPUBbits.WPUB4 = 1;

    IOCBFbits.IOCBF4 = 0;
    IOCBNbits.IOCBN4 = 1;
    PIE0bits.IOCIE = 1;

    if (ISR_bRegisterIsrCbk(ISR_ePERIPHERAL_INPUT_GPIO, GPIO_handleGpioInterrupt))
    {
        return GPIO_OK;
    }
    else
    {
        return GPIO_NOK;
    }
}

GPIO_status GPIO_setGpioHigh(void)
{
    LATAbits.LATA4 = 1;

    return GPIO_OK;
}

GPIO_status GPIO_setGpioLow(void)
{
    LATAbits.LATA4 = 0;

    return GPIO_OK;
}

GPIO_status GPIO_registerCallback(GPIO_callback callback)
{
    if (callback == NULL)
    {
        return GPIO_NOK;
    }
    AppManagerCallback = callback;
    return GPIO_OK;
}

/*--------------------------------------------------------------------------------------------------------------------*/
