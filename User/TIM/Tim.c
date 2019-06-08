/**********************************************
 * 文件名  ：Time_test.c
 * 描述    ：TIM2 1ms定时应用函数库
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 硬件连接：无
 * 库版本  ：ST3.5.0
***********************************************/
#include "Tim.h"
volatile uint32_t TIM2_Time; //ms计时变量

/* TIM2中断优先级配置 */
void TIM2_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/* 中断周期为1ms */
void TIM2_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period=1000; //自动重装载寄存器周期的值(计数值)
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1); //时钟预分频数 72M/72
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update); //清除溢出中断标志
    TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM2, ENABLE); //开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE); //先关闭等待使用
}

void TIM2_Init(void)
{
	TIM2_NVIC_Config();
	TIM2_Config();
}

void TIM2_StartTime(void)
{
	TIM2_Time = 0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

void TIM2_StopTime(void)
{
	TIM_Cmd(TIM2, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);
}

uint32_t TIM2_GetTime(void)
{
	return TIM2_Time;
}

void TIM2_ResetTime(void)
{
	TIM2_Time = 0;
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
	  TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
    TIM2_Time++;
	}
}
