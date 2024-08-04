#include "freertos_demo.h"

#include "main.h"
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"

/******************************************************************************************************/
/*FreeRTOS配置*/

// #if ( configSUPPORT_STATIC_ALLOCATION == 1 )/* configSUPPORT_STATIC_ALLOCATION必须设置为 1 in FreeRTOSConfig.h使此 RTOS API 函数可用 */
//     TaskHandle_t xTaskCreateStatic( TaskFunction_t pxTaskCode,                              /* 指向任务函数的指针 */
//                                     const char * const pcName,                              /* 任务函数名 */
//                                     const uint32_t ulStackDepth,                            /* 任务堆栈大小，单位4字节 */
//                                     void * const pvParameters,                              /* 传递的任务函数参数  */
//                                     UBaseType_t uxPriority,                                 /* 任务优先级 */
//                                     StackType_t * const puxStackBuffer,                     /* 任务堆栈，一般为数组，由用户分配 */
//                                     StaticTask_t * const pxTaskBuffer ) PRIVILEGED_FUNCTION;/* 任务控制块指针，由用户分配 */
// #endif

//     /* 正在创建的任务将用作其堆栈的缓冲区的空间。注意:这是堆栈将保存的字数，而不是字节数。例如，如果每个堆栈项都是32位，并将其设置为100，那么将分配400字节(100 * 32位) */
//     #define STACK_SIZE 200

//     /* 结构，该结构将保存正在创建的任务的TCB */
//     StaticTask_t xTaskBuffer;

//     /* 被创建的任务将用作其堆栈的缓冲区。注意这是StackType_t数组变量。StackType_t的大小取决于RTOS端口 */
//     StackType_t xStack[ STACK_SIZE ];

//     /* 函数，该函数实现正在创建的任务 */
//     void vTaskCode( void * pvParameters )
//     {
//         /* 参数值预期为1，因为1是在调用xTaskCreateStatic()的pvParameters值中传递的。 */
//         configASSERT( ( uint32_t ) pvParameters == 1UL );

//         for( ;; )
//         {
//             /* 任务代码 */
//         }
//     }

//     /* 创建任务的函数 */
//     void vOtherFunction( void )
//     {
//         TaskHandle_t xHandle = NULL;

//         /* 在不使用任何动态内存分配的情况下创建任务 */
//         xHandle = xTaskCreateStatic(
//                       vTaskCode,       /* 实现任务的函数 */
//                       "NAME",          /* 任务的文本名称 */
//                       STACK_SIZE,      /* xStack数组的索引 */
//                       ( void * ) 1,    /* 参数传递到任务 */
//                       tskIDLE_PRIORITY,/* 创建任务的优先级 */
//                       xStack,          /* 数组用作任务的堆栈 */
//                       &xTaskBuffer );  /* 变量来保存任务的数据结构 */

//         /* puxStackBuffer和pxTaskBuffer不是NULL，所以任务将被创建，xHandle将是任务的句柄。使用句柄挂起任务 */
//         [vTaskSuspend](/Documentation/02-Kernel/04-API-references/02-Task-control/06-vTaskSuspend)( xHandle );
//     }

/*********************任务设置***************************/
/* 启动任务配置 */
/* 任务：任务句柄、优先级、堆栈大小、堆栈大小 */
#define START_TASK_PRIO 1
#define START_TASK_STACK_SIZE 128
TaskHandle_t start_task_handler;
StackType_t  start_task_stack[START_TASK_STACK_SIZE];
StaticTask_t start_task_tcb;
void start_task(void *pvParameters);

/* TASK1任务配置 */
/* 任务：任务句柄、优先级、堆栈大小、堆栈大小 */
#define TASK1_PRIO 2
#define TASK1_STACK_SIZE 128
TaskHandle_t task1_handler;
StackType_t  task1_stack[TASK1_STACK_SIZE];
StaticTask_t task1_tcb;
void Task1(void *pvParameters);

/* TASK2任务配置 */
/* 任务：任务句柄、优先级、堆栈大小、堆栈大小 */
#define TASK2_PRIO 3
#define TASK2_STACK_SIZE 128
TaskHandle_t task2_handler;
StackType_t  task2_stack[TASK2_STACK_SIZE];
StaticTask_t task2_tcb;
void Task2(void *pvParameters);

/* TASK3任务配置 */
/* 任务：任务句柄、优先级、堆栈大小、堆栈大小 */
#define TASK3_PRIO 4
#define TASK3_STACK_SIZE 128
TaskHandle_t task3_handler;
StackType_t  task3_stack[TASK3_STACK_SIZE];
StaticTask_t task3_tcb;
void Task3(void *pvParameters);

/************************空闲任务和定时器配置及其接口***************************/
/* 空闲任务配置 */
StaticTask_t idle_task_tcb;
StackType_t  idle_task_stack[configMINIMAL_STACK_SIZE];

/* 软件定时器任务配置 */
StaticTask_t timer_task_tcb;
StackType_t  timer_task_stack[configTIMER_TASK_STACK_DEPTH];

/* 空闲任务内存分配 */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t  **ppxIdleTaskStackBuffer,
                                   uint32_t     * pulIdleTaskStackSize)
{
    * ppxIdleTaskTCBBuffer   = &idle_task_tcb;
    * ppxIdleTaskStackBuffer = idle_task_stack;
    * pulIdleTaskStackSize   = configMINIMAL_STACK_SIZE;
}

/* 软件定时器内存分配 */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t  **ppxTimerTaskStackBuffer,
                                     uint32_t     * pulTimerTaskStackSize )
{
    * ppxTimerTaskTCBBuffer   = &timer_task_tcb;
    * ppxTimerTaskStackBuffer = timer_task_stack;
    * pulTimerTaskStackSize   = configTIMER_TASK_STACK_DEPTH;
}

/*入口函数*/
/**
 * @description:FreeRTOS 入口函数：创建任务函数并开始调度
 * @return：none
 */
void freertos_demo(void)
{
    start_task_handler = xTaskCreateStatic(start_task,
                                           "start_task",
                                           START_TASK_STACK_SIZE,
                                           NULL,
                                           START_TASK_PRIO,
                                           start_task_stack,
                                           &start_task_tcb);
    vTaskStartScheduler();
}


/* 启动函数 */
/**
 * @description:启动函数
 * @return：none
 */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();/* 进入临界区 */

    task1_handler = xTaskCreateStatic(Task1,
                                      "Task1",
                                      TASK1_STACK_SIZE,
                                      NULL,
                                      TASK1_PRIO,
                                      task1_stack,
                                      &task1_tcb);
    
    task2_handler = xTaskCreateStatic(Task2,
                                     "Task2",
                                     TASK2_STACK_SIZE,
                                     NULL,
                                     TASK2_PRIO,
                                     task2_stack,
                                     &task2_tcb);

    task3_handler = xTaskCreateStatic(Task3,
                                      "Task3",
                                      TASK3_STACK_SIZE,
                                      NULL,
                                      TASK3_PRIO,
                                      task3_stack,
                                      &task3_tcb);
    vTaskDelete(NULL);

    taskEXIT_CRITICAL();/*退出临界区*/
}

/* 实现函数 */
void Task1(void *pvParameters)
{
    while(1)
    {
        HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
        vTaskDelay(5);
    }
}

void Task2(void *pvParameters)
{
    while(1)
    {
        HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);
        vTaskDelay(10);
    }
}

void Task3(void *pvParameters)
{
    while(1)
    {
        HAL_GPIO_TogglePin(LED3_GPIO_Port,LED3_Pin);
        vTaskDelay(1000);
    }
}


