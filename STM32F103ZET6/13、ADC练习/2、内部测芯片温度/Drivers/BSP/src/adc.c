#include "adc.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\adc.c
*              .\Drivers\BSP\inc\adc.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-27
* @brief     ：ADC测芯片内部温度代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/*****************************************
* F407的ADC时钟频率官方建议: 不超过36Mhz
* F103的ADC时钟频率官方建议: 不超过14Mhz
******************************************/



ADC_HandleTypeDef g_adc_handle;   /* ADC句柄 */

/**********************************************************
* @funcName ：ADC1_In16_Init
* @brief    ：ADC1的第16个通道初始化配置
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
*            无需引脚，用于测芯片温度
************************************************************/
void ADC1_In16_Init(void)
{
    RCC_PeriphCLKInitTypeDef adc_clk_init = {0};                /* ADCx一般配置结构体 */
    ADC_ChannelConfTypeDef adc_ch_conf;                         /* ADCx通道配置结构体 */
    ADC_MultiModeTypeDef multimode;                             /* 多模式结构体 */

    /* 时钟使能  ADC1 */
    __HAL_RCC_ADC1_CLK_ENABLE();

    adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC;     /* ADC外设时钟 */
    adc_clk_init.AdcClockSelection    = RCC_ADCPCLK2_DIV6;     /* 分频因子6时钟为72M/6=12MHz */
    HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);                  /* 设置ADC时钟 */

    /* ADCx初始化设置 */
    g_adc_handle.Instance                   = ADC1;                   /* 选择哪个ADC */
    g_adc_handle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;    /* 数据对齐：右对齐(ADC_CR2寄存器的第11位) */
    g_adc_handle.Init.ScanConvMode          = ADC_SCAN_DISABLE;       /* 非扫描(ADC_CR1寄存器的第8位) */
    g_adc_handle.Init.ContinuousConvMode    = DISABLE;                /* 单次转换模式(ADC_CR2寄存器的第1位) */
    g_adc_handle.Init.NbrOfConversion       = 1;                      /* 有几个转换(ADC_SQR1寄存器的第20-23位) */
    g_adc_handle.Init.DiscontinuousConvMode = DISABLE;                /* 禁止规则通道组间断模式 */
    g_adc_handle.Init.NbrOfDiscConversion   = 0;                      /* 配置间断模式的规则通道个数，禁止规则通道组间断模式后，此参数忽略 */
    g_adc_handle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;     /* 触发方式：软件触发(ADC_CR2寄存器的第17-19位) */
    HAL_ADC_Init(&g_adc_handle);                                      /* 初始化 */

    multimode.Mode = ADC_MODE_INDEPENDENT;                            /* 独立模式(ADC_CR1寄存器的第16-19位) */
    HAL_ADCEx_MultiModeConfigChannel(&g_adc_handle, &multimode);      /* 配置模式 */

    adc_ch_conf.Channel      = ADC_CHANNEL_16;                        /* 通道 */
    adc_ch_conf.Rank         = 1;                                     /* 序列 */
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;            /* 采样时间 */
    HAL_ADC_ConfigChannel(&g_adc_handle, &adc_ch_conf);               /* 通道配置 */

    /* 开启内部温度传感器(ADC_CR2寄存器的第23位) */
    SET_BIT(g_adc_handle.Instance->CR2, ADC_CR2_TSVREFE);             /* TSVREFE = 1, 启用内部温度传感器和Vrefint */

    HAL_ADCEx_Calibration_Start(&g_adc_handle);                       /* 校准ADC */
}



/**********************************************************
* @funcName ：ADC1_In16_Data
* @brief    ：获取ADC1通道16的转换数据
* @param    ：void
* @retval   ：uint16_t
* @details  ：
* @fn       ：
*            检测芯片温度
************************************************************/
uint16_t ADC1_In16_Data(void)
{
    uint16_t data;

    HAL_ADC_Start(&g_adc_handle);                            /* 开启ADC */
    HAL_ADC_PollForConversion(&g_adc_handle, 10);            /* 轮询转换 */
    data = (uint16_t)HAL_ADC_GetValue(&g_adc_handle);        /* 返回最近一次ADC1规则组的转换结果 */

    return data;
}



/**********************************************************
* @funcName ：Oversampling
* @brief    ：ADC过采样算法，提高分辨率，由12分辨率变为16分辨率
* @param    ：void
* @retval   ：uint16_t  ---  (ADC的16分辨率数据)
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
        sum+=ADC1_In16_Data();
    }

    return (sum>>4);
}




/**********************************************************
* @funcName ：ADC1_Chip_Temperature1
* @brief    ：将ADC值转换为芯片温度值(非过采样)
* @param    ：void
* @retval   ：double
* @details  ：
* @fn       ：
*            用于检测芯片温度
************************************************************/
double ADC1_Chip_Temperature1(void)
{
    uint8_t i;
    uint32_t data;
    double sum = 0;
    double stm_t[6];

    for(i=0;i<5;i++)
    {
        data = ADC1_In16_Data();
        stm_t[i]=(float)data*(3.3/4096);     /* 电压值 */
        stm_t[i]=(1.43-stm_t[i])/0.0043+25;  /* 转换为温度值 */
        sum += stm_t[i];
    }

    stm_t[5] = sum/5;

    return stm_t[5];
}



/**********************************************************
* @funcName ：ADC1_Chip_Temperature2
* @brief    ：将ADC值转换为芯片温度值(过采样)
* @param    ：void
* @retval   ：double
* @details  ：
* @fn       ：
*            用于检测芯片温度
************************************************************/
double ADC1_Chip_Temperature2(void)
{
    uint8_t i;
    uint32_t data;
    double sum = 0;
    double stm_t[6];

    for(i=0;i<5;i++)
    {
        data = Oversampling();
        stm_t[i]=(float)data*(3.3/65536);     /* 电压值 */
        stm_t[i]=(1.43-stm_t[i])/0.0043+25;  /* 转换为温度值 */
        sum += stm_t[i];
    }

    stm_t[5] = sum/5;

    return stm_t[5];
}

