#include "driver_systick.h"

#include "stm32f1xx_hal.h"

void udelay(int us)
{
	uint32_t told=SysTick->VAL;
	uint32_t tnow;
	uint32_t load=SysTick->LOAD;
	//load+1个时钟对应1ms n个us对应 n*（load+1）/1000个时钟
	uint32_t ticks=us*(load+1)/1000;
	uint32_t cnt=0;
	while(1)
	{
		tnow=SysTick->VAL;
		if(tnow>told)
		{
			cnt+=told+load+1-tnow;
		}
		else
			cnt+=told-tnow;
		told=tnow;
		if(cnt>=ticks)
			break;		
	}
}

void mdelay(int ms)
{
	for(int i=0;i<ms;i++)
		udelay(1000);
}

uint64_t system_get_ns(void)
{
	uint64_t ms=HAL_GetTick();
	uint64_t ns=ms*1000000;
	uint32_t tnow=SysTick->VAL;
	uint32_t load=SysTick->LOAD;
	
	uint64_t cnt;
	cnt=load+1-tnow;
	ns+=cnt*1000000/(load+1);
	return ns;
}
