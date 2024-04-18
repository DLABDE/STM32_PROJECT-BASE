#include "usart_lay.h"
#include "usart.h"

#define HAURT	(huart1)

//lwrb_t usart_rx_format_rb;
//lwrb_t usart_tx_format_rb;

//uint8_t usart_rx_format_data[sizeof(uint8_t) * USART_FIFO_MAX_SIZE + 1];
//uint8_t usart_tx_format_data[sizeof(uint8_t) * USART_FIFO_MAX_SIZE + 1];

//void usart1_init(void)
//{
//	//lwrb_init(&usart_rx_format_rb, usart_rx_format_data, sizeof(usart_rx_format_data));
//	//lwrb_init(&usart_tx_format_rb, usart_tx_format_data, sizeof(usart_tx_format_data));

////	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
////	HAL_UART_Receive_DMA(&huart1, uart1rx, USART_BUFF);

//	//HAL_UART_Receive_IT(&huart5,uart5rx,1);
//	//DLOG("init\r\n");
//	//UART5_Transmit(); 
//}


//void log_user(const char * format, ...)
//{
//	va_list args;
//	unsigned int length;

//	memset(uart1tx, 0, USART_BUFF);
//	va_start(args, format);
//	length = vsnprintf((char *) uart1tx, sizeof(uart1tx), format, args);
//	va_end(args);

// 	//HAL_UART_Transmit_DMA(&huart5,(uint8_t *)txbuf, length);
// 	//lwrb_write(&usart_tx_format_rb, uart1tx, length);
//}


int fputc(int ch, FILE * f)
{
	//HAL_UART_Transmit_DMA(&huart5, (uint8_t *)&ch, 1);
	HAL_UART_Transmit(&HAURT, (uint8_t *) &ch, 1, 1);
	return ch;
}


//void Usart_Dataframe(uint8_t control, uint16_t data) //发�?�数据帧
//{
//	uint8_t fram[Max_FRA_Len] =
//	{
//		FRA_HEAD, control, (uint8_t) (data & 0xff), (uint8_t) (data >> 8), FRA_END
//	};
//	HAL_UART_Transmit(&huart1, fram, Max_FRA_Len, 0xF);
//}


//void UART1_Transmit_Ser(void)
//{
////	uint32_t len = lwrb_read(&usart_tx_format_rb, uart1tx, sizeof(uart1tx));

////	if(len > 0 && huart1.gState == HAL_UART_STATE_READY)
////	{
////		HAL_UART_Transmit_DMA(&huart1, uart1tx, len);
////	}
//}


//void HAL_UART_IDLE_Callback(UART_HandleTypeDef * huart)
//{
//	if((__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET))
//	{
//		__HAL_UART_CLEAR_IDLEFLAG(huart);
//		HAL_UART_DMAStop(huart);					// 停止本次DMA传输

//		if(huart->Instance == USART1)
//		{
//			uint32_t len = USART_BUFF - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);

//			//DLOG("len:%d\t%s\r\n",len,uart5rx);
//			//lwrb_write(&usart_rx_format_rb, uart1rx, len);
//			for(uint16_t i=0; i<len; i++)
//			{
//				xQueueSendFromISR(xRxedChars, &g_usart2_rx_buff[i], &xHigherPriorityTaskWoken);	
//				portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
//			}
//			HAL_UART_Receive_DMA(&huart1, uart1rx, USART_BUFF);
//			len = 0;
//		}
//	}
//}