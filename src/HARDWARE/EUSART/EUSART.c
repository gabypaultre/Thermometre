/**
 ***********************************************************************************************************************
 * Company: Esme Sudria
 * Project: Projet Esme
 *
 ***********************************************************************************************************************
 * @file      EUSART.c
 *
 * @author    Jean DEBAINS
 * @date      Wednesday, January 31, 2024.
 *
 * @version   0.0.0
 *
 * @brief     EUSART Hardware core part
 * @details   Module in charge of the management of the EUSART2 peripheral connected of the pins RD0 (TX) and RD1 (RX)
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
#include "CLOCK.h"
#include "ISR.h"
#include "EUSART.h"


/**********************************************************************************************************************/
/* CONSTANTS, MACROS                                                                                                  */
/**********************************************************************************************************************/
/**
 * @brief This macro is used to check whether the Fosc frequency is compliant to make this peripheral working
 */
#if((CLOCK_CONFIG_FOSC_FREQUENCY_MHZ != CLOCK_FOSC_FREQUENCY_01MHZ) &&                                                 \
    (CLOCK_CONFIG_FOSC_FREQUENCY_MHZ != CLOCK_FOSC_FREQUENCY_04MHZ) &&                                                 \
    (CLOCK_CONFIG_FOSC_FREQUENCY_MHZ != CLOCK_FOSC_FREQUENCY_08MHZ) &&                                                 \
    (CLOCK_CONFIG_FOSC_FREQUENCY_MHZ != CLOCK_FOSC_FREQUENCY_32MHZ))
  #error This module can work only with a clock between 1 and 8 MHz
#endif //CLOCK_CONFIG_FOSC_FREQUENCY_MHZ


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief This macro is used to configure the baudrate to be used by the peripheral
 */
#define SELECTED_BAUDRATE                                   eBAUDRATE_115200


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief Defines the time to wait before considering a reception of a RX data from the ISR as failed
 */
#define RECEIVE_TIMEOUT_FROM_ISR_MS                         CMN_10_MS


/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
/**
 * @brief This enumeration is used to define the list of the allowed clock values for the Fosc to make this peripheral
 *        working
 */
typedef enum tenuClockFreqId
{
  eFOSC1MHZ                                                 = 0,
  eFOSC4MHZ,
  eFOSC8MHZ,
  eFOSC32MHZ,
  eFOSC_COUNT
}tenuClockFreqId;


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief This enumeration is used to define the allowed baudrate which could be used
 */
typedef enum tenuBaudRate
{
  eBAUDRATE_300                                             = 0,
  eBAUDRATE_1200,
  eBAUDRATE_2400,
  eBAUDRATE_9600,
  eBAUDRATE_10417,
  eBAUDRATE_19200,
  eBAUDRATE_57600,
  eBAUDRATE_115200,
  eBAUDRATE_COUNT
}tenuBaudRate;


/**********************************************************************************************************************/
/* PRIVATE VARIABLES                                                                                                  */
/**********************************************************************************************************************/
/**
 * @brief This array of array is used to set the desired baudrate accordingly to toe Fosc
 * @details See "PIC18F47Q10 - Datasheet", P.509, 28.2"
 */
static const uint16_t EUSART_kau16BaudRateCoef[eFOSC_COUNT][eBAUDRATE_COUNT] =
{
  { 832,   207,  103,  25,  23,  12,  0,   0  }, //eFOSC1MHZ
  { 3332,  832,  416,  103, 95,  51,  16,  8  }, //eFOSC4MHZ
  { 6666,  1666, 832,  207, 191, 103, 34,  16 }, //eFOSC8MHZ
  { 26666, 6666, 3332, 832, 767, 416, 138, 68 }, //eFOSC32MHZ
};


/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief This pointer of function is used to be called if there was an user function registered previously and once a
 *        Rx data will be received
 */
static EUSART_tpfvidRxCallback EUSART_pfRxCallback        = NULL;


/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS PROTOTYPES                                                                                       */
/**********************************************************************************************************************/
/**
 * @brief Function used to be registered by the interruption module to be called once an interruption is triggered, if
 *        the interruption is from the EUSART then this function will perform the needed actions
 */
static bool bInterruptHandler(void);


/**********************************************************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
static bool bInterruptHandler(void)
{
  EUSART_tenuStatus enuStatus    = EUSART_eSTATUS_NO_OK;
  bool               bIsIsrFound  = false;
  char               s8RxData     = 0;
  uint8_t            u8TimeoutIdx = 0;

  // Check the interrupt flag for RX2:
  if((PIE3bits.RC2IE == 1) && (PIR3bits.RC2IF == 1))
  {
    // The ISR was found, then the flag is updated to inform the ISR module:
    bIsIsrFound = true;

    // Wait until the EUSARTx Receive Interrupt Flag bit is set to 1 (no empty):)
    while((PIR3bits.RC2IF == 0) && (u8TimeoutIdx < RECEIVE_TIMEOUT_FROM_ISR_MS))
    {
      __delay_ms(CMN_1_MS);

      u8TimeoutIdx++;
    }

    // Check if there is an overrun and reset the EUSART if true:
    if(RC2STAbits.OERR == 1)
    {
      RC2STAbits.SPEN = 0;
      RC2STAbits.SPEN = 1;
      enuStatus       = EUSART_eSTATUS_RW_OVERRUN;
    }
    // Check if there is a receive framing error and reset the EUSART if true:
    else if(RC2STAbits.FERR == 1)
    {
      RC2STAbits.SPEN = 0;
      RC2STAbits.SPEN = 1;
      enuStatus       = EUSART_eSTATUS_FRAMING_ERROR;
    }
    else
    {
      enuStatus = EUSART_eSTATUS_OK;
    }

    // Read the RX value (will also clear the interrupt flag):
    s8RxData = RC2REG;

    // If a callback was registered, then it can be called:
    if(EUSART_pfRxCallback != NULL)
    {
      // Send the received data to the upper layer:
      EUSART_pfRxCallback(&s8RxData, sizeof(s8RxData), enuStatus);
    }
  }

  // The ISR found flag is returned to the ISR module to indicate whether the ISR was found from the EUSART module of not:
  return bIsIsrFound;
}


/**********************************************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                                                        */
/**********************************************************************************************************************/
void EUSART_vidInitialize(void)
{
  tenuClockFreqId enuCLockId = eFOSC_COUNT;

  // - 1) Check if the baudrate is correct
  CMN_assert((0 <= SELECTED_BAUDRATE) && (SELECTED_BAUDRATE < eBAUDRATE_COUNT));

  // - 2) Check if the frequency of Fosc is in a valid range to make this peripheral working:
  switch(CLOCK_CONFIG_FOSC_FREQUENCY_MHZ)
  {
    case CLOCK_FOSC_FREQUENCY_01MHZ:
      enuCLockId = eFOSC1MHZ;
      break;
    case CLOCK_FOSC_FREQUENCY_04MHZ:
      enuCLockId = eFOSC4MHZ;
      break;
    case CLOCK_FOSC_FREQUENCY_08MHZ:
      enuCLockId = eFOSC8MHZ;
      break;
    case CLOCK_FOSC_FREQUENCY_32MHZ:
      enuCLockId = eFOSC32MHZ;
      break;
    default:
      CMN_abortAll();
      break;
  }

  // - 3) Apply the configuration of the peripheral (see "PIC18F47Q10 - Datasheet", P.502 - 28.1.1.7):
  //    - 3.1) Initialize the SPxBRGH, SPxBRGL register pair and the BRGH and BRG16 bits to achieve the desired
  //           baudrate:
  SP2BRGL            = ((EUSART_kau16BaudRateCoef[enuCLockId][SELECTED_BAUDRATE] & 0x00ff));       // Set the baudrate for the LSB Byte
  SP2BRGH            = ((EUSART_kau16BaudRateCoef[enuCLockId][SELECTED_BAUDRATE] & 0xff00) >> 8);  // Set the baudrate for the MSB Byte
  TX2STAbits.BRGH    = 1;                                                                           // High Baud Rate Select
  BAUD2CONbits.BRG16 = 1;                                                                           // 16-bit Baud Rate Generator is used

  //    - 3.2) Select the transmit output pin by writing the appropriate value to the RxyPPS register:
  RD0PPS             = 0x0B;       // RD0 is TX2
  RX2PPS             = 0b00011001; // RD1 is RX2
  TRISDbits.TRISD0   = 0;          // Configure RD0 as output
  TRISDbits.TRISD1   = 1;          // Configure RD1 as input
  ANSELDbits.ANSELD1 = 0;          // Enable RD1 digital input buffers

  //    - 3.3) Enable the asynchronous serial port by clearing the SYNC bit and setting the SPEN bit:
  RC2STAbits.SPEN = 1;
  TXSTA2bits.SYNC = 0;

  //    - 3.4) Enable the transmission by setting the TXEN control bit. This will cause the TXxIF interrupt bit to be set:
  TX2STAbits.TXEN = 1;

  //    - 3.5) Enable the continuous reception:
  RC2STAbits.CREN = 1;
}


/*--------------------------------------------------------------------------------------------------------------------*/
EUSART_tenuStatus EUSART_enuRegisterRxCbk(const EUSART_tpfvidRxCallback kpfvidCallback)
{
  EUSART_tenuStatus enuStatus = EUSART_eSTATUS_NO_OK;

  if(kpfvidCallback == NULL)
  {
    enuStatus = EUSART_eSTATUS_NULL_POINTER;
  }
  else if(!ISR_bRegisterIsrCbk(ISR_ePERIPHERAL_EUSART, bInterruptHandler))
  {
    enuStatus = EUSART_eSTATUS_NO_OK;
  }
  else
  {
    EUSART_pfRxCallback = (EUSART_tpfvidRxCallback)kpfvidCallback;
    enuStatus            = EUSART_eSTATUS_OK;

    // Once if the callback is registered, the RX interruption of the EUSART can be enabled:
    // (see "PIC18F47Q10 - Datasheet", P.209 - 14.13.13)
    PIE3bits.RC2IE = 1;
  }

  return enuStatus;
}


/*--------------------------------------------------------------------------------------------------------------------*/
EUSART_tenuStatus EUSART_enuUnRegisterRxCbk(void)
{
  EUSART_tenuStatus enuStatus = EUSART_eSTATUS_NO_OK;

  EUSART_pfRxCallback = NULL;

  // If the RX callback is unregistered, no need to have the EUSART anymore:
  // (see "PIC18F47Q10 - Datasheet", P.209 - 14.13.13)
  PIE3bits.RC2IE = 0;

  // Unregister the EUSART from the ISR manager:
  if(ISR_bUnregisterIsrCbk(ISR_ePERIPHERAL_EUSART))
  {
    enuStatus = EUSART_eSTATUS_OK;
  }

  return enuStatus;
}


/*--------------------------------------------------------------------------------------------------------------------*/
EUSART_tenuStatus EUSART_vidSendChar(const char ks8Data, const uint32_t ku32TimeoutMs)
{
  EUSART_tenuStatus enuStatus     = EUSART_eSTATUS_OK;
  uint32_t           u32TimeoutIdx = 0;

  while((PIR3bits.TX2IF == 0) && (u32TimeoutIdx < ku32TimeoutMs))
  {
    __delay_ms(CMN_1_MS);

    u32TimeoutIdx++;
  }

  if(u32TimeoutIdx >= ku32TimeoutMs)
  {
    enuStatus       = EUSART_eSTATUS_TIMEOUT;
    RC2STAbits.SPEN = 0;
    RC2STAbits.SPEN = 1;
  }
  else
  {
    enuStatus = EUSART_eSTATUS_OK;
  }

  // The data is written to the EUSART TX pin (see "PIC18F47Q10 - Datasheet", P.527 - 28.6.6):
  TX2REG = ks8Data;

  return enuStatus;
}


/*--------------------------------------------------------------------------------------------------------------------*/
