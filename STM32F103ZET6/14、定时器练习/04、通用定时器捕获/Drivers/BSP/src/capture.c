#include "capture.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\capture.c
*              .\Drivers\BSP\inc\capture.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-27
* @brief     ：捕获练习代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



TIM_HandleTypeDef g_timx_cap_chy_handle;      /* 定时器x句柄 */

/**********************************************************
* @funcName ：TIM4_Capture_Init
* @brief    ：对通用定时器4进行初始化设置并捕获脉冲信号
* @param    ：uint16_t arr (重载值)
* @param    ：uint16_t psc (预分频值)
* @retval   ：void
* @details  ：
*            脉冲输入口  PB6   TIM4_CH1  输入通道1  映射通道IC1、IC2
* @fn       ：
************************************************************/
void TIM4_Capture_Init(uint16_t arr, uint16_t psc)
{
    TIM_IC_InitTypeDef timx_ic_cap_chy = {0};                             /* 捕获配置结构体 */
    TIM_SlaveConfigTypeDef sSlaveConfig = {0};

    /*TIMx配置*/
    __HAL_RCC_TIM4_CLK_ENABLE();      /* 开启TIM4时钟 */

    /* TIMx初始化设置 */
    g_timx_cap_chy_handle.Instance           = TIM4;                      /* 定时器4 */
    g_timx_cap_chy_handle.Init.Period        = arr;                       /* 重载值(TIMx_ARR寄存器) */
    g_timx_cap_chy_handle.Init.Prescaler     = psc;                       /* 分频(TIMx_PSC寄存器) */
    g_timx_cap_chy_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;    /* TDTS = Tck_tim(TIMx_CR1寄存器的第9-8位)(基本定时器没有) */
    g_timx_cap_chy_handle.Init.CounterMode   = TIM_COUNTERMODE_UP;        /* 向上计数(TIMx_CR1寄存器的第4位)(基本定时器只递增) */
    HAL_TIM_IC_Init(&g_timx_cap_chy_handle);                              /* 初始化 */

    /* 从模式控制寄存器 */
    sSlaveConfig.SlaveMode    = TIM_SLAVEMODE_RESET;   /* 复用模式 */
    sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;         /* 滤波后的定时器输入 1 (TI1FP1) */
    HAL_TIM_SlaveConfigSynchro_IT(&g_timx_cap_chy_handle, &sSlaveConfig);

    /*********ICC1通道**********/
    /* 捕获通道配置 */
    /* 通道1映射到T1 */
    timx_ic_cap_chy.ICPolarity  = TIM_ICPOLARITY_RISING;                /* 上升沿捕获(TIMx->CCER寄存器的第1位) */
    timx_ic_cap_chy.ICSelection = TIM_ICSELECTION_DIRECTTI;             /* 映射到TI1上(TIMx->CCMR1寄存器的输入模式的第0-1位) */
    timx_ic_cap_chy.ICPrescaler = TIM_ICPSC_DIV1;                       /* 配置输入分频：不分频(TIMx->CCMR1寄存器的输入模式的第2-3位) */
    timx_ic_cap_chy.ICFilter = 0;                                       /* 配置输入滤波器：不滤波(TIMx->CCMR1寄存器的输入模式的第4-7位) */
    HAL_TIM_IC_ConfigChannel(&g_timx_cap_chy_handle, &timx_ic_cap_chy, TIM_CHANNEL_1);  /* 捕获通道1配置初始化 */
    TIM_CCxChannelCmd(TIM4, TIM_CHANNEL_1, ENABLE);      /* 使能捕获 */


    /*********ICC2通道**********/
    /* 通道1映射到T2 */
    /* 因为配置一样，所以一些变量无需改变 */
    timx_ic_cap_chy.ICPolarity  = TIM_ICPOLARITY_FALLING;               /* 下降沿捕获(TIMx->CCER寄存器的第1位) */
    timx_ic_cap_chy.ICSelection = TIM_ICSELECTION_INDIRECTTI;           /* 映射到通道1上(TI1)上(这个必须结合手册才能理解)(TIMx->CCMR1寄存器的输入模式的第0-1位) */
    HAL_TIM_IC_ConfigChannel(&g_timx_cap_chy_handle, &timx_ic_cap_chy, TIM_CHANNEL_2);  
    TIM_CCxChannelCmd(TIM4, TIM_CHANNEL_2, ENABLE);        /* 使能捕获 */

    /* 中断配置 */
    __HAL_TIM_ENABLE_IT(&g_timx_cap_chy_handle, TIM_IT_CC1);         /* 允许CC1IE捕获中断 */
    __HAL_TIM_ENABLE_IT(&g_timx_cap_chy_handle, TIM_IT_CC2);         /* 允许CC2IE捕获中断 */

    HAL_NVIC_SetPriority(TIM4_IRQn, 1, 1);                           /* 设置中断优先级，抢占优先级1，子优先级3 */
    HAL_NVIC_EnableIRQ(TIM4_IRQn);

    /* TIMx使能 */
    HAL_TIM_IC_Start_IT(&g_timx_cap_chy_handle, TIM_CHANNEL_1);     /* 开始捕获TIM5 */
}



/**********************************************************
* @funcName ：HAL_TIM_IC_MspInit
* @brief    ：TIM底层初始化函数(底层初始化回调钩子函数)
* @param    ：TIM_HandleTypeDef *htim (定时器句柄)
* @retval   ：void
* @details  ：
* @fn       ：
*        这个函数会在HAL_TIM_IC_Init()函数中被调用，是TIM底层初
*    始化回调钩子函数。
************************************************************/
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef gpio_init_struct;                          /* GPIO配置结构体 */

    if (htim->Instance == TIM4)
    {
        /* 时钟使能   GPIOB AFIO */
        __HAL_RCC_GPIOB_CLK_ENABLE();                       /* GPIOB时钟使能 */

        /* GPIOx配置部分 */
        gpio_init_struct.Pin = GPIO_PIN_6;                  /* 输入捕获的GPIO口 */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;            /* 复用推挽输出 */
        gpio_init_struct.Pull = GPIO_PULLDOWN;              /* 下拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;      /* 高速 */
        HAL_GPIO_Init(GPIOB, &gpio_init_struct);
    }
}



/**********************************************************
* @funcName ：TIM4_IRQHandler
* @brief    ：定时器4中断函数
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void TIM4_IRQHandler(void)
{
    static uint16_t data_2;
    uint16_t data_1;
    uint16_t p_val;

    if(__HAL_TIM_GET_FLAG(&g_timx_cap_chy_handle, TIM_FLAG_CC2) ==  SET)
    {
        /* 清除捕通道2的获标志位 */
        __HAL_TIM_CLEAR_FLAG(&g_timx_cap_chy_handle,TIM_FLAG_CC2);
        /* 紧急事件 */
        /* CCR2 */
        data_2 = HAL_TIM_ReadCapturedValue(&g_timx_cap_chy_handle, TIM_CHANNEL_2);
    }

    if(__HAL_TIM_GET_FLAG(&g_timx_cap_chy_handle, TIM_FLAG_CC1OF) ==  SET)
    {
        /* 清除通道1重复捕获标志位 */
        __HAL_TIM_CLEAR_FLAG(&g_timx_cap_chy_handle,TIM_FLAG_CC1OF);
        /* 清除通道1的捕获标志位 */
        __HAL_TIM_CLEAR_FLAG(&g_timx_cap_chy_handle,TIM_FLAG_CC1);
        /* 紧急事件 */
        /* 记录值CCR1 */
        data_1 = HAL_TIM_ReadCapturedValue(&g_timx_cap_chy_handle, TIM_CHANNEL_1);
        /* 计算周期和占空比 */
        p_val = (float)data_2/data_1*100;

        printf("周期：%d   高电平：%d   占空比：%d%%\r\n",data_1,data_2,p_val);
    }
}

