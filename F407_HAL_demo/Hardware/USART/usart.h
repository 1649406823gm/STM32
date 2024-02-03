#ifndef __MY_USART_H
#define __MY_USART_H

#include "stm32f4xx.h"

#define USART_TX_GPIO_PORT GPIOA
#define USART_TX_GPIO_PIN GPIO_PIN_9
#define USART_TX_GPIO_AF GPIO_AF7_USART1
#define USART_TX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART_TX_GPIO_AF GPIO_AF7_USART1

#define USART_RX_GPIO_PORT GPIOA
#define USART_RX_GPIO_PIN GPIO_PIN_10
#define USART_RX_GPIO_AF GPIO_AF7_USART1
#define USART_RX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART_RX_GPIO_AF GPIO_AF7_USART1

#define USART_UX USART1
#define USART_UX_IRQn USART1_IRQn
#define USART_UX_IRQHandler USART1_IRQHandler
#define USART_UX_CLK_ENABLE() __HAL_RCC_USART1_CLK_ENABLE()

#define USART_RECEIVE_LENGTH 256

void USARTX_UART_Init(uint32_t baudrate);

extern uint8_t g_recv_buff[USART_RECEIVE_LENGTH]; // 接受字符保存缓冲区
extern uint8_t g_recv_length;                     // 接受字符长度

#endif