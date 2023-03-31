#ifndef _EXTI_H
#define _EXTI_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "./SYSTEM/inc/sys.h"
    #include "./SYSTEM/inc/delay.h"
    #include "./BSP/inc/led.h"
    
    
    
    /********************   �궨��   ********************/
    /* LED���ʱ��ʹ�� */
    #define KEY_GPIO_CLK_ENABLE()          do{__HAL_RCC_GPIOE_CLK_ENABLE();}while(0)
    
    /* KEY0���Ŷ��� */
    #define KEY0_INT_GPIO_PORT              GPIOE
    #define KEY0_INT_GPIO_PIN               GPIO_PIN_4
    /* KEY1���Ŷ��� */
    #define KEY1_INT_GPIO_PORT              GPIOE
    #define KEY1_INT_GPIO_PIN               GPIO_PIN_3
    
    /* �жϱ�� & �жϷ����� */
    #define KEY0_INT_IRQn                   EXTI4_IRQn
    #define KEY0_INT_IRQHandler             EXTI4_IRQHandler
    #define KEY1_INT_IRQn                   EXTI3_IRQn
    #define KEY1_INT_IRQHandler             EXTI3_IRQHandler
    
    /* ��ȡ������ƽ */
    #define KEY0        HAL_GPIO_ReadPin(KEY0_INT_GPIO_PORT, KEY0_INT_GPIO_PIN)     /* ��ȡKEY0���� */
    #define KEY1        HAL_GPIO_ReadPin(KEY1_INT_GPIO_PORT, KEY1_INT_GPIO_PIN)     /* ��ȡKEY1���� */
    
    
    
    /******************   �����ⲿ����   ******************/
    void Extix_Init(void);

#endif

