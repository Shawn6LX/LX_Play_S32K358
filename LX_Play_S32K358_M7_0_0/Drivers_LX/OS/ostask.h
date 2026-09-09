/*!
*
*   @file    MyTask.h
*
*   @brief   This head file contains declarations of My Tasks.
*
******************************************************************************/

#ifndef __OSTASK_H
#define __OSTASK_H


#include <stdint.h>


#define MTASK_DELAY_10_MS       (uint16_t)(10U)
#define MTASK_DELAY_50_MS       (uint16_t)(50U)
#define MTASK_DELAY_100_MS      (uint16_t)(100U)
#define MTASK_DELAY_500_MS      (uint16_t)(500U)

/* The unit of Stask Size is Word --> 4 bytes. */
#define MTASK_STACK_2K          (uint16_t)(512U)    /* 2k bytes */
#define MTASK_STACK_1K          (uint16_t)(256U)    /* 1k bytes */


void OSTask_RTOS_Init(void);
void OSTask_RTOS_Startup(void);

#endif /* __OSTASK_H */
