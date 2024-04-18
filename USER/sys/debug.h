

#ifndef __debug_d_H
#define __debug_d_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "usart.h"
#include "serial.h"

#define 	BUF_DEBUG_MAX								128*5

	typedef enum 
	{
		DEBUG_TEST = 0, 							//测试
		DEBUG_READ, 								//读取参数
		DEBUG_WRITE,								//写参数
		DEBUG_TASK = 0xfe,							//任务控制
		DEBUG_NULL = 0xff,							//默认状态
	} UART_DEBUG;


	typedef struct 
	{
		uint8_t 		id; 						//功能码
		uint16_t		data;						//数据
		uint16_t		data_t;
	} UsartGetStruct;


	typedef struct 
	{
		UART_DEBUG		msgCmdID;					//功能码
		void(*msgCallback_recv) (UsartGetStruct * arg); //回调函数
		char *			tip;						//描述
	} usartMsgCallback;


	extern uint8_t pcWriteBuffer[BUF_DEBUG_MAX];

	void Debug_usart1(void);

	static void usart1_get_rx(UsartGetStruct * uart5_data); //获取列队数据
	static void usartRecv_test(UsartGetStruct * arg);
	static void usartRecv_read(UsartGetStruct * arg);
	static void usartRecv_write(UsartGetStruct * arg);
	static void usartRecv_task(UsartGetStruct * arg);

#ifdef __cplusplus
}


#endif

#endif /*__debug_d_H*/

