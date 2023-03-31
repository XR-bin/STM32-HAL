#ifndef _KEY_H
#define _KEY_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "./SYSTEM/inc/sys.h"
    #include "./SYSTEM/inc/delay.h"
    
    
    
    /********************   �궨��   ********************/
    /* KEY���ʱ��ʹ�� */
    #define KEY_GPIO_CLK_ENABLE()          do{\
                                               __HAL_RCC_GPIOA_CLK_ENABLE();\
                                               __HAL_RCC_GPIOE_CLK_ENABLE();\
                                           }while(0)
                                           
    /* KEY0���Ŷ��� */
    #define KEY0_GPIO_PORT                  GPIOE
    #define KEY0_GPIO_PIN                   GPIO_PIN_4
    /* KEY1���Ŷ��� */
    #define KEY1_GPIO_PORT                  GPIOE
    #define KEY1_GPIO_PIN                   GPIO_PIN_3
    /* KEY_UP���Ŷ��� */
    #define KEYUP_GPIO_PORT                 GPIOA
    #define KEYUP_GPIO_PIN                  GPIO_PIN_0
    
    /* ��ȡ������ƽ */
    #define KEY0        HAL_GPIO_ReadPin(KEY0_GPIO_PORT, KEY0_GPIO_PIN)     /* ��ȡKEY0���� */
    #define KEY1        HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN)     /* ��ȡKEY1���� */
    #define KEY_UP      HAL_GPIO_ReadPin(KEYUP_GPIO_PORT, KEYUP_GPIO_PIN)   /* ��ȡKEY1���� */
    
    /* ������ʶ */
    #define KEY0_PRES    1              /* KEY0���� */
    #define KEY1_PRES    2              /* KEY1���� */
    #define WKUP_PRES    3              /* KEY_UP����(��WK_UP) */
    
    
    
    /****************    �����ⲿ����   ****************/
    void Key_Init(void);
    uint8_t Key_Scan(uint8_t mode);

#endif

