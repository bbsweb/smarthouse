#include "stm32f10x.h"
#include "SysTick.h"
#include "Tim.h"
#include "esp8266.h"
#include "usart.h"
#include "led.h"
#include "TLINK.h"

int main(void)
{
	SystemInit();	//����ϵͳʱ��Ϊ72M
	SysTick_Init(); //SysTick��ʱ����ʼ��
	TIM2_Init(); //TIM��ʱ����ʼ��
	LED_GPIO_Config(); //LED�˿ڳ�ʼ��
	USART1_Init(); //USART1��ʼ��
	USART2_Init(); //USART2��ʼ��
	TIM2_StartTime();

	while(!esp8266Begin())
	{
		esp8266TCPClose();
		Delay_us(1000000);
	}

	while(!esp8266ConnectAP("wifi名称", "wifi密码"))
	{
		Delay_us(1000000);
	}

	while(!esp8266TCPConnect((uint8_t *)"tcp.tlink.io", (uint8_t *)"8647"))
	{
		Delay_us(1000000);
	}

	while(!esp8266TCPSend((uint8_t *)"TLINK平台序列号"))
	{
		Delay_us(1000000);
	}

	while(1)
	{
		if(esp8266TLINKBufferAvailable())
		{
			TlinkProcess();
		}
		Delay_us(100000); //ÿ100����ɨ����յ���ָ��
	}
}
