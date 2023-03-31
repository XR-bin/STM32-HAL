#ifndef _IIC_H
#define _IIC_H

    /****************   外部头文件声明   ****************/
    #include "./SYSTEM/inc/sys.h"
    #include "./SYSTEM/inc/delay.h"
    
    
    
    /********************   宏定义   ********************/
    /* IIC引脚相关时钟使能 */
    #define IIC_SCL_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */
    #define IIC_SDA_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */
    
    /* SCL引脚定义 */
    #define IIC_SCL_GPIO_PORT               GPIOB
    #define IIC_SCL_GPIO_PIN                GPIO_PIN_6
    /* SDA引脚定义 */
    #define IIC_SDA_GPIO_PORT               GPIOB
    #define IIC_SDA_GPIO_PIN                GPIO_PIN_7
    
    /* SCL电平操作 */
    #define IIC_SCL(x)        do{ x ? \
                                  HAL_GPIO_WritePin(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, GPIO_PIN_RESET); \
                              }while(0)     
    /* SDA电平操作 */
    #define IIC_SDA(x)        do{ x ? \
                                  HAL_GPIO_WritePin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, GPIO_PIN_RESET); \
                              }while(0)     
    /* 读取SDA操作 */
    #define IIC_READ_SDA     HAL_GPIO_ReadPin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN) 


    
    /****************    函数外部声明   ****************/
    void IIC_Init(void);
    void IIC_Start(void);
    void IIC_Stop(void);
    uint8_t IIC_Wait_Ack(void);
    void IIC_Send_Ack(uint8_t ack);
    void IIC_Send_Byte(uint8_t data);
    uint8_t IIC_Receive_Byte(uint8_t ack);
    
#endif
