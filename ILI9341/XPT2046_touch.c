#include <stdio.h>
#include <stdlib.h>
#include <main.h>

#include "FreeRTOS.h"
#include "task.h"

#include "XPT2046_touch.h"
//#include "usart.h"

static uint16_t sam_x[XPT_MAX_SAM] = {0};
static uint16_t sam_y[XPT_MAX_SAM] = {0};

static void XPT2046_TouchSelect()
{
    HAL_GPIO_WritePin(XPT2046_CS_GPIO_Port, XPT2046_CS_Pin, GPIO_PIN_RESET);
}

static void XPT2046_TouchUnselect()
{
    HAL_GPIO_WritePin(XPT2046_CS_GPIO_Port, XPT2046_CS_Pin, GPIO_PIN_SET);
}

bool XPT2046_TouchPressed()
{
    return HAL_GPIO_ReadPin(XPT2046_IRQ_GPIO_Port, XPT2046_IRQ_Pin) == GPIO_PIN_RESET;
}

bool XPT2046_TouchGetCoordinates(uint16_t* x, uint16_t* y)
{
    static const uint8_t cmd_read_x[] = { READ_X };
    static const uint8_t cmd_read_y[] = { READ_Y };
    static const uint8_t zeroes_tx[] = { 0x00, 0x00 };

		taskENTER_CRITICAL();
		
    XPT2046_TouchSelect();

    uint32_t avg_x = 0;
    uint32_t avg_y = 0;
    uint8_t nsamples = 0;

    for(uint8_t i = 0; i < XPT_MAX_SAM; i++)
    {
        if(!XPT2046_TouchPressed())
            break;

        nsamples++;
        HAL_SPI_Transmit(&XPT2046_SPI_PORT, (uint8_t*)cmd_read_y, sizeof(cmd_read_y), XPT_SPI_MAX_DELAY);
        uint8_t y_raw[2];
        HAL_SPI_TransmitReceive(&XPT2046_SPI_PORT, (uint8_t*)zeroes_tx, y_raw, sizeof(y_raw), XPT_SPI_MAX_DELAY);
        HAL_SPI_Transmit(&XPT2046_SPI_PORT, (uint8_t*)cmd_read_x, sizeof(cmd_read_x), XPT_SPI_MAX_DELAY);
        uint8_t x_raw[2];
        HAL_SPI_TransmitReceive(&XPT2046_SPI_PORT, (uint8_t*)zeroes_tx, x_raw, sizeof(x_raw), XPT_SPI_MAX_DELAY);
				
				sam_x[i] = (((uint16_t)x_raw[0]) << 8) | ((uint16_t)x_raw[1]);
				sam_y[i] = (((uint16_t)y_raw[0]) << 8) | ((uint16_t)y_raw[1]);
				
        avg_x += (((uint16_t)x_raw[0]) << 8) | ((uint16_t)x_raw[1]);
        avg_y += (((uint16_t)y_raw[0]) << 8) | ((uint16_t)y_raw[1]);
    }
    XPT2046_TouchUnselect();
		
		taskEXIT_CRITICAL();
    if(nsamples < XPT_MAX_SAM)
        return false;
		
		//滤波算法
		uint16_t raw_x = (avg_x / XPT_MAX_SAM);	//得到平均值
    uint16_t raw_y = (avg_y / XPT_MAX_SAM);
		uint8_t	cnt = 0;
		for(uint8_t i = 0; i < XPT_MAX_SAM; i++)
    {
			if(abs((int32_t)sam_x[i]-raw_x) + abs((int32_t)sam_y[i]-raw_y) > XPT_FLT_SAM)
			{
				avg_x -= sam_x[i];
				avg_y -= sam_y[i];
				cnt++;
			}
    }
    raw_x = (avg_x / (XPT_MAX_SAM - cnt));
    raw_y = (avg_y / (XPT_MAX_SAM - cnt) );
		
		//ULOG("x%d\ty%d\t\r\n",raw_x,raw_y);
		
		//比例换算和大小限
		if(raw_x < XPT2046_MIN_RAW_X) raw_x = XPT2046_MIN_RAW_X;
    if(raw_x > XPT2046_MAX_RAW_X) raw_x = XPT2046_MAX_RAW_X;
    if(raw_y < XPT2046_MIN_RAW_Y) raw_y = XPT2046_MIN_RAW_Y;
    if(raw_y > XPT2046_MAX_RAW_Y) raw_y = XPT2046_MAX_RAW_Y;
    *x = (raw_x - XPT2046_MIN_RAW_X) * XPT2046_SCALE_X / (XPT2046_MAX_RAW_X - XPT2046_MIN_RAW_X);
    *y = (raw_y - XPT2046_MIN_RAW_Y) * XPT2046_SCALE_Y / (XPT2046_MAX_RAW_Y - XPT2046_MIN_RAW_Y);

    return true;
}


