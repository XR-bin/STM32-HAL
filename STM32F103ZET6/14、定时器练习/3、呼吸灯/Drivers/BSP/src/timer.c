#include "timer.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\timer.c
*              .\Drivers\BSP\inc\timer.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-27
* @brief     ：基本定时器中断练习代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



TIM_HandleTypeDef g_timx_handle;  /* 定时器句柄 */

/**********************************************************
* @funcName ：TIM6_Init
* @brief    ：对定时器6进行初始化设置
* @param    ：uint16_t arr (重载值)
* @param    ：uint16_t psc (预分频值)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void TIM6_Init(uint16_t arr, uint16_t psc)
{
    /* TIMx时钟使能 */
    __HAL_RCC_TIM6_CLK_ENABLE();

    /* 定时器配置 */
    g_timx_handle.Instance         = TIM6;                      /* 通用定时器X */
    g_timx_handle.Init.Prescaler   = psc;                       /* 分频(TIMx_PSC寄存器) */
    g_timx_handle.Init.Period      = arr;                       /* 重载值(TIMx_ARR寄存器) */
    g_timx_handle.Init.CounterMode = TIM_COUNTERMODE_UP;        /* 向上计数(TIMx_CR1寄存器的第4位)(基本定时器只递增) */
    HAL_TIM_Base_Init(&g_timx_handle);                          /* 初始化TIM6 */

    /* 中断配置 */
    __HAL_TIM_ENABLE_IT(&g_timx_handle, TIM_IT_UPDATE);         /* 允许TIMx更新中断 */
    HAL_NVIC_SetPriority(TIM6_IRQn, 1, 1);                      /* 抢占1，子优先级1，组2 */
    HAL_NVIC_EnableIRQ(TIM6_IRQn);                              /* 开启ITM6中断 */

    HAL_TIM_Base_Start_IT(&g_timx_handle);                      /* 使能定时器x及其更新中断 */
}



/**********************************************************
* @funcName ：TIM6_IRQHandler
* @brief    ：定时器6的中断服务函数
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void TIM6_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_handle); /* 定时器中断公共处理函数 */
}



/**********************************************************
* @funcName ：HAL_TIM_PeriodElapsedCallback
* @brief    ：TIME定时器更新中断回调函数
* @param    ：UART_HandleTypeDef *huart (UART句柄类型指针)
* @retval   ：void
* @details  ：
* @fn       ：
*            当触发定时器更新中断后会调用该回调函数
************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static uint8_t temp = 0;

    if (htim->Instance == TIM6)
    {
        /* 紧急事件 */
        if(temp == 0)
        {
            if(TIM3->CCR2 == 0) temp = 1;
            else TIM3->CCR2 -=20;
        }
        if(temp == 1)
        {
            if(TIM3->CCR2 == 1000) temp = 0;
            else TIM3->CCR2 +=20;
        }
        __HAL_TIM_CLEAR_FLAG(&g_timx_handle, TIM_FLAG_UPDATE);
    }
}


