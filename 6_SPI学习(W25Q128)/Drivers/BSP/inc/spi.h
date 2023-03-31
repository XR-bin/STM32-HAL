#ifndef _SPI_H
#define _SPI_H
    
    /****************   外部头文件声明   ****************/
    #include "./SYSTEM/inc/sys.h"
    
    
    
    /********************   宏定义   ********************/
    /* SPI2的SCK引脚定义、相关时钟定义 */
    #define SPI2_SCK_GPIO_PORT              GPIOB
    #define SPI2_SCK_GPIO_PIN               GPIO_PIN_13
    #define SPI2_SCK_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */
    /* SPI2的MISO引脚定义、相关时钟定义 */
    #define SPI2_MISO_GPIO_PORT             GPIOB
    #define SPI2_MISO_GPIO_PIN              GPIO_PIN_14
    #define SPI2_MISO_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */
    /* SPI2的MOSI引脚定义、相关时钟定义 */
    #define SPI2_MOSI_GPIO_PORT             GPIOB
    #define SPI2_MOSI_GPIO_PIN              GPIO_PIN_15
    #define SPI2_MOSI_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */
    /* SPI2相关定义 */
    #define SPI2_SPI                        SPI2
    #define SPI2_SPI_CLK_ENABLE()           do{ __HAL_RCC_SPI2_CLK_ENABLE(); }while(0)    /* SPI2时钟使能 */
    
    /* SPI总线速度设置 */
    #define SPI_SPEED_2         0
    #define SPI_SPEED_4         1
    #define SPI_SPEED_8         2
    #define SPI_SPEED_16        3
    #define SPI_SPEED_32        4
    #define SPI_SPEED_64        5
    #define SPI_SPEED_128       6
    #define SPI_SPEED_256       7
    
    /****************    函数外部声明   ****************/
    void SPI2_Init(void);
    void SPI2_Set_Speed(uint8_t speed);
    uint8_t SPI2_RW_Byte(uint8_t txdata);

#endif
