#include "usart.h"



UART_HandleTypeDef g_uart1_handle;  /* UART1句柄 用于配置串口信息的全局变量 */



/**********************************************************
* @funcName ：USART1_Init
* @brief    ：对USART1对应的GPIO口进行初始化设置
* @param    ：uint32_t baud
* @retval   ：void
* @details  ：
*             PA9     TX     ---------输出
*             PA10    RX     ---------输入
* @fn       ：
************************************************************/
void USART_Init(uint32_t baud)
{
    /*UART 初始化设置*/
    g_uart1_handle.Instance        = USART_UX;               /* USART_UX---选择串口1 */
    g_uart1_handle.Init.BaudRate   = baud;                   /* 波特率 */
    g_uart1_handle.Init.WordLength = UART_WORDLENGTH_8B;     /* 字长为8位数据格式 */
    g_uart1_handle.Init.StopBits   = UART_STOPBITS_1;        /* 一个停止位 */
    g_uart1_handle.Init.Parity     = UART_PARITY_NONE;       /* 无奇偶校验位 */
    g_uart1_handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;    /* 无硬件流控 */
    g_uart1_handle.Init.Mode       = UART_MODE_TX_RX;        /* 收发模式 */
    HAL_UART_Init(&g_uart1_handle);                          /* HAL_UART_Init()会使能UART1 */
}



/**********************************************************
* @funcName ：HAL_UART_MspInit
* @brief    ：UART底层初始化函数(底层初始化回调钩子函数)
* @param    ：UART_HandleTypeDef *huart (UART句柄类型指针)
* @retval   ：void
* @details  ：
* @fn       ：
*        这个函数会在HAL_UART_Init()函数中被调用，是UART底层初
*    始化回调钩子函数。
************************************************************/
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio_init_struct;                          /* GPIO配置结构体 */

    if (huart->Instance == USART_UX)                            /* 如果是串口1，进行串口1 MSP初始化 */
    {
        USART_TX_GPIO_CLK_ENABLE();                             /* 使能串口TX脚时钟 */
        USART_RX_GPIO_CLK_ENABLE();                             /* 使能串口RX脚时钟 */
        USART_UX_CLK_ENABLE();                                  /* 使能串口时钟 */

        gpio_init_struct.Pin   = USART_TX_GPIO_PIN;             /* 串口发送引脚号 */
        gpio_init_struct.Mode  = GPIO_MODE_AF_PP;               /* 复用推挽输出 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* IO速度设置为高速 */
        HAL_GPIO_Init(USART_TX_GPIO_PORT, &gpio_init_struct);   /* 串口TX脚 设置成复用推挽输出模式 */

        gpio_init_struct.Pin   = USART_RX_GPIO_PIN;             /* 串口RX脚 模式设置 */
        gpio_init_struct.Mode  = GPIO_MODE_AF_INPUT;            /* 复用输入 */
        gpio_init_struct.Pull  = GPIO_PULLUP;                   /* 上拉 */
        HAL_GPIO_Init(USART_RX_GPIO_PORT, &gpio_init_struct);   /* 串口RX脚 必须设置成复用上拉输入模式 */
    }
}


























/******************************************************************************************/
/* 加入以下代码, 支持printf函数, 而不需要选择use MicroLIB */

#if 1

#if (__ARMCC_VERSION >= 6010050)            /* 使用AC6编译器时 */
__asm(".global __use_no_semihosting\n\t");  /* 声明不使用半主机模式 */
__asm(".global __ARM_use_no_argv \n\t");    /* AC6下需要声明main函数为无参数格式，否则部分例程可能出现半主机模式 */

#else
/* 使用AC5编译器时, 要在这里定义__FILE 和 不使用半主机模式 */
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

#endif

/* 不使用半主机模式，至少需要重定义_ttywrch\_sys_exit\_sys_command_string函数,以同时兼容AC6和AC5模式 */
int _ttywrch(int ch)
{
    ch = ch;
    return ch;
}

/* 定义_sys_exit()以避免使用半主机模式 */
void _sys_exit(int x)
{
    x = x;
}

char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}


/* FILE 在 stdio.h里面定义. */
FILE __stdout;

/* MDK下需要重定义fputc函数, printf函数最终会通过调用fputc输出字符串到串口 */
int fputc(int ch, FILE *f)
{
    while ((USART1->SR & 0X40) == 0);     /* 等待上一个字符发送完成 */

    USART1->DR = (uint8_t)ch;             /* 将要发送的字符 ch 写入到DR寄存器 */
    return ch;
}
#endif
/******************************************************************************************/



