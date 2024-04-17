

#ifndef __io_ctrl_H
#define __io_ctrl_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f4xx_hal.h"
#include "main.h"

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)
#define Read_Touch_Pin	PCin(5)

struct unionGPIO_Write
{
	uint32_t		all;
	struct 
	{
		uint32_t		bLed			: 1;
		uint32_t		bLcd_Light		: 1;
		uint32_t		bReserved		: 30;
	} bit;
};


struct unionGPIO_Read
{
	uint32_t		all;
	struct 
	{
		uint32_t		bLed		: 1;
		uint32_t		bLcd_touch	: 1;
		uint32_t		bKEY0		: 1;
		uint32_t		bReserved	: 29;
	} bit;
};


struct structGPIOCtrl
{
	struct unionGPIO_Read read;
	struct unionGPIO_Write write;
};


extern struct structGPIOCtrl sys_gpio;
void PWM_Init(void);
void GPIO_Ctrl_Ser(void);


#ifdef __cplusplus
}


#endif

#endif /* __io_ctrl_H */

