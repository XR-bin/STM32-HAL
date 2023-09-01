#ifndef __EXTI_H
#define __EXTI_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h"
    #include "delay.h"



    /********************   �궨��   ********************/
    /* ���� �� �жϱ�� & �жϷ����� ���� */ 
    #define KEY0_INT_GPIO_PORT              GPIOE
    #define KEY0_INT_GPIO_PIN               GPIO_PIN_4
    #define KEY0_INT_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE��ʱ��ʹ�� */
    #define KEY0_INT_IRQn                   EXTI4_IRQn
    #define KEY0_INT_IRQHandler             EXTI4_IRQHandler



    /****************    �����ⲿ����   *****************/
    void EXTI4_Init(void);      /* �ⲿ�ж�4��ʼ�� */

#endif









