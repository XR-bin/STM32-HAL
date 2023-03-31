#ifndef _IIC_H
#define _IIC_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "./SYSTEM/inc/sys.h"
    #include "./SYSTEM/inc/delay.h"
    
    
    
    /********************   �궨��   ********************/
    /* IIC�������ʱ��ʹ�� */
    #define IIC_SCL_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB��ʱ��ʹ�� */
    #define IIC_SDA_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB��ʱ��ʹ�� */
    
    /* SCL���Ŷ��� */
    #define IIC_SCL_GPIO_PORT               GPIOB
    #define IIC_SCL_GPIO_PIN                GPIO_PIN_6
    /* SDA���Ŷ��� */
    #define IIC_SDA_GPIO_PORT               GPIOB
    #define IIC_SDA_GPIO_PIN                GPIO_PIN_7
    
    /* SCL��ƽ���� */
    #define IIC_SCL(x)        do{ x ? \
                                  HAL_GPIO_WritePin(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, GPIO_PIN_RESET); \
                              }while(0)     
    /* SDA��ƽ���� */
    #define IIC_SDA(x)        do{ x ? \
                                  HAL_GPIO_WritePin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, GPIO_PIN_RESET); \
                              }while(0)     
    /* ��ȡSDA���� */
    #define IIC_READ_SDA     HAL_GPIO_ReadPin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN) 


    
    /****************    �����ⲿ����   ****************/
    void IIC_Init(void);
    void IIC_Start(void);
    void IIC_Stop(void);
    uint8_t IIC_Wait_Ack(void);
    void IIC_Send_Ack(uint8_t ack);
    void IIC_Send_Byte(uint8_t data);
    uint8_t IIC_Receive_Byte(uint8_t ack);
    
#endif
