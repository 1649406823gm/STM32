
#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

//自定义延时函数
void MY_Delay_Init(void);
//us延时
void MY_Delay_us(uint32_t nus);
//ms延时
void MY_Delay_ms(uint32_t nms);

//自定义延时函数,基于DWT
void DWT_Delay_Init(void);
void DWT_Delay_us(__IO uint32_t us);
void DWT_Delay_ms(__IO uint32_t ms);



//宏开关，不使用freertos时，为0，否则为1
#define SUPPORT_OS 1
#define USE_DWT_DELAY 1

#ifdef __cplusplus
}
#endif

#endif