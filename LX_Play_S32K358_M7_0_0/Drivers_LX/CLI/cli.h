/**
  ******************************************************************************
  * @file    cli.h
  * @brief   Minimal command line interface (CLI) core.
  * @details Data driven CLI: commands are described in cli_cfg.c only.
  *          - No dynamic memory, no runtime allocation.
  *          - Commands are indexed by a small open-addressing hash table
  *            (built once, 1 byte per slot) -> O(1) lookup.
  *          - Line parsing uses a single pass, in place tokenizer (no copies).
  * @author  Shawn.Liu
  * @date    2026-11-17
  * @version V1.0
  ******************************************************************************
  */

#ifndef CLI_H_
#define CLI_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Included Files
 ******************************************************************************/
#include <stdint.h>

/*******************************************************************************
 * Macro Definitions
 ******************************************************************************/
#define CLI_LINE_MAX            (64U)       /* Max command line length, incl. '\0'. */
#define CLI_MAX_ARGC            (5U)        /* Max tokens: command + 4 arguments.    */
#define CLI_ARGC_UNLIMITED      (0xFFU)     /* Value for Cli_CommandType.maxArgs.    */

/*******************************************************************************
 * Type Definitions
 ******************************************************************************/
/* Command handler: argv[0] is the command name, argc includes it. */
typedef void (*Cli_HandlerType)(uint8_t argc, char *argv[]);

/* One command description, placed in flash by const. */
typedef struct
{
    const char      *name;      /* Command word (lower case, no spaces).        */
    Cli_HandlerType  handler;   /* Function called when the command matches.    */
    uint8_t          minArgs;   /* Min argument count, command name excluded.   */
    uint8_t          maxArgs;   /* Max argument count, CLI_ARGC_UNLIMITED = any.*/
    const char      *usage;     /* One line help text, e.g. "led <on|off>".     */
} Cli_CommandType;

/*******************************************************************************
 * Public Function Prototypes
 ******************************************************************************/
/* Poll from a periodic task (e.g. 10 ms). Drains UART Rx FIFO and executes lines. */
void    CLI_MainFunction(void);

/* Execute one NUL terminated command line (tokenized in place). */
void    CLI_Execute(char *line);

/* Output helpers, non blocking (they only fill the UART Tx ring). */
void    CLI_Print(const char *str);
void    CLI_PrintLine(const char *str);
void    CLI_PrintInt(int32_t value);

/* Local echo control (off by default, Tera Term uses its own local echo). */
void    CLI_SetEcho(uint8_t enable);
uint8_t CLI_GetEcho(void);

#ifdef __cplusplus
}
#endif

#endif /* CLI_H_ */
