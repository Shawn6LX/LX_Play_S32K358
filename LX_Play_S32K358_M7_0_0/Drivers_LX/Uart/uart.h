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

#define UART_RX_DMA_MAX                 ((uint8_t)64U)              /* Max Length of DMA Rx Buffer. */
#define UART_RX_FIFO_MAX                (UART_RX_DMA_MAX * 2U)      /* Max Lenght of Rx Ring-FIFO. */

#define UART_TX_DMA_MAX                 ((uint8_t)64U)              /* Max Length of DMA Tx Buffer. */
#define UART_TX_FIFO_MAX                (UART_TX_DMA_MAX * 2U)      /* Max Lenght of Tx Ring-FIFO. */

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
void UART_Init(void);
void UART_MainFunction(void);
Std_ReturnType UART_Write(const uint8_t *txData, uint16_t len);
uint16_t UART_Read(uint8_t *rxData, uint16_t maxLen);

#ifdef __cplusplus
}
#endif

#endif /* UART_H_ */
