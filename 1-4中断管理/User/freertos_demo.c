#include "freertos_demo.h"

#include "main.h"
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"



/******************************************************************************************************/
/*FreeRTOS配置*/

/* 任务启动函数 */
/* 任务：优先级、堆栈大小、句柄、函数名称 */
#define START_TASK_PRIORITY 1
#define START_TASK_STACK_DEPTH 128
TaskHandle_t Start_task_handler;
void Start_Task(void *pvParameters);

/* 任务1函数 */
/* 任务：优先级、堆栈大小、句柄、函数名称 */
#define TASK1_PRIORITY 2
#define TASK1_STACK_DEPTH 128
TaskHandle_t task1_handler;
void Task1(void *pvParameters);




/* 入口函数 */
/*
* @description：freertos入口函数
* @param：none
* @ret none
*/
void freertos_demo(void)
{
    xTaskCreate( Start_Task,
                 "Start_Task",
                 START_TASK_STACK_DEPTH,
                 NULL,
                 START_TASK_PRIORITY,
                 &Start_task_handler);
    vTaskStartScheduler();    
}

/* 任务启动函数 */
/*
* @description：任务启动函数
* @param：*pvParameters
* @ret none
*/
void Start_Task(void *pvParameters)
{
    taskENTER_CRITICAL();               /* 进入临界区，防止创建任务时被打断 */

    xTaskCreate( Task1,
                 "Task1",
                TASK1_STACK_DEPTH,
                 NULL,
                 TASK1_PRIORITY,
                 &task1_handler);
    
    vTaskDelete(NULL);
    taskEXIT_CRITICAL();                  /* 任务创建完成，退出临界区 */
  
}

/*任务函数*/

/* 任务1函数 */
/*
* @description：LED1 每10ms翻转一次
* @param：*pvParameters
* @ret none
*/
void Task1(void *pvParameters)
{
    while(1)
    {
        uint8_t count = 0;
        uint32_t delay = 0;
        while(1)
        {
            delay = 500000;
            if (++count == 5)
            {
                /* code */
                count = 0;
                portDISABLE_INTERRUPTS();
                while (delay--);
                portENABLE_INTERRUPTS();   
            }
            vTaskDelay(1000);
            
        }
    }
}

/* 任务2函数 */
/*
* @description：LED1 每50ms翻转一次
* @param：*pvParameters
* @ret none
*/
void Task2(void *pvParameters)
{
    while(1)
    {
       HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);
       vTaskDelay(50);
    }
}

/* 任务3函数 */
/*
* @description：2000ms后挂起任务1 3000ms后恢复任务1
* @param：*pvParameters
* @ret none
*/
void Task3(void *pvParameters)
{
    while(1)
    {
       vTaskDelay(2000);
       vTaskSuspend(task1_handler);
       vTaskDelay(3000);
       vTaskResume(task1_handler);
    }
}



