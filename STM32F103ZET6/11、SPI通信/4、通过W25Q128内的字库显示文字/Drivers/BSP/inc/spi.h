#ifndef __SPI_H
#define __SPI_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h"



    /********************   �궨��   ********************/
    /* SPI2 ���Ŷ��� */
    /* SCK   ʱ���� */
    #define SPI2_SCK_GPIO_PORT              GPIOB
    #define SPI2_SCK_GPIO_PIN               GPIO_PIN_13
    #define SPI2_SCK_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()   /* PB��ʱ��ʹ�� */
    /* MISO ������ */
    #define SPI2_MISO_GPIO_PORT             GPIOB
    #define SPI2_MISO_GPIO_PIN              GPIO_PIN_14
    #define SPI2_MISO_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()   /* PB��ʱ��ʹ�� */
    /* MOSI ����� */
    #define SPI2_MOSI_GPIO_PORT             GPIOB
    #define SPI2_MOSI_GPIO_PIN              GPIO_PIN_15
    #define SPI2_MOSI_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()   /* PB��ʱ��ʹ�� */

    /* SPI2��ض��� */
    #define SPI2_SPI                        SPI2
    #define SPI2_SPI_CLK_ENABLE()           __HAL_RCC_SPI2_CLK_ENABLE()    /* SPI2ʱ��ʹ�� */



    /****************    �����ⲿ����   *****************/
    void SPI2_Init(void);                 /* SPI2���ó�ʼ�� */
    uint8_t SPI2_RS_Byte(uint8_t data);   /* SPI2�շ����� */

#endif






