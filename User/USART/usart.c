/**********************************************
 * 文件名  ：usart.c
 * 描述    ：配置USART1、2串口
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 硬件连接：------------------------
 *          |    PA2  - USART2(Tx)   |
 *          |    PA3  - USART2(Rx)   |
 *          |    PA9  - USART1(Tx)   |
 *          |    PA10 - USART1(Rx)   |
 *           ------------------------
 * 库版本  ：ST3.5.0
***********************************************/
#include "usart.h"
#include "esp8266.h"
#include "TLINK.h"
#include <string.h>

uint8_t RXBuffer[RXBufferMaxLength] = "";
uint32_t RXLength = 0;
extern uint8_t esp8266RXBuffer[ESP8266RXBufferMaxLength];
extern uint32_t bufferHead;

/* USART1 I/O端口配置 */
void USART1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 使能 USART1 时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

	/* USART1 复位 */
	USART_DeInit(USART1);

	/* USART1 使用IO端口配置 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化GPIOA
	  
	/* USART1 工作模式配置 */
	USART_InitStructure.USART_BaudRate = 115200; //波特率设置：115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No; //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //接收与发送都使能
	USART_Init(USART1, &USART_InitStructure); //初始化USART1

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //使能USART1接收中断
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE); //使能USART1空闲中断
	USART_Cmd(USART1, ENABLE); //USART1使能
}

/* USART1中断优先级配置 */
void USART1_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //组优先级
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //副优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void USART1_Init(void)
{
	USART1_GPIO_Config();
	USART1_NVIC_Config();
}

/* USART2初始化配置 */
void USART2_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 使能 USART2 时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* USART2 复位 */
	USART_DeInit(USART2);

	/* USART2 使用IO端口配置 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化GPIOA
	  
	/* USART2 工作模式配置 */
	USART_InitStructure.USART_BaudRate = 115200; //波特率设置：115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No; //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //接收与发送都使能
	USART_Init(USART2, &USART_InitStructure); //初始化USART2

  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //使能USART2接收中断
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE); //使能USART2空闲中断
	USART_Cmd(USART2, ENABLE); //USART2使能
}

/* USART2中断优先级配置 */
void USART2_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //组优先级
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //副优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void USART2_Init(void)
{
	USART2_GPIO_Config();
	USART2_NVIC_Config();
}

/* 发送1字节数据 */
void USARTSendByte(USART_TypeDef *USART, uint8_t data)
{
	while(USART_GetFlagStatus(USART, USART_FLAG_TXE) == RESET);
	USART_SendData(USART, (uint8_t)data);
	while(USART_GetFlagStatus(USART, USART_FLAG_TC) == RESET);
}

/* 发送字符串 */
void USARTSendArrar(USART_TypeDef *USART, uint8_t *Arrar)
{
	uint16_t length = strlen((char *)Arrar);
	while(length--)
	{
		USART_ClearFlag(USART, USART_FLAG_TC);	//不加这一句第一个字符会丢失
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
		USART1->DR; //清除中断
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
		USART2->DR; //清除中断
		esp8266RXBuffer[bufferHead] = '\0';
		USARTSendArrar(USART1, (uint8_t *)esp8266RXBuffer);
		bufferHead = 0;
	}
}
