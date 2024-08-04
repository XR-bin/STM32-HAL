#include "spi.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\spi.c
*              .\Drivers\BSP\inc\spi.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-01
* @brief     ：SPI配置代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



SPI_HandleTypeDef g_spi2_handler; /* SPI2句柄 */

/**********************************************************
* @funcName ：SPI2_Init
* @brief    ：对SPI2通信要用的GPIO口进行初始化和对SPI2的寄存器进行设置
* @param    ：void
* @retval   ：void
* @details  ：
*            PB13   SPI2_SCK   时钟线  复用推挽
*            PB14   SPI2_MISO  主输入  复用推挽
*            PB15   SPI2_MOSI  主输出  复用推挽
*            极性：SCK为低电平为空闲状态
*            相位：数据采样从第一个时钟边沿开始
* @fn       ：
************************************************************/
void SPI2_Init(void)
{
    /* 时钟使能   SPI2 */
    SPI2_SPI_CLK_ENABLE(); /* SPI2时钟使能 */

    g_spi2_handler.Instance               = SPI2_SPI;                   /* SPI2 */
    g_spi2_handler.Init.Mode              = SPI_MODE_MASTER;            /* 设置为主SPI(SPI_CR1寄存器的第2、8位) */
    g_spi2_handler.Init.Direction         = SPI_DIRECTION_2LINES;       /* SPI设置为双线双向全双工,且NSS上的电平为高电平(SPI_CR1寄存器的第10、15位) */
    g_spi2_handler.Init.DataSize          = SPI_DATASIZE_8BIT;          /* 使用8位数据帧格式进行发送/接收(SPI_CR1寄存器的第11位) */
    g_spi2_handler.Init.CLKPolarity       = SPI_POLARITY_LOW;           /* 空闲状态时， SCK保持低电平(SPI_CR1寄存器的第1位) */
    g_spi2_handler.Init.CLKPhase          = SPI_PHASE_1EDGE;            /* 数据采样从第一个时钟边沿开始(SPI_CR1寄存器的第0位) */
    g_spi2_handler.Init.NSS               = SPI_NSS_SOFT;               /* 启用软件从设备管理，NSS引脚上的电平由SSI位的值决定(SPI_CR1寄存器的第9位) */
    g_spi2_handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;    /* 波特率预分频值为2  波特率：fPCLK/2  ==  36M/2  ==  18M (SPI_CR1寄存器的第3-5位) */
    g_spi2_handler.Init.FirstBit          = SPI_FIRSTBIT_MSB;           /* 数据传输从MSB位开始(对齐标准:左对齐)(SPI_CR1寄存器的第7位) */
    g_spi2_handler.Init.TIMode            = SPI_TIMODE_DISABLE;         /* 关闭TI模式 */
    g_spi2_handler.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE; /* 关闭硬件CRC校验 */
    g_spi2_handler.Init.CRCPolynomial     = 7;                          /* CRC值计算的多项式:0x7是复位值(SPI_CRCPR寄存器) */
    HAL_SPI_Init(&g_spi2_handler);                                      /* 初始化 */

    __HAL_SPI_ENABLE(&g_spi2_handler);                                  /* 使能SPI2 */
}



/**********************************************************
* @funcName ：HAL_SPI_MspInit
* @brief    ：SPI底层初始化函数(底层初始化回调钩子函数)
* @param    ：SPI_HandleTypeDef *hspi (SPI句柄类型指针)
* @retval   ：void
* @details  ：
* @fn       ：
*        这个函数会在HAL_SPI_Init()函数中被调用，是SPI底层初
*    始化回调钩子函数。
************************************************************/
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef gpio_init_struct;                          /* GPIO配置结构体 */

    if (hspi->Instance == SPI2_SPI)
    {
        SPI2_SCK_GPIO_CLK_ENABLE();                             /* SPI2_SCK脚时钟使能 */
        SPI2_MISO_GPIO_CLK_ENABLE();                            /* SPI2_MISO脚时钟使能 */
        SPI2_MOSI_GPIO_CLK_ENABLE();                            /* SPI2_MOSI脚时钟使能 */

        /* SCK引脚模式设置(复用输出) */
        gpio_init_struct.Pin   = SPI2_SCK_GPIO_PIN;             /* SCK引脚 */
        gpio_init_struct.Mode  = GPIO_MODE_AF_PP;               /* 复用推挽输 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* IO口速度为50MHz */
        HAL_GPIO_Init(SPI2_SCK_GPIO_PORT, &gpio_init_struct);   /* 初始化 */

        /* MISO引脚模式设置(复用输出) */
        gpio_init_struct.Pin = SPI2_MISO_GPIO_PIN;              /* MISO引脚 */
        HAL_GPIO_Init(SPI2_MISO_GPIO_PORT, &gpio_init_struct);  /* 初始化 */

        /* MOSI引脚模式设置(复用输出) */
        gpio_init_struct.Pin = SPI2_MOSI_GPIO_PIN;              /* MOSI引脚 */
        HAL_GPIO_Init(SPI2_MOSI_GPIO_PORT, &gpio_init_struct);  /* 初始化 */
    }
}



/**********************************************************
* @funcName ：SPI2_RS_Byte
* @brief    ：SPI2接收/发送一个字节数据(8位)
* @param    ：uint8_t data
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
uint8_t SPI2_RS_Byte(uint8_t data)
{
    /* 判断发送缓存区是否为空 */
    while(!(SPI2->SR & (1<<1)));
    SPI2->DR = data;
    /* 判断接收缓存区是否为空 */
    while(!(SPI2->SR & (1<<0)));
    data = SPI2->DR;

    return data;
}






