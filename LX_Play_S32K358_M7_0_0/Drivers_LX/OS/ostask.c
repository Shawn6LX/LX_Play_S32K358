/* Standard includes. */
#include <stdlib.h>
#include <string.h>
/* User includes. */
#include "ostask.h"
#include "Global_Cfg.h"


static void OSTask_10ms(void *pvParameters)
{
    (void)pvParameters;
    while(1)
    {
        // if(Key_Is_Pressed(KEY_1_INDEX))
        // {
        //     LED_Set_Light(LED_PORT_YELLOW, LED_LIGHT_ON);
        // }
        // else
        // {
        //     LED_Set_Light(LED_PORT_YELLOW, LED_LIGHT_OFF);
        // }
        vTaskDelay(MTASK_DELAY_10_MS);
    }
}

#if 0
static void OSTask_50ms(void *pvParameters)
{
    (void)pvParameters;
    while(1)
    {
        // UART_MainFunction();
        // if(Key_Is_Pressed(KEY_2_INDEX))
        // {
        //     LED_Set_Light(LED_PORT_RED, LED_LIGHT_ON);
        // }
        // else
        // {
        //     LED_Set_Light(LED_PORT_RED, LED_LIGHT_OFF);
        // }
        vTaskDelay(MTASK_DELAY_50_MS);
    }
}
#endif

static void OSTask_100ms(void *pvParameters)
{
    (void)pvParameters;
    while(1)
    {
        // OLED_write_number(10, 12, Timer_Get_Counter(), 16, 0);
        // if(Key_Is_Pressed(KEY_3_INDEX))
        // {
        //     LED_Set_Light(LED_PORT_GREEN, LED_LIGHT_ON);
        // }
        // else
        // {
        //     LED_Set_Light(LED_PORT_GREEN, LED_LIGHT_OFF);
        // }
        vTaskDelay(MTASK_DELAY_100_MS);
    }
}

static void OSTask_500ms(void *pvParameters)
{
    (void)pvParameters;
    while(1)
    {
        /* It shall be PIN_Mask for the 2nd parm of TogglePins. */
        Siul2_Dio_Ip_TogglePins(LED0_GREEN_PORT, (1 << LED0_GREEN_PIN));
        // Siul2_Dio_Ip_WritePin(LED0_GREEN_PORT, LED0_GREEN_PIN, 1U);

        vTaskDelay(MTASK_DELAY_500_MS);
    }
}

void OSTask_RTOS_Init(void)
{
    /* 
     * BaseType_t xTaskCreate(
     *    TaskFunction_t pvTaskCode,            // 任务函数指针
     *    const char *pcName,                   // 任务名字（调试用）
     *    configSTACK_DEPTH_TYPE usStackDepth,  // 栈大小（字）
     *    void *pvParameters,                   // 传递给任务的参数
     *    UBaseType_t uxPriority,               // 任务优先级
     *    TaskHandle_t *pvCreatedTask           // 任务句柄（输出）
     * );
     */
    xTaskCreate(OSTask_10ms,    "10ms_Task",    MTASK_STACK_2K ,  NULL,   8, NULL);
    // xTaskCreate(OSTask_50ms,    "50ms_Task",    MTASK_STACK_1K ,  NULL,   6, NULL);
    xTaskCreate(OSTask_100ms,   "100ms_Task",   MTASK_STACK_2K ,  NULL,   4, NULL);
    xTaskCreate(OSTask_500ms,   "500ms_Task",   MTASK_STACK_1K ,  NULL,   2, NULL);

    //LOG_PRINT_INFO("Initialization Complete. MCU Freq: %dMhz.", Delay_GetMcuFreq());
}


void OSTask_RTOS_Startup(void)
{
    vTaskStartScheduler();
}




