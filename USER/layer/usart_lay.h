
#ifndef __USART_LAY_H__
#define __USART_LAY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define FRA_HEAD	0xa5
#define FRA_END		0xaa
#define Max_FRA_Len 5

#define USART_BUFF	(128)
#define USART_FIFO_MAX_SIZE	(USART_BUFF*5)

#define ULOG	log_user


//void Usart_Dataframe(uint8_t control,uint16_t data);
//void usart1_init(void);
//void UART1_Transmit_Ser(void);
//void HAL_UART_IDLE_Callback(UART_HandleTypeDef *huart);

void log_user(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /* __USART_LAY_H__ */

