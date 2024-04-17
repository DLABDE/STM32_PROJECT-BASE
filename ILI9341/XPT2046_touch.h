#ifndef XPT2046_TOUCH_H_
#define XPT2046_TOUCH_H_

#include "main.h"
#include <stdbool.h>

#define READ_X 0x90
#define READ_Y 0xD0

#define XPT2046_SPI_PORT hspi2
extern SPI_HandleTypeDef XPT2046_SPI_PORT;

#define XPT2046_IRQ_Pin       T_PEN_Pin
#define XPT2046_IRQ_GPIO_Port T_PEN_GPIO_Port
#define XPT2046_CS_Pin        T_CS_Pin
#define XPT2046_CS_GPIO_Port  T_CS_GPIO_Port

// 电阻屏矫正参数
#define XPT2046_SCALE_X 330
#define XPT2046_SCALE_Y 310

#define XPT2046_MIN_RAW_X 2000
#define XPT2046_MAX_RAW_X 31000
#define XPT2046_MIN_RAW_Y 1500
#define XPT2046_MAX_RAW_Y 38000 

//通讯最大阻塞时长
#define XPT_SPI_MAX_DELAY	0x1


#define XPT_MAX_SAM	(15)	//最大采样数(>6)

#define XPT_FLT_SAM	(600)	//剔除半径外的离群值 x差值+y差值

// call before initializing any SPI devices
bool XPT2046_TouchPressed(void);
bool XPT2046_TouchGetCoordinates(uint16_t* x, uint16_t* y);

#endif 
