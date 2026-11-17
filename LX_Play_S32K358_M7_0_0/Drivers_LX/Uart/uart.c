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
#define UART_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Uart_MemMap.h"

uint8 lpuart6_Rx_Dma[UART_RX_DMA_MAX];
uint8 lpuart6_Tx_Dma[UART_TX_DMA_MAX];

#define UART_STOP_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Uart_MemMap.h"
/*******************************************************************************
 * Static Local Variables
 ******************************************************************************/
/* Vairables for Rx */
static uint8 Uart_Rx_FIFO[UART_RX_FIFO_MAX];
static Uart_FifoIdxType Uart_RxFifoIdx;
/* Vairables for Tx */
static uint8 Uart_Tx_FIFO[UART_TX_FIFO_MAX];
static Uart_FifoIdxType Uart_TxFifoIdx;
static boolean Uart_Flag_TxBusy = false;
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
    return (((Uart_RxFifoIdx.tail + 1U) % UART_RX_FIFO_MAX) == Uart_RxFifoIdx.head);
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
    return (Uart_RxFifoIdx.tail  == Uart_RxFifoIdx.head);
}

/*!
 * @brief Append Rx Byte to FIFO.
 *
 * @param[in]   inByte: Byte to be appended.
 * @param[out]  void
 * @return      E_OK:       Enqueue in success.
 *              E_NOT_OK:   Enqueue in failed as FIFO is full.
 */
static Std_ReturnType UART_RxFIFO_Enqueue(uint8 inByte)
{
    Std_ReturnType retVal = E_OK;

    if(UART_RxFIFO_Is_Full())
    {
        retVal = E_NOT_OK;
    }
    else
    {
        Uart_Rx_FIFO[Uart_RxFifoIdx.tail] = inByte;
        Uart_RxFifoIdx.tail = (Uart_RxFifoIdx.tail + 1U) % UART_RX_FIFO_MAX;
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
static Std_ReturnType UART_RxFIFO_Dequeue(uint8 *outByte)
{
    Std_ReturnType retVal = E_OK;

    if(UART_RxFIFO_Is_Empty())
    {
        retVal = E_NOT_OK;
    }
    else
    {
        *outByte = Uart_Rx_FIFO[Uart_RxFifoIdx.head];
        Uart_RxFifoIdx.head = (Uart_RxFifoIdx.head + 1U) % UART_RX_FIFO_MAX;
    }

    return retVal;
}

/*!
 * @brief Handler for UART receiving variable-length frame.
 *
 * @param[in]   rxLen: The length of transmitted bytes from this interrupt.
 * @param[out]  void
 * @return      void
 */
static void UART_Rx_Data_Handler(uint32 rxLen)
{
    uint16 idx = 0;
    uint8 getChar = 0;

    /* Send back to Console. */
    UART_Write(lpuart6_Rx_Dma, rxLen);
    /* Append to Rx-FIFO. */
    for(; idx < rxLen; idx++)
    {
        getChar = lpuart6_Rx_Dma[idx];
#if 0
        if(getChar == '\r')
        {
            getChar = '\n';
        }
#endif
        if(E_NOT_OK == UART_RxFIFO_Enqueue(getChar))
        {
            // PRINT_LOG();
            /* Discard remaining data. */
            break;
        }
    }
}

/*!
 * @brief Tx FIFO queue is Full
 *
 * @param[in]   void
 * @param[out]  void
 * @return      true:   FIFO is full
 *              false:  FIFO is not full
 */
static boolean UART_TxFIFO_Is_Full(void)
{
    return (((Uart_TxFifoIdx.tail + 1U) % UART_TX_FIFO_MAX) == Uart_TxFifoIdx.head);
}

/*!
 * @brief Tx FIFO queue is Empty
 *
 * @param[in]   void
 * @param[out]  void
 * @return      true:   FIFO is empty
 *              false:  FIFO is not empty
 */
static boolean UART_TxFIFO_Is_Empty(void)
{
    return (Uart_TxFifoIdx.tail == Uart_TxFifoIdx.head);
}

/*!
 * @brief Append Byte to Tx FIFO.
 *
 * @param[in]   inByte: Byte to be appended.
 * @param[out]  void
 * @return      E_OK:       Enqueue in success.
 *              E_NOT_OK:   Enqueue in failed as FIFO is full.
 */
static Std_ReturnType UART_TxFIFO_Enqueue(uint8 inByte)
{
    Std_ReturnType retVal = E_OK;

    if(UART_TxFIFO_Is_Full())
    {
        retVal = E_NOT_OK;
    }
    else
    {
        Uart_Tx_FIFO[Uart_TxFifoIdx.tail] = inByte;
        Uart_TxFifoIdx.tail = (Uart_TxFifoIdx.tail + 1U) % UART_TX_FIFO_MAX;
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
static Std_ReturnType UART_TxFIFO_Dequeue(uint8 *outByte)
{
    Std_ReturnType retVal = E_OK;

    if(UART_TxFIFO_Is_Empty())
    {
        retVal = E_NOT_OK;
    }
    else
    {
        *outByte = Uart_Tx_FIFO[Uart_TxFifoIdx.head];
        Uart_TxFifoIdx.head = (Uart_TxFifoIdx.head + 1U) % UART_TX_FIFO_MAX;
    }

    return retVal;
}

/*!
 * @brief   Handler for UART Tx-FIFO.
 *
 * @param[in]   void
 * @param[out]  void
 * @return      void
 */
static void UART_Tx_FIFO_Handler(void)
{
    uint16 txLen = 0;
    uint32 dummy = 0;

    /* Check whether Uart-Tx is Busy. */
    if((UART_STATUS_OPERATION_ONGOING != Uart_GetStatus(UART_CHANNEL_6, &dummy, UART_SEND)))
    {
        while ((!UART_TxFIFO_Is_Empty()) && (txLen < UART_TX_DMA_MAX))
        {
            UART_TxFIFO_Dequeue(&lpuart6_Tx_Dma[txLen++]);
        }
    }

    if(txLen > 0)
    {
        Uart_Flag_TxBusy = true;
        (void)Uart_AsyncSend(UART_CHANNEL_6, lpuart6_Tx_Dma, txLen);
    }
}

/*******************************************************************************
 * Public Function Implementation
 ******************************************************************************/

/*!
 * @brief Send Data to UART Driver --> Append to Tx-FIFO.
 *
 * @param[in]   txData: Pointer to tx data address.
 * @param[out]  len: The length of tx data.
 * @return      E_OK:       Send in success.
 *              E_NOT_OK:   Send in failed.
 */
Std_ReturnType UART_Write(const uint8 *txData, uint16 len)
{
    uint16 idx;
    Std_ReturnType retVal = E_OK;

    /* Protection: Might be called by Task or RxCallback. */
    OsIf_SuspendAllInterrupts();
    for (idx = 0U; idx < len; idx++)
    {
        if(E_OK != UART_TxFIFO_Enqueue(txData[idx]))
        {
            retVal = E_NOT_OK;
        }
    }
    OsIf_ResumeAllInterrupts();

    return retVal;
}

/*!
 * @brief Read Data from UART Driver --> Pop out of Rx-FIFO.
 *
 * @param[in]   rxData: Pointer to rx data address.
 * @param[in]   maxLen: Max number of bytes to pop.
 * @param[out]  void
 * @return      Number of bytes actually read (0 when the FIFO is empty).
 */
uint16 UART_Read(uint8 *rxData, uint16 maxLen)
{
    uint16 readCnt = 0U;

    /* Rx-FIFO is written by the callback and read here, keep the read atomic. */
    OsIf_SuspendAllInterrupts();
    while ((readCnt < maxLen) && (E_OK == UART_RxFIFO_Dequeue(&rxData[readCnt])))
    {
        readCnt++;
    }
    OsIf_ResumeAllInterrupts();

    return readCnt;
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
void LPUART_Event_Callback(uint8 HwInstance, Uart_EventType Event)
{
    uint32 byteRemin = 0;
    uint32 byteRxLen = 0;

    (void)HwInstance;

    if (Event == UART_EVENT_IDLE_STATE)
    {
        (void)Uart_GetStatus(UART_CHANNEL_6, &byteRemin, UART_RECEIVE);
        byteRxLen = UART_RX_DMA_MAX - byteRemin;
        UART_Rx_Data_Handler(byteRxLen);
        (void)Uart_AsyncReceive(UART_CHANNEL_6, &lpuart6_Rx_Dma[0], UART_RX_DMA_MAX);
    }
    else if (Event == UART_EVENT_RX_FULL)
    {
        UART_Rx_Data_Handler(UART_RX_DMA_MAX);
        (void)Uart_AsyncReceive(UART_CHANNEL_6, &lpuart6_Rx_Dma[0], UART_RX_DMA_MAX);
    }
    else if ((Event == UART_EVENT_TX_EMPTY) || (Event == UART_EVENT_END_TRANSFER))
    {
        Uart_Flag_TxBusy = false;
    }
    else /* UART_EVENT_ERROR */
    {
        /* Reload the Rx data buffer. */
        (void)Uart_AsyncReceive(UART_CHANNEL_6, &lpuart6_Rx_Dma[0], UART_RX_DMA_MAX);
    }
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
    memset(lpuart6_Rx_Dma, 0, UART_RX_DMA_MAX);
    /* Start receive data in specific Channel. */
    (void)Uart_AsyncReceive(UART_CHANNEL_6, &lpuart6_Rx_Dma[0], UART_RX_DMA_MAX);
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
    if(!Uart_Flag_TxBusy)
    {
        UART_Tx_FIFO_Handler();
    }
}

