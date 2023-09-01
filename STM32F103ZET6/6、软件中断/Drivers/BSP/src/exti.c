#include "exti.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\exti.c
*              .\Drivers\BSP\inc\exti.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-06-16
* @brief     ：软件中断代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：EXTI5_Swier_Init
* @brief    ：软件外部中断初始化
* @param    ：void
* @retval   ：void
* @details  ：
*            无需端口
* @fn       ：
************************************************************/
void EXTI5_Swier_Init(void)
{
    /* 外部中断配置 */
    /* 软件中断事件寄存器 */
    EXTI->SWIER &= ~(1<<5);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);               /* 抢占0，子优先级0 */
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);                       /* 使能软件中断5 */

    /* 中断屏蔽寄存器 */
    EXTI->IMR |= (1<<5);
}



/**********************************************************
* @funcName ：EXTI9_5_IRQHandler
* @brief    ：外部中断服务函数
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void EXTI9_5_IRQHandler(void)
{
    if(EXTI->PR & (1<<5))
    {
        GPIOB->ODR ^= (1<<5);            /* LED0翻转 */
        EXTI->PR |= (1<<5);              /* 写1清除中断标志 */
    }
}







