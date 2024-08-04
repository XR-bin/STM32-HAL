#include "pwm.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\pwm.c
*              .\Drivers\BSP\inc\pwm.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-27
* @brief     ：pwm练习代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



TIM_HandleTypeDef g_timx_pwm_chy_handle;     /* 定时器x句柄 */

/**********************************************************
* @funcName ：TIM3_PWM_Init
* @brief    ：对通用定时器3进行初始化设置并输出PWM
* @param    ：uint16_t arr (重载值)
* @param    ：uint16_t psc (预分频值)
* @param    ：uint16_t ccr (比较值/占空比)
* @retval   ：void
* @details  ：
*            PWM输出口  PB5   TIM3_CH2  输出通道2
* @fn       ：
*            有效电平为高电平
*            定时器向上计数
*            先输出有效电平后输出无效电平
************************************************************/
void TIM3_PWM_Init(uint16_t arr, uint16_t psc, uint16_t ccr)
{
    TIM_OC_InitTypeDef timx_oc_pwm_chy  = {0};                          /* 定时器PWM输出配置 */

    /* 时钟使能  GPIOB  TIM3 */
    __HAL_RCC_TIM3_CLK_ENABLE();

    /* GPIOx配置 */
    g_timx_pwm_chy_handle.Instance           = TIM3;                      /* 定时器x */
    g_timx_pwm_chy_handle.Init.Period        = arr;                       /* 重载值(TIMx_ARR寄存器) */
    g_timx_pwm_chy_handle.Init.Prescaler     = psc;                       /* 分频(TIMx_PSC寄存器) */
    g_timx_pwm_chy_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;    /* TDTS = Tck_tim(TIMx_CR1寄存器的第9-8位)(基本定时器没有) */
    g_timx_pwm_chy_handle.Init.CounterMode   = TIM_COUNTERMODE_UP;        /* 向上计数(TIMx_CR1寄存器的第4位)(基本定时器只递增) */
    HAL_TIM_PWM_Init(&g_timx_pwm_chy_handle);                             /* 初始化 */

    /* PWM 通道初始化配置 */
    /* 通道2 输出配置 */
    timx_oc_pwm_chy.OCMode     = TIM_OCMODE_PWM1;                          /* 配置为PWM模式1(TIMx_CCMR1的OC1M位) */
    timx_oc_pwm_chy.Pulse      = ccr;                                      /* 设置比较寄存器(TIMx->CCR2) */
    timx_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                       /* 设置有效电平为低电平(TIMx_CCER的CC1P位) */
    HAL_TIM_PWM_ConfigChannel(&g_timx_pwm_chy_handle, &timx_oc_pwm_chy, TIM_CHANNEL_2); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_timx_pwm_chy_handle, TIM_CHANNEL_2);              /* 开启对应PWM通道 */
}



/**********************************************************
* @funcName ：HAL_TIM_PWM_MspInit
* @brief    ：PWM底层初始化函数(底层初始化回调钩子函数)
* @param    ：TIM_HandleTypeDef *htim (定时器句柄)
* @retval   ：void
* @details  ：
* @fn       ：
*        这个函数会在HAL_TIM_PWM_Init()函数中被调用，是PWM底层初
*    始化回调钩子函数。
************************************************************/
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef gpio_init_struct;                          /* GPIO配置结构体 */

    if (htim->Instance == TIM3)
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();                      /* 开启通道y的CPIO时钟 */
        __HAL_RCC_AFIO_CLK_ENABLE();                       /* AFIO时钟使能 */
        __HAL_AFIO_REMAP_TIM3_PARTIAL();                   /* 部分映像,使PB5变成PWM通道2 */

        gpio_init_struct.Pin   = GPIO_PIN_5;               /* 通道y的CPIO口 */
        gpio_init_struct.Mode  = GPIO_MODE_AF_PP;          /* 复用推完输出 */
        gpio_init_struct.Pull  = GPIO_PULLUP;              /* 上拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;     /* 高速 */
        HAL_GPIO_Init(GPIOB, &gpio_init_struct);           /* 初始化 */
    }
}

