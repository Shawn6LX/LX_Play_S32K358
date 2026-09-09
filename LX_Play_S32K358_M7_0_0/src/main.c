/*==================================================================================================
* Project : RTD AUTOSAR 4.9
* Platform : CORTEXM
* Peripheral : S32K3XX
* Dependencies : none
*
* Autosar Version : 4.9.0
* Autosar Revision : ASR_REL_4_9_REV_0000
* Autosar Conf.Variant :
* SW Version : 7.0.0
* Build Version : S32K3_RTD_7_0_0_QLP03_D2512_ASR_REL_4_9_REV_0000_20251210
*
* Copyright 2020 - 2025 NXP
*
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be
*   used strictly in accordance with the applicable license terms. By expressly
*   accepting such terms or by downloading, installing, activating and/or otherwise
*   using the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms. If you do not agree to be
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
==================================================================================================*/

/**
*   @file main.c
*
*   @addtogroup main_module main module documentation
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/* Including necessary configuration files. */
#include "Mcal.h"

volatile int exit_code = 0;
/* User includes */
#include "system.h"
#include "Siul2_Dio_Ip.h"
#include "Siul2_Port_Ip.h"
#include "OsIf.h"
#include "OsIf_Timer_System.h"
#include "Platform.h"
#include "Mcu.h"


void DelayMs(uint32 delayMs) {
	uint32 cur = OsIf_GetCounter(OSIF_COUNTER_SYSTEM);
	uint32 elapsed = 0U;
	uint32 timeout = OsIf_MicrosToTicks(delayMs * 1000U, OSIF_COUNTER_SYSTEM);
	while (elapsed < timeout) {
		elapsed += OsIf_GetElapsed(&cur, OSIF_COUNTER_SYSTEM);
	}
}


/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
    /* Write your code here */
	/* Init clock */
#if (STD_ON == MCU_PRECOMPILE_SUPPORT)
    Mcu_Init(NULL_PTR);
#else
    Mcu_Init(&Mcu_Config);
#endif

#if (STD_ON == MCU_INIT_CLOCK)
    /* Initialize the clock tree and apply PLL as system clock */
    Mcu_InitClock(McuClockSettingConfig_0);

 #if (STD_OFF == MCU_NO_PLL)
    while (MCU_PLL_LOCKED != Mcu_GetPllStatus()) {
        /* Busy wait until the System PLL is locked */
    }
    Mcu_DistributePllClock();
 #endif

    Mcu_SetMode(McuModeSettingConf_0);

#else
    #error "The Mcu Init Clock API should be enabled from the Mcu driver"
#endif

    /* Initialize Platform driver */
    Platform_Init(NULL_PTR);

    OsIf_Init(NULL_PTR);
    OsIf_Timer_System_Init();

    Siul2_Port_Ip_Init(
    NUM_OF_CONFIGURED_PINS_PortContainer_0_BOARD_InitPeripherals,
    g_pin_mux_InitConfigArr_PortContainer_0_BOARD_InitPeripherals);

    while(true)
    {
        Siul2_Dio_Ip_WritePin(LED0_GREEN_PORT, LED0_GREEN_PIN, 0);
        DelayMs(2000U);
        Siul2_Dio_Ip_WritePin(LED0_GREEN_PORT, LED0_GREEN_PIN, 1);
        DelayMs(2000U);
    }


    for(;;)
    {
        if(exit_code != 0)
        {
            break;
        }
    }
    return exit_code;
}

#ifdef __cplusplus
}
#endif

/** @} */
