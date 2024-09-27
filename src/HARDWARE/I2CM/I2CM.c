/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      I2CM.c
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



/**********************************************************************************************************************/
/* INCLUDE FILES                                                                                                      */
/**********************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "CLOCK.h"
#include "I2CM.h"


/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/
#define I2C_RW_BIT                                          0x01


/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* PRIVATE VARIABLES                                                                                                  */
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS PROTOTYPES                                                                                       */
/**********************************************************************************************************************/
static bool bI2cReservedAddress(const uint8_t ku8I2cAddress);


/**********************************************************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
static bool bI2cReservedAddress(const uint8_t ku8I2cAddress)
{
  return ((ku8I2cAddress & 0x78) == 0) || ((ku8I2cAddress & 0x78) == 0x78);
}


/*--------------------------------------------------------------------------------------------------------------------*/
static void vidI2c1IsrFlagPolling(void)
{
  /* Polling Interrupt Flag */
  while(!PIR3bits.SSP1IF);

  /* Clear Interrupt Flag */
  PIR3bits.SSP1IF = 0;
}


/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/
void I2CM_vidInitalize(void)
{
  /* PPS setting for using RB1 as SCL */
  SSP1CLKPPS = 0x09;
  RB1PPS     = 0x0F;

  /* PPS setting for using RB2 as SDA */
  SSP1DATPPS = 0x0A;
  RB2PPS     = 0x10;

  /* Set pins RB1 and RB2 as Digital */
  ANSELBbits.ANSELB1 = 0;
  ANSELBbits.ANSELB2 = 0;

  /* Set pull-up resistorsfor RB1 and RB2 */
  WPUBbits.WPUB1 = 1;
  WPUBbits.WPUB2 = 1;

  /* I2C Master Mode: Clock = F_OSC / (4 * (SSP1ADD + 1)) */
  SSP1CON1bits.SSPM3 = 1;

  /* Set the boud rate devider to obtain the I2C clock at 400000 Hz*/
  SSP1ADD = 39;
}


/*--------------------------------------------------------------------------------------------------------------------*/
I2CM_tenuStatus I2CM_enuWriteBuffer(const I2CM_tenuI2cId kenuI2cId,
                                        const uint8_t ku8I2cSlaveAddress,
                                        uint8_t const * const kpku8TxBuffer,
                                        const uint16_t ku16TxBufferSize)
{
  I2CM_tenuStatus enuStatus = I2CM_eSTATUS_OK;
  uint16_t            u16Index  = 0;

  if(kenuI2cId >= I2CM_I2C_ID_COUNT)
  {
    enuStatus = I2CM_eSTATUS_INVALID_I2C_ID;
  }
  else if(bI2cReservedAddress(ku8I2cSlaveAddress))
  {
    enuStatus = I2CM_eSTATUS_INVALID_SLAVE_ADDRESS;
  }
  else if(kpku8TxBuffer == NULL)
  {
    enuStatus = I2CM_eSTATUS_NULL_POINTER;
  }
  else if(ku16TxBufferSize == 0)
  {
    enuStatus = I2CM_eSTATUS_EMPTY_BUFFER;
  }
  else
  {
    /* Clear IRQ */
    PIR3bits.SSP1IF = 0;

    /* I2C Master Open */
    SSP1CON1bits.SSPEN = 1;

    /* START Condition*/
    SSP1CON2bits.SEN = 1;

    vidI2c1IsrFlagPolling();

    SSP1BUF  = (uint8_t)((ku8I2cSlaveAddress << 1) & ~I2C_RW_BIT);

    vidI2c1IsrFlagPolling();

    if(SSP1CON2bits.ACKSTAT)
    {
        enuStatus = I2CM_eSTATUS_NO_ACKED;

        /* Disable I2C1 */
        SSP1CON1bits.SSPEN = 0;
    }
    else
    {
      for(u16Index = 0, enuStatus = I2CM_eSTATUS_OK; ((u16Index < ku16TxBufferSize) && (enuStatus == I2CM_eSTATUS_OK)); u16Index++)
      {
        SSP1BUF = kpku8TxBuffer[u16Index];

        vidI2c1IsrFlagPolling();

        if(SSP1CON2bits.ACKSTAT)
        {
          enuStatus = I2CM_eSTATUS_NO_OK;

          /* Disable I2C1 */
          SSP1CON1bits.SSPEN = 0;
        }
      }

      /* STOP Condition */
      SSP1CON2bits.PEN = 1;

      vidI2c1IsrFlagPolling();

      /* Disable I2C1 */
      SSP1CON1bits.SSPEN = 0;
    }
  }

  return enuStatus;
}


/*--------------------------------------------------------------------------------------------------------------------*/
I2CM_tenuStatus I2CM_enuReadBuffer(const I2CM_tenuI2cId kenuI2cId,
                                       const uint8_t ku8I2cSlaveAddress,
                                       uint8_t const * const kpku8TxBuffer,
                                       const uint16_t ku16TxBufferSize,
                                       uint8_t * const kpu8RxBuffer,
                                       const uint16_t ku16RxBufferSize)
{
  I2CM_tenuStatus enuStatus        = I2CM_eSTATUS_OK;
  uint16_t            u16Index         = 0;
  uint16_t            u16RxBufferIndex = 0;

  if(kenuI2cId >= I2CM_I2C_ID_COUNT)
  {
    enuStatus = I2CM_eSTATUS_INVALID_I2C_ID;
  }
  else if(bI2cReservedAddress(ku8I2cSlaveAddress))
  {
    enuStatus = I2CM_eSTATUS_INVALID_SLAVE_ADDRESS;
  }
  else if((kpku8TxBuffer == NULL) || (kpu8RxBuffer == NULL))
  {
    enuStatus = I2CM_eSTATUS_NULL_POINTER;
  }
  else if((ku16TxBufferSize == 0) || (ku16RxBufferSize == 0))
  {
    enuStatus = I2CM_eSTATUS_EMPTY_BUFFER;
  }
  else
  {
    /* Clear IRQ */
    PIR3bits.SSP1IF = 0;

    /* I2C Master Open */
    SSP1CON1bits.SSPEN = 1;

    /* START Condition */
    SSP1CON2bits.SEN = 1;

    vidI2c1IsrFlagPolling();

    // Sending the slave address in write mode:
    SSP1BUF  = (uint8_t)((ku8I2cSlaveAddress << 1) & ~I2C_RW_BIT);

    vidI2c1IsrFlagPolling();

    if(SSP1CON2bits.ACKSTAT)
    {
      enuStatus = I2CM_eSTATUS_NO_ACKED;

      /* Disable I2C1 */
      SSP1CON1bits.SSPEN = 0;
    }
    else
    {
      for(u16Index = 0, enuStatus = I2CM_eSTATUS_OK; ((u16Index < ku16TxBufferSize) && (enuStatus == I2CM_eSTATUS_OK)); u16Index++)
      {
        SSP1BUF = kpku8TxBuffer[u16Index];

        vidI2c1IsrFlagPolling();

        if(SSP1CON2bits.ACKSTAT)
        {
          enuStatus = I2CM_eSTATUS_NO_OK;

          /* Disable I2C1 */
          SSP1CON1bits.SSPEN = 0;
        }
      }

      if(enuStatus == I2CM_eSTATUS_OK)
      {
        /* STOP Condition */
        SSP1CON2bits.PEN = 1;

        vidI2c1IsrFlagPolling();

        /* START Condition*/
        SSP1CON2bits.SEN = 1;

        vidI2c1IsrFlagPolling();

        // Sending the slave address in read mode:
        SSP1BUF  = (uint8_t)((ku8I2cSlaveAddress << 1) | I2C_RW_BIT);

        vidI2c1IsrFlagPolling();

        if(SSP1CON2bits.ACKSTAT)
        {
          enuStatus = I2CM_eSTATUS_NO_ACKED;

          /* Disable I2C1 */
          SSP1CON1bits.SSPEN = 0;
        }
        else
        {
          while(u16RxBufferIndex < ku16RxBufferSize)
          {
            // The ACK is received from the slave, then let continue by enabling the master in receiver mode:
            SSP1CON2bits.RCEN = 1;

            // After the eighth falling edge of SCL, SSPxIF and BF are set:
            vidI2c1IsrFlagPolling();

            // Master clears SSPxIF and reads the received byte from SSPUF which clears BF:
            kpu8RxBuffer[u16RxBufferIndex] = SSP1BUF;

            if(u16RxBufferIndex  == (ku16RxBufferSize - 1))
            {
              // Master sends a not ACK or Stop to end communication:
              SSP1CON2bits.ACKDT = 1;
              SSP1CON2bits.ACKEN = 1;
            }
            else
            {
              // Master sets the ACK value to be sent to slave in the ACKDT bit and initiates the ACK by setting the ACKEN bit:
              SSP1CON2bits.ACKDT = 0;
              SSP1CON2bits.ACKEN = 1;
            }

            // Master?s ACK is clocked out to the slave and SSPxIF is set:
            // User clears SSPxIF;
            vidI2c1IsrFlagPolling();

            u16RxBufferIndex++;
          }

          /* STOP Condition */
          SSP1CON2bits.PEN = 1;

          vidI2c1IsrFlagPolling();

          /* Disable I2C1 */
          SSP1CON1bits.SSPEN = 0;
        }
      }
    }
  }

  return enuStatus;
}


/*--------------------------------------------------------------------------------------------------------------------*/
