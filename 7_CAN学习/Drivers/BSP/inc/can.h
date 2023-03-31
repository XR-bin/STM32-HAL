#ifndef _CAN_H
#define _CAN_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "./SYSTEM/inc/sys.h"
    
    
    
    /********************   �궨��   ********************/
    /* CAN_RX���Ŷ��� */
    #define CAN_RX_GPIO_PORT                GPIOA
    #define CAN_RX_GPIO_PIN                 GPIO_PIN_11
    #define CAN_RX_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */
    /* CAN_TX���Ŷ��� */
    #define CAN_TX_GPIO_PORT                GPIOA
    #define CAN_TX_GPIO_PIN                 GPIO_PIN_12
    #define CAN_TX_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */
    
    
    
    /****************    �����ⲿ����   ****************/
    uint8_t CAN_Init(uint32_t tsjw, uint32_t tbs1, uint32_t tbs2, uint16_t brp, uint32_t mode);
    uint8_t CAN_Send_Message(uint32_t id, uint8_t *msg, uint8_t len);
    uint8_t CAN_Receive_Message(uint32_t id, uint8_t *buf);
    
#endif
