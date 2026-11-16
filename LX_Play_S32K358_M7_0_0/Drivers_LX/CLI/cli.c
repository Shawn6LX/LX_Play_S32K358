/**
  ******************************************************************************
  * @file    cli.c
  * @brief   Minimal command line interface (CLI) core implementation.
  * @details Resource footprint:
  *          - Flash : ~600 bytes (core, without command handlers)
  *          - RAM   : 64 B line buffer + 32 B hash index + 4 B state
  *          - Stack : < 40 bytes (no recursion, no printf inside)
  *          - CPU   : one hash + one strcmp per command, single pass parsing
  * @author  Shawn.Liu
  * @date    2026-11-17
  * @version V1.0
  ******************************************************************************
  */

/*******************************************************************************
 * Included Files
 ******************************************************************************/
#include <string.h>
#include "cli.h"
#include "cli_cfg.h"
#include "uart.h"

/*******************************************************************************
 * Internal Macro Definitions
 ******************************************************************************/
/* Hash table size, MUST be a power of two and at least 2 x CLI_CMD_COUNT. */
#define CLI_HASH_SIZE           (32U)
#define CLI_HASH_MASK           (CLI_HASH_SIZE - 1U)
#define CLI_HASH_EMPTY          (0xFFU)

#if ((CLI_CMD_COUNT * 2U) > CLI_HASH_SIZE)
#error "CLI_HASH_SIZE is too small for CLI_CMD_COUNT"
#endif

/*******************************************************************************
 * Static Local Variables
 ******************************************************************************/
static char    Cli_acLine[CLI_LINE_MAX];                     /* Line assembly buffer. */
static uint8_t Cli_u8LineLen;                                /* Current line length.  */
static uint8_t Cli_u8Echo;                                   /* Local echo enable.    */
static uint8_t Cli_u8IndexReady;                             /* Hash index valid.     */
static uint8_t Cli_au8Hash[CLI_HASH_SIZE];                   /* Command index table.  */

/*******************************************************************************
 * Static Internal Functions
 ******************************************************************************/
/*!
 * @brief  djb2 hash, folded to the table size.
 */
static uint8_t CLI_Hash(const char *name)
{
    uint32_t hash = 5381U;

    while (*name != '\0')
    {
        hash = ((hash << 5U) + hash) ^ (uint8_t)(*name);
        name++;
    }

    return (uint8_t)(hash & CLI_HASH_MASK);
}

/*!
 * @brief  Build the open-addressing index once (CLI_CMD_COUNT x hash).
 */
static void CLI_BuildIndex(void)
{
    uint8_t i;
    uint8_t slot;

    for (i = 0U; i < CLI_HASH_SIZE; i++)
    {
        Cli_au8Hash[i] = CLI_HASH_EMPTY;
    }

    for (i = 0U; i < (uint8_t)CLI_CMD_COUNT; i++)
    {
        slot = CLI_Hash(Cli_aCommandTable[i].name);
        while (Cli_au8Hash[slot] != CLI_HASH_EMPTY)
        {
            slot = (uint8_t)((slot + 1U) & CLI_HASH_MASK);
        }
        Cli_au8Hash[slot] = i;
    }

    Cli_u8IndexReady = 1U;
}

/*!
 * @brief  Look up a command by name, O(1) expected.
 */
static const Cli_CommandType *CLI_Find(const char *name)
{
    uint8_t slot = CLI_Hash(name);

    while (Cli_au8Hash[slot] != CLI_HASH_EMPTY)
    {
        const Cli_CommandType *cmd = &Cli_aCommandTable[Cli_au8Hash[slot]];
        if (strcmp(cmd->name, name) == 0)
        {
            return cmd;
        }
        slot = (uint8_t)((slot + 1U) & CLI_HASH_MASK);
    }

    return NULL;
}

/*!
 * @brief  Split a line in place: spaces/tabs -> '\0', argv points into the line.
 * @return Number of tokens (0 for a blank line).
 */
static uint8_t CLI_Tokenize(char *line, char *argv[])
{
    uint8_t argc = 0U;
    char   *p    = line;

    while ((*p != '\0') && (argc < CLI_MAX_ARGC))
    {
        while ((*p == ' ') || (*p == '\t'))
        {
            *p = '\0';
            p++;
        }
        if (*p == '\0')
        {
            break;
        }

        argv[argc] = p;
        argc++;

        while ((*p != '\0') && (*p != ' ') && (*p != '\t'))
        {
            p++;
        }
    }

    return argc;
}

/*!
 * @brief  Execute the assembled line (if it is not empty).
 */
static void CLI_RunLine(void)
{
    if (Cli_u8LineLen == 0U)
    {
        return;                         /* Ignore blank lines (CR/LF only). */
    }

    Cli_acLine[Cli_u8LineLen] = '\0';

    if (Cli_u8Echo != 0U)
    {
        CLI_Print("\r\n");
    }

    CLI_Execute(Cli_acLine);
    Cli_u8LineLen = 0U;
}

/*!
 * @brief  Feed one received byte to the line editor.
 */
static void CLI_Input(char c)
{
    if (c == '\0')                      /* Line terminator, produced by the UART layer. */
    {
        CLI_RunLine();
    }
    else if ((c == '\b') || (c == (char)0x7F))   /* Backspace / DEL. */
    {
        if (Cli_u8LineLen > 0U)
        {
            Cli_u8LineLen--;
            if (Cli_u8Echo != 0U)
            {
                CLI_Print("\b \b");
            }
        }
    }
    else if ((c >= 0x20) && (c <= 0x7E))         /* Printable ASCII only. */
    {
        if (Cli_u8LineLen < (CLI_LINE_MAX - 1U))
        {
            Cli_acLine[Cli_u8LineLen] = c;
            Cli_u8LineLen++;
            if (Cli_u8Echo != 0U)
            {
                char echo[2];
                echo[0] = c;
                echo[1] = '\0';
                CLI_Print(echo);
            }
        }
    }
    else
    {
        /* Ignore other control characters. */
    }
}

/*******************************************************************************
 * Public Functions
 ******************************************************************************/
/*!
 * @brief  Execute one command line.
 */
void CLI_Execute(char *line)
{
    char *argv[CLI_MAX_ARGC];
    const Cli_CommandType *cmd;
    uint8_t argc;
    uint8_t nargs;

    if (Cli_u8IndexReady == 0U)
    {
        CLI_BuildIndex();
    }

    argc = CLI_Tokenize(line, argv);
    if (argc == 0U)
    {
        return;
    }

    cmd = CLI_Find(argv[0]);
    if (cmd == NULL)
    {
        CLI_Print("unknown command: ");
        CLI_PrintLine(argv[0]);
        return;
    }

    nargs = (uint8_t)(argc - 1U);
    if ((nargs < cmd->minArgs) ||
        ((cmd->maxArgs != CLI_ARGC_UNLIMITED) && (nargs > cmd->maxArgs)))
    {
        CLI_Print("usage: ");
        CLI_PrintLine(cmd->usage);
        return;
    }

    cmd->handler(argc, argv);
}

/*!
 * @brief  Poll function, call it periodically (10 ms task).
 */
void CLI_MainFunction(void)
{
    uint8_t  buf[8];
    uint16_t n;
    uint16_t i;
    uint8_t  guard = 0U;

    if (Cli_u8IndexReady == 0U)
    {
        CLI_BuildIndex();
    }

    /* Bounded draining, keeps the 10 ms task deterministic. */
    while ((guard < 8U) && ((n = UART_Read(buf, (uint16_t)sizeof(buf))) > 0U))
    {
        guard++;
        for (i = 0U; i < n; i++)
        {
            CLI_Input((char)buf[i]);
        }
    }
}

/*!
 * @brief  Non blocking string output.
 */
void CLI_Print(const char *str)
{
    if ((str != NULL) && (*str != '\0'))
    {
        (void)UART_Write((const uint8_t *)str, (uint16_t)strlen(str));
    }
}

/*!
 * @brief  Non blocking string output with CRLF.
 */
void CLI_PrintLine(const char *str)
{
    CLI_Print(str);
    CLI_Print("\r\n");
}

/*!
 * @brief  Minimal decimal integer output (no printf pulled in).
 */
void CLI_PrintInt(int32_t value)
{
    char     buf[12];
    uint8_t  idx = (uint8_t)sizeof(buf);
    uint32_t mag;

    mag = (value < 0) ? (uint32_t)(-value) : (uint32_t)value;

    buf[--idx] = '\0';
    do
    {
        buf[--idx] = (char)('0' + (mag % 10U));
        mag /= 10U;
    } while (mag > 0U);

    if (value < 0)
    {
        buf[--idx] = '-';
    }

    CLI_Print(&buf[idx]);
}

/*!
 * @brief  Enable/disable local echo.
 */
void CLI_SetEcho(uint8_t enable)
{
    Cli_u8Echo = (enable != 0U) ? 1U : 0U;
}

/*!
 * @brief  Get local echo state.
 */
uint8_t CLI_GetEcho(void)
{
    return Cli_u8Echo;
}
