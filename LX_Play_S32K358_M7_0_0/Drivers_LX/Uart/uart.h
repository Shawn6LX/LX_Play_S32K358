/**
  ******************************************************************************
  * @file    uart.h
  * @brief   UART abstraction layer for application / LPUART driver separation
  * @details This module provides abstract UART interfaces.
  *          It isolates the upper application layer from the LPUART driver.
  * @author  Shawn.Liu
  * @date    2026-06-03
  * @version V1.0
  ******************************************************************************
  */

#ifndef UART_H_
#define UART_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Included Files
 ******************************************************************************/
#include <stdio.h>
#include "Global_Cfg.h"
/*******************************************************************************
 * Macro Definitions
 ******************************************************************************/
#define UART_CHANNEL_6                  ((uint8_t)0U)

#define UART_RX_DATA_MAX                 ((uint8_t)64U)
#define UART_RX_FIFO_MAX                (UART_RX_DATA_MAX * 2U)

#define UART_FLG_RX_NOT_RECEIVED        ((uint8_t)0U)
#define UART_FLG_RX_RECEIVED            ((uint8_t)1U)
/*******************************************************************************
 * Type Definitions
 ******************************************************************************/
/* Type of Ring-Buffer Index */
typedef struct
{
    uint16_t head;      /* Index of output handling-data in buffer. */
    uint16_t tail;      /* Index of input rx-data pushed in buffer. */
} Uart_FifoIdxType;

/*******************************************************************************
 * Extern Global Variables
 ******************************************************************************/

/*******************************************************************************
 * Public Function Prototypes
 ******************************************************************************/
Std_ReturnType UART_RxFIFO_Enqueue(uint8_t inByte);
Std_ReturnType UART_RxFIFO_Dequeue(uint8_t *outByte);
void UART_Init(void);
void UART_MainFunction(void);

#ifdef __cplusplus
}
#endif

#endif /* UART_H_ */
