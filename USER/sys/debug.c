
#include "debug.h"

uint8_t pcWriteBuffer[BUF_DEBUG_MAX];

usartMsgCallback usartMsgCallbackArray[] =
{
	{
		DEBUG_TEST, usartRecv_test, "测试"
	},
	{
		DEBUG_READ, usartRecv_read, "参数读取"
	},
	{
		DEBUG_WRITE, usartRecv_write, "参数写入"
	},
	{
		DEBUG_TASK, usartRecv_task, "任务控制"
	},
};


static void usartRecv_write(UsartGetStruct * arg)
{
	if(arg->data == 1)
	{
		sys_msg.led_status = arg->data_t;
	}

	if(arg->data == 2)
	{
		sys_msg.led_pwm = arg->data_t;
	}

}


static void usartRecv_read(UsartGetStruct * arg)
{
	;
}


static void usartRecv_task(UsartGetStruct * arg)
{
	;
}


static void usartRecv_test(UsartGetStruct * arg)
{
	uint8_t i = 0;

	ULOG("dubug_test[%d]:%x\r\n", arg->id, arg->data);

	if(arg->data == 0) //显示调试接口函数
	{
		for(; i < sizeof(usartMsgCallbackArray) / sizeof(usartMsgCallback); i++)
		{
			sprintf((char *) pcWriteBuffer, "[%X]%s\r\n", usartMsgCallbackArray[i].msgCmdID, usartMsgCallbackArray[i].tip);
			HAL_UART_Transmit_DMA(&huart1, pcWriteBuffer, strlen((char *) pcWriteBuffer));
			vTaskDelay(10);
		}
	}
	else if(arg->data == 0x0001)
	{
		sprintf((char *) pcWriteBuffer, "\r\n任务名\t\t任务状态\t优先级\t剩余栈\t任务序号\r\n");
		HAL_UART_Transmit_DMA(&huart1, pcWriteBuffer, strlen((char *) pcWriteBuffer));
		vTaskDelay(10);
		vTaskList((char *) &pcWriteBuffer);
		HAL_UART_Transmit_DMA(&huart1, pcWriteBuffer, strlen((char *) pcWriteBuffer));
		vTaskDelay(30);

		//sprintf(pcWriteBuffer,"\r\n-------------------------------\r\n");
		//HAL_UART_Transmit_DMA(&huart5, pcWriteBuffer, strlen(pcWriteBuffer));
		ULOG("----------------------------\r\n");
		vTaskDelay(10);
	}
}


//获取列队数据
static void usart1_get_rx(UsartGetStruct * uart_data)
{
//	uint32_t len = lwrb_read(&usart_rx_format_rb, uart1rx, sizeof(uart1rx));

//	if(len > 0)
//	{
//		if((uart1rx[0] == 0xA5) && (uart1rx[len - 1] == 0xAA))
//		{
//			uart_data->id = uart1rx[1];
//			uart_data->data = (* ((uint16_t *) (uart1rx + 2)));
//			uart_data->data_t = (* ((uint16_t *) (uart1rx + 4)));
//		}
//	}
}


void Debug_usart1(void)
{
	uint8_t i = 0;
	UsartGetStruct data =
	{
		DEBUG_NULL, 0, 0
	};

	usart1_get_rx(&data);

	for(; i < sizeof(usartMsgCallbackArray) / sizeof(usartMsgCallback); i++)
	{
		if(data.id == usartMsgCallbackArray[i].msgCmdID)
		{
			usartMsgCallbackArray[i].msgCallback_recv(&data);
		}
	}

}


