#ifndef __TOUCH_H
#define __TOUCH_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h"
    #include "delay.h"



    /********************   �궨��   ********************/
    /* ���败��������IC T_PEN/T_CS/T_MISO/T_MOSI/T_SCK ���� ���� */
    /* T_PEN���� */
    #define T_PEN_GPIO_PORT                 GPIOF
    #define T_PEN_GPIO_PIN                  GPIO_PIN_10
    #define T_PEN_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOF_CLK_ENABLE()    /* ����IO��ʱ��ʹ�� */
    /* T_CS���� */
    #define T_CS_GPIO_PORT                  GPIOF
    #define T_CS_GPIO_PIN                   GPIO_PIN_11
    #define T_CS_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOF_CLK_ENABLE()    /* ����IO��ʱ��ʹ�� */
    /* T_MISO���� */
    #define T_MISO_GPIO_PORT                GPIOB
    #define T_MISO_GPIO_PIN                 GPIO_PIN_2
    #define T_MISO_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()    /* ����IO��ʱ��ʹ�� */
    /* T_MOSI���� */
    #define T_MOSI_GPIO_PORT                GPIOF
    #define T_MOSI_GPIO_PIN                 GPIO_PIN_9
    #define T_MOSI_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOF_CLK_ENABLE()    /* ����IO��ʱ��ʹ�� */
    /* T_SCK���� */
    #define T_SCK_GPIO_PORT                 GPIOB
    #define T_SCK_GPIO_PIN                  GPIO_PIN_1
    #define T_SCK_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()    /* ����IO��ʱ��ʹ�� */

    /* Ƭѡ�� */
    #define T_CS_H    HAL_GPIO_WritePin(T_CS_GPIO_PORT, T_CS_GPIO_PIN, GPIO_PIN_SET)   /* Ƭѡʧ�� */
    #define T_CS_L    HAL_GPIO_WritePin(T_CS_GPIO_PORT, T_CS_GPIO_PIN, GPIO_PIN_RESET) /* Ƭѡʹ�� */
    /* ʱ���� */
    #define T_SCK_H   HAL_GPIO_WritePin(T_SCK_GPIO_PORT, T_SCK_GPIO_PIN, GPIO_PIN_SET)
    #define T_SCK_L   HAL_GPIO_WritePin(T_SCK_GPIO_PORT, T_SCK_GPIO_PIN, GPIO_PIN_RESET)
    /* ����� */
    #define T_MOSI_H  HAL_GPIO_WritePin(T_MOSI_GPIO_PORT, T_MOSI_GPIO_PIN, GPIO_PIN_SET)
    #define T_MOSI_L  HAL_GPIO_WritePin(T_MOSI_GPIO_PORT, T_MOSI_GPIO_PIN, GPIO_PIN_RESET)
    /* ������ */
    #define T_MISO    HAL_GPIO_ReadPin(T_MISO_GPIO_PORT, T_MISO_GPIO_PIN)
    /* �ʽӴ��� */
    #define T_PEN     HAL_GPIO_ReadPin(T_PEN_GPIO_PORT, T_PEN_GPIO_PIN)

    /* ָ�� */
    #define TOUCH_X   0xD0    //��ȡx��������
    #define TOUCH_Y   0x90    //��ȡy��������



    /****************    �����ⲿ����   *****************/
    void Touch_IO_Init(void);                /* ���败�������ų�ʼ�� */
    uint16_t Touch_Data(uint8_t command);    /* ����败�����������ݲ����յ��败�������������� */

#endif







