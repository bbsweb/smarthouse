/**********************************************
 * �ļ���  ��usart.c
 * ����    ������USART1��2����
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * Ӳ�����ӣ�------------------------
 *          |    PA2  - USART2(Tx)   |
 *          |    PA3  - USART2(Rx)   |
 *          |    PA9  - USART1(Tx)   |
 *          |    PA10 - USART1(Rx)   |
 *           ------------------------
 * ��汾  ��ST3.5.0
***********************************************/
#include "usart.h"
#include "esp8266.h"
#include "TLINK.h"
#include <string.h>

uint8_t RXBuffer[RXBufferMaxLength] = "";
uint32_t RXLength = 0;
extern uint8_t esp8266RXBuffer[ESP8266RXBufferMaxLength];
extern uint32_t bufferHead;

/* USART1 I/O�˿����� */
void USART1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* ʹ�� USART1 ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

	/* USART1 ��λ */
	USART_DeInit(USART1);

	/* USART1 ʹ��IO�˿����� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��GPIOA
	  
	/* USART1 ����ģʽ���� */
	USART_InitStructure.USART_BaudRate = 115200; //���������ã�115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//����λ�����ã�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No; //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�����뷢�Ͷ�ʹ��
	USART_Init(USART1, &USART_InitStructure); //��ʼ��USART1

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //ʹ��USART1�����ж�
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE); //ʹ��USART1�����ж�
	USART_Cmd(USART1, ENABLE); //USART1ʹ��
}

/* USART1�ж����ȼ����� */
void USART1_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void USART1_Init(void)
{
	USART1_GPIO_Config();
	USART1_NVIC_Config();
}

/* USART2��ʼ������ */
void USART2_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* ʹ�� USART2 ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* USART2 ��λ */
	USART_DeInit(USART2);

	/* USART2 ʹ��IO�˿����� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��GPIOA
	  
	/* USART2 ����ģʽ���� */
	USART_InitStructure.USART_BaudRate = 115200; //���������ã�115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//����λ�����ã�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No; //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�����뷢�Ͷ�ʹ��
	USART_Init(USART2, &USART_InitStructure); //��ʼ��USART2

  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //ʹ��USART2�����ж�
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE); //ʹ��USART2�����ж�
	USART_Cmd(USART2, ENABLE); //USART2ʹ��
}

/* USART2�ж����ȼ����� */
void USART2_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void USART2_Init(void)
{
	USART2_GPIO_Config();
	USART2_NVIC_Config();
}

/* ����1�ֽ����� */
void USARTSendByte(USART_TypeDef *USART, uint8_t data)
{
	while(USART_GetFlagStatus(USART, USART_FLAG_TXE) == RESET);
	USART_SendData(USART, (uint8_t)data);
	while(USART_GetFlagStatus(USART, USART_FLAG_TC) == RESET);
}

/* �����ַ��� */
void USARTSendArrar(USART_TypeDef *USART, uint8_t *Arrar)
{
	uint16_t length = strlen((char *)Arrar);
	while(length--)
	{
		USART_ClearFlag(USART, USART_FLAG_TC);	//������һ���һ���ַ��ᶪʧ
		USARTSendByte(USART, *(Arrar++));
	}
}

void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
		RXLength < RXBufferMaxLength ? RXBuffer[RXLength++] = USART1->DR : USART1->DR;
  }
	else if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		USART1->DR; //����ж�
		RXBuffer[RXLength] = '\0';
		USARTSendArrar(USART2, RXBuffer);
		RXLength = 0;
	}
}

void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
		bufferHead < ESP8266RXBufferMaxLength ? esp8266RXBuffer[bufferHead++] = USART2->DR : USART2->DR;
  }
  if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
		USART2->DR; //����ж�
		esp8266RXBuffer[bufferHead] = '\0';
		USARTSendArrar(USART1, (uint8_t *)esp8266RXBuffer);
		bufferHead = 0;
	}
}
