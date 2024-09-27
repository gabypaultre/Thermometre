/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      I2CM.h
 *
 * @author    Jean DEBAINS
 * @date      Wednesday, January 31, 2024.
 *
 * @version   0.0.0
 *
 * @brief     I2CM Hardware core part
 * @details   Module in charge of the management of the I2C peripheral as a master
 *
 * @remark    Coding Language: C
 *
 * @copyright Copyright (c) 2024 This software is used for education proposal
 *
 ***********************************************************************************************************************
 */
#ifndef I2CM_H_
#define I2CM_H_


/**********************************************************************************************************************/
/* INCLUDE FILES                                                                                                      */
/**********************************************************************************************************************/
#include "Common.h"


/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
/**
 * @brief Enum to set the list all error codes
 */
typedef enum I2CM_tenuStatus
{
  I2CM_eSTATUS_OK                                           = 0,
  I2CM_eSTATUS_NO_OK,
  I2CM_eSTATUS_INVALID_I2C_ID,
  I2CM_eSTATUS_INVALID_SLAVE_ADDRESS,
  I2CM_eSTATUS_NULL_POINTER,
  I2CM_eSTATUS_EMPTY_BUFFER,
  I2CM_eSTATUS_NO_ACKED,
  I2CM_eSTATUS_COUNT
}I2CM_tenuStatus;


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Enum used to list all I2C instances (not used here, only the the standardization syntax)
 */
typedef enum I2CM_tenuI2cId
{
  I2CM_I2C_ID0                                              = 0,
  I2CM_I2C_ID_COUNT
}I2CM_tenuI2cId;


/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
/**
 * @brief Initialise the I2C Bus
 */
void I2CM_vidInitalize();


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to send data to the I2C bus
 * @param kenuI2cId: ID of the I2C instance
 * @param ku8I2cSlaveAddress: I2C slave address
 * @param kpku8Buffer: Pointer to the buffer which contains the data to be send
 * @param ku16BufferSize: Size of the buffer
 * @return 0 if the function ran successfully, other value in case of any error
 */
I2CM_tenuStatus I2CM_enuWriteBuffer(const I2CM_tenuI2cId kenuI2cId,
                                        const uint8_t ku8I2cSlaveAddress,
                                        uint8_t const * const kpku8Buffer,
                                        const uint16_t ku16BufferSize);


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Function used to send data to the I2C bus and to reveive a data
 * @param kenuI2cId: ID of the I2C instance
 * @param ku8I2cSlaveAddress: I2C slave address
 * @param kpku8TxBuffer: Pointer to the Tx buffer which contains the data to be send
 * @param ku16TxBufferSize: Size of the Tx buffer
 * @param kpu8RxBuffer Pointer to the buffer used to contain the received data
 * @param ku16RxBufferSize Size of the Rx buffer
 * @return 0 if the function ran successfully, other value in case of any error
 */
I2CM_tenuStatus I2CM_enuReadBuffer(const I2CM_tenuI2cId kenuI2cId,
                                       const uint8_t ku8I2cSlaveAddress,
                                       uint8_t const * const kpku8TxBuffer,
                                       const uint16_t ku16TxBufferSize,
                                       uint8_t * const kpu8RxBuffer,
                                       const uint16_t ku16RxBufferSize);


/*--------------------------------------------------------------------------------------------------------------------*/
#endif /* I2CM_H_ */
/*--------------------------------------------------------------------------------------------------------------------*/
