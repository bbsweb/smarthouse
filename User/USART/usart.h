#ifndef __USART_H
#define	__USART_H

#include "stm32f10x.h"

#define RXBufferMaxLength 1024

void USART1_Init(void);
void USART2_Init(void);
void USARTSendArrar(USART_TypeDef *USART, uint8_t *Arrar);
	
#endif /* __USART_H */
