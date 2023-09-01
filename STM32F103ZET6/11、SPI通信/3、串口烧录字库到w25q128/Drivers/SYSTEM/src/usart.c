#include "usart.h"
#include "w25q128.h"
#include "led.h"



UART_HandleTypeDef g_uart1_handle;      /* UART1句柄 用于配置串口信息的全局变量 */

uint8_t g_rx_buffer[RXBUFFERSIZE];      /* HAL库使用的串口接收缓冲 */



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

    /* 中断优先级配置 */
    HAL_NVIC_EnableIRQ(USART_UX_IRQn);                       /* 使能USART1中断通道 */
    HAL_NVIC_SetPriority(USART_UX_IRQn, 1, 1);               /* 组2，最低优先级:抢占优先级1，子优先级1 */

    /* 中断使能 */
     __HAL_UART_ENABLE_IT(&g_uart1_handle,UART_IT_IDLE);     /* 使能空闲中断 */
     __HAL_UART_ENABLE_IT(&g_uart1_handle,UART_IT_RXNE);     /* 使能接收中断 */

    /* 该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量 */
    HAL_UART_Receive_IT(&g_uart1_handle, (uint8_t *)g_rx_buffer, RXBUFFERSIZE);
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



/**********************************************************
* @funcName ：USART1_Send_Byte
* @brief    ：串口1发送一个字节的数据（uint8_t）
* @param    ：uint8_t data
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void USART1_Send_Byte(uint8_t data)
{
    while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC) != SET);             /* 等待发送结束 */
    HAL_UART_Transmit(&g_uart1_handle,&data, 1, 0xffff);                         /* 发送的数据 */
}



/**********************************************************
* @funcName ：USART1_Send_Str
* @brief    ：串口1发送一个字符串
* @param    ：uint8_t *str
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void USART1_Send_Str(uint8_t *str)
{
    while(*str != '\0')
    {
        while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC) != SET);             /* 等待发送结束 */
        HAL_UART_Transmit(&g_uart1_handle,str, 1, 0xffff);                           /* 发送的数据 */
        str++;
    }
}



/**********************************************************
* @funcName ：USART1_Receive_Byte
* @brief    ：串口1接收一个字节的数据（uint8_t）
* @param    ：void
* @retval   ：uint8_t
* @details  ：
* @fn       ：
************************************************************/
uint8_t USART1_Receive_Byte(void)
{
    uint8_t data;

    while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_RXNE) != SET);           /* 等待接收不为空 */
    HAL_UART_Receive(&g_uart1_handle, &data, 1, 0xffff);                         /* 获取接收到的数据 */

    return data;
}



/**********************************************************
* @funcName ：USART1_Receive_Str
* @brief    ：串口1接收一个字符串
* @param    ：uint8_t *str
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void USART1_Receive_Str(uint8_t *str)
{
    while(1)
    {
        while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_RXNE) != SET);           /* 等待接收不为空 */
        HAL_UART_Receive(&g_uart1_handle, str, 1, 0xffff);                           /* 获取接收到的数据 */
        if(*str == '\r')
        {
            while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_RXNE) != SET);           /* 等待接收不为空 */
            HAL_UART_Receive(&g_uart1_handle, str, 1, 0xffff);                           /* 获取接收到的数据 */
            if(*str == '\n') break;
        }
        str++;
    }
    *str = '\0';
}



/**********************************************************
* @funcName ：USART_UX_IRQHandler
* @brief    ：串口中断函数
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void USART_UX_IRQHandler(void)
{
    uint8_t data;
    static uint32_t addr = 0x0b0000;

    /* 判断中断触发的形式 */
    /* 接收完成中断 */
    if(__HAL_UART_GET_FLAG(&g_uart1_handle, UART_FLAG_RXNE) ==  SET)
    {
        /* 中断要执行的任务 */
        LED0_ON;
        /* 字库数据写入W25Q128 */
        data = USART1->DR;
        W25Q128_Page_Write(addr,1,&data);
        addr++;

        /* 先读USART1->SR，再改USART1->DR，可以清除中断标志 */
        /* 或者直接修改USART1->SR，从而清除中断标志 */
        /* 我这里两种方式都用 */
        USART1->SR &= ~(1<<5);
    }

    if(__HAL_UART_GET_FLAG(&g_uart1_handle, UART_FLAG_IDLE) ==  SET)
    {
        LED0_OFF;

        /* 先读USART1->SR，再改USART1->DR，可以清除中断标志 */
        /* 或者直接修改USART1->SR，从而清除中断标志 */
        /* 我这里两种方式都用 */
        USART1->SR &= ~(1<<4);
        USART1->DR;
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



