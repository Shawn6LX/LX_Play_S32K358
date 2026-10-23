/**
 * @file     Global_Cfg.h
 * @brief    Global configuration header file
 * @details  This file provides universal macro definitions, global includes,
 *           and common configuration options for the entire project.
 *           It centralizes shared definitions to ensure consistency
 *           and simplify maintenance across all modules.
 * @author   Shawn.Liu
 * @date     2026-06-03
 * @version  V1.0
 */

#ifndef GLOBAL_CFG_H_
#define GLOBAL_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Included Files
 ******************************************************************************/
/* ----- RTD Inclusions ----- */
#include "Mcu.h"
#include "Platform.h"
#include "OsIf.h"
#include "OsIf_Timer_System.h"
#include "Port.h"
#include "Dio.h"
#include "Icu.h"
#include "FreeRTOS.h"
#include "task.h"
/* ----- User Inclusions ----- */
#include "sysctrl.h"
#include "ostask.h"
/*******************************************************************************
 * Macro Definitions
 ******************************************************************************/
#define PEX_RTOS_INIT               OSTask_RTOS_Init
#define PEX_RTOS_START              OSTask_RTOS_Startup

#define CONFIG_USE_MCAL             STD_ON
/*******************************************************************************
 * Type Definitions
 ******************************************************************************/

/*******************************************************************************
 * Extern Global Variables
 ******************************************************************************/

/*******************************************************************************
 * Public Function Prototypes
 ******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* GLOBAL_CFG_H_ */
