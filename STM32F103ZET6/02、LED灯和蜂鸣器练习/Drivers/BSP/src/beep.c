#include "beep.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\beep.c
*              .\Drivers\BSP\inc\beep.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-06-16
* @brief     ：BEEP蜂鸣器驱动配置代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：BEEP_Init
* @brief    ：对BEEP对应的GPIO口进行初始化设置
* @param    ：void
* @retval   ：void
* @details  ：
*            BEEP     PB8
*            高电平响，低电平不响-----输出模式
* @fn       ：
************************************************************/
void BEEP_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;                      /* GPIO配置结构体 */

    /* BEEP时钟使能 */
    BEEP_GPIO_CLK_ENABLE();

    /* BEEP引脚配置 */
    gpio_init_struct.Pin   = BEEP_GPIO_PIN;                 /* BEEP引脚 */
    gpio_init_struct.Mode  = GPIO_MODE_OUTPUT_PP;           /* 推挽输出 */
    gpio_init_struct.Pull  = GPIO_PULLUP;                   /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(BEEP_GPIO_PORT, &gpio_init_struct);       /* 初始化BEEP引脚 */

    BEEP(0);                                                /* 关闭 BEEP */
}



