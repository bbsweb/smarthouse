#ifndef __TIM_H
#define __TIM_H

#include "stm32f10x.h"

void TIM2_Init(void);
void TIM2_StartTime(void);
void TIM2_StopTime(void);
uint32_t TIM2_GetTime(void);
void TIM2_ResetTime(void);

#endif	/* __TIM_H */
