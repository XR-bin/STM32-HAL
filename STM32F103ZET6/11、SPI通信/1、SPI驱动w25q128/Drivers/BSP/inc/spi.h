#ifndef __SPI_H
#define __SPI_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"



    /********************   宏定义   ********************/
    /* SPI2 引脚定义 */
    /* SCK   时钟线 */
    #define SPI2_SCK_GPIO_PORT              GPIOB
    #define SPI2_SCK_GPIO_PIN               GPIO_PIN_13
    #define SPI2_SCK_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()   /* PB口时钟使能 */
    /* MISO 主输入 */
    #define SPI2_MISO_GPIO_PORT             GPIOB
    #define SPI2_MISO_GPIO_PIN              GPIO_PIN_14
    #define SPI2_MISO_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()   /* PB口时钟使能 */
    /* MOSI 主输出 */
    #define SPI2_MOSI_GPIO_PORT             GPIOB
    #define SPI2_MOSI_GPIO_PIN              GPIO_PIN_15
    #define SPI2_MOSI_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()   /* PB口时钟使能 */

    /* SPI2相关定义 */
    #define SPI2_SPI                        SPI2
    #define SPI2_SPI_CLK_ENABLE()           __HAL_RCC_SPI2_CLK_ENABLE()    /* SPI2时钟使能 */



    /****************    函数外部声明   *****************/
    void SPI2_Init(void);                 /* SPI2配置初始化 */
    uint8_t SPI2_RS_Byte(uint8_t data);   /* SPI2收发函数 */

#endif






