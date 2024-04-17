

#ifndef __clock_H
#define __clock_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"


	typedef struct
	{
		uint32_t		all;


		struct 
		{
			uint32_t b1ms					: 1;
			uint32_t b5ms					: 1;
			uint32_t b10ms					: 1;
			uint32_t b20ms					: 1;
			uint32_t b50ms					: 1;
			uint32_t b100ms					: 1;
			uint32_t b300ms					: 1;
			uint32_t b500ms					: 1;
			uint32_t b1s 					: 1;
			uint32_t b5s 					: 1;
			uint32_t b10s					: 1;
			uint32_t b1min					: 1;
			uint32_t b5min					: 1;
			uint32_t b10min					: 1;
			uint32_t b1hour					: 1;
		} bit;


	}ClockFlagStruct;


	typedef struct
	{
		uint16_t c1ms;
		uint16_t c5ms;
		uint16_t c10ms;
		uint16_t c20ms;
		uint16_t c50ms;
		uint16_t c100ms;
		uint16_t c300ms;
		uint16_t c500ms;
		uint16_t c1s;
		uint16_t c5s;
		uint16_t c10s;
		uint16_t c1min;
		uint32_t c5min;
		uint32_t c10min;
		uint32_t c1hour;
	}ClockCntStruct;


	void sys_clock_handle(ClockCntStruct * sCnt,ClockFlagStruct * sFlag);


#endif /* __clock_H */

