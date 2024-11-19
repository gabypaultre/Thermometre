/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      AppManager.c
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

#include "AppManager.h"
#include "GPIO.h"
#include "MCP9700.h"
#include "CLOCK.h"
#include "Common.h"

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
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/

static bool AppManager_handleInterrupt(ISR_tenuPeripheral peripheralId);

/**********************************************************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

static bool AppManager_handleInterrupt(ISR_tenuPeripheral peripheralId)
{
    if (peripheralId == ISR_ePERIPHERAL_INPUT_GPIO)
    {
        static bool ledState = false;

        CMN_systemPrintf("Button Pressed!\r\n");

        if (ledState)
        {
            GPIO_setGpioLow();
        }
        else
        {
            GPIO_setGpioHigh();
        }
        ledState = !ledState;

        return true;
    }
    return false;
}

/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/

AppManager_status AppManager_init(void)
{
    GPIO_registerCallback(AppManager_handleInterrupt);

    return APPMANAGER_OK;
}

void APPM_vidStart(void)
{
    int16_t temperature = 0;
    MCP9700_status mcpStatus;

    while (true)
    {
        mcpStatus = MCP9700_getTemperature(&temperature);

        if (mcpStatus == MCP9700_OK)
        {
            CMN_systemPrintf("Temperature value = %d°C\r\n", temperature);
        }
        else
        {
            CMN_systemPrintf("Error: Unable to retrieve temperature data.\r\n");
        }

        __delay_ms(1000);
    }
}

/*--------------------------------------------------------------------------------------------------------------------*/
