
#include "io_ctrl.h"
#include "tim.h"


struct structGPIOCtrl sys_gpio ={0};


static void gpio_read(void)
{
	sys_gpio.read.bit.bLed = HAL_GPIO_ReadPin(LED_GPIO_Port, LED_Pin);
	sys_gpio.read.bit.bKEY0 = HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin);
	//sys_gpio.read.bit.bLcd_touch = HAL_GPIO_ReadPin(LCD_TOUCH_GPIO_Port, LCD_TOUCH_Pin);
	sys_gpio.read.bit.bLcd_touch = Read_Touch_Pin;
}


static void gpio_write(void)
{
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, (GPIO_PinState) sys_gpio.write.bit.bLed);
}


void PWM_Init(void)
{
	HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
}


void GPIO_Ctrl_Ser(void)
{
	gpio_read();
	gpio_write();
	__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, sys_msg.led_pwm);
}


