#include "freertos_demo.h"

#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#include "queue.h"
#include "semphr.h"

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
QueueSetHandle_t queueset_handle;
QueueHandle_t queue_handle;
QueueHandle_t semphr_handle;
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
    /* 创建队列集，可以存放2个队列 */
    queueset_handle = xQueueCreateSet(2);
    if (queueset_handle != NULL)
    {
        printf("queueSet create success\r\n");
    }

    queue_handle = xQueueCreate(1, sizeof(uint8_t));
    semphr_handle = xSemaphoreCreateBinary();

    xQueueAddToSet(queue_handle, queueset_handle);
    xQueueAddToSet(semphr_handle, queueset_handle);
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
* @description：实现队列发送以及信号量释放
* @param：*pvParameters
* @ret none
*/
void Task1(void *pvParameters)
{
    uint8_t count = 0;
    BaseType_t err = 0;
     while (1)
     {
        count++;
        if(count>5)
        {
            count = 0;
        }
        if ((count>0) && (count<4))
        {
            err = xQueueSend(queue_handle, &count, portMAX_DELAY);
            if (err == pdPASS)
            {
                printf("to queue_handle is success\r\n");
            }
            }
            else 
             {
                err = xSemaphoreGive(semphr_handle);
                if (err == pdPASS)
                {
                    printf("give is success\r\n");
                }
            }
        vTaskDelay(10);
    }
}

/* 任务2函数 */
/*
* @description：获取队列集的消息
* @param：*pvParameters
* @ret none
*/
void Task2(void *pvParameters)
{
    QueueSetMemberHandle_t member_handle;
    uint8_t key;
    while (1)
    {
        member_handle = xQueueSelectFromSet(queueset_handle, portMAX_DELAY);
        if (member_handle == queue_handle)
        {
             xQueueReceive(member_handle, &key, portMAX_DELAY);
            printf("get data =%d\r\n", key);
         }
         else if (member_handle == semphr_handle)
         {
            xSemaphoreTake(member_handle, portMAX_DELAY);
             printf("take Binary is success\r\n");
        }
    }
}

/* 任务3函数 */
/*
* @description：LED1 每100ms翻转一次
* @param：*pvParameters
* @ret none
*/
void Task3(void *pvParameters)
{
    while(1)
    {
       HAL_GPIO_TogglePin(LED3_GPIO_Port,LED3_Pin);
       vTaskDelay(100);
    }
}
