#include "led.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\led.c
*              .\Drivers\BSP\inc\led.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-06-16
* @brief     ：LED灯驱动配置代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：LED_Init
* @brief    ：对LED对应的GPIO口进行初始化设置
* @param    ：void
* @retval   ：void
* @details  ：
*            LED0     PB5
*            LED1     PE5
*            高电平灭，低电平亮-----输出模式
* @fn       ：
************************************************************/
void LED_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;                      /* GPIO配置结构体 */

    /* LED0和LED1时钟使能 */
    LED0_GPIO_CLK_ENABLE();
    LED1_GPIO_CLK_ENABLE();

    /* LED0引脚配置 */
    gpio_init_struct.Pin   = LED0_GPIO_PIN;                 /* LED0引脚 */
    gpio_init_struct.Mode  = GPIO_MODE_OUTPUT_PP;           /* 推挽输出 */
    gpio_init_struct.Pull  = GPIO_PULLUP;                   /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(LED0_GPIO_PORT, &gpio_init_struct);       /* 初始化LED0引脚 */

    /* LED1引脚配置 */
    gpio_init_struct.Pin = LED1_GPIO_PIN;                   /* LED1引脚 */
    HAL_GPIO_Init(LED1_GPIO_PORT, &gpio_init_struct);       /* 初始化LED1引脚 */

    LED0(1);                                                /* 关闭 LED0 */
    LED1(1);                                                /* 关闭 LED1 */
}
