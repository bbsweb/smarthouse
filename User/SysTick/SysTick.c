/**********************************************
 * �ļ���  ��SysTick.c
 * ����    ��SysTickϵͳ�δ�ʱ��1us�жϺ�����
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * Ӳ�����ӣ���
 * ��汾  ��ST3.5.0
***********************************************/
#include "SysTick.h"

static __IO u32 TimingDelay = 0;

/* ��ʼ��SysTick */
void SysTick_Init(void)
{
	/* SystemFrequency / 10000   1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
	if (SysTick_Config(SystemCoreClock / 1000000)) //1us
  {
    /* Capture error */
    while (1);
  }
}

/* us��ʱ����,1usΪһ����λ */
void Delay_us(__IO u32 nTime)
{
  TimingDelay = nTime;
  while(TimingDelay != 0);
}

/* ��ȡ���ĳ����� SysTick �жϺ��� SysTick_Handler()���� */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}
