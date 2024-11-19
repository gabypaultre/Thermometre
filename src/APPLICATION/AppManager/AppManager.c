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
#include "TIMER.h"
#include "LCD.h"
#include "Common.h"

/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/

#define TIMER_PERIOD_IN_MS 250

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

typedef enum
{
    APPM_STATE_SUSPENDED = 0,
    APPM_STATE_RUNNING
} AppManager_state;

typedef enum
{
    APPM_EVENT_NONE,
    APPM_EVENT_TIMER,
    APPM_EVENT_BUTTON_PRESSED
} AppManager_event;

/**********************************************************************************************************************/
/* PRIVATE VARIABLES                                                                                                  */
/**********************************************************************************************************************/

static AppManager_state currentState = APPM_STATE_SUSPENDED;
static AppManager_event pendingEvent = APPM_EVENT_NONE;

/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/

static bool AppManager_handleInterrupt(ISR_tenuPeripheral peripheralId);
static void AppManager_timerCallback(void);
static void AppManager_handleEvent(void);
static void AppManager_displayWelcomeMessage(void);

/**********************************************************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

static void AppManager_timerCallback(void)
{
    if (currentState == APPM_STATE_RUNNING)
    {
        pendingEvent = APPM_EVENT_TIMER;
        CMN_systemPrintf("Timer triggered!\r\n");
    }
}

static bool AppManager_handleInterrupt(ISR_tenuPeripheral peripheralId)
{
    if (peripheralId == ISR_ePERIPHERAL_INPUT_GPIO)
    {
        pendingEvent = APPM_EVENT_BUTTON_PRESSED;
        CMN_systemPrintf("Button pressed!\r\n");
        return true;
    }
    return false;
}

static void AppManager_displayWelcomeMessage(void)
{
    GPIO_setGpioLow();
    LCD_enuClearAll(LCD_eDEVICE_ID_DISPLAY);
    LCD_enuSetCursor(LCD_eDEVICE_ID_DISPLAY, 1, 1);
    LCD_enuWriteText(LCD_eDEVICE_ID_DISPLAY, "Welcome!");
}

static void AppManager_handleEvent(void)
{
    static int16_t temperature = 0;
    MCP9700_status mcpStatus;

    switch (currentState)
    {
        case APPM_STATE_SUSPENDED:
            if (pendingEvent == APPM_EVENT_BUTTON_PRESSED)
            {
                currentState = APPM_STATE_RUNNING;
                CMN_systemPrintf("State changed to RUNNING\r\n");

                LCD_enuClearAll(LCD_eDEVICE_ID_DISPLAY);
                LCD_enuSetCursor(LCD_eDEVICE_ID_DISPLAY, 1, 1);
                LCD_enuPrintf(LCD_eDEVICE_ID_DISPLAY, "State: RUNNING");
            }
            else
            {
                AppManager_displayWelcomeMessage();
            }
            break;

        case APPM_STATE_RUNNING:
            if (pendingEvent == APPM_EVENT_TIMER)
            {
                mcpStatus = MCP9700_getTemperature(&temperature);

                if (mcpStatus == MCP9700_OK)
                {
                    LCD_enuClearAll(LCD_eDEVICE_ID_DISPLAY);
                    LCD_enuSetCursor(LCD_eDEVICE_ID_DISPLAY, 1, 1);
                    LCD_enuPrintf(LCD_eDEVICE_ID_DISPLAY, "Temp: %d deg C", temperature);
                }
                else
                {
                    LCD_enuClearAll(LCD_eDEVICE_ID_DISPLAY);
                    LCD_enuSetCursor(LCD_eDEVICE_ID_DISPLAY, 1, 1);
                    LCD_enuPrintf(LCD_eDEVICE_ID_DISPLAY, "Temp: Error");
                }

                GPIO_toggleGpio();
                CMN_systemPrintf("Performing periodic action in RUNNING state\r\n");
            }
            else if (pendingEvent == APPM_EVENT_BUTTON_PRESSED)
            {
                currentState = APPM_STATE_SUSPENDED;
                CMN_systemPrintf("State changed to SUSPENDED\r\n");

                AppManager_displayWelcomeMessage();
            }
            break;

        default:
            CMN_systemPrintf("Unknown state\r\n");
            break;
    }

    pendingEvent = APPM_EVENT_NONE;
}

/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/

AppManager_status AppManager_init(void)
{
    GPIO_registerCallback(AppManager_handleInterrupt);

    TIM0_vidInitialize();

    bool success = TIM0_bRegisterTimerCbk(AppManager_timerCallback);

    if (!success)
    {
        CMN_systemPrintf("Error: Unable to register TIMER callback\r\n");
        return APPMANAGER_NOK;
    }

    success = TIM0_bStart(TIM0_eLFINTOSC,
                          TIM0_ePRESCALER_64,
                          TIM0_ePOSTSCALER_1,
                          TIMER_PERIOD_IN_MS);

    if (!success)
    {
        CMN_systemPrintf("Error: Unable to start TIMER\r\n");
        return APPMANAGER_NOK;
    }

    LCD_vidInitialize();
    AppManager_displayWelcomeMessage();

    CMN_systemPrintf("LCD initialized successfully.\r\n");

    return APPMANAGER_OK;
}

void APPM_vidStart(void)
{
    while (true)
    {
        if (pendingEvent != APPM_EVENT_NONE)
        {
            AppManager_handleEvent();
        }

        __delay_ms(10);
    }
}
