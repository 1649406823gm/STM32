#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"

void LED_Init(void);

#define RCC_ALL_LED     RCC_AHB1Periph_GPIOF
#define LED0_GPIO_Port  GPIOF
#define LED0_GPIO_Pin   GPIO_Pin_9
#define LED1_GPIO_Port  GPIOF
#define LED1_GPIO_Pin   GPIO_Pin_10

#endif