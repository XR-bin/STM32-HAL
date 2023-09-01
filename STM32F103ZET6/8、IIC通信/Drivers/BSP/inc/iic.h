#ifndef __IIC_H
#define __IIC_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"
    #include "delay.h"



    /********************   宏定义   ********************/
    /* 引脚 定义 */
    /* SCL */
    #define IIC_SCL_GPIO_PORT               GPIOB
    #define IIC_SCL_GPIO_PIN                GPIO_PIN_6
    #define IIC_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()                        /* PB口时钟使能 */
    /* SDA */
    #define IIC_SDA_GPIO_PORT               GPIOB
    #define IIC_SDA_GPIO_PIN                GPIO_PIN_7
    #define IIC_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()                        /* PB口时钟使能 */

    /* SCL */
    #define SCL_H      HAL_GPIO_WritePin(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, GPIO_PIN_SET)     /* 时钟线拉高 */
    #define SCL_L      HAL_GPIO_WritePin(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, GPIO_PIN_RESET)   /* 时钟线拉低 */
    /* SDA */
    /* 输出： */
    #define SDA_OUT_H  HAL_GPIO_WritePin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, GPIO_PIN_SET)     /* 输出模式下数据线拉高 */
    #define SDA_OUT_L  HAL_GPIO_WritePin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, GPIO_PIN_RESET)   /* 输出模式下数据线拉低 */
    /* 输入：*/
    #define SDA_IN     HAL_GPIO_WritePin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, GPIO_PIN_SET)     /* 输入模式 */
    #define SDA_INT    HAL_GPIO_ReadPin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN)                    /* 接收到的值 */



    /****************    函数外部声明   *****************/
    void IIC_Init(void);                /* IIC初始化 */
    void IIC_Start(void);               /* IIC起始信号 */
    void IIC_Stop(void);                /* IIC停止信号 */
    void IIC_Send_Ack(uint8_t ack);     /* IIC应答信号 */
    uint8_t IIC_Receive_Ack(void);      /* 接收IIC应答信号 */
    void IIC_Send_Byte(uint8_t data);   /* 发送一个字节数据 */
    uint8_t IIC_Receive_Byte(void);     /* 接收一个字节数据 */

#endif











