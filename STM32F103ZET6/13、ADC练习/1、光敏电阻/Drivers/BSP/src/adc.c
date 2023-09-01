#include "adc.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\adc.c
*              .\Drivers\BSP\inc\adc.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-27
* @brief     ：ADC配置代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/***************************************
* F407的ADC时钟频率官方建议: 不超过36Mhz
* F103的ADC时钟频率官方建议: 不超过14Mhz
****************************************/



ADC_HandleTypeDef g_adc3_handle;         /* ADC句柄 */

/**********************************************************
* @funcName ：ADC3_In6_Init
* @brief    ：ADC3的第6个通道初始化配置
* @param    ：void
* @retval   ：void
* @details  ：
*             PF8----------ADC3_IN6   模拟模式
* @fn       ：
************************************************************/
void ADC3_In6_Init(void)
{
    RCC_PeriphCLKInitTypeDef adc_clk_init = {0};                /* ADCx一般配置结构体 */
    ADC_ChannelConfTypeDef adc_ch_conf;                         /* ADCx通道配置结构体 */
    ADC_MultiModeTypeDef multimode;                             /* 多模式结构体 */

    /* 时钟使能  ADC3 */
    ADC3_CHY_CLK_ENABLE();                                     /* ADC时钟使能 */

    adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC;     /* ADC外设时钟 */
    adc_clk_init.AdcClockSelection    = RCC_ADCPCLK2_DIV6;     /* 分频因子6时钟为72M/6=12MHz */
    HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);                  /* 设置ADC时钟 */

    g_adc3_handle.Instance                   = ADC_ADCX;               /* 选择哪个ADC */
    g_adc3_handle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;    /* 数据对齐：右对齐(ADC_CR2寄存器的第11位) */
    g_adc3_handle.Init.ScanConvMode          = ADC_SCAN_DISABLE;       /* 非扫描(ADC_CR1寄存器的第8位) */
    g_adc3_handle.Init.ContinuousConvMode    = DISABLE;                /* 单次转换模式(ADC_CR2寄存器的第1位) */
    g_adc3_handle.Init.NbrOfConversion       = 1;                      /* 有几个转换(ADC_SQR1寄存器的第20-23位) */
    g_adc3_handle.Init.DiscontinuousConvMode = DISABLE;                /* 禁止规则通道组间断模式 */
    g_adc3_handle.Init.NbrOfDiscConversion   = 0;                      /* 配置间断模式的规则通道个数，禁止规则通道组间断模式后，此参数忽略 */
    g_adc3_handle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;     /* 触发方式：软件触发(ADC_CR2寄存器的第17-19位) */
    HAL_ADC_Init(&g_adc3_handle);                                      /* 初始化 */

    multimode.Mode = ADC_MODE_INDEPENDENT;                             /* 独立模式(ADC_CR1寄存器的第16-19位) */
    HAL_ADCEx_MultiModeConfigChannel(&g_adc3_handle, &multimode);      /* 配置模式 */

    adc_ch_conf.Channel      = ADC3_CHY;                               /* 通道 */
    adc_ch_conf.Rank         = 1;                                      /* 序列 */
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;             /* 采样时间 */
    HAL_ADC_ConfigChannel(&g_adc3_handle, &adc_ch_conf);               /* 通道配置 */

    HAL_ADCEx_Calibration_Start(&g_adc3_handle);                       /* 校准ADC */
}



/**********************************************************
* @funcName ：HAL_ADC_MspInit
* @brief    ：ADC底层初始化函数(底层初始化回调钩子函数)
* @param    ：ADC_HandleTypeDef *hadc (ADC句柄类型指针)
* @retval   ：void
* @details  ：
* @fn       ：
*        这个函数会在HAL_ADC_Init()函数中被调用，是ADC底层初
*    始化回调钩子函数。
************************************************************/
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    GPIO_InitTypeDef gpio_init_struct;                          /* GPIO配置结构体 */

    if(hadc->Instance == ADC_ADCX)
    {
        /* 时钟使能  GPIOF */
        ADC3_CHY_GPIO_CLK_ENABLE();                             /* IO口时钟使能 */

        /* GPIOx初始化设置 */
        /* GPIOx端口配置 */
        gpio_init_struct.Pin   = ADC3_CHY_GPIO_PIN;                /* ADC通道对应的IO引脚 */
        gpio_init_struct.Mode  = GPIO_MODE_ANALOG;                 /* 模拟输入引脚 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;             /* IO口速度为50MHz */
        HAL_GPIO_Init(ADC3_CHY_GPIO_PORT, &gpio_init_struct);      /* 根据设定参数初始化 */
    }
}



/**********************************************************
* @funcName ：ADC3_In6_Data
* @brief    ：获取ADC3通道6的转换数据
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
*            用于转换光敏电阻数据
************************************************************/
uint16_t ADC3_In6_Data(void)
{
    uint16_t data;

    HAL_ADC_Start(&g_adc3_handle);                            /* 开启ADC */
    HAL_ADC_PollForConversion(&g_adc3_handle, 10);            /* 轮询转换 */
    data = (uint16_t)HAL_ADC_GetValue(&g_adc3_handle);        /* 返回最近一次ADC1规则组的转换结果 */

    return data;
}



/**********************************************************
* @funcName ：Oversampling
* @brief    ：ADC过采样算法，提高分辨率，由12分辨率变为16分辨率
* @param    ：void
* @retval   ：uint32_t --- ADC的16分辨率数据
* @details  ：
* @fn       ：
*            用于转换光敏电阻数据
************************************************************/
uint16_t Oversampling(void)
{
    int i;
    uint32_t sum=0;

    for(i=0; i<256; i++)
    {
        sum+=ADC3_In6_Data();
    }

    return (sum>>4);
}



