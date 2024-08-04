#include "freertos_demo.h"

#include "main.h"
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"

/******************************************************************************************************/
/*FreeRTOS����*/

// #if ( configSUPPORT_STATIC_ALLOCATION == 1 )/* configSUPPORT_STATIC_ALLOCATION��������Ϊ 1 in FreeRTOSConfig.hʹ�� RTOS API �������� */
//     TaskHandle_t xTaskCreateStatic( TaskFunction_t pxTaskCode,                              /* ָ����������ָ�� */
//                                     const char * const pcName,                              /* �������� */
//                                     const uint32_t ulStackDepth,                            /* �����ջ��С����λ4�ֽ� */
//                                     void * const pvParameters,                              /* ���ݵ�����������  */
//                                     UBaseType_t uxPriority,                                 /* �������ȼ� */
//                                     StackType_t * const puxStackBuffer,                     /* �����ջ��һ��Ϊ���飬���û����� */
//                                     StaticTask_t * const pxTaskBuffer ) PRIVILEGED_FUNCTION;/* ������ƿ�ָ�룬���û����� */
// #endif

//     /* ���ڴ����������������ջ�Ļ������Ŀռ䡣ע��:���Ƕ�ջ��������������������ֽ��������磬���ÿ����ջ���32λ������������Ϊ100����ô������400�ֽ�(100 * 32λ) */
//     #define STACK_SIZE 200

//     /* �ṹ���ýṹ���������ڴ����������TCB */
//     StaticTask_t xTaskBuffer;

//     /* �������������������ջ�Ļ�������ע������StackType_t���������StackType_t�Ĵ�Сȡ����RTOS�˿� */
//     StackType_t xStack[ STACK_SIZE ];

//     /* �������ú���ʵ�����ڴ��������� */
//     void vTaskCode( void * pvParameters )
//     {
//         /* ����ֵԤ��Ϊ1����Ϊ1���ڵ���xTaskCreateStatic()��pvParametersֵ�д��ݵġ� */
//         configASSERT( ( uint32_t ) pvParameters == 1UL );

//         for( ;; )
//         {
//             /* ������� */
//         }
//     }

//     /* ��������ĺ��� */
//     void vOtherFunction( void )
//     {
//         TaskHandle_t xHandle = NULL;

//         /* �ڲ�ʹ���κζ�̬�ڴ���������´������� */
//         xHandle = xTaskCreateStatic(
//                       vTaskCode,       /* ʵ������ĺ��� */
//                       "NAME",          /* ������ı����� */
//                       STACK_SIZE,      /* xStack��������� */
//                       ( void * ) 1,    /* �������ݵ����� */
//                       tskIDLE_PRIORITY,/* ������������ȼ� */
//                       xStack,          /* ������������Ķ�ջ */
//                       &xTaskBuffer );  /* ������������������ݽṹ */

//         /* puxStackBuffer��pxTaskBuffer����NULL���������񽫱�������xHandle��������ľ����ʹ�þ���������� */
//         [vTaskSuspend](/Documentation/02-Kernel/04-API-references/02-Task-control/06-vTaskSuspend)( xHandle );
//     }

/*********************��������***************************/
/* ������������ */
/* ���������������ȼ�����ջ��С����ջ��С */
#define START_TASK_PRIO 1
#define START_TASK_STACK_SIZE 128
TaskHandle_t start_task_handler;
StackType_t  start_task_stack[START_TASK_STACK_SIZE];
StaticTask_t start_task_tcb;
void start_task(void *pvParameters);

/* TASK1�������� */
/* ���������������ȼ�����ջ��С����ջ��С */
#define TASK1_PRIO 2
#define TASK1_STACK_SIZE 128
TaskHandle_t task1_handler;
StackType_t  task1_stack[TASK1_STACK_SIZE];
StaticTask_t task1_tcb;
void Task1(void *pvParameters);

/* TASK2�������� */
/* ���������������ȼ�����ջ��С����ջ��С */
#define TASK2_PRIO 3
#define TASK2_STACK_SIZE 128
TaskHandle_t task2_handler;
StackType_t  task2_stack[TASK2_STACK_SIZE];
StaticTask_t task2_tcb;
void Task2(void *pvParameters);

/* TASK3�������� */
/* ���������������ȼ�����ջ��С����ջ��С */
#define TASK3_PRIO 4
#define TASK3_STACK_SIZE 128
TaskHandle_t task3_handler;
StackType_t  task3_stack[TASK3_STACK_SIZE];
StaticTask_t task3_tcb;
void Task3(void *pvParameters);

/************************��������Ͷ�ʱ�����ü���ӿ�***************************/
/* ������������ */
StaticTask_t idle_task_tcb;
StackType_t  idle_task_stack[configMINIMAL_STACK_SIZE];

/* �����ʱ���������� */
StaticTask_t timer_task_tcb;
StackType_t  timer_task_stack[configTIMER_TASK_STACK_DEPTH];

/* ���������ڴ���� */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t  **ppxIdleTaskStackBuffer,
                                   uint32_t     * pulIdleTaskStackSize)
{
    * ppxIdleTaskTCBBuffer   = &idle_task_tcb;
    * ppxIdleTaskStackBuffer = idle_task_stack;
    * pulIdleTaskStackSize   = configMINIMAL_STACK_SIZE;
}

/* �����ʱ���ڴ���� */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t  **ppxTimerTaskStackBuffer,
                                     uint32_t     * pulTimerTaskStackSize )
{
    * ppxTimerTaskTCBBuffer   = &timer_task_tcb;
    * ppxTimerTaskStackBuffer = timer_task_stack;
    * pulTimerTaskStackSize   = configTIMER_TASK_STACK_DEPTH;
}

/*��ں���*/
/**
 * @description:FreeRTOS ��ں�������������������ʼ����
 * @return��none
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


/* �������� */
/**
 * @description:��������
 * @return��none
 */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();/* �����ٽ��� */

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

    taskEXIT_CRITICAL();/*�˳��ٽ���*/
}

/* ʵ�ֺ��� */
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


