/**********************************************
 * �ļ���  ��Time_test.c
 * ����    ��TIM2 1ms��ʱӦ�ú�����
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * Ӳ�����ӣ���
 * ��汾  ��ST3.5.0
***********************************************/
#include "Tim.h"
volatile uint32_t TIM2_Time; //ms��ʱ����

/* TIM2�ж����ȼ����� */
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

/* �ж�����Ϊ1ms */
void TIM2_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period=1000; //�Զ���װ�ؼĴ������ڵ�ֵ(����ֵ)
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1); //ʱ��Ԥ��Ƶ�� 72M/72
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update); //�������жϱ�־
    TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM2, ENABLE); //����ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE); //�ȹرյȴ�ʹ��
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
