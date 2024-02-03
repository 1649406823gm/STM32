
#ifndef __DELAY_H
#define __DELAY_H

#include "main.h"

//自定义延时函数
void MY_Delay_Init(void);
//us延时
void MY_Delay_us(uint32_t nus);
//ms延时
void MY_Delay_ms(uint32_t nms);

//宏开关，使用freertos时，为0，否则为1
#define SUPPORT_OS 0


#endif