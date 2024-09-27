/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      ADC_cfg.h
 *
 * @author    Jean DEBAINS
 * @date      Wednesday, January 31, 2024.
 *
 * @version   0.0.0
 *
 * @brief     ADC Hardware conf part
 * @details   Module in charge of the management of the ADC peripheral
 *
 * @remark    Coding Language: C
 *
 * @copyright Copyright (c) 2024 This software is used for education proposal
 *
 ***********************************************************************************************************************
 */
#ifndef ADC_CFG_H_
#define ADC_CFG_H_


/**********************************************************************************************************************/
/* INCLUDE FILES                                                                                                      */
/**********************************************************************************************************************/
#include "Common.h"


/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/
/**
 * @brief Defines whether the ADC is connected to the potentiometer (RA0 pin) or in the RB3 pin
 */
#define ADC_CONFIG_USE_POTENTIOMETER                        true


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Defines the register address of the TRIS* pin used by the ADC
 * @remark The value "0x0F87" is for the potentiometer (RA0 pin) in the Curiosity board
 * @remark The value "0x0F88" is for the potentiometer (RB3 pin) in the Curiosity board
 */
#if(ADC_CONFIG_USE_POTENTIOMETER == true)
#  define ADC_CONFIG_REG_ADDRESS_TRIS                       0x0F87
#elif(ADC_CONFIG_USE_POTENTIOMETER == false)
#  define ADC_CONFIG_REG_ADDRESS_TRIS                       0x0F88
#endif //ADC_CONFIG_USE_POTENTIOMETER


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Defines the register address of the ANSEL* pin used by the ADC
 * @remark The value "0x0F0C" is for the potentiometer (RA0 pin) in the Curiosity board
 * @remark The value "0x0F14" is for the potentiometer (RB3 pin) in the Curiosity board
 */
#if(ADC_CONFIG_USE_POTENTIOMETER == true)
#  define ADC_CONFIG_REG_ADDRESS_ANSEL                      0x0F0C
#elif(ADC_CONFIG_USE_POTENTIOMETER == false)
#  define ADC_CONFIG_REG_ADDRESS_ANSEL                      0x0F14
#endif //ADC_CONFIG_USE_POTENTIOMETER


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Defines the ADPCH channel used to connect the ADC to one of the available pin
 * @remark The value "0b00000000" is for the potentiometer (RA0 pin) in the Curiosity board
 * @remark The value "0b00001011" is for the potentiometer (RB3 pin) in the Curiosity board
 */
#if(ADC_CONFIG_USE_POTENTIOMETER == true)
#  define ADC_CONFIG_CHANNEL_ADPCH                          0b00000000
#elif(ADC_CONFIG_USE_POTENTIOMETER == false)
#  define ADC_CONFIG_CHANNEL_ADPCH                          0b00001011
#endif //ADC_CONFIG_USE_POTENTIOMETER


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Defines the position of the TRIS* register (a.k.a the ID of the pin)
 * @remark The value "_TRISA_TRISA0_POSITION" is for the potentiometer (RA0 pin) in the Curiosity board
 * @remark The value "_TRISB_TRISB3_POSITION" is for the potentiometer (RB3 pin) in the Curiosity board
 */
#if(ADC_CONFIG_USE_POTENTIOMETER == true)
#  define ADC_CONFIG_CHANNEL_PORT                           _TRISA_TRISA0_POSITION
#elif(ADC_CONFIG_USE_POTENTIOMETER == false)
#  define ADC_CONFIG_CHANNEL_PORT                           _TRISB_TRISB3_POSITION
#endif //ADC_CONFIG_USE_POTENTIOMETER


/*--------------------------------------------------------------------------------------------------------------------*/
#endif /* ADC_CFG_H_ */
/*--------------------------------------------------------------------------------------------------------------------*/
