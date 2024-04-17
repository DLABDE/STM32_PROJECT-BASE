/*
 * FreeRTOS V202212.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/*
	BASIC INTERRUPT DRIVEN SERIAL PORT DRIVER FOR UART0.
*/

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

/* Library includes. */
#include "stm32f4xx_hal.h"

/* application includes. */
#include "serial.h"
/*-----------------------------------------------------------*/

/* Misc defines. */
#define serINVALID_QUEUE				( ( QueueHandle_t ) 0 )
#define serNO_BLOCK						( ( TickType_t ) 0 )
#define serTX_BLOCK_TIME				( 40 / portTICK_PERIOD_MS )

/*-----------------------------------------------------------*/

/* The queue used to hold received characters. */
static QueueHandle_t xRxedChars;
static QueueHandle_t xCharsForTx;

static uint8_t uart1rx[USART_BUFF];
static uint8_t uart1tx[USART_BUFF];
/*-----------------------------------------------------------*/

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

/*
 * See the serial2.h header file.
 */
xComPortHandle xSerialPortInitMinimal( unsigned long ulWantedBaud, unsigned portBASE_TYPE uxQueueLength )
{
xComPortHandle xReturn;

	/* Create the queues used to hold Rx/Tx characters. */
	xRxedChars = xQueueCreate( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed char ) );
	xCharsForTx = xQueueCreate( uxQueueLength + 1, ( unsigned portBASE_TYPE ) sizeof( signed char ) );
	
	/* If the queue/semaphore was created correctly then setup the serial port
	hardware. */
	if( ( xRxedChars != serINVALID_QUEUE ) && ( xCharsForTx != serINVALID_QUEUE ) )
	{
		__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
		HAL_UART_Receive_DMA(&huart1, uart1rx, USART_BUFF);
	}
	else
	{
		xReturn = ( xComPortHandle ) 0;
	}

	/* This demo file only supports a single port but we have to return
	something to comply with the standard demo header file. */
	return xReturn;
}
/*-----------------------------------------------------------*/

signed portBASE_TYPE xSerialGetChar( xComPortHandle pxPort, signed char *pcRxedChar, TickType_t xBlockTime )
{
	/* The port handle is not required as this driver only supports one port. */
	( void ) pxPort;

	/* Get the next character from the buffer.  Return false if no characters
	are available, or arrive before xBlockTime expires. */
	if( xQueueReceive( xRxedChars, pcRxedChar, xBlockTime ) )
	{
		return pdTRUE;
	}
	else
	{
		return pdFALSE;
	}
}
/*-----------------------------------------------------------*/

void vSerialPutString( xComPortHandle pxPort, const signed char * const pcString, unsigned short usStringLength )
{
signed char *pxNext;

	/* A couple of parameters that this port does not use. */
	( void ) usStringLength;
	( void ) pxPort;

	/* NOTE: This implementation does not handle the queue being full as no
	block time is used! */

	/* The port handle is not required as this driver only supports UART1. */
	( void ) pxPort;

	/* Send each character in the string, one at a time. */
	pxNext = ( signed char * ) pcString;
	while( *pxNext )
	{
		xSerialPutChar( pxPort, *pxNext, serNO_BLOCK );
		pxNext++;
	}
}
/*-----------------------------------------------------------*/

signed portBASE_TYPE xSerialPutChar( xComPortHandle pxPort, signed char cOutChar, TickType_t xBlockTime )
{
signed portBASE_TYPE xReturn;

	if( xQueueSend( xCharsForTx, &cOutChar, xBlockTime ) == pdPASS )
	{
		xReturn = pdPASS;
		uint8_t cChar;
    /* 发送队列中有数据，通过轮询方式发送出去 */
    if(xQueueReceive(xCharsForTx, &cChar, 0) == pdTRUE) {
      if((HAL_UART_GetState(&huart1) & HAL_UART_STATE_BUSY_TX) != HAL_UART_STATE_BUSY_TX) {
        HAL_UART_Transmit(&huart1, &cChar, 1, 1000);
      }
    }
	}
	else
	{
		xReturn = pdFAIL;
	}

	return xReturn;
}

/*-----------------------------------------------------------*/

void vSerialClose( xComPortHandle xPort )
{
	/* Not supported as not required by the demo application. */
}
/*-----------------------------------------------------------*/

void log_user(const char * format, ...)
{
	va_list args;
	unsigned int length;

	memset(uart1tx, 0, USART_BUFF);
	va_start(args, format);
	length = vsnprintf((char *) uart1tx, sizeof(uart1tx), format, args);
	va_end(args);

 	HAL_UART_Transmit_DMA(&huart1,(uint8_t *)uart1tx, length);
 	//lwrb_write(&usart_tx_format_rb, uart1tx, length);
}

void xSerialTransmit_Ser( void )
{
	if(xQueueReceive(xCharsForTx, uart1tx, 0) == pdTRUE) 
	{
      if((HAL_UART_GetState(&huart1) & HAL_UART_STATE_BUSY_TX) != HAL_UART_STATE_BUSY_TX)
			{
        //HAL_UART_Transmit_DMA(&huart1, uart1tx, 1);
      }
   }
}

void vUARTInterruptHandler( UART_HandleTypeDef * huart )
{
portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	
	if((__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET))
	{
		__HAL_UART_CLEAR_IDLEFLAG(huart);
		HAL_UART_DMAStop(huart);					// 停止本次DMA传输

		if(huart->Instance == USART1)
		{
			uint32_t len = USART_BUFF - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);

			//DLOG("len:%d\t%s\r\n",len,uart5rx);
			//lwrb_write(&usart_rx_format_rb, uart1rx, len);
			for(uint16_t i=0; i<len; i++)
			{
				xQueueSendFromISR(xRxedChars, &uart1rx[i], &xHigherPriorityTaskWoken);	
			}
			HAL_UART_Receive_DMA(&huart1, uart1rx, USART_BUFF);
		}
	}
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}



