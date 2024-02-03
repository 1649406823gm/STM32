#include "led.h"

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //打开GPIO时钟
    RCC_AHB1PeriphClockCmd(RCC_ALL_LED, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;

    GPIO_InitStructure.GPIO_Pin = LED0_GPIO_Pin;
    GPIO_Init(LED0_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED1_GPIO_Pin;
    GPIO_Init(LED1_GPIO_Port, &GPIO_InitStructure);
}