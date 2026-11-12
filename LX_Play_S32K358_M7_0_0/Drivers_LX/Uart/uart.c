/**
  ******************************************************************************
  * @file    uart.c
  * @brief   Implementation of UART abstraction layer.
  * @details This file implements the UART abstraction layer APIs.
  *          It encapsulates the underlying LPUART driver (S32K LPUART)
  *          and provides  commmon interfaces for the application layer.
  * @author  Shawn.Liu
  * @date    2026-09-08
  * @version V1.0
  ******************************************************************************
  */

/*******************************************************************************
 * Included Files
 ******************************************************************************/
#include <string.h>
#include "uart.h"
/*******************************************************************************
 * Internal Macro Definitions
 ******************************************************************************/

/*******************************************************************************
 * Internal Type Definitions
 ******************************************************************************/

/*******************************************************************************
 * Global Variables Definition
 ******************************************************************************/

/*******************************************************************************
 * Static Local Variables
 ******************************************************************************/
static uint8_t lpuart6_Rx_Data[UART_RX_DATA_MAX];
static uint8_t Uart_Rx_FIFO[UART_RX_FIFO_MAX];
static Uart_FifoIdxType Uart_FifoIdx;
/*******************************************************************************
 * Static Internal Function Declarations
 ******************************************************************************/
/*!
 * @brief Rx FIFO queue is Full
 *
 * @param[in]   void
 * @param[out]  void
 * @return      true:   FIFO is full
 *              false:  FIFO is not full
 */
static boolean UART_RxFIFO_Is_Full(void)
{
    return (((Uart_FifoIdx.tail + 1U) % UART_RX_FIFO_MAX) == Uart_FifoIdx.head);
}

/*!
 * @brief Rx FIFO queue is Empty
 *
 * @param[in]   void
 * @param[out]  void
 * @return      true:   FIFO is empty
 *              false:  FIFO is not empty
 */
static boolean UART_RxFIFO_Is_Empty(void)
{
    return (Uart_FifoIdx.tail  == Uart_FifoIdx.head);
}

/*!
 * @brief Handler for UART receiving variable-length frame.
 *
 * @param[in]   rxLen: The length of transmitted bytes from this interrupt.
 * @param[out]  void
 * @return      void
 */
static void UART_Rx_Data_Handler(uint32_t rxLen)
{
    uint16_t idx = 0;
    uint8_t getChar = 0;

    for(; idx < rxLen; idx++)
    {
        getChar = lpuart6_Rx_Data[idx];
        /* Send back the Byte. */
        //UART_Tx_Data_Handler();

        if((getChar == '\r') || (getChar == '\n'))
        {
            getChar = '\0';
        }
        /* Append to Rx Buffer. */
        if(E_NOT_OK == UART_RxFIFO_Enqueue(getChar))
        {
            // PRINT_LOG();
            /* Discard remaining data. */
            break;
        }
    }
}

/*******************************************************************************
 * Public Function Implementation
 ******************************************************************************/
/*!
 * @brief Append Rx Byte to FIFO.
 *
 * @param[in]   inByte: Byte to be appended.
 * @param[out]  void
 * @return      E_OK:       Enqueue in success.
 *              E_NOT_OK:   Enqueue in failed as FIFO is full.
 */
Std_ReturnType UART_RxFIFO_Enqueue(uint8_t inByte)
{
    Std_ReturnType retVal = E_OK;

    if(UART_RxFIFO_Is_Full())
    {
        retVal = E_NOT_OK;
    }
    else
    {
        Uart_Rx_FIFO[Uart_FifoIdx.tail] = inByte;
        Uart_FifoIdx.tail = (Uart_FifoIdx.tail + 1U) % UART_RX_FIFO_MAX;
    }

    return retVal;
}

/*!
 * @brief Pop out Byte from FIFO.
 *
 * @param[in]   outByte: Pointer to the popped out Byte.
 * @param[out]  void
 * @return      E_OK:       Dequeue in success.
 *              E_NOT_OK:   Dequeue in failed as FIFO is empty.
 */
Std_ReturnType UART_RxFIFO_Dequeue(uint8_t *outByte)
{
    Std_ReturnType retVal = E_OK;

    if(UART_RxFIFO_Is_Empty())
    {
        retVal = E_NOT_OK;
    }
    else
    {
        *outByte = Uart_Rx_FIFO[Uart_FifoIdx.head];
        Uart_FifoIdx.head = (Uart_FifoIdx.head + 1U) % UART_RX_FIFO_MAX;
    }

    return retVal;
}

/*!
 * @brief       UART Rx callback function, triggered only if data is fully received.
 * @details     This callback only supports fixed-length reception mode; 
 *              It can't support variable-length frame.
 *
 * @param[in]   HwInstance: LPUART Hardware driver Instance.
 * @param[in]   Event: UART event flag, including RX_FULL, END_TRANSFER...
 * @param[out]  void
 * @return      void
 */
void LPUART_Rx_Callback(uint8 HwInstance, Uart_EventType Event)
{
    uint32_t byteRemin = 0;
    uint32_t byteRxLen = 0;

    (void)HwInstance;

    if (Event == UART_EVENT_IDLE_STATE)
    {
        (void)Uart_GetStatus(UART_CHANNEL_6, &byteRemin, UART_RECEIVE);
        byteRxLen = UART_RX_DATA_MAX - byteRemin;
        UART_Rx_Data_Handler(byteRxLen);
    }
    else if (Event == UART_EVENT_RX_FULL) 
    {
        UART_Rx_Data_Handler(UART_RX_DATA_MAX);
    }
    else /* UART_EVENT_END_TRANSFER /UART_EVENT_ERROR */
    {
        /* Do Nothing */
    }
    /* Reload the Rx data buffer. */
    (void)Uart_AsyncReceive(UART_CHANNEL_6, &lpuart6_Rx_Data[0], UART_RX_DATA_MAX);
}

/*!
 * @brief       Initialize LPUART Drivers.
 *
 * @param[in]   void
 * @param[in]   void
 * @param[out]  void
 * @return      void
 */
void UART_Init(void)
{
    /* Init Uart in Mcal. */
    Uart_Init(NULL_PTR);
    /* Init for UART1 */
    memset(lpuart6_Rx_Data, 0, UART_RX_DATA_MAX);
    /* Start receive data in specific Channel. */
    (void)Uart_AsyncReceive(UART_CHANNEL_6, &lpuart6_Rx_Data[0], UART_RX_DATA_MAX);
}

/*!
 * @brief       MainFunction of UART in 10ms Task.
 *
 * @param[in]   void
 * @param[in]   void
 * @param[out]  void
 * @return      void
 */
void UART_MainFunction(void)
{
    //Txsend
}

