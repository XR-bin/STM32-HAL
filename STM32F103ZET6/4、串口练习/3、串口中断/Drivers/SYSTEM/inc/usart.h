#ifndef __USART_H
#define __USART_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"
    #include "stdio.h"



    /********************   宏定义   ********************/
    /**
    * 引脚 和 串口 定义 
    * 默认是针对USART1的.
    * 注意: 通过修改这几个宏定义,可以支持USART1~UART5任意一个串口.
    */
    /* USART_TX */
    #define USART_TX_GPIO_PORT                  GPIOA
    #define USART_TX_GPIO_PIN                   GPIO_PIN_9
    #define USART_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */
    /* USART_RX */
    #define USART_RX_GPIO_PORT                  GPIOA
    #define USART_RX_GPIO_PIN                   GPIO_PIN_10
    #define USART_RX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */
    /* USAER_x */
    #define USART_UX                            USART1
    #define USART_UX_CLK_ENABLE()               do{ __HAL_RCC_USART1_CLK_ENABLE(); }while(0)  /* USART1 时钟使能 */
    /* USAER_x中断 */
    #define USART_UX_IRQn                       USART1_IRQn
    #define USART_UX_IRQHandler                 USART1_IRQHandler
    /* 缓存设置 */
    #define USART_REC_LEN               200         /* 定义最大接收字节数 200 */
    #define RXBUFFERSIZE                1           /* 缓存大小 */



    /*****************   外部变量声明   *****************/
    extern UART_HandleTypeDef g_uart1_handle;       /* HAL UART句柄 */



    /*****************   外部函数声明   *****************/
    void USART_Init(uint32_t baud);                /* 串口初始化函数 */
    void USART1_Send_Byte(uint8_t data);           /* 串口1发生一个字节 */
    uint8_t USART1_Receive_Byte(void);             /* 串口1接收一个字节 */
    void USART1_Send_Str(uint8_t *str);            /* 串口1发生一个字符串 */
    void USART1_Receive_Str(uint8_t *str);         /* 串口1接收一个字符串 */

#endif


