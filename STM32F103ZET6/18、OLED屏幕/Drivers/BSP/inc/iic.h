#ifndef __IIC_H
#define __IIC_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h"
    #include "delay.h"



    /********************   �궨��   ********************/
    /* ���� ���� */
    /* SCL */
    #define IIC_SCL_GPIO_PORT               GPIOB
    #define IIC_SCL_GPIO_PIN                GPIO_PIN_6
    #define IIC_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()                        /* PB��ʱ��ʹ�� */
    /* SDA */
    #define IIC_SDA_GPIO_PORT               GPIOB
    #define IIC_SDA_GPIO_PIN                GPIO_PIN_7
    #define IIC_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()                        /* PB��ʱ��ʹ�� */

    /* SCL */
    #define SCL_H      HAL_GPIO_WritePin(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, GPIO_PIN_SET)     /* ʱ�������� */
    #define SCL_L      HAL_GPIO_WritePin(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, GPIO_PIN_RESET)   /* ʱ�������� */
    /* SDA */
    /* ����� */
    #define SDA_OUT_H  HAL_GPIO_WritePin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, GPIO_PIN_SET)     /* ���ģʽ������������ */
    #define SDA_OUT_L  HAL_GPIO_WritePin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, GPIO_PIN_RESET)   /* ���ģʽ������������ */
    /* ���룺*/
    #define SDA_IN     HAL_GPIO_WritePin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, GPIO_PIN_SET)     /* ����ģʽ */
    #define SDA_INT    HAL_GPIO_ReadPin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN)                    /* ���յ���ֵ */



    /****************    �����ⲿ����   *****************/
    void IIC_Init(void);                /* IIC��ʼ�� */
    void IIC_Start(void);               /* IIC��ʼ�ź� */
    void IIC_Stop(void);                /* IICֹͣ�ź� */
    void IIC_Send_Ack(uint8_t ack);     /* IICӦ���ź� */
    uint8_t IIC_Receive_Ack(void);      /* ����IICӦ���ź� */
    void IIC_Send_Byte(uint8_t data);   /* ����һ���ֽ����� */
    uint8_t IIC_Receive_Byte(void);     /* ����һ���ֽ����� */

#endif











