#include "freertos_demo.h"

#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/**
 * @description: for循环实现延时函数
 * @param {uint32_t} ms
 * @return {*}
 */
void for_delay_ms(uint32_t ms)
{
    uint32_t Delay = ms * 72000/9; /* 72M时钟频率，9是PLL倍频 */
    do
    {
        __NOP(); /* 空指令（NOP）来占用 CPU 时间 */
    }
    while (Delay --);
}

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

/* 任务2函数 */
/* 任务：优先级、堆栈大小、句柄、函数名称 */
#define TASK2_PRIORITY 3
#define TASK2_STACK_DEPTH 128
TaskHandle_t task2_handler;
void Task2(void *pvParameters);

/* 任务3函数 */
/* 任务：优先级、堆栈大小、句柄、函数名称 */
#define TASK3_PRIORITY 4
#define TASK3_STACK_DEPTH 128
TaskHandle_t task3_handler;
void Task3(void *pvParameters);

/* 入口函数 */
QueueHandle_t mutex_semphore_handle;
/*
* @description：freertos入口函数
* @param：none
* @ret none
*/
void freertos_demo(void)
{
    mutex_semphore_handle = xSemaphoreCreateMutex();
    if(mutex_semphore_handle != NULL)
    {
        printf("Mutex create success\r\n");
    }
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
                
    xTaskCreate( Task2,
                 "Task2",
                TASK2_STACK_DEPTH,
                 NULL,
                 TASK2_PRIORITY,
                 &task2_handler);

    xTaskCreate( Task3,
                 "Task3",
                TASK3_STACK_DEPTH,
                 NULL,
                 TASK3_PRIORITY,
                 &task3_handler);
    
    vTaskDelete(NULL);
    taskEXIT_CRITICAL();                  /* 任务创建完成，退出临界区 */
  
}

/*任务函数*/

/* 任务1函数 */
/*
* @description：低优先级任务
* @param：*pvParameters
* @ret none
*/
void Task1(void *pvParameters)
{
    while(1)
    {
        printf("task1 is taking \r\n");
        xSemaphoreTake(mutex_semphore_handle,portMAX_DELAY);
        printf("task1 take over\r\n");

        printf("task1 running\r\n");
        for_delay_ms(3000);
        
        printf("task1 is giving \r\n");
        xSemaphoreGive(mutex_semphore_handle); 
         printf("task1 give over\r\n");
        vTaskDelay(1000);
    }
}

/* 任务2函数 */
/*
* @description：任务2：获取二值信号量，打印相关信息
* @param：*pvParameters
* @ret none
*/
void Task2(void *pvParameters)
{
    while(1)
    {
        printf("task2 running\r\n");
        for_delay_ms(1500);
        printf("Task2 for is over \r\n");
        vTaskDelay(1000);
    }
}

/* 任务3函数 */
/*
* @description：高优先级任务
* @param：*pvParameters
* @ret none
*/
void Task3(void *pvParameters)
{
    while(1)
    {
        printf("task3 is taking \r\n");
        xSemaphoreTake(mutex_semphore_handle,portMAX_DELAY);
        printf("task3 take over\r\n");    

        printf("task3 running \r\n");
        for_delay_ms(1000);

        printf("task3 is giving \r\n");
        xSemaphoreGive(mutex_semphore_handle); 
        printf("task3 give over\r\n");

        vTaskDelay(1000);
    }
}
