#include "touch.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\touch.c
*              .\Drivers\BSP\inc\touch.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-03
* @brief     ：电阻触摸屏驱动配置和操作代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：Touch_IO_Init
* @brief    ：对电阻触摸屏芯片对应的GPIO口进行初始化设置
* @param    ：void
* @retval   ：void
* @details  ：
*            CS     PF11  推挽输出
*            SCK    PB1   推挽输出
*            PEN    PF10  通用输入
*            MISO   PB2   通用输入
*            MOSI   PF9   推挽输出
* @fn       ：
************************************************************/
void Touch_IO_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;    /* GPIO配置结构体 */

    /* 时钟使能 */
    T_PEN_GPIO_CLK_ENABLE();    /* T_PEN脚时钟使能 */
    T_CS_GPIO_CLK_ENABLE();     /* T_CS脚时钟使能 */
    T_MISO_GPIO_CLK_ENABLE();   /* T_MISO脚时钟使能 */
    T_MOSI_GPIO_CLK_ENABLE();   /* T_MOSI脚时钟使能 */
    T_SCK_GPIO_CLK_ENABLE();    /* T_SCK脚时钟使能 */

    /* GPIOx初始化设置 */
    /* GPIOx端口配置 */
    /* T_PEN */
    gpio_init_struct.Pin   = T_PEN_GPIO_PIN;                 /* PF10 端口配置 */
    gpio_init_struct.Mode  = GPIO_MODE_INPUT;                /* 输入 */
    gpio_init_struct.Pull  = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;           /* 高速 */
    HAL_GPIO_Init(T_PEN_GPIO_PORT, &gpio_init_struct);       /* 初始化T_PEN引脚 */
    /* T_MISO */
    gpio_init_struct.Pin = T_MISO_GPIO_PIN;                  /* PB2 */
    HAL_GPIO_Init(T_MISO_GPIO_PORT, &gpio_init_struct);      /* 初始化T_MISO引脚 */
    /* T_MOSI */
    gpio_init_struct.Pin = T_MOSI_GPIO_PIN;                  /* PF9 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;             /* 推挽输出 */
    HAL_GPIO_Init(T_MOSI_GPIO_PORT, &gpio_init_struct);      /* 初始化T_MOSI引脚 */
    /* SCK */
    gpio_init_struct.Pin = T_SCK_GPIO_PIN;                   /* PB1 */
    HAL_GPIO_Init(T_SCK_GPIO_PORT, &gpio_init_struct);       /* 初始化T_CLK引脚 */
    /* CS */
    gpio_init_struct.Pin = T_CS_GPIO_PIN;                    /* F11 */
    HAL_GPIO_Init(T_CS_GPIO_PORT, &gpio_init_struct);        /* 初始化T_CS引脚 */

    /* 初始化状态空闲 */
    T_CS_H;
    T_SCK_L;
    T_MOSI_L;
}



/**********************************************************
* @funcName ：Touch_Data
* @brief    ：对电阻触摸屏芯片对应的GPIO口进行初始化设置
* @param    ：uint8_t command (指令)
* @retval   ：uint16_t
* @details  ：
*            片选线        T_CS_H       T_CS_L  
*            时钟线        T_SCK_H      T_SCK_L  
*            输出数据线    T_MOSI_H     T_MOSI_L
*            输入数据线    T_MISO
*            笔接触线      T_PEN
* @fn       ：
************************************************************/
uint16_t Touch_Data(uint8_t command)
{
    uint8_t i;
    uint16_t data = 0;

    T_MOSI_L;            /* 数据线拉低 */
    T_CS_L;              /* 片选使能 */

    for(i=0;i<8;i++)
    {
        T_SCK_L;             /* 时钟线拉低 */

        /* 判断指令位电平 */
        if(command & 0x80)T_MOSI_H;
        else T_MOSI_L;

        command <<= 1;  /* 指令数据左移 */

        T_SCK_H;          /* 时钟线拉高 */
        delay_us(1);      /* 延时，用于从机读取数据 */
    }
    T_SCK_L;            /* 时钟线拉低 */
    T_MOSI_L;           /* 数据线拉低 */
    delay_us(6);        /* 延时6us，用于等待ADC把数据转换好 */

    /* 空出一个空脉冲 */
    T_SCK_L;
    delay_us(1);
    T_SCK_H;
    delay_us(1);

    for(i=0;i<12;i++)
    {
        data <<= 1;              /* 指令数据左移 */
        T_SCK_L;                 /* 时钟线拉低 */
        delay_us(1);             /* 延时，用于数据建立 */
        T_SCK_H;                 /* 时钟线拉高 */
        if(T_MISO) data |= 0x1;  /* 读取数据位电平 */
        delay_us(1);             /* 延时，用于数据建立 */
    }

    T_CS_H;    /* 片选拉高 */
    T_SCK_L;   /* 时钟线拉低 */

    return data;
}





