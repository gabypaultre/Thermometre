/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      LCD_cfg.h
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
#ifndef LCD_CFG_H_
#define LCD_CFG_H_


/**********************************************************************************************************************/
/* INCLUDE FILES                                                                                                      */
/**********************************************************************************************************************/
#include "Common.h"
#include "I2CM.h"


/**********************************************************************************************************************/
/* MACROS, CONFIGURATIONS                                                                                             */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
/**
 * @brief Enumeration used to declare each LCD to handle them through the publics APIs of the driver.
 */
typedef enum LCD_tenuDeviceId
{
  /*-----[DO NOT EDIT/USE THIS]-----*/
  LCD_eDEVICE_ID_BEGIN = -1, /*     */
  /*--------------------------------*/

  LCD_eDEVICE_ID_DISPLAY,

  /*-------[DO NOT EDIT THIS]-------*/
  LCD_eDEVICE_ID_END /*-------------*/
  /*--------------------------------*/
}LCD_tenuDeviceId;


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Type used to set a static configuration of a LCD item.
 */
typedef struct LCD_tstrLcdConfig
{
  bool                                                      bEnable;            //!< If this display is enabled or not.
  I2CM_tenuI2cId                                            enuI2cInstance;     //!< The I2C instance where the LCD is wired
  uint8_t                                                   u8I2cSlaveAddress;  //!< The I2C slave address
  uint8_t                                                   u8NumberOfColums;   //!< The number of columns of the LCD
  uint8_t                                                   u8NumberOfRows;     //!< The number of rows of the LCD
  uint8_t                                                   u8EnableBacklight;  //!< Is the backlight has to be enabled at the startup
  uint8_t                                                   u8EnableBlink;      //!< Is the cursor blink has to be enabled at the startup
  uint8_t                                                   u8EnableCursor;     //!< Is the cursor has to be enabled at the startup
}LCD_tstrLcdConfig;


/**********************************************************************************************************************/
/* GETTERS PROTOTYPES                                                                                                 */
/**********************************************************************************************************************/
/**
 * @brief Function used to get a static configuration for the specified ID of the LCD.
 * @return Return a LCD item with its static configuration.
 */
LCD_tstrLcdConfig const * const LCD_kpkstrGetLcdConfig(const LCD_tenuDeviceId kenuDeviceId);


/*--------------------------------------------------------------------------------------------------------------------*/
#endif /* LCD_CFG_H_ */
/*--------------------------------------------------------------------------------------------------------------------*/
