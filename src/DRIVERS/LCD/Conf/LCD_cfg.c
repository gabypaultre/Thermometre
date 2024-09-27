/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      LCD_cfg.c
 *
 * @author    Jean DEBAINS
 * @date      Wednesday, January 31, 2024.
 *
 * @version   0.0.0
 *
 * @brief     LCD Driver configuration part
 * @details   Module in charge of the management of a LCD display 16*2 Display
 *
 * @remark    This file contains only the editable configuration parts
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
#include "LCD_cfg.h"


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
 * @brief The static database of each LCD configuration.
 */
static const LCD_tstrLcdConfig LCD_kastrLcdConfigList[LCD_eDEVICE_ID_END] =
{
  [LCD_eDEVICE_ID_DISPLAY]                                  =
  {
    .bEnable                                                = true,
    .u8I2cSlaveAddress                                      = 0x27,
    .enuI2cInstance                                         = I2CM_I2C_ID0,
    .u8NumberOfColums                                       = 20,
    .u8NumberOfRows                                         = 4,
    .u8EnableBacklight                                      = true,
    .u8EnableBlink                                          = false,
    .u8EnableCursor                                         = false,
  },
};


/**********************************************************************************************************************/
/* GETTERS DEFINITIONS                                                                                                */
/**********************************************************************************************************************/
LCD_tstrLcdConfig const * const LCD_kpkstrGetLcdConfig(const LCD_tenuDeviceId kenuDeviceId)
{
  LCD_tstrLcdConfig const *pkstrThisConfig = NULL;

  if((LCD_eDEVICE_ID_BEGIN < kenuDeviceId) && (kenuDeviceId < LCD_eDEVICE_ID_END))
  {
    pkstrThisConfig = &LCD_kastrLcdConfigList[kenuDeviceId];
  }

  return pkstrThisConfig;
}


/*--------------------------------------------------------------------------------------------------------------------*/
