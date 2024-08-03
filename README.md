# Keil_project
keil工程
# FreeRTOS移植成功及创建简单任务
*2024.7.31*
**移植步骤**
1. 目录添加源码文件
   1. 在工程中创建 “FreeRTOS” 文件夹，并且创建 “portable” 和“source” 两个空文件夹。
   2. 拷贝FreeRTOS源码Source文件下7个.文件到新创建的 “source”文件夹下。
   3. 拷贝FreeRTOS源码Portable文件夹下的 “Keil”、“RVDS”、 “MemMang”到新创建的 “portable”文件夹下。--其中MenMang可以只保留heap_4.c文件。
   4. 拷贝FreeRTOS源码include文件夹到 “FreeRTOS”文件夹下。
   5. FreeRTOSConfig.h 文件是 FreeRTOS 的工程配置文件，在源码“..\FreeRTOS\Demo”文件夹下面找到 “CORTEX_STM32F103_Keil” 这个文件夹下，找到 “FreeRTOSConfig.h”文件，然后拷贝到我们工程下的 “Core/Inc” 文件夹下即可。
2. 工程中添加源码文件
   1. 工程新建Group“FreeRTOS/Source”和“FreeRTOS/Portable”。。
   2. 分别按照上述内容添加C文件。
   3. 添加头文件，添加相应路径。
3. 系统配置文件的修改
   1. FreeRTOSConfig.h中添加如下3个配置：
   `#define xPortPendSVHandler  PendSV_Handler`
   `#define vPortSVCHandler     SVC_Handler` 
   `#define INCLUDE_xTaskGetSchedulerState   1`
   2. 修改stm32f1xx_it.c
     1. 引入头文件
      `#include "FreeRTOS.h"`
      `#include "task.h"`--注意这两个头文件有先后次序
     2. 注释头文件
      将PendSV_Handler、SVC_Handler 两个头文件注释掉。
     3. 添加SysTick时钟中断服务函数
      `extern void xPortSysTickHandler(void);`
      `if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) `
        `xPortSysTickHandler();`

    