/**
  ******************************************************************************
  * @file    cli_cfg.h
  * @brief   CLI command configuration interface.
  * @details Only cli_cfg.c has to be edited to add/remove CLI commands.
  * @author  Shawn.Liu
  * @date    2026-11-17
  * @version V1.0
  ******************************************************************************
  */

#ifndef CLI_CFG_H_
#define CLI_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Included Files
 ******************************************************************************/
#include "cli.h"

/*******************************************************************************
 * Macro Definitions
 ******************************************************************************/
/* Number of entries in Cli_aCommandTable[], keep the two in sync. */
#define CLI_CMD_COUNT           (4U)

/*******************************************************************************
 * Extern Global Variables
 ******************************************************************************/
extern const Cli_CommandType Cli_aCommandTable[CLI_CMD_COUNT];

#ifdef __cplusplus
}
#endif

#endif /* CLI_CFG_H_ */
