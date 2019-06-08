/**********************************************
 * 文件名  ：SysTick.c
 * 描述    ：SysTick系统滴答时钟1us中断函数库
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 硬件连接：无
 * 库版本  ：ST3.5.0
***********************************************/
#include "SysTick.h"

static __IO u32 TimingDelay = 0;

/* 初始化SysTick */
void SysTick_Init(void)
{
	/* SystemFrequency / 10000   1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	if (SysTick_Config(SystemCoreClock / 1000000)) //1us
  {
    /* Capture error */
    while (1);
  }
}

/* us延时程序,1us为一个单位 */
void Delay_us(__IO u32 nTime)
{
  TimingDelay = nTime;
  while(TimingDelay != 0);
}

/* 获取节拍程序，在 SysTick 中断函数 SysTick_Handler()调用 */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}
