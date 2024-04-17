

#ifndef __OLED_H
#define __OLED_H
#include "main.h"
#include "i2c.h"

#define SIZE										16
#define XLevelL 									0x00
#define XLevelH 									0x10
#define Max_Column									128
#define Max_Row 									64
#define Brightness									0xFF 
#define X_WIDTH 									128
#define Y_WIDTH 									64		


/*SPI iic定义*/
#define IIC_ADS 									0x78			//设备地址
#define OLED_CMD									0				//写命令
#define OLED_DATA									1				//写数据

void OLED_Clear(void);
void OLED_Display_On(void); /* OLED开 */
void OLED_ShowPosture(void); /* 提示信息 */
void OLED_Display_Off(void); /* OLED关 */
void GPIO_OLED_InitConfig(void); /* OLED初始化 */
void OLED_WR_Byte(uint8_t dat, uint8_t cmd); /* 写字节 */
void OLED_Set_Pos(uint8_t x, uint8_t y); /* 设置坐标 */
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr); /* 显示字符 */
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t * p); /* 显示字符串 */
void OLED_ShowCHinese(uint8_t x, uint8_t y, uint8_t no); /* 显示中文 */
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size); /* 显示数字 */
void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[]); /* 显示图片 */

#endif /* __OLED_H */

