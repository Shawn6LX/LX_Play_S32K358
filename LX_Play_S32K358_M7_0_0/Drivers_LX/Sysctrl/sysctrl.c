/*==================================================================================================
*   Project              : LX_Play_S32K358
*   Platform             : Cortex-M7
*   Board                : FRDM-A-S32K358
*   Peripheral           : SBC, OpenSDA, J-Tag, Buttons, LED, SD Card, Type-C USB...
*
*   Toolchain            : S32 Design Studio 3.6.10
*   RTD Version          : S32K3_RTD_7_0_0_QLP03_D2512
*   SW  Version          : V0.0.1
==================================================================================================*/
/**
*   @file       sysctrl.c
*   @brief      System Control on Mcal / Hardware Drviers.
*   @author     Shawn.Liu
*   @version    V1.0
*   @date       2026-09-08
*
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                          INCLUDE FILES
==================================================================================================*/
#include "sysctrl.h"
/*==================================================================================================
*                      LOCAL TYPEDEFS(STRUCTURES, UNIONS, ENUMS) OR MACROS
==================================================================================================*/

/*==================================================================================================
*                                 LOCAL CONSTANTS OR VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                 GLOBAL CONSTANTS OR VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL FUNCTIONS
==================================================================================================*/
/*!
 * @brief       Initialize MCU Drivers.
 * @details     The function focus on the sequence of MCU Init.
 *
 * @param[in]   void
 * @param[out]  void
 * 
 * @return      void
 */
static void Sysctrl_MCU_Init(void)
{
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
}

/*!
 * @brief       Initialize Timer Drivers.
 * @details     The system timer is from OsIf.
 *
 * @param[in]   void
 * @param[out]  void
 * 
 * @return      void
 */
static void Sysctrl_Timer_Init(void)
{
    OsIf_Init(NULL_PTR);
    OsIf_Timer_System_Init();
}

/*!
 * @brief       Initialize Icu Drivers.
 * @details     The system timer is from OsIf.
 *
 * @param[in]   void
 * @param[out]  void
 * 
 * @return      void
 */
static void Sysctrl_Icu_Init(void)
{
    Siul2_Icu_Ip_Init(0U, &Siul2_Icu_Ip_0_Config_PB);
    Siul2_Icu_Ip_EnableInterrupt(0U, 17U);
    Siul2_Icu_Ip_EnableNotification(0U, 17U);
}

/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/
/*!
 * @brief       Initialize System related Drivers.
 * @details     Init for MCU, Clock, Platform, IO.
 *
 * @param[in]   void
 * @param[out]  void
 * 
 * @return      void
 */
void Sysctrl_DelayMs(uint32 delayMs)
{
    uint32 cur = OsIf_GetCounter(OSIF_COUNTER_SYSTEM);
    uint32 elapsed = 0U;
    uint32 timeout = OsIf_MicrosToTicks(delayMs * 1000U, OSIF_COUNTER_SYSTEM);
    while (elapsed < timeout) {
        elapsed += OsIf_GetElapsed(&cur, OSIF_COUNTER_SYSTEM);
    }
}

/*!
 * @brief       Initialize System related Drivers.
 * @details     Init for MCU, Clock, Platform, IO.
 *
 * @param[in]   void
 * @param[out]  void
 * 
 * @return      void
 */
void Sysctrl_Init(void)
{
    Sysctrl_MCU_Init();
    /* Initialize Platform Settings */
    Platform_Init(NULL_PTR);

    Sysctrl_Timer_Init();

    Sysctrl_Icu_Init();
}

/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/


#ifdef __cplusplus
}
#endif

/** @} */
