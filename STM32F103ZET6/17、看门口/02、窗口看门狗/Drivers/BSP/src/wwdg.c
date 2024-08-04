#include "wwdg.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\wwdg.c
*              .\Drivers\BSP\inc\wwdg.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-08-05
* @brief     ：窗口看门狗练习代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



WWDG_HandleTypeDef g_wwdg_handle;  /* WWDG句柄 */

/**********************************************************
* @funcName ：WWDG_Init
* @brief    ：初始化窗口看门狗
* @param    ：uint8_t tr (计数器值)
* @param    ：uint8_t wr (窗口值)
* @param    ：uint8_t fprer (分频系数（WDGTB）,范围:0~7,表示2^WDGTB分频)
* @retval   ：void
* @details  ：
* @fn       ：
*           Fwwdg=PCLK1/(4096*2^fprer). 一般PCLK1=36Mhz
*           溢出时间=(4096*2^fprer)*(tr-0X3F)/PCLK3
*           假设fprer=4,tr=7f,PCLK1=36Mhz
*           则溢出时间=4096*16*64/36Mhz=116.50ms
************************************************************/
void WWDG_Init(uint8_t tr, uint8_t wr, uint8_t fprer)
{
    __HAL_RCC_WWDG_CLK_ENABLE();                    /* 使能窗口看门狗时钟 */

    g_wwdg_handle.Instance       = WWDG;            /* 窗口看门狗 */
    g_wwdg_handle.Init.Prescaler = fprer;           /* 设置分频系数 */
    g_wwdg_handle.Init.Window    = wr;              /* 设置窗口值 */
    g_wwdg_handle.Init.Counter   = tr;              /* 设置计数器值 */
    g_wwdg_handle.Init.EWIMode   = WWDG_EWI_ENABLE; /* 使能窗口看门狗提前唤醒中断 */

    HAL_NVIC_SetPriority(WWDG_IRQn, 1, 1);          /* 抢占优先级1，子优先级为1 */
    HAL_NVIC_EnableIRQ(WWDG_IRQn);                  /* 使能窗口看门狗中断 */

    HAL_WWDG_Init(&g_wwdg_handle);                  /* 初始化WWDG */
}



/**********************************************************
* @funcName ：WWDG_IRQHandler
* @brief    ：窗口看门狗中断服务程序
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void WWDG_IRQHandler(void)
{
    HAL_WWDG_IRQHandler(&g_wwdg_handle);  /* 调用WWDG共用中断处理函数 */
}



/**********************************************************
* @funcName ：HAL_WWDG_EarlyWakeupCallback
* @brief    ：窗口看门狗中断回调函数
* @param    ：WWDG_HandleTypeDef *hwwdg (窗口看门狗句柄)
* @retval   ：void
* @details  ：
* @fn       ：
*            当触发窗口看门狗中断后会调用该回调函数
************************************************************/
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg)
{
    HAL_WWDG_Refresh(&g_wwdg_handle); /* 更新窗口看门狗值 */
    GPIOE->ODR ^= (1<<5);             /* LED1绿灯闪烁 */
}





