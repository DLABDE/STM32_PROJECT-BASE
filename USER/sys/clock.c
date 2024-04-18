
#include "clock.h"



void sys_clock_handle(ClockCntStruct * sCnt, ClockFlagStruct * sFlag)
{
	sFlag->bit.b1ms = 1;

	sFlag->bit.b5ms = 0;
	sCnt->c5ms++;
	if(sCnt->c5ms >= 4)
	{
		sCnt->c5ms = 0;
		sFlag->bit.b5ms = 1;
	}

	sFlag->bit.b10ms = 0;
	sCnt->c10ms++;
	if(sCnt->c10ms >= 10)
	{
		sCnt->c10ms = 0;
		sFlag->bit.b10ms = 1;
	}

	sFlag->bit.b20ms = 0;
	sCnt->c20ms++;
	if(sCnt->c20ms >= 20)
	{
		sCnt->c20ms = 0;
		sFlag->bit.b20ms = 1;
	}

	sFlag->bit.b50ms = 0;
	sCnt->c50ms++;
	if(sCnt->c50ms >= 50)
	{
		sCnt->c50ms = 0;
		sFlag->bit.b50ms = 1;
	}

	sFlag->bit.b100ms = 0;
	sCnt->c100ms++;
	if(sCnt->c100ms >= 100)
	{
		sCnt->c100ms = 0;
		sFlag->bit.b100ms = 1;
	}

	sFlag->bit.b300ms = 0;
	sCnt->c300ms++;
	if(sCnt->c300ms >= 300)
	{
		sCnt->c300ms = 0;
		sFlag->bit.b300ms = 1;
	}

	sFlag->bit.b500ms = 0;
	sCnt->c500ms++;
	if(sCnt->c500ms >= 500)
	{
		sCnt->c500ms = 0;
		sFlag->bit.b500ms = 1;
	}

	sFlag->bit.b1s = 0;
	sCnt->c1s++;
	if(sCnt->c1s >= 1000)
	{
		sCnt->c1s = 0;
		sFlag->bit.b1s = 1;
	}

	sFlag->bit.b5s = 0;
	sCnt->c5s++;
	if(sCnt->c5s >= 5000)
	{
		sCnt->c5s = 0;
		sFlag->bit.b5s = 1;
	}

	sFlag->bit.b10s = 0;
	sCnt->c10s++;
	if(sCnt->c10s >= 10000)
	{
		sCnt->c10s = 0;
		sFlag->bit.b10s = 1;
	}

	sFlag->bit.b1min = 0;
	sCnt->c1min++;
	if(sCnt->c1min >= 60000)
	{
		sCnt->c1min = 0;
		sFlag->bit.b1min = 1;
	}

	sFlag->bit.b5min = 0;
	sCnt->c5min++;
	if(sCnt->c5min >= 300000)
	{
		sCnt->c5min = 0;
		sFlag->bit.b5min = 1;
	}

	sFlag->bit.b10min = 0;
	sCnt->c10min++;
	if(sCnt->c10min >= 600000)
	{
		sCnt->c10min = 0;
		sFlag->bit.b10min = 1;
	}

	sFlag->bit.b1hour = 0;
	sCnt->c1hour++;
	if(sCnt->c1hour >= 3600000)
	{
		sCnt->c1hour = 0;
		sFlag->bit.b1hour = 1;
	}
}


