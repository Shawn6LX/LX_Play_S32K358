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
static uint8_t lpuart6_Rx_Buffer[UART_RX_MAX_LEN];
static uint8_t Flag_Lpuart6_Rx = UART_FLG_RX_NOT_RECEIVED;
/*******************************************************************************
 * Static Internal Function Declarations
 ******************************************************************************/
/*!
 * @brief       UART Rx callback function, triggered only if data is fully received.
 * @details     This callback only supports fixed-length reception mode; 
 *              It can't support variable-length frame.
 *
 * @param[in]   HwInstance: LPUART Hardware driver Instance.
 * @param[in]   Event: UART event flag, including RX_FULL, END_TRANSFER...
 * @param[out]  void
 */
void LPUART_Rx_Callback(uint8 HwInstance, Uart_EventType Event)
{
    (void)HwInstance;

    if (Event == UART_EVENT_RX_FULL) 
    {
        // Handle received data.
    }
    else if ((Event == UART_EVENT_END_TRANSFER) || (Event == UART_EVENT_ERROR))
    {
        /* Re-Start receving channel. */
        (void)Uart_AsyncReceive(UART_CHANNEL_6, &lpuart6_Rx_Buffer[0], 1U);
    }
}

/*******************************************************************************
 * Public Function Implementation
 ******************************************************************************/

 
/*!
 * @brief Initialize LPUART Drivers according to requirements.
 *
 */
void UART_Init(void)
{
    /* Init Uart in Mcal. */
    Uart_Init(NULL_PTR);
    /* Init for UART1 */
    memset(lpuart6_Rx_Buffer, 0, UART_RX_MAX_LEN);
    /* Start receive data in specific Channel. */
    (void)Uart_AsyncReceive(UART_CHANNEL_6, &lpuart6_Rx_Buffer[0], 1U);
}


/*!
 * @brief Handler for UART receiving variable-length frame.
 *
 * @param[in]   instance: LPUART instance number
 */
void UART_Rx_Data_Handler(uint32_t instance)
{
    if(UART_CHANNEL_6 == instance)
    {
        Flag_Lpuart6_Rx = UART_FLG_RX_RECEIVED;
    }
}

/*!
 * @brief Main Function of UART.
 *
 * @param[in]   instance: LPUART instance number
 */
void UART_MainFunction(void)
{
    if(UART_FLG_RX_RECEIVED == Flag_Lpuart6_Rx)
    {
        Flag_Lpuart6_Rx = UART_FLG_RX_NOT_RECEIVED;
        // OLED_ShowString(5, 15, lpuart6_Rx_Buffer, 8, 0);
    }
}

