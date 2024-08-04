#include "key.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\key.c
*              .\Drivers\BSP\inc\key.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-06-16
* @brief     ：按键驱动配置代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：KEY_Init
* @brief    ：对KEY对应的GPIO口进行初始化设置
* @param    ：void
* @retval   ：void
* @details  ：
*            KEY0     PE4
*            KEY1     PE3
*            KEY_UP   PA0
*                                 -------------输出模式
* @fn       ：
************************************************************/
void KEY_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;                          /* GPIO配置结构体 */

    /* KEY0、KEY1和KEY_UP时钟使能 */
    KEY0_GPIO_CLK_ENABLE();
    KEY1_GPIO_CLK_ENABLE();
    KEYUP_GPIO_CLK_ENABLE();

    /* KEY0引脚配置 */
    gpio_init_struct.Pin   = KEY0_GPIO_PIN;                     /* KEY0引脚 */
    gpio_init_struct.Mode  = GPIO_MODE_INPUT;                   /* 输入 */
    gpio_init_struct.Pull  = GPIO_PULLUP;                       /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* 高速 */
    HAL_GPIO_Init(KEY0_GPIO_PORT, &gpio_init_struct);           /* KEY0引脚模式设置,上拉输入 */

    /* KEY1引脚配置 */
    gpio_init_struct.Pin = KEY1_GPIO_PIN;                       /* KEY1引脚 */
    HAL_GPIO_Init(KEY1_GPIO_PORT, &gpio_init_struct);           /* KEY1引脚模式设置,上拉输入 */

    /* KEY_UP引脚配置 */
    gpio_init_struct.Pin = KEYUP_GPIO_PIN;                      /* KEY_UP引脚 */
    gpio_init_struct.Pull = GPIO_PULLDOWN;                      /* 下拉 */
    HAL_GPIO_Init(KEYUP_GPIO_PORT, &gpio_init_struct);          /* KEY_UP引脚模式设置,下拉输入 */
}



/**********************************************************
* @funcName ：KEY_Scan
* @brief    ：按键扫描函数
* @param    ：void
* @retval   ：uint8_t
* @details  ：
*            KEY0、KEY1 低电平按下，高电平抬起
*            KEY_UP     高电平按下，低电平抬起
* @fn       ：
************************************************************/
uint8_t KEY_Scan(void)
{
    uint8_t temp = 0;
    static uint8_t key_flay = 1;

    if(key_flay && ( KEY_UP==1 || KEY0==0 || KEY1==0 ))
    {
        delay_ms(10);
        if( KEY_UP==1 || KEY0==0 || KEY1==0 )
        {
            key_flay = 0;
            if(KEY0==0)        temp = 1;
            else if(KEY1==0)   temp = 2;
            else if(KEY_UP==1) temp = 3;
        }
    }

    if(KEY_UP==0 && KEY0==1 && KEY1==1)
    {
        key_flay = 1;
    }

    return temp;
}








