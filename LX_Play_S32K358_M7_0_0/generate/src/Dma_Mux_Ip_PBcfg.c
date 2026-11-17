/*==================================================================================================
*   Project              : RTD AUTOSAR 4.9
*   Platform             : CORTEXM
*   Peripheral           : 
*   Dependencies         : none
*
*   Autosar Version      : 4.9.0
*   Autosar Revision     : ASR_REL_4_9_REV_0000
*   Autosar Conf.Variant :
*   SW Version           : 7.0.0
*   Build Version        : S32K3_RTD_7_0_0_QLP03_D2512_ASR_REL_4_9_REV_0000_20251210
*
*   Copyright 2020 - 2025 NXP
*
*   NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be 
*   used strictly in accordance with the applicable license terms. By expressly 
*   accepting such terms or by downloading, installing, activating and/or otherwise 
*   using the software, you are agreeing that you have read, and that you agree to 
*   comply with and are bound by, such license terms. If you do not agree to be 
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
==================================================================================================*/

/**
*   @file Dma_Mux_Ip_PBcfg.c
*
*   @addtogroup DMA_MUX_IP DMA_MUX IPV Driver
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Dma_Mux_Ip.h"
#include  "DeviceDefinition.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define RM_DMA_MUX_IP_PBCFG_VENDOR_ID_C                     43
#define RM_DMA_MUX_IP_PBCFG_AR_RELEASE_MAJOR_VERSION_C      4
#define RM_DMA_MUX_IP_PBCFG_AR_RELEASE_MINOR_VERSION_C      9
#define RM_DMA_MUX_IP_PBCFG_AR_RELEASE_REVISION_VERSION_C   0
#define RM_DMA_MUX_IP_PBCFG_SW_MAJOR_VERSION_C              7
#define RM_DMA_MUX_IP_PBCFG_SW_MINOR_VERSION_C              0
#define RM_DMA_MUX_IP_PBCFG_SW_PATCH_VERSION_C              0

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Checks against Dma_Mux_Ip.h */
#if (RM_DMA_MUX_IP_PBCFG_VENDOR_ID_C != RM_DMA_MUX_IP_VENDOR_ID)
    #error "Dma_Mux_Ip_PBcfg.c and Dma_Mux_Ip.h have different vendor ids"
#endif
#if ((RM_DMA_MUX_IP_PBCFG_AR_RELEASE_MAJOR_VERSION_C    != RM_DMA_MUX_IP_AR_RELEASE_MAJOR_VERSION) || \
     (RM_DMA_MUX_IP_PBCFG_AR_RELEASE_MINOR_VERSION_C    != RM_DMA_MUX_IP_AR_RELEASE_MINOR_VERSION) || \
     (RM_DMA_MUX_IP_PBCFG_AR_RELEASE_REVISION_VERSION_C != RM_DMA_MUX_IP_AR_RELEASE_REVISION_VERSION) \
    )
     #error "AUTOSAR Version Numbers of Dma_Mux_Ip_PBcfg.c and Dma_Mux_Ip.h are different"
#endif
#if ((RM_DMA_MUX_IP_PBCFG_SW_MAJOR_VERSION_C != RM_DMA_MUX_IP_SW_MAJOR_VERSION) || \
     (RM_DMA_MUX_IP_PBCFG_SW_MINOR_VERSION_C != RM_DMA_MUX_IP_SW_MINOR_VERSION) || \
     (RM_DMA_MUX_IP_PBCFG_SW_PATCH_VERSION_C != RM_DMA_MUX_IP_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Dma_Mux_Ip_PBcfg.c and Dma_Mux_Ip.h are different"
#endif

/* Checks against DeviceDefinition.h */
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
#if ((RM_DMA_MUX_IP_PBCFG_AR_RELEASE_MAJOR_VERSION_C != DEVICEDEFINITION_AR_RELEASE_MAJOR_VERSION_H) || \
     (RM_DMA_MUX_IP_PBCFG_AR_RELEASE_MINOR_VERSION_C != DEVICEDEFINITION_AR_RELEASE_MINOR_VERSION_H))
    #error "AutoSar Version Numbers of Dma_Mux_Ip_PBcfg.c and DeviceDefinition.h are different"
#endif
#endif
/*==================================================================================================
                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
                                        LOCAL MACROS
==================================================================================================*/
#if defined(S32K388) || defined(S32K389)
#define DMAMUX_CHANNEL_CFG_ENBL_MASK        DMAMUX_CHCONF_ENBL_MASK
#define DMAMUX_CHANNEL_CFG_TRIG             DMAMUX_CHCONF_TRIG
#else
#define DMAMUX_CHANNEL_CFG_ENBL_MASK        DMAMUX_CHCFG_ENBL_MASK
#define DMAMUX_CHANNEL_CFG_TRIG             DMAMUX_CHCFG_TRIG
#endif
/*==================================================================================================
                                       LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
                                       GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       GLOBAL VARIABLES
==================================================================================================*/
#define RM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Rm_MemMap.h"

static const Dma_Mux_Ip_ChannelConfigType Dma_Mux_Channel_Config[2] =
{
    {
        DMA_MUX_INSTANCE_1,
        0U,
        (DMA_MUX_1_LPUART_6_RECEIVE | DMAMUX_CHANNEL_CFG_ENBL_MASK | DMAMUX_CHANNEL_CFG_TRIG(0U))
    },
    {
        DMA_MUX_INSTANCE_1,
        1U,
        (DMA_MUX_1_LPUART_6_TRANSMIT | DMAMUX_CHANNEL_CFG_ENBL_MASK | DMAMUX_CHANNEL_CFG_TRIG(0U))
    }
};
const Dma_Mux_Ip_ConfigType Dma_Mux_Config =
{
    2U,
    Dma_Mux_Channel_Config
};

#define RM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Rm_MemMap.h"
/*==================================================================================================
                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
                                       LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
                                       GLOBAL FUNCTIONS
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */

