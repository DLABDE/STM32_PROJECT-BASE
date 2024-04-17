/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file	 usart.c
  * @brief	 This file provides code for the configuration
  * 		 of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
//lwrb_t usart_rx_format_rb;
//lwrb_t usart_tx_format_rb;

//uint8_t usart_rx_format_data[sizeof(uint8_t) * USART_FIFO_MAX_SIZE + 1];
//uint8_t usart_tx_format_data[sizeof(uint8_t) * USART_FIFO_MAX_SIZE + 1];


/* USER CODE END 0 */

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA2_Stream2;
    hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA2_Stream7;
    hdma_usart1_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 8, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
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
	HAL_UART_Transmit(&huart1, (uint8_t *) &ch, 1, 1);
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


/* USER CODE END 1 */
