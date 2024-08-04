#include "iwdg.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\iwdg.c
*              .\Drivers\BSP\inc\iwdg.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-08-05
* @brief     ：独立看门狗练习代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



IWDG_HandleTypeDef g_iwdg_handle;  /* 独立看门狗句柄 */

/**********************************************************
* @funcName ：IWDG_Init
* @brief    ：对独立看门狗进行配置
* @param    ：uint8_t prer (预分频，分频数:0~7(只有低3位有效!))
*   @arg    ：分频因子 = 4 * 2^prer. 但最大值只能是256!
* @param    ：uint16_t rlr (自动重装载值,0~0XFFF)
* @retval   ：void
* @details  ：
* @fn       ：
*           时间计算(大概):Tout=((4 * 2^prer) * rlr) / 40 (ms).
************************************************************/
void IWDG_Init(uint8_t prer, uint16_t rlr)
{
    g_iwdg_handle.Instance       = IWDG; /* 独立看门狗 */
    g_iwdg_handle.Init.Prescaler = prer; /* 设置分频系数 (IWDG_PR寄存器) */
    g_iwdg_handle.Init.Reload    = rlr;  /* 设置IWDG装载值(IWDG->RLR寄存器) */
    HAL_IWDG_Init(&g_iwdg_handle);       /* 初始化IWDG并启动 */
}




/**********************************************************
* @funcName ：IWDG_Feed
* @brief    ：喂独立看门狗
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void IWDG_Feed(void)
{
    HAL_IWDG_Refresh(&g_iwdg_handle);  /* 重装载计数器 */
}






