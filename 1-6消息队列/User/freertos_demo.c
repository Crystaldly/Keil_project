#include "freertos_demo.h"

#include "main.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


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
QueueHandle_t queue1;/* 小数据句柄 */
QueueHandle_t big_queue;/* 大数据句柄 */
char buff[100] = {"abcdefg334446666hijklmn"};

/*
* @description：freertos入口函数
* @param：none
* @ret none
*/
void freertos_demo(void)
{
    queue1 = xQueueCreate(2,sizeof(uint8_t));
    if (queue1 != NULL)
    {
        printf("队列queue1创建成功，oh yeah！\r\n");
    }
    else
    {
        printf("队列queue1创建失败，oh NO！\r\n");
    }
    big_queue = xQueueCreate(1,sizeof(char*));
    if (queue1 != NULL)
    {
        printf("队列big_queue创建成功，oh yeah！\r\n");
    }
    else
    {
        printf("队列big_queue创建失败，oh NO！\r\n");
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
* @description：入队
* @param：*pvParameters
* @ret none
*/

void Task1(void *pvParameters)
{
    static uint16_t count = 0;
    BaseType_t err = 0;
    char *buf  = &buff[0];
    while(1)
    {
        count++;
        if(count > 105)
        {
            count = 0;
        }
        printf("%d\r\n",count);
        if ((count > 100) && (count<102))
        {
            err = xQueueSend(queue1, &count, portMAX_DELAY);
            if(err != pdTRUE)
            {
                printf("queue1 is fail !!\r\n");
            }
            else
            {
                printf("queue1 is success!!\r\n");
                printf("count is %d\r\n",count+1000);
                
            }
        }
        else if((count > 103) && (count < 105))
        {
            err = xQueueSend(big_queue,&buf,portMAX_DELAY);
            if(err != pdTRUE)
            {
                printf("big_queue is fail !!\r\n");
            }
            else
            {
                printf("big_queue is success!!\r\n");
                printf("count is %d\r\n",count+1000);
            }
        }
       vTaskDelay(10);
    }
}

/* 任务2函数 */
/*
* @description：小数据出队
* @param：*pvParameters
* @ret none
*/
void Task2(void *pvParameters)
{
    uint16_t count = 0;
    BaseType_t err = 0;

    while(1)
    {
       err = xQueueReceive(queue1,&count,portMAX_DELAY);
       if(err != pdTRUE)
        {
            printf("queue1 reading is fail !!\r\n");
        }
       else
        {
            printf("queue1 reading is success!!\r\n");
            printf("count is %d\r\n",count);
        }
    }
}

/* 任务3函数 */
/*
* @description：大数据出队
* @param：*pvParameters
* @ret none
*/
void Task3(void *pvParameters)
{
    char *buf;
    BaseType_t err = 0;
    while(1)
    {
       err = xQueueReceive(big_queue, &buf, portMAX_DELAY);
        if(err != pdTRUE)
        {
            printf("big_queue reading is fail !!\r\n");
        }
        else
        {
            printf("big_queue reading is success!!\r\n");
            printf("buf is %s\r\n",buf);
        }
    }
}
