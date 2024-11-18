/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "serial.h"
#include "adc.h"
#include "debug.h"
#include "clock.h"
#include "io_ctrl.h"
#include "spi_flash.h"
#include "FreeRTOS_CLI.h"

#include "oled.h"
#include "bmp.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
ClockCntStruct SysClockCnt;
ClockFlagStruct SysClockFlag;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for admin_task */
osThreadId_t admin_taskHandle;
uint32_t admin_taskBuffer[ 128 ];
osStaticThreadDef_t admin_taskControlBlock;
const osThreadAttr_t admin_task_attributes = {
  .name = "admin_task",
  .cb_mem = &admin_taskControlBlock,
  .cb_size = sizeof(admin_taskControlBlock),
  .stack_mem = &admin_taskBuffer[0],
  .stack_size = sizeof(admin_taskBuffer),
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for oled_task */
osThreadId_t oled_taskHandle;
const osThreadAttr_t oled_task_attributes = {
  .name = "oled_task",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for io_task */
osThreadId_t io_taskHandle;
uint32_t io_taskBuffer[ 128 ];
osStaticThreadDef_t io_taskControlBlock;
const osThreadAttr_t io_task_attributes = {
  .name = "io_task",
  .cb_mem = &io_taskControlBlock,
  .cb_size = sizeof(io_taskControlBlock),
  .stack_mem = &io_taskBuffer[0],
  .stack_size = sizeof(io_taskBuffer),
  .priority = (osPriority_t) osPriorityHigh,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void Admin_Task(void *argument);
void OLED_Task(void *argument);
void IO_Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationIdleHook(void);
void vApplicationTickHook(void);
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);

/* USER CODE BEGIN 2 */
void vApplicationIdleHook( void )
{
   /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
   task. It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()). If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
}
/* USER CODE END 2 */

/* USER CODE BEGIN 3 */
void vApplicationTickHook( void )
{
   /* This function will be called by each tick interrupt if
   configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
   added here, but the tick hook is called from an interrupt context, so
   code must not attempt to block, and only the interrupt safe FreeRTOS API
   functions can be used (those that end in FromISR()). */
   sys_clock_handle(&SysClockCnt,&SysClockFlag);
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
   HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, LED_SET);
	 sprintf((char *) pcWriteBuffer, "[栈溢出]\t任务:%s\r\n", pcTaskName);
	 HAL_UART_Transmit_DMA(&huart1, pcWriteBuffer, strlen((char *) pcWriteBuffer));
}
/* USER CODE END 4 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	GPIO_OLED_InitConfig();
	PWM_Init();
	vRegisterSampleCLICommands();
	vUARTCommandConsoleStart( 1024, 1 );

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of admin_task */
  admin_taskHandle = osThreadNew(Admin_Task, NULL, &admin_task_attributes);

  /* creation of oled_task */
  oled_taskHandle = osThreadNew(OLED_Task, NULL, &oled_task_attributes);

  /* creation of io_task */
  io_taskHandle = osThreadNew(IO_Task, NULL, &io_task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	if(W25QXX_Init()==0)
		ULOG("FLASH INIT %X\t%d\r\n",flash_msg.type,flash_msg.size);
  /* Infinite loop */
  for(;;)
  {
		if(SysClockFlag.bit.b500ms)
		{
			if(0x00 == sys_msg.led_status)
				sys_gpio.write.bit.bLed = !sys_gpio.write.bit.bLed;

			if(0x01 == sys_msg.led_status)
				sys_gpio.write.bit.bLed = 1;

			if(0x02 == sys_msg.led_status)
				sys_gpio.write.bit.bLed = 0;
		}
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Admin_Task */
/**
* @brief Function implementing the admin_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Admin_Task */
void Admin_Task(void *argument)
{
  /* USER CODE BEGIN Admin_Task */
  /* Infinite loop */
  for(;;)
  {
		//Debug_usart1();
		//xSerialTransmit_Ser();
		
    osDelay(1);
  }
  /* USER CODE END Admin_Task */
}

/* USER CODE BEGIN Header_OLED_Task */
/**
* @brief Function implementing the oled_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_OLED_Task */
void OLED_Task(void *argument)
{
  /* USER CODE BEGIN OLED_Task */
	uint8_t Bufferr[15];
	int Num=1208;
	float Data=3.14;
	
	OLED_Clear();
	
  /* Infinite loop */
  for(;;)
  {
		OLED_Clear();
		OLED_ShowNum(0, 2, Num, 8, 16);
		sprintf((char *)Bufferr, "%0.2f", Data);
		OLED_ShowString(0,0, Bufferr);
		OLED_ShowNum(0, 0, Data, 8, 16);
		osDelay(600);

		
		OLED_Clear();
		OLED_ShowCHinese(0, 2, 0);
		OLED_ShowCHinese(14, 2, 1);
		OLED_ShowCHinese(28, 2, 2);
		OLED_ShowCHinese(42, 2, 3);
		osDelay(600);

		OLED_Clear();
		OLED_DrawBMP(0,0,126,8,BMP);
		osDelay(500);
    osDelay(1);
  }
  /* USER CODE END OLED_Task */
}

/* USER CODE BEGIN Header_IO_Task */
/**
* @brief Function implementing the io_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_IO_Task */
void IO_Task(void *argument)
{
  /* USER CODE BEGIN IO_Task */

  /* Infinite loop */
  for(;;)
  {
		GPIO_Ctrl_Ser();
		if(SysClockFlag.bit.b10ms)
			sys_msg.temp = adc_tem();
    osDelay(1);
  }
  /* USER CODE END IO_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

