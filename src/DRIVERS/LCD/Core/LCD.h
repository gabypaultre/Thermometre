/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      LCD.h
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
#ifndef LCD_H_
#define LCD_H_


/**********************************************************************************************************************/
/* INCLUDE FILES                                                                                                      */
/**********************************************************************************************************************/
#include "Common.h"
#include "LCD_cfg.h"



/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
/**
 * @enum LCD_tenuStatus
 * @brief Enum to set the list all return codes.
 */
typedef enum LCD_tenuStatus
{
  LCD_eSTATUS_OK                                            = 0,  //!< Everything is OK
  LCD_eSTATUS_NO_OK,                                              //!< Generic/default error code
  LCD_eSTATUS_INVALID_DEVICE_ID,                                  //!< The passed LCD ID is out of the allowed range
  LCD_eSTATUS_INVALID_DIGITS_NUMBER,                              //!< The needed digits to display a number is invalid
  LCD_eSTATUS_DEVICE_IS_NOT_ENABLED,                              //!< The handled device is not enabled
  LCD_eSTATUS_NULL_POINTER,                                       //!< The pointer passed as an argument is NULL
  LCD_eSTATUS_PRINTF_ERROR,                                       //!< The printf function has failed to format the data
  LCD_eSTATUS_COUNT                                               //!< The total return code available
}LCD_tenuStatus;


/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
/**
 * @brief Function used to initialize each LCD declared in the configuration file.
 */
void LCD_vidInitialize(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to check if a LCD device ID is in a valid range or not
 * @param[in] kenuDeviceId The LCD device ID to be checked
 * @return Returns "true" if the ID is between ] @ref LCD_eDEVICE_ID_BEGIN ; @ref LCD_eDEVICE_ID_END [, returns "false"
 *         otherwise
*/
bool LCD_bIsDeviceIdValid(const LCD_tenuDeviceId kenuDeviceId);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to enable the backlight of the LCD.
 * @param[in] kenuDeviceId The ID of the LCD.
 * @return Return @ref LCD_eSTATUS_OK if the function ran successfully, return other codes in the other cases.
 */
LCD_tenuStatus LCD_enuSetBacklightOn(const LCD_tenuDeviceId kenuDeviceId);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to disable the backlight of the LCD.
 * @param[in] kenuDeviceId The ID of the LCD.
 * @return Return @ref LCD_eSTATUS_OK if the function ran successfully, return other codes in the other cases.
 */
LCD_tenuStatus LCD_enuSetBacklightOff(const LCD_tenuDeviceId kenuDeviceId);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to set the cursor position where a text has to be written.
 * @param[in] kenuDeviceId The ID of the LCD.
 * @param[in]     u8Column The column to be set.
 * @param[in]        u8Row The row to be set.
 * @return Return @ref LCD_eSTATUS_OK if the function ran successfully, return other codes in the other cases.
 */
LCD_tenuStatus LCD_enuSetCursor(const LCD_tenuDeviceId kenuDeviceId, uint8_t u8Column, uint8_t u8Row);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to print text on the LCD.
 * @param[in] kenuDeviceId The ID of the LCD.
 * @param[in]    kpks8Text The text to be printed
 * @return Return @ref LCD_eSTATUS_OK if the function ran successfully, return other codes in the other cases.
 */
LCD_tenuStatus LCD_enuWriteText(const LCD_tenuDeviceId kenuDeviceId, char const * const kpks8Text);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to emulate a printf function redirected to the LCD
 * @param[in] kenuDeviceId The ID of the LCD.
 * @param[in]  kpks8Format It is a string that specifies the data to be printed. It may also contain a format specifier to
 *                         print the value of any variable such as a character and an integer
 * @param[in]         ... These are the variable names corresponding to the format specifier
 * @return Return @ref LCD_eSTATUS_OK if the function ran successfully, return other codes in the other cases.
 */
LCD_tenuStatus LCD_enuPrintf(const LCD_tenuDeviceId kenuDeviceId, char const * const kpks8Format, ...);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to clear the LCD display
 * @param[in] kenuDeviceId The ID of the LCD.
 * @return Return @ref LCD_eSTATUS_OK if the function ran successfully, return other codes in the other cases.
 */
LCD_tenuStatus LCD_enuClearAll(const LCD_tenuDeviceId kenuDeviceId);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to enable the cursor blink of the LCD.
 * @param[in] kenuDeviceId The ID of the LCD.
 * @return Return @ref LCD_eSTATUS_OK if the function ran successfully, return other codes in the other cases.
 */
LCD_tenuStatus LCD_enuBlinkOn(const LCD_tenuDeviceId kenuDeviceId);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to disable the cursor blink of the LCD.
 * @param[in] kenuDeviceId The ID of the LCD.
 * @return Return @ref LCD_eSTATUS_OK if the function ran successfully, return other codes in the other cases.
 */
LCD_tenuStatus LCD_enuBlinkOff(const LCD_tenuDeviceId kenuDeviceId);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to enable the cursor of the LCD.
 * @param[in] kenuDeviceId The ID of the LCD.
 * @return Return @ref LCD_eSTATUS_OK if the function ran successfully, return other codes in the other cases.
 */
LCD_tenuStatus LCD_enuCursorOn(const LCD_tenuDeviceId kenuDeviceId);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to disable the cursor of the LCD.
 * @param[in] kenuDeviceId The ID of the LCD.
 * @return Return @ref LCD_eSTATUS_OK if the function ran successfully, return other codes in the other cases.
 */
LCD_tenuStatus LCD_enuCursorOff(const LCD_tenuDeviceId kenuDeviceId);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to enable the display of the LCD.
 * @param[in] kenuDeviceId The ID of the LCD.
 * @return Return @ref LCD_eSTATUS_OK if the function ran successfully, return other codes in the other cases.
 */
LCD_tenuStatus LCD_enuDisplayOn(const LCD_tenuDeviceId kenuDeviceId);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to disable the display of the LCD.
 * @param[in] kenuDeviceId The ID of the LCD.
 * @return Return @ref LCD_eSTATUS_OK if the function ran successfully, return other codes in the other cases.
 */
LCD_tenuStatus LCD_enuDisplayOff(const LCD_tenuDeviceId kenuDeviceId);


/*--------------------------------------------------------------------------------------------------------------------*/
#endif /* LCD_H_ */
/*--------------------------------------------------------------------------------------------------------------------*/
