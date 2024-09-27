/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      LCD.c
 *
 * @author    Jean DEBAINS
 * @date      Wednesday, January 31, 2024.
 *
 * @version   0.0.0
 *
 * @brief     LCD Driver core part
 * @details   Module in charge of the management of a LCD display 16*2 Display
 *
 * @remark    This file contains only the Core part of the module, all configurations are defined in the LCD_cfg.c
 *            and LCD_cfg.h
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
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "I2CM.h"
#include "LCD.h"


/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/
// commands
#define LCD_u8CLEAR_DISPLAY                                 0x01
#define LCD_u8RETURN_HOME                                   0x02
#define LCD_u8ENTRY_MODE_SET                                0x04
#define LCD_u8DISPLAY_CONTROL                               0x08
#define LCD_u8CURSOR_SHIFT                                  0x10
#define LCD_u8FUNCTION_SET                                  0x20
#define LCD_u8SET_CG_RAMADDR                                0x40
#define LCD_u8SET_DDRAM_ADDR                                0x80


/*--------------------------------------------------------------------------------------------------------------------*/
// flags for display entry mode
#define LCD_u8ENTRY_RIGHT                                   0x00
#define LCD_u8ENTRY_LEFT                                    0x02
#define LCD_u8ENTRY_SHIFT_INCREMENT                         0x01
#define LCD_u8ENTRY_SHIFT_DECREMENT                         0x00


/*--------------------------------------------------------------------------------------------------------------------*/
// flags for display on/off control
#define LCD_u8DISPLAY_ON                                    0x04
#define LCD_u8DISPLAY_OFF                                   0x00
#define LCD_u8CURSOR_ON                                     0x02
#define LCD_u8CURSOR_OFF                                    0x00
#define LCD_u8BLINK_ON                                      0x01
#define LCD_u8BLINK_OFF                                     0x00


/*--------------------------------------------------------------------------------------------------------------------*/
// flags for display/cursor shift
#define LCD_u8DISPLAY_MOVE                                  0x08
#define LCD_u8CURSOR_MOVE                                   0x00
#define LCD_u8MOVE_RIGHT                                    0x04
#define LCD_u8MOVE_LEFT                                     0x00


/*--------------------------------------------------------------------------------------------------------------------*/
// flags for function set
#define LCD_u88BIT_MODE                                     0x10
#define LCD_u84BIT_MODE                                     0x00
#define LCD_u82LINE                                         0x08
#define LCD_u81LINE                                         0x00
#define LCD_u85x10_DOTS                                     0x04
#define LCD_u85x8_DOTS                                      0x00


/*--------------------------------------------------------------------------------------------------------------------*/
// flags for backlight control
#define LCD_u8BACKLIGHT                                     0x08
#define LCD_u8NO_BACKLIGHT                                  0x00


/*--------------------------------------------------------------------------------------------------------------------*/
#define LCD_EN                                              0b00000100  // Enable bit
#define LCD_RW                                              0b00000010  // Read/Write bit
#define LCD_RS                                              0b00000001  // Register select bit


/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
/**
 * @brief Type used to set a dynamic database of a LCD item.
 */
typedef struct tstrDisplayData
{
  uint8_t                                                   u8DisplayControl;   //!< The current configurations of the display.
  uint8_t                                                   u8DisplayMode;      //!< The current mode of the display.
  uint8_t                                                   u8BacklightLevel;   //!< Is the display has its backlight enabled or not.
  uint8_t                                                   u8DisplayFunction;  //!< The current functions of the display.
  uint8_t                                                   u8CurrentColumnPos; //!< The current column position of the display.
  uint8_t                                                   u8CurrentRowPos;    //!< The current row position of the display.
  uint8_t*                                                  pau8DataBuffer;     //!< Pointer to the data buffer used to format the data before a print
  uint16_t                                                  u16DataSize;        //!< The size of the data buffer
}tstrDisplayData;


/**********************************************************************************************************************/
/* PRIVATE VARIABLES                                                                                                  */
/**********************************************************************************************************************/
/**
 * @brief The dynamic database of each LCD item.
 */
static tstrDisplayData LCD_astrDisplayData[LCD_eDEVICE_ID_END] = { 0 };


/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS PROTOTYPES                                                                                       */
/**********************************************************************************************************************/
/**
 * @brief Function used to check if a LCD device ID is in a valid range or not
 * @param[in] kenuDeviceId The LCD device ID to be checked
 * @return Returns "true" if the ID is between ] @ref LCD_eDEVICE_ID_BEGIN ; @ref LCD_eDEVICE_ID_END [, returns "false"
 *         otherwise
*/
static bool bIsDeviceIdValid(const LCD_tenuDeviceId kenuDeviceId);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Low layer function used to send data through the I2C bus.
 * @param[in] kenuDeviceId The ID of the LCD.
 * @param[in]      ku8Data The data to be send to the LCD. The data to be send to the LCD.
 * @return Return "true" if the function ran successfully, return "false" in the other cases.
 */
static bool bSendToI2c(const LCD_tenuDeviceId kenuDeviceId, const uint8_t ku8Data);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to send a pulse to the EN pin of the LCD when a data has to be set.
 * @param[in]      ku8Data The data to be send to the LCD.
 * @param[in] kenuDeviceId The ID of the LCD.
 * @return Return "true" if the function ran successfully, return "false" in the other cases.
 */
static bool bPulseEnable(const LCD_tenuDeviceId kenuDeviceId, const uint8_t ku8Data);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to send data with the 4 Bits mode of the LCD.
 * @param[in] kenuDeviceId The ID of the LCD.
 * @param[in]      ku8Data The data to be send to the LCD.
 * @return Return "true" if the function ran successfully, return "false" in the other cases.
 */
static bool bWrite4Bits(const LCD_tenuDeviceId kenuDeviceId, const uint8_t ku8Data);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Generic function used to either send a command or send a data.
 * @param[in] kenuDeviceId The ID of the LCD.
 * @param[in]      ku8Data The data to be send to the LCD.
 * @param[in]      ku8Mode If set to "0", then that content of "ku8Data" has tp be a command. If set to "1" the content of "ku8Data" is
 *                     a data.
 * @return Return "true" if the function ran successfully, return "false" in the other cases.
 */
static bool bGenericSend(const LCD_tenuDeviceId kenuDeviceId, const uint8_t ku8Data, const uint8_t ku8Mode);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to send a command to the LCD.
 * @param[in] kenuDeviceId The ID of the LCD.
 * @param[in]      ku8Data The data to be send to the LCD.
 * @return Return "true" if the function ran successfully, return "false" in the other cases.
 */
static bool bSendCommand(const LCD_tenuDeviceId kenuDeviceId, const uint8_t ku8Data);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to send a data to the LCD.
 * @param[in] kenuDeviceId The ID of the LCD.
 * @param[in]      ku8Data The data to be send to the LCD.
 * @return Return "true" if the function ran successfully, return "false" in the other cases.
 */
static bool bSendData(const LCD_tenuDeviceId kenuDeviceId, const uint8_t ku8Data);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to send a text to the LCD.
 * @param[in]  kenuDeviceId The ID of the LCD.
 * @param[in]      kps8Text The text to be printed
 * @param[in] ku8TextLenght The text length
 * @return Return "true" if the function ran successfully, return "false" in the other cases.
 */
static bool bWriteText(const LCD_tenuDeviceId kenuDeviceId, const char *kps8Text, const uint16_t ku16TextLength);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Set the display ON or OFF.
 * @param[in] kenuDeviceId The ID of the LCD.
 * @param[in]     bSetToOn If "true" the display will be set to On, if set to "false" the diplay will be set fo Off
 * @return Return "true" if the function ran successfully, return "false" in the other cases.
 */
static bool bSetDisplayOn(const LCD_tenuDeviceId kenuDeviceId, const bool bSetToOn);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Set the cursor position where a text has to be written.
 * @param[in] kenuDeviceId The ID of the LCD.
 * @param[in]     u8Column The column to be set.
 * @param[in]        u8Row The row to be set.
 * @return Return "true" if the function ran successfully, return "false" in the other cases.
 */
static bool bSetCursor(const LCD_tenuDeviceId kenuDeviceId, uint8_t u8Column, uint8_t u8Row);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to clear the entire display.
 * @param[in] kenuDeviceId The ID of the LCD.
 * @return Return "true" if the function ran successfully, return "false" in the other cases.
 */
static bool bClearDisplay(const LCD_tenuDeviceId kenuDeviceId);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to set the cursor at the default position.
 * @param[in] kenuDeviceId The ID of the LCD.
 * @return Return "true" if the function ran successfully, return "false" in the other cases.
 */
static bool bReturnHome(const LCD_tenuDeviceId kenuDeviceId);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to enable or disable the backlight of the LCD.
 * @param[in] kenuDeviceId The ID of the LCD.
 * @param[in]     kbEnable If set to "true", then the backlight will be enabled, of set to "false" the backlight will be disabled.
 * @return Return "true" if the function ran successfully, return "false" in the other cases.
 */
static bool bEnableBackLight(const LCD_tenuDeviceId kenuDeviceId, const bool kbEnable);


/**********************************************************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
static bool bIsDeviceIdValid(const LCD_tenuDeviceId kenuDeviceId)
{
  bool bIsValid = false;

  if((LCD_eDEVICE_ID_BEGIN < kenuDeviceId) && (kenuDeviceId < LCD_eDEVICE_ID_END))
  {
    bIsValid = true;
  }

  return bIsValid;
}


/*--------------------------------------------------------------------------------------------------------------------*/
static bool bSendToI2c(const LCD_tenuDeviceId kenuDeviceId, const uint8_t ku8Data)
{
  bool                    bStatus          = false;
  uint8_t                 u8Message        = (ku8Data | LCD_astrDisplayData[kenuDeviceId].u8BacklightLevel);
  LCD_tstrLcdConfig const *pkstrThisConfig = LCD_kpkstrGetLcdConfig(kenuDeviceId);

  if(I2CM_enuWriteBuffer(pkstrThisConfig->enuI2cInstance, pkstrThisConfig->u8I2cSlaveAddress, &u8Message, sizeof(u8Message)) == I2CM_eSTATUS_OK)
  {
    bStatus = true;
  }

  return bStatus;
}


/*--------------------------------------------------------------------------------------------------------------------*/
static bool bPulseEnable(const LCD_tenuDeviceId kenuDeviceId, const uint8_t ku8Data)
{
  bool bReturnCode = false;

  if(bSendToI2c(kenuDeviceId, (ku8Data | LCD_EN)))
  {
    bReturnCode = bSendToI2c(kenuDeviceId, (ku8Data & ~LCD_EN));
    CMN_vidDelayMs(CMN_1_MS);
  }

  return bReturnCode;
}


/*--------------------------------------------------------------------------------------------------------------------*/
static bool bWrite4Bits(const LCD_tenuDeviceId kenuDeviceId, const uint8_t ku8Data)
{
  bool bReturnCode = false;

  if(bSendToI2c(kenuDeviceId, ku8Data))
  {
    bReturnCode = bPulseEnable(kenuDeviceId, ku8Data);
  }

  return bReturnCode;
}


/*--------------------------------------------------------------------------------------------------------------------*/
static bool bGenericSend(const LCD_tenuDeviceId kenuDeviceId, const uint8_t ku8Data, const uint8_t ku8Mode)
{
  bool bReturnCode = false;

  if(bWrite4Bits(kenuDeviceId, (((ku8Data << 0) & 0xf0) | ku8Mode)))
  {
    bReturnCode = bWrite4Bits(kenuDeviceId, (((ku8Data << 4) & 0xf0) | ku8Mode));
  }

  return bReturnCode;
}


/*--------------------------------------------------------------------------------------------------------------------*/
static bool bSendCommand(const LCD_tenuDeviceId kenuDeviceId, const uint8_t ku8Data)
{
  return bGenericSend(kenuDeviceId, ku8Data, 0);
}


/*--------------------------------------------------------------------------------------------------------------------*/
static bool bSendData(const LCD_tenuDeviceId kenuDeviceId, const uint8_t ku8Data)
{
  return bGenericSend(kenuDeviceId, ku8Data, 1);
}


/*--------------------------------------------------------------------------------------------------------------------*/
static bool bWriteText(const LCD_tenuDeviceId kenuDeviceId, const char *kps8Text, const uint16_t ku16TextLength)
{
  bool    bReturnCode  = false;
  uint8_t u16TextIndex = 0;
  bool    bExitLoop    = false;

  CMN_vidDelayMs(CMN_1_MS);

  for(u16TextIndex = 0, bExitLoop = false; ((u16TextIndex < ku16TextLength) && (bExitLoop == false)); u16TextIndex++)
  {
    bReturnCode = bSendData(kenuDeviceId, kps8Text[u16TextIndex]);

    if(!bReturnCode)
    {
      bExitLoop = true;
    }
  }

  return bReturnCode;
}


/*--------------------------------------------------------------------------------------------------------------------*/
static bool bSetDisplayOn(const LCD_tenuDeviceId kenuDeviceId, const bool bSetToOn)
{
  bool bReturnCode = false;

  if(bSetToOn)
  {
    LCD_astrDisplayData[kenuDeviceId].u8DisplayControl |= LCD_u8DISPLAY_ON;
  }
  else
  {
    LCD_astrDisplayData[kenuDeviceId].u8DisplayControl &= ~LCD_u8DISPLAY_ON;
  }

  bReturnCode  = bSendCommand(kenuDeviceId, (LCD_u8DISPLAY_CONTROL | LCD_astrDisplayData[kenuDeviceId].u8DisplayControl));

  return bReturnCode;
}


/*--------------------------------------------------------------------------------------------------------------------*/
static bool bSetCursor(const LCD_tenuDeviceId kenuDeviceId, uint8_t u8Column, uint8_t u8Row)
{
  LCD_tstrLcdConfig const *pkstrThisConfig = LCD_kpkstrGetLcdConfig(kenuDeviceId);
  const uint8_t           kau8RowOffset[]  = { 0x00, 0x40, 0x14, 0x54 };

  if(u8Column != 0)
  {
    u8Column--;
  }

  if(u8Row != 0)
  {
    u8Row--;
  }

  if(u8Column > pkstrThisConfig->u8NumberOfColums)
  {
    u8Column = pkstrThisConfig->u8NumberOfColums;
  }

  if(u8Row > pkstrThisConfig->u8NumberOfRows)
  {
    u8Row = pkstrThisConfig->u8NumberOfRows;
  }

  if(u8Row >= sizeof(kau8RowOffset))
  {
    u8Row = (sizeof(kau8RowOffset) - 1);
  }

  LCD_astrDisplayData[kenuDeviceId].u8CurrentColumnPos = u8Column;
  LCD_astrDisplayData[kenuDeviceId].u8CurrentRowPos    = u8Row;

  return bSendCommand(kenuDeviceId, (LCD_u8SET_DDRAM_ADDR | (u8Column + kau8RowOffset[u8Row])));
}


/*--------------------------------------------------------------------------------------------------------------------*/
static bool bClearDisplay(const LCD_tenuDeviceId kenuDeviceId)
{
  bool bReturnCode  = false;

  if(bSendCommand(kenuDeviceId, LCD_u8CLEAR_DISPLAY))
  {
    CMN_vidDelayMs(CMN_2_MS);

    bReturnCode = bSetCursor(kenuDeviceId, 0, 0);
  }

  return bReturnCode;
}


/*--------------------------------------------------------------------------------------------------------------------*/
static bool bReturnHome(const LCD_tenuDeviceId kenuDeviceId)
{
  return bSendCommand(kenuDeviceId, LCD_u8RETURN_HOME);
}


/*--------------------------------------------------------------------------------------------------------------------*/
static bool bEnableBackLight(const LCD_tenuDeviceId kenuDeviceId, const bool kbEnable)
{
  if(kbEnable)
  {
    LCD_astrDisplayData[kenuDeviceId].u8BacklightLevel = LCD_u8BACKLIGHT;
  }
  else
  {
    LCD_astrDisplayData[kenuDeviceId].u8BacklightLevel = LCD_u8NO_BACKLIGHT;
  }

  return bSendToI2c(kenuDeviceId, 0);
}


/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/
void LCD_vidInitialize(void)
{
  bool                    bCommandStatus   = false;
  LCD_tenuDeviceId        enuLcdId         = 0;
  LCD_tstrLcdConfig const *pkstrThisConfig = NULL;
  tstrDisplayData         *pstrThisData    = NULL;

  CMN_vidDelayMs(CMN_50_MS);

  for(enuLcdId = 0; enuLcdId < LCD_eDEVICE_ID_END; enuLcdId++)
  {
    pkstrThisConfig = LCD_kpkstrGetLcdConfig(enuLcdId);
    CMN_assert(pkstrThisConfig != NULL);

    if(pkstrThisConfig->bEnable)
    {
      pstrThisData = &LCD_astrDisplayData[enuLcdId];
      CMN_assert(pstrThisData != NULL);

      pstrThisData->u8DisplayMode = (LCD_u8ENTRY_LEFT | LCD_u8ENTRY_SHIFT_DECREMENT);

      if(pkstrThisConfig->u8EnableBacklight)
      {
        pstrThisData->u8BacklightLevel = LCD_u8BACKLIGHT;
      }
      else
      {
        pstrThisData->u8BacklightLevel = LCD_u8NO_BACKLIGHT;
      }

      bCommandStatus = bSendToI2c(enuLcdId, 0);
      CMN_assert(bCommandStatus == true);

      pstrThisData->u8DisplayControl  |= (pkstrThisConfig->u8EnableBlink ? LCD_u8BLINK_ON : LCD_u8BLINK_OFF);
      pstrThisData->u8DisplayControl  |= (pkstrThisConfig->u8EnableCursor ? LCD_u8CURSOR_ON : LCD_u8CURSOR_OFF);
      pstrThisData->u8DisplayFunction |= (LCD_u82LINE | LCD_u84BIT_MODE | LCD_u85x8_DOTS);

      bCommandStatus = bWrite4Bits(enuLcdId, 0x03 << 4);
      CMN_assert(bCommandStatus == true);
      CMN_vidDelayMs(CMN_5_MS);
      bCommandStatus = bWrite4Bits(enuLcdId, 0x03 << 4);
      CMN_assert(bCommandStatus == true);
      CMN_vidDelayMs(CMN_5_MS);
      bCommandStatus = bWrite4Bits(enuLcdId, 0x03 << 4);
      CMN_assert(bCommandStatus == true);
      CMN_vidDelayMs(CMN_5_MS);
      bCommandStatus = bWrite4Bits(enuLcdId, 0x02 << 4);
      CMN_assert(bCommandStatus == true);
      CMN_vidDelayMs(CMN_5_MS);
      bCommandStatus = bSendCommand(enuLcdId, (pstrThisData->u8DisplayFunction | LCD_u8FUNCTION_SET));
      CMN_assert(bCommandStatus == true);
      bCommandStatus = bSendCommand(enuLcdId, (LCD_u8DISPLAY_CONTROL | pstrThisData->u8DisplayControl));
      bCommandStatus = bSetDisplayOn(enuLcdId, true);
      CMN_assert(bCommandStatus == true);
      bCommandStatus = bClearDisplay(enuLcdId);
      CMN_assert(bCommandStatus == true);
      bCommandStatus = bSendCommand(enuLcdId, (pstrThisData->u8DisplayMode | LCD_u8ENTRY_MODE_SET));
      CMN_assert(bCommandStatus == true);
      bCommandStatus = bReturnHome(enuLcdId);
      CMN_assert(bCommandStatus == true);

      pstrThisData->u16DataSize    = (pkstrThisConfig->u8NumberOfRows * pkstrThisConfig->u8NumberOfColums);
      pstrThisData->pau8DataBuffer = malloc(pstrThisData->u16DataSize * sizeof(uint8_t));
      CMN_assert(pstrThisData->pau8DataBuffer != NULL);

      memset(pstrThisData->pau8DataBuffer, 0, pstrThisData->u16DataSize);
    }
  }
}


/*--------------------------------------------------------------------------------------------------------------------*/
bool LCD_bIsDeviceIdValid(const LCD_tenuDeviceId kenuDeviceId)
{
  return bIsDeviceIdValid(kenuDeviceId);
}


/*--------------------------------------------------------------------------------------------------------------------*/
LCD_tenuStatus LCD_enuSetBacklightOn(const LCD_tenuDeviceId kenuDeviceId)
{
  LCD_tenuStatus          enuReturnCode    = LCD_eSTATUS_NO_OK;
  LCD_tstrLcdConfig const *pkstrThisConfig = NULL;

  if(!bIsDeviceIdValid(kenuDeviceId))
  {
    enuReturnCode = LCD_eSTATUS_INVALID_DEVICE_ID;
  }
  else
  {
    pkstrThisConfig = LCD_kpkstrGetLcdConfig(kenuDeviceId);

    if(!pkstrThisConfig->bEnable)
    {
      enuReturnCode = LCD_eSTATUS_DEVICE_IS_NOT_ENABLED;
    }
    else if(!bEnableBackLight(kenuDeviceId, true))
    {
      enuReturnCode = LCD_eSTATUS_NO_OK;
    }
    else
    {
      enuReturnCode = LCD_eSTATUS_OK;
    }
  }

  return enuReturnCode;
}


/*--------------------------------------------------------------------------------------------------------------------*/
LCD_tenuStatus LCD_enuSetBacklightOff(const LCD_tenuDeviceId kenuDeviceId)
{
  LCD_tenuStatus          enuReturnCode    = LCD_eSTATUS_NO_OK;
  LCD_tstrLcdConfig const *pkstrThisConfig = NULL;

  if(!bIsDeviceIdValid(kenuDeviceId))
  {
    enuReturnCode = LCD_eSTATUS_INVALID_DEVICE_ID;
  }
  else
  {
    pkstrThisConfig = LCD_kpkstrGetLcdConfig(kenuDeviceId);

    if(!pkstrThisConfig->bEnable)
    {
      enuReturnCode = LCD_eSTATUS_DEVICE_IS_NOT_ENABLED;
    }
    else if(!bEnableBackLight(kenuDeviceId, false))
    {
      enuReturnCode = LCD_eSTATUS_NO_OK;
    }
    else
    {
      enuReturnCode = LCD_eSTATUS_OK;
    }
  }

  return enuReturnCode;
}


/*--------------------------------------------------------------------------------------------------------------------*/
LCD_tenuStatus LCD_enuSetCursor(const LCD_tenuDeviceId kenuDeviceId, uint8_t u8Column, uint8_t u8Row)
{
  LCD_tenuStatus          enuReturnCode    = LCD_eSTATUS_NO_OK;
  LCD_tstrLcdConfig const *pkstrThisConfig = NULL;

  if(!bIsDeviceIdValid(kenuDeviceId))
  {
    enuReturnCode = LCD_eSTATUS_INVALID_DEVICE_ID;
  }
  else
  {
    pkstrThisConfig = LCD_kpkstrGetLcdConfig(kenuDeviceId);

    if(!pkstrThisConfig->bEnable)
    {
      enuReturnCode = LCD_eSTATUS_DEVICE_IS_NOT_ENABLED;
    }
    else
    {
      if(u8Column > pkstrThisConfig->u8NumberOfColums)
      {
        u8Column = pkstrThisConfig->u8NumberOfColums;
      }

      if(u8Row > pkstrThisConfig->u8NumberOfRows)
      {
        u8Row = pkstrThisConfig->u8NumberOfRows;
      }

      if(bSetCursor(kenuDeviceId, u8Column, u8Row))
      {
        enuReturnCode = LCD_eSTATUS_OK;
      }
    }
  }

  return enuReturnCode;
}


/*--------------------------------------------------------------------------------------------------------------------*/
LCD_tenuStatus LCD_enuWriteText(const LCD_tenuDeviceId kenuDeviceId, const char *kps8Text)
{
  LCD_tenuStatus          enuReturnCode    = LCD_eSTATUS_NO_OK;
  LCD_tstrLcdConfig const *pkstrThisConfig = NULL;

  if(!bIsDeviceIdValid(kenuDeviceId))
  {
    enuReturnCode = LCD_eSTATUS_INVALID_DEVICE_ID;
  }
  else
  {
    pkstrThisConfig = LCD_kpkstrGetLcdConfig(kenuDeviceId);

    if(!pkstrThisConfig->bEnable)
    {
      enuReturnCode = LCD_eSTATUS_DEVICE_IS_NOT_ENABLED;
    }
    else if(kps8Text == NULL)
    {
      enuReturnCode = LCD_eSTATUS_NULL_POINTER;
    }
    else if(bWriteText(kenuDeviceId, kps8Text, (uint8_t)strlen(kps8Text)))
    {
      enuReturnCode = LCD_eSTATUS_OK;
    }
  }

  return enuReturnCode;
}


/*--------------------------------------------------------------------------------------------------------------------*/
LCD_tenuStatus LCD_enuPrintf(const LCD_tenuDeviceId kenuDeviceId, char const * const kpks8Format, ...)
{
  LCD_tenuStatus          enuReturnCode    = LCD_eSTATUS_NO_OK;
  LCD_tstrLcdConfig const *pkstrThisConfig = NULL;
  tstrDisplayData         *pstrThisData    = NULL;
  int16_t                 s16WrittenData   = 0;
  va_list                 args;

  if(!bIsDeviceIdValid(kenuDeviceId))
  {
    enuReturnCode = LCD_eSTATUS_INVALID_DEVICE_ID;
  }
  else if(kpks8Format == NULL)
  {
    enuReturnCode = LCD_eSTATUS_NULL_POINTER;
  }
  else
  {
    pkstrThisConfig = LCD_kpkstrGetLcdConfig(kenuDeviceId);
    pstrThisData    = &LCD_astrDisplayData[kenuDeviceId];

    if(!pkstrThisConfig->bEnable)
    {
      enuReturnCode = LCD_eSTATUS_DEVICE_IS_NOT_ENABLED;
    }
    else
    {
      va_start(args, kpks8Format);
      s16WrittenData = vsnprintf((char *)pstrThisData->pau8DataBuffer, pstrThisData->u16DataSize, kpks8Format, args);
      va_end(args);

      if(s16WrittenData < 0)
      {
        enuReturnCode = LCD_eSTATUS_PRINTF_ERROR;
      }
      else if(strlen((char *)pstrThisData->pau8DataBuffer) != s16WrittenData)
      {
        enuReturnCode = LCD_eSTATUS_INVALID_DIGITS_NUMBER;
      }
      else if(!bWriteText(kenuDeviceId, (char *)pstrThisData->pau8DataBuffer, (uint16_t)s16WrittenData))
      {
        enuReturnCode = LCD_eSTATUS_NO_OK;
      }
      else
      {
        enuReturnCode = LCD_eSTATUS_OK;
      }
    }
  }

  return enuReturnCode;
}


/*--------------------------------------------------------------------------------------------------------------------*/
LCD_tenuStatus LCD_enuClearAll(const LCD_tenuDeviceId kenuDeviceId)
{
  LCD_tenuStatus          enuReturnCode    = LCD_eSTATUS_NO_OK;
  LCD_tstrLcdConfig const *pkstrThisConfig = NULL;
  uint8_t                 u8DigitIdx       = 0;
  bool                    bStatus          = false;

  if(!bIsDeviceIdValid(kenuDeviceId))
  {
    enuReturnCode = LCD_eSTATUS_INVALID_DEVICE_ID;
  }
  else
  {
    pkstrThisConfig = LCD_kpkstrGetLcdConfig(kenuDeviceId);

    if(bSetCursor(kenuDeviceId, 1, 1))
    {
      for(u8DigitIdx = 0, bStatus = true; ((u8DigitIdx < pkstrThisConfig->u8NumberOfColums) && (bStatus == true)); u8DigitIdx++)
      {
        bStatus = bSendData(kenuDeviceId, ' ');
      }

      if(bStatus && bSetCursor(kenuDeviceId, 1, 2))
      {
        for(u8DigitIdx = 0, bStatus = true; ((u8DigitIdx < pkstrThisConfig->u8NumberOfColums) && (bStatus == true)); u8DigitIdx++)
        {
          bStatus = bSendData(kenuDeviceId, ' ');
        }
      }

      if(bStatus)
      {
        enuReturnCode = LCD_eSTATUS_OK;
      }
    }
  }

  return enuReturnCode;
}


/*--------------------------------------------------------------------------------------------------------------------*/
LCD_tenuStatus LCD_enuBlinkOn(const LCD_tenuDeviceId kenuDeviceId)
{
  LCD_tenuStatus enuReturnCode = LCD_eSTATUS_NO_OK;

  if(!bIsDeviceIdValid(kenuDeviceId))
  {
    enuReturnCode = LCD_eSTATUS_INVALID_DEVICE_ID;
  }
  else
  {
    LCD_astrDisplayData[kenuDeviceId].u8DisplayControl |= LCD_u8BLINK_ON;

    if(bSendCommand(kenuDeviceId, (LCD_u8DISPLAY_CONTROL | LCD_astrDisplayData[kenuDeviceId].u8DisplayControl)))
    {
      enuReturnCode = LCD_eSTATUS_OK;
    }
  }

  return enuReturnCode;
}


/*--------------------------------------------------------------------------------------------------------------------*/
LCD_tenuStatus LCD_enuBlinkOff(const LCD_tenuDeviceId kenuDeviceId)
{
  LCD_tenuStatus enuReturnCode = LCD_eSTATUS_NO_OK;

  if(!bIsDeviceIdValid(kenuDeviceId))
  {
    enuReturnCode = LCD_eSTATUS_INVALID_DEVICE_ID;
  }
  else
  {
    LCD_astrDisplayData[kenuDeviceId].u8DisplayControl &= ~LCD_u8BLINK_ON;

    if(bSendCommand(kenuDeviceId, (LCD_u8DISPLAY_CONTROL | LCD_astrDisplayData[kenuDeviceId].u8DisplayControl)))
    {
      enuReturnCode = LCD_eSTATUS_OK;
    }
  }

  return enuReturnCode;
}


/*--------------------------------------------------------------------------------------------------------------------*/
LCD_tenuStatus LCD_enuCursorOn(const LCD_tenuDeviceId kenuDeviceId)
{
  LCD_tenuStatus enuReturnCode = LCD_eSTATUS_NO_OK;

  if(!bIsDeviceIdValid(kenuDeviceId))
  {
    enuReturnCode = LCD_eSTATUS_INVALID_DEVICE_ID;
  }
  else
  {
    LCD_astrDisplayData[kenuDeviceId].u8DisplayControl |= LCD_u8CURSOR_ON;

    if(bSendCommand(kenuDeviceId, (LCD_u8DISPLAY_CONTROL | LCD_astrDisplayData[kenuDeviceId].u8DisplayControl)))
    {
        enuReturnCode = LCD_eSTATUS_OK;
    }
  }

  return enuReturnCode;
}


/*--------------------------------------------------------------------------------------------------------------------*/
LCD_tenuStatus LCD_enuCursorOff(const LCD_tenuDeviceId kenuDeviceId)
{
  LCD_tenuStatus enuReturnCode = LCD_eSTATUS_NO_OK;

  if(!bIsDeviceIdValid(kenuDeviceId))
  {
    enuReturnCode = LCD_eSTATUS_INVALID_DEVICE_ID;
  }
  else
  {
    LCD_astrDisplayData[kenuDeviceId].u8DisplayControl &= ~LCD_u8CURSOR_ON;

    if(bSendCommand(kenuDeviceId, (LCD_u8DISPLAY_CONTROL | LCD_astrDisplayData[kenuDeviceId].u8DisplayControl)))
    {
      enuReturnCode = LCD_eSTATUS_OK;
    }
  }

  return enuReturnCode;
}


/*--------------------------------------------------------------------------------------------------------------------*/
LCD_tenuStatus LCD_enuDisplayOn(const LCD_tenuDeviceId kenuDeviceId)
{
  LCD_tenuStatus enuReturnCode = LCD_eSTATUS_NO_OK;

  if(!bIsDeviceIdValid(kenuDeviceId))
  {
    enuReturnCode = LCD_eSTATUS_INVALID_DEVICE_ID;
  }
  else
  {
    LCD_astrDisplayData[kenuDeviceId].u8DisplayControl |= LCD_u8DISPLAY_ON;

    if(bSendCommand(kenuDeviceId, (LCD_u8DISPLAY_CONTROL | LCD_astrDisplayData[kenuDeviceId].u8DisplayControl)))
    {
      enuReturnCode = LCD_eSTATUS_OK;
    }
  }

  return enuReturnCode;
}


/*--------------------------------------------------------------------------------------------------------------------*/
LCD_tenuStatus LCD_enuDisplayOff(const LCD_tenuDeviceId kenuDeviceId)
{
  LCD_tenuStatus enuReturnCode = LCD_eSTATUS_NO_OK;

  if(!bIsDeviceIdValid(kenuDeviceId))
  {
    enuReturnCode = LCD_eSTATUS_INVALID_DEVICE_ID;
  }
  else
  {
    LCD_astrDisplayData[kenuDeviceId].u8DisplayControl &= ~LCD_u8DISPLAY_ON;

    if(bSendCommand(kenuDeviceId, (LCD_u8DISPLAY_CONTROL | LCD_astrDisplayData[kenuDeviceId].u8DisplayControl)))
    {
      enuReturnCode = LCD_eSTATUS_OK;
    }
  }

  return enuReturnCode;
}


/*--------------------------------------------------------------------------------------------------------------------*/
