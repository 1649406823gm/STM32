#include "stm32f4xx.h"
#include "led.h"


int main()
{
    LED_Init();
    while(1)
    {
        GPIO_ResetBits(LED0_GPIO_Port, LED0_GPIO_Pin);
        GPIO_ResetBits(LED1_GPIO_Port, LED1_GPIO_Pin);
    } 
}
    


