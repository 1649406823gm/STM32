#include "delay.h"

#if SUPPORT_OS
//使用外部定时器注意修改对应的定时器
extern TIM_HandleTypeDef htim6;
#define Timebase_htim    htim6
#endif

static uint32_t g_fac_us = 0;            /* us 延时倍乘数 */
/**
* @brief    初始化延时函数
*    		当使用FreeRTOS时，系统滴答定时器被FreeRTOS使用，时基定时器为外部定时器，
*    		为了不占用高级定时器，外部定时器，应该选择tim6或者tim7
*    		使用滴答定时器作为计数，计数频率为系统时钟，使用外部定时器计数频率为1Mhz
*    		由此可以得到us计数次数
* @param    无
* @retval   无
*/
void MY_Delay_Init()
{
#if SUPPORT_OS
	g_fac_us = 1000000 / 1000000;
#else
	g_fac_us = HAL_RCC_GetHCLKFreq() / 1000000;   //获取MCU的主频
#endif
}
/**
* @brief    us延时函数
* @note     使用时钟摘取法来做us延时
* @param    nus:要延时的us数
* @note     nus取值范围：0 ~ (2^32 / fac_us)(fac_us一般等于系统主频)
* @retval   无
*/
void MY_Delay_us(uint32_t nus)
{
#if SUPPORT_OS
//采用开关定时器的方法
//	uint32_t cnt=0;
//	__HAL_TIM_ENABLE(&htim6);
//	__HAL_TIM_SetCounter(&htim6,0);
//	while( cnt < nus-1){
//
//		cnt=__HAL_TIM_GetCounter(&htim6);
//	}
//	__HAL_TIM_DISABLE(&htim6);

	uint32_t ticks;
	uint32_t told,tnow,tcnt = 0;
	uint32_t reload = __HAL_TIM_GetAutoreload(&Timebase_htim);   /*LOAD的值*/
	ticks = nus * g_fac_us;             /*需要的节拍数*/

	told = __HAL_TIM_GetCounter(&Timebase_htim);
	while(1)
	{
		tnow = __HAL_TIM_GetCounter(&Timebase_htim);
		if(tnow != told)
		{
			if(tnow > told)
			{
				tcnt += tnow - told; //注意外部定时器是递增计数
			}
			else
			{
				tcnt += reload - told + tnow;
			}
			told = tnow;
			if(tcnt >= ticks)
			{
				break;            /*时间超过/等于要延时的时间，则退出*/
			}
		}
	}
#else
	uint32_t ticks;
	uint32_t told,tnow,tcnt = 0;
	uint32_t reload = SysTick->LOAD;    /*LOAD的值*/
	ticks = nus * g_fac_us;             /*需要的节拍数*/

	told = SysTick->VAL;                /*刚进入时的计数器值*/
	while(1)
	{
		tnow = SysTick->VAL;
		if(tnow != told)
		{
			if(tnow < told)
			{
				tcnt += told - tnow; /*注意一下SYSTICK是一个递减的计数器*/
			}
			else
			{
				tcnt += reload - tnow + told;
			}
			told = tnow;
			if(tcnt >= ticks)
			{
				break;            /*时间超过/等于要延时的时间，则退出*/
			}
		}
	}
#endif
}
/**
* @brief    ms延时函数
* @param    nms:要延时的ms数
* @note     nms取值范围：0 ~ (2^32 / fac_us / 1000)(fac_us一般等于系统主频)
* @retval   无
*/
void MY_Delay_ms(uint32_t nms)
{
	MY_Delay_us((uint32_t)(nms * 1000));
}




//DWT延时函数实现,支持裸机和RTOS
#if USE_DWT_DELAY

#define  DWT_CR      *(__IO uint32_t *)0xE0001000
#define  DWT_CYCCNT  *(__IO uint32_t *)0xE0001004
#define  DEM_CR      *(__IO uint32_t *)0xE000EDFC


#define  DEM_CR_TRCENA                   (1 << 24)
#define  DWT_CR_CYCCNTENA                (1 <<  0)

/**
  * @brief  初始化时间戳
  * @param  无
  * @retval 无
  * @note   使用延时函数前，必须调用本函数
  */
void DWT_Delay_Init(void)
{
	/* 使能DWT外设 */
	DEM_CR |= (uint32_t)DEM_CR_TRCENA;

	/* DWT CYCCNT寄存器计数清0 */
	DWT_CYCCNT = (uint32_t)0u;

	/* 使能Cortex-M DWT CYCCNT寄存器 */
	DWT_CR |= (uint32_t)DWT_CR_CYCCNTENA;
}

void DWT_Delay_us(__IO uint32_t us)
{
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;

	ticks = us * (HAL_RCC_GetHCLKFreq() / 1000000);  /* 需要的节拍数 */
	tcnt = 0;
	told = ((uint32_t)DWT_CYCCNT);         /* 刚进入时的计数器值 */

	while(1)
	{
		tnow = ((uint32_t)DWT_CYCCNT);
		if(tnow != told)
		{
			/* 32位计数器是递增计数器 */
			if(tnow > told)
			{
				tcnt += tnow - told;
			}
				/* 重新装载 */
			else
			{
				tcnt += UINT32_MAX - told + tnow;
			}

			told = tnow;

			/*时间超过/等于要延迟的时间,则退出 */
			if(tcnt >= ticks)break;
		}
	}
}

void DWT_Delay_ms(__IO uint32_t ms)
{
	DWT_Delay_us(1000 * ms);
}

#endif