
#ifndef __SYS_H__
#define __SYS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

struct System_Msg
{
	uint16_t led_status;
	uint16_t led_pwm;
	uint16_t temp;
};

extern struct System_Msg sys_msg;

#ifdef __cplusplus
}
#endif

#endif /* __SYS_H__ */

