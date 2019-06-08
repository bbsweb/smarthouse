#ifndef __DHT11_H__
#define __DHT11_H__

#include "stm32f10x_gpio.h"
#include "SysTick.h"

#define DHT11_GPIO_TYPE  GPIOA
#define DHT11_GPIO_PIN   GPIO_Pin_4
#define DHT11_RCC        RCC_APB2Periph_GPIOA
#define DHT11_OUT_H GPIO_SetBits(DHT11_GPIO_TYPE, DHT11_GPIO_PIN)
#define DHT11_OUT_L GPIO_ResetBits(DHT11_GPIO_TYPE, DHT11_GPIO_PIN)
#define DHT11_IN    GPIO_ReadInputDataBit(DHT11_GPIO_TYPE, DHT11_GPIO_PIN)

void dht11_gpio_input(void);
void dht11_gpio_output(void);
uint16_t dht11_scan(void);
uint16_t dht11_read_bit(void);
uint16_t dht11_read_byte(void);
uint16_t dht11_read_data(uint8_t buffer[4]);

#endif
