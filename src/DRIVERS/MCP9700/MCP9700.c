/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      MCP9700.c
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

#include "MCP9700.h"
#include "Common.h"

/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/

#define ADC_REF_VOLTAGE_MV 3300
#define ADC_RESOLUTION 1024

#define ADC_MIN_VALUE 31
#define ADC_MAX_VALUE 558
#define TEMP_MIN -40
#define TEMP_MAX 125

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE VARIABLES                                                                                                  */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/

static MCP9700_status MCP9700_calculateTemperature(uint16_t adcValue, int16_t *temperature);

/**********************************************************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

static MCP9700_status MCP9700_calculateTemperature(uint16_t adcValue, int16_t *temperature)
{
    if (temperature == NULL)
    {
        return MCP9700_NOK;
    }

    // Limiter adcValue aux bornes définies
    if (adcValue <= ADC_MIN_VALUE)
    {
        *temperature = TEMP_MIN;
    }
    else if (adcValue >= ADC_MAX_VALUE)
    {
        *temperature = TEMP_MAX;
    }
    else
    {
        // Interpolation linéaire pour les valeurs intermédiaires
        *temperature = (int16_t)(TEMP_MIN + ((int32_t)(adcValue - ADC_MIN_VALUE) * (TEMP_MAX - TEMP_MIN)) / (ADC_MAX_VALUE - ADC_MIN_VALUE));
    }

    return MCP9700_OK;
}

/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/

MCP9700_status MCP9700_getTemperature(int16_t *temperature)
{
    uint16_t adcValue = 0;

    if (MCP9700_getRawValue(&adcValue) != MCP9700_OK)
    {
        return MCP9700_NOK;
    }

    return MCP9700_calculateTemperature(adcValue, temperature);
}

MCP9700_status MCP9700_getRawValue(uint16_t *adcValue)
{
    if (adcValue == NULL)
    {
        return MCP9700_NOK;
    }

    MCP9700_status status = (ADC_enuGetRawValue(adcValue, 1000) == ADC_eSTATUS_OK) ? MCP9700_OK : MCP9700_NOK;

    CMN_systemPrintf("Raw ADC Value: %d\r\n", *adcValue); 

    return status;
}

/*--------------------------------------------------------------------------------------------------------------------*/