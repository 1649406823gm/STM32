#include "main.h"
#include "stm32f4xx.h"


uint32_t sysclock = 0;
uint32_t HClk = 0;
uint32_t PClk1 = 0;
uint32_t PClk2 = 0;


int main(void)
{
    /* 系统时钟初始化成168MHz */
    SystemClock_Config();
    MY_Delay_Init();

    /* 控制LED灯 */
    USARTX_UART_Init(115200);
    Led_Init();
    sysclock = HAL_RCC_GetSysClockFreq();
    HClk = HAL_RCC_GetHCLKFreq();
    PClk1 = HAL_RCC_GetPCLK1Freq();
    PClk2 = HAL_RCC_GetPCLK2Freq();
    while (1)
    {
      LED0(0);
      MY_Delay_ms(1000);
      LED0(1);
      MY_Delay_ms(1000);
      printf("%d,%d,%d,%d\r\n",sysclock,HClk,PClk1,PClk2);
    }
}






