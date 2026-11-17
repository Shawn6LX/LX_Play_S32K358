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
#ifndef DMA_MUX_IP_H
#define DMA_MUX_IP_H

/**
*   @file Dma_Mux_Ip.h
*
*   @addtogroup DMA_MUX_IP DMA_MUX IPV Driver
*   @{
*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "Dma_Mux_Ip_Cfg.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define RM_DMA_MUX_IP_VENDOR_ID                      43
#define RM_DMA_MUX_IP_AR_RELEASE_MAJOR_VERSION       4
#define RM_DMA_MUX_IP_AR_RELEASE_MINOR_VERSION       9
#define RM_DMA_MUX_IP_AR_RELEASE_REVISION_VERSION    0
#define RM_DMA_MUX_IP_SW_MAJOR_VERSION               7
#define RM_DMA_MUX_IP_SW_MINOR_VERSION               0
#define RM_DMA_MUX_IP_SW_PATCH_VERSION               0

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Checks against Dma_Mux_Ip_Cfg.h */
#if (RM_DMA_MUX_IP_VENDOR_ID != RM_DMA_MUX_IP_CFG_VENDOR_ID)
    #error "Dma_Mux_Ip.h and Dma_Mux_Ip_Cfg.h have different vendor ids"
#endif
#if (( RM_DMA_MUX_IP_AR_RELEASE_MAJOR_VERSION    != RM_DMA_MUX_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     ( RM_DMA_MUX_IP_AR_RELEASE_MINOR_VERSION    != RM_DMA_MUX_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     ( RM_DMA_MUX_IP_AR_RELEASE_REVISION_VERSION != RM_DMA_MUX_IP_CFG_AR_RELEASE_REVISION_VERSION) \
    )
     #error "AUTOSAR Version Numbers of Dma_Mux_Ip.h and Dma_Mux_Ip_Cfg.h are different"
#endif
#if (( RM_DMA_MUX_IP_SW_MAJOR_VERSION != RM_DMA_MUX_IP_CFG_SW_MAJOR_VERSION) || \
     ( RM_DMA_MUX_IP_SW_MINOR_VERSION != RM_DMA_MUX_IP_CFG_SW_MINOR_VERSION) || \
     ( RM_DMA_MUX_IP_SW_PATCH_VERSION != RM_DMA_MUX_IP_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Dma_Mux_Ip.h and Dma_Mux_Ip_Cfg.h are different"
#endif
/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/
#ifdef RM_IP_ENABLE_DMA_MUX
#if (RM_IP_ENABLE_DMA_MUX == STD_ON)

#define RM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Rm_MemMap.h"

/*  Export DMA_MUX configurations.*/
DMA_MUX_CONFIG_EXT

#define RM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Rm_MemMap.h"

#define RM_START_SEC_CODE
#include "Rm_MemMap.h"

/**
 * @brief         Initializes the Memory Protection Unit general parameters and
 *                region configurations
 *
 * @details       This function is non-reentrant
 *
 * @param[in]     pConfig: pointer to configuration structure for DMA_MUX.
 * @return        void
 *
 * @pre           None
 *
 **/
void Dma_Mux_Ip_Init(const Dma_Mux_Ip_ConfigType * pConfig);

#define RM_STOP_SEC_CODE
#include "Rm_MemMap.h"

#endif /* if (RM_IP_ENABLE_DMA_MUX == STD_ON) */
#endif /* ifdef RM_IP_ENABLE_DMA_MUX */

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DMA_MUX_IP_H */
