#include "exti.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\exti.c
*              .\Drivers\BSP\inc\exti.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-06-16
* @brief     ：外部中断代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：EXTI4_Init
* @brief    ：外部中断初始化
* @param    ：void
* @retval   ：void
* @details  ：
*            KEY0--------PE4 低电平按下，高电平抬起
* @fn       ：
************************************************************/
void EXTI4_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;                       /* GPIO配置结构体 */

    KEY0_INT_GPIO_CLK_ENABLE();                              /* KEY0时钟使能 */
    
    gpio_init_struct.Pin  = KEY0_INT_GPIO_PIN;               /* KEY0引脚 */
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* 下升沿触发 */
    gpio_init_struct.Pull = GPIO_PULLUP;                     /* 上拉 */
    HAL_GPIO_Init(KEY0_INT_GPIO_PORT, &gpio_init_struct);    /* KEY0配置为下降沿触发中断 */

    HAL_NVIC_SetPriority(KEY0_INT_IRQn, 0, 0);               /* 抢占0，子优先级0 */
    HAL_NVIC_EnableIRQ(KEY0_INT_IRQn);                       /* 使能中断线4 */

    __HAL_GPIO_EXTI_CLEAR_IT(KEY0_INT_GPIO_PIN);             /* 清除外部中断4标志 */
}



/**********************************************************
* @funcName ：KEY0_INT_IRQHandler
* @brief    ：KEY0外部中断服务函数
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void KEY0_INT_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(KEY0_INT_GPIO_PIN);         /* 调用中断处理公用函数 清除KEY0所在中断线 的中断标志位 */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY0_INT_GPIO_PIN);         /* HAL库默认先清中断再处理回调，退出时再清一次中断，避免按键抖动误触发 */
}



/**********************************************************
* @funcName ：HAL_GPIO_EXTI_Callback
* @brief    ：外部中断回调函数
* @param    ：uint16_t GPIO_Pin
* @retval   ：void
* @details  ：
*            KEY0--------PE4 低电平按下，高电平抬起
* @fn       ：
************************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    delay_ms(20);      /* 消抖 */

    switch(GPIO_Pin)
    {
        case KEY0_INT_GPIO_PIN:
            if((GPIOE->IDR & (1<<4)) == 0) GPIOB->ODR ^= (1<<5);     /* LED0翻转 */
    }
}












