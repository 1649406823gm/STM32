#include "usart.h"
#include "string.h"
#include "stdio.h"

uint8_t data_buffer;                       // 接受缓冲区
uint8_t usart1_rx_finish_flag = 0;         // 接受完成标志位
uint8_t g_recv_buff[USART_RECEIVE_LENGTH]; // 接受字符保存缓冲区
uint8_t g_recv_length = 0;                 // 接受字符长度

UART_HandleTypeDef huart1;

// 串口参数的初始化
void USARTX_UART_Init(uint32_t baudrate)
{
    huart1.Instance = USART_UX;
    huart1.Init.BaudRate = baudrate;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;     // 无硬件控制流
    huart1.Init.OverSampling = UART_OVERSAMPLING_16; // 十六倍过采样

    HAL_UART_Init(&huart1);

    /* 该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量 */
    /* 以一字节一字节接受 */
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&data_buffer, 1);
}

// 串口引脚的初始化
void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (uartHandle->Instance == USART_UX)
    {
        USART_UX_CLK_ENABLE();
        USART_TX_GPIO_CLK_ENABLE();
        USART_RX_GPIO_CLK_ENABLE();

        GPIO_InitStruct.Pin = USART_TX_GPIO_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = USART_TX_GPIO_AF;
        HAL_GPIO_Init(USART_TX_GPIO_PORT, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = USART_RX_GPIO_PIN;
        GPIO_InitStruct.Alternate = USART_RX_GPIO_AF;
        HAL_GPIO_Init(USART_RX_GPIO_PORT, &GPIO_InitStruct);

        // 设置接受中断优先级
        HAL_NVIC_SetPriority(USART_UX_IRQn, 0, 1);
        HAL_NVIC_EnableIRQ(USART_UX_IRQn);
    }
}

void USART_UX_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart1);
}

// 串口回调函数,实际处理代码
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *uartHandle)
{
    if (uartHandle->Instance == USART_UX)
    {
        if (g_recv_length >= 255) // 溢出判断
        {
            g_recv_length = 0;
            memset(g_recv_buff, 0x00, sizeof(g_recv_buff));
            HAL_UART_Transmit(&huart1, (uint8_t *)"数据溢出", 12, 0xFFFF);
        }
        else
        {
            g_recv_buff[g_recv_length++] = data_buffer;
            if ((g_recv_buff[g_recv_length - 1] == 0x0A) && (g_recv_buff[g_recv_length - 2] == 0x0D)) // 判断结束使
            {
                usart1_rx_finish_flag = 1;
                printf("\n接受到的数据为：\n");
                HAL_UART_Transmit(&huart1, (uint8_t *)&g_recv_buff, g_recv_length, 0xFFFF); // 将收到的信息发鿁出县
                printf("发送接受到的数据\n");
                while (HAL_UART_GetState(&huart1) == HAL_UART_STATE_BUSY_TX)
                    ; // 检测发送是否结束
                g_recv_length = 0;
                memset(g_recv_buff, 0x00, sizeof(g_recv_buff)); // 清空数组
            }
        }
        HAL_UART_Receive_IT(&huart1, (uint8_t *)&data_buffer, 1); // HAL_UART_IRQHandler中会失能中断导致下次数据来临不会接受因此需要再打开接受中断函数
    }
}

int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
    return ch;
}

int fgetc(FILE *f)
{
    uint8_t ch = 0;
    HAL_UART_Receive(&huart1, &ch, 1, 0xffff);
    return ch;
}
