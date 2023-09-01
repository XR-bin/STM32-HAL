#ifndef __USART_H
#define __USART_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h"
    #include "stdio.h"



    /********************   �궨��   ********************/
    /**
    * ���� �� ���� ���� 
    * Ĭ�������USART1��.
    * ע��: ͨ���޸��⼸���궨��,����֧��USART1~UART5����һ������.
    */
    /* USART_TX */
    #define USART_TX_GPIO_PORT                  GPIOA
    #define USART_TX_GPIO_PIN                   GPIO_PIN_9
    #define USART_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */
    /* USART_RX */
    #define USART_RX_GPIO_PORT                  GPIOA
    #define USART_RX_GPIO_PIN                   GPIO_PIN_10
    #define USART_RX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */
    /* USAER_x */
    #define USART_UX                            USART1
    #define USART_UX_CLK_ENABLE()               do{ __HAL_RCC_USART1_CLK_ENABLE(); }while(0)  /* USART1 ʱ��ʹ�� */



    /*****************   �ⲿ��������   *****************/
    extern UART_HandleTypeDef g_uart1_handle;       /* HAL UART��� */



    /*****************   �ⲿ��������   *****************/
    void USART_Init(uint32_t baud);                /* ���ڳ�ʼ������ */
    void USART1_Send_Byte(uint8_t data);           /* ����1����һ���ֽ� */
    uint8_t USART1_Receive_Byte(void);             /* ����1����һ���ֽ� */
    void USART1_Send_Str(uint8_t *str);            /* ����1����һ���ַ��� */
    void USART1_Receive_Str(uint8_t *str);         /* ����1����һ���ַ��� */

#endif


