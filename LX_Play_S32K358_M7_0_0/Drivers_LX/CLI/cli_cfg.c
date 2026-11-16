/**
  ******************************************************************************
  * @file    cli_cfg.c
  * @brief   CLI command table and command handlers.
  * @details THIS IS THE ONLY FILE TO EDIT when adding/removing a CLI command:
  *          1) add the handler prototype below,
  *          2) add one entry in Cli_aCommandTable[],
  *          3) update CLI_CMD_COUNT in cli_cfg.h (a compile time check in this
  *             file verifies both stay consistent).
  * @author  Shawn.Liu
  * @date    2026-11-17
  * @version V1.0
  ******************************************************************************
  */

/*******************************************************************************
 * Included Files
 ******************************************************************************/
#include <string.h>
#include "Global_Cfg.h"                 /* Dio, UART, project config */
#include "cli.h"
#include "cli_cfg.h"

/*******************************************************************************
 * Static Internal Function Declarations
 ******************************************************************************/
static void CLI_CmdHelp(uint8_t argc, char *argv[]);
static void CLI_CmdLed(uint8_t argc, char *argv[]);
static void CLI_CmdEcho(uint8_t argc, char *argv[]);
static void CLI_CmdVer(uint8_t argc, char *argv[]);

/*******************************************************************************
 * Global Constants
 ******************************************************************************/
/*!
 * @brief  Command table.
 * @note   Order does not matter (lookup uses a hash index).
 *         minArgs/maxArgs do NOT count the command word itself.
 */
const Cli_CommandType Cli_aCommandTable[CLI_CMD_COUNT] =
{
    /* name     handler          min  max                    usage                          */
    { "help",   CLI_CmdHelp,     0U,  0U,                    "help"                          },
    { "led",    CLI_CmdLed,      1U,  2U,                    "led <on|off> [green|red|blue]" },
    { "echo",   CLI_CmdEcho,     1U,  1U,                    "echo <on|off>"                 },
    { "ver",    CLI_CmdVer,      0U,  0U,                    "ver"                           },
};

/* Compile time check: table size must match CLI_CMD_COUNT. */
typedef char Cli_CommandCountCheck
    [(sizeof(Cli_aCommandTable) / sizeof(Cli_aCommandTable[0])) == CLI_CMD_COUNT ? 1 : -1];

/*******************************************************************************
 * Static Internal Functions
 ******************************************************************************/
/*!
 * @brief  help : list all commands with their usage text.
 */
static void CLI_CmdHelp(uint8_t argc, char *argv[])
{
    uint8_t i;

    (void)argc;
    (void)argv;

    for (i = 0U; i < (uint8_t)CLI_CMD_COUNT; i++)
    {
        CLI_PrintLine(Cli_aCommandTable[i].usage);
    }
}

/*!
 * @brief  led <on|off> [green|red|blue] : drive one board RGB LED.
 */
static void CLI_CmdLed(uint8_t argc, char *argv[])
{
    Dio_ChannelType channel = DioConf_DioChannel_LED0_GREEN;
    Dio_LevelType   level;

    if (strcmp(argv[1], "on") == 0)
    {
        level = STD_HIGH;
    }
    else if (strcmp(argv[1], "off") == 0)
    {
        level = STD_LOW;
    }
    else
    {
        CLI_PrintLine("usage: led <on|off> [green|red|blue]");
        return;
    }

    if (argc >= 3U)
    {
        if (strcmp(argv[2], "red") == 0)
        {
            channel = DioConf_DioChannel_LED0_RED;
        }
        else if (strcmp(argv[2], "blue") == 0)
        {
            channel = DioConf_DioChannel_LED0_BLUE;
        }
        else if (strcmp(argv[2], "green") == 0)
        {
            channel = DioConf_DioChannel_LED0_GREEN;
        }
        else
        {
            CLI_PrintLine("usage: led <on|off> [green|red|blue]");
            return;
        }
    }

    Dio_WriteChannel(channel, level);
}

/*!
 * @brief  echo <on|off> : enable/disable the CLI local echo.
 */
static void CLI_CmdEcho(uint8_t argc, char *argv[])
{
    (void)argc;

    if (strcmp(argv[1], "on") == 0)
    {
        CLI_SetEcho(1U);
        CLI_PrintLine("echo on");
    }
    else if (strcmp(argv[1], "off") == 0)
    {
        CLI_SetEcho(0U);
        CLI_PrintLine("echo off");
    }
    else
    {
        CLI_PrintLine("usage: echo <on|off>");
    }
}

/*!
 * @brief  ver : print firmware version.
 */
static void CLI_CmdVer(uint8_t argc, char *argv[])
{
    (void)argc;
    (void)argv;

    CLI_PrintLine("LX_Play_S32K358 CLI v1.0");
}
