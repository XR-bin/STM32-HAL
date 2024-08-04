#include "adc.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\adc.c
*              .\Drivers\BSP\inc\adc.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-07-27
* @brief     ��ADC��оƬ�ڲ��¶ȴ���
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/*****************************************
* F407��ADCʱ��Ƶ�ʹٷ�����: ������36Mhz
* F103��ADCʱ��Ƶ�ʹٷ�����: ������14Mhz
******************************************/



ADC_HandleTypeDef g_adc_handle;   /* ADC��� */

/**********************************************************
* @funcName ��ADC1_In16_Init
* @brief    ��ADC1�ĵ�16��ͨ����ʼ������
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
*            �������ţ����ڲ�оƬ�¶�
************************************************************/
void ADC1_In16_Init(void)
{
    RCC_PeriphCLKInitTypeDef adc_clk_init = {0};                /* ADCxһ�����ýṹ�� */
    ADC_ChannelConfTypeDef adc_ch_conf;                         /* ADCxͨ�����ýṹ�� */
    ADC_MultiModeTypeDef multimode;                             /* ��ģʽ�ṹ�� */

    /* ʱ��ʹ��  ADC1 */
    __HAL_RCC_ADC1_CLK_ENABLE();

    adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC;     /* ADC����ʱ�� */
    adc_clk_init.AdcClockSelection    = RCC_ADCPCLK2_DIV6;     /* ��Ƶ����6ʱ��Ϊ72M/6=12MHz */
    HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);                  /* ����ADCʱ�� */

    /* ADCx��ʼ������ */
    g_adc_handle.Instance                   = ADC1;                   /* ѡ���ĸ�ADC */
    g_adc_handle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;    /* ���ݶ��룺�Ҷ���(ADC_CR2�Ĵ����ĵ�11λ) */
    g_adc_handle.Init.ScanConvMode          = ADC_SCAN_DISABLE;       /* ��ɨ��(ADC_CR1�Ĵ����ĵ�8λ) */
    g_adc_handle.Init.ContinuousConvMode    = DISABLE;                /* ����ת��ģʽ(ADC_CR2�Ĵ����ĵ�1λ) */
    g_adc_handle.Init.NbrOfConversion       = 1;                      /* �м���ת��(ADC_SQR1�Ĵ����ĵ�20-23λ) */
    g_adc_handle.Init.DiscontinuousConvMode = DISABLE;                /* ��ֹ����ͨ������ģʽ */
    g_adc_handle.Init.NbrOfDiscConversion   = 0;                      /* ���ü��ģʽ�Ĺ���ͨ����������ֹ����ͨ������ģʽ�󣬴˲������� */
    g_adc_handle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;     /* ������ʽ���������(ADC_CR2�Ĵ����ĵ�17-19λ) */
    HAL_ADC_Init(&g_adc_handle);                                      /* ��ʼ�� */

    multimode.Mode = ADC_MODE_INDEPENDENT;                            /* ����ģʽ(ADC_CR1�Ĵ����ĵ�16-19λ) */
    HAL_ADCEx_MultiModeConfigChannel(&g_adc_handle, &multimode);      /* ����ģʽ */

    adc_ch_conf.Channel      = ADC_CHANNEL_16;                        /* ͨ�� */
    adc_ch_conf.Rank         = 1;                                     /* ���� */
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;            /* ����ʱ�� */
    HAL_ADC_ConfigChannel(&g_adc_handle, &adc_ch_conf);               /* ͨ������ */

    /* �����ڲ��¶ȴ�����(ADC_CR2�Ĵ����ĵ�23λ) */
    SET_BIT(g_adc_handle.Instance->CR2, ADC_CR2_TSVREFE);             /* TSVREFE = 1, �����ڲ��¶ȴ�������Vrefint */

    HAL_ADCEx_Calibration_Start(&g_adc_handle);                       /* У׼ADC */
}



/**********************************************************
* @funcName ��ADC1_In16_Data
* @brief    ����ȡADC1ͨ��16��ת������
* @param    ��void
* @retval   ��uint16_t
* @details  ��
* @fn       ��
*            ���оƬ�¶�
************************************************************/
uint16_t ADC1_In16_Data(void)
{
    uint16_t data;

    HAL_ADC_Start(&g_adc_handle);                            /* ����ADC */
    HAL_ADC_PollForConversion(&g_adc_handle, 10);            /* ��ѯת�� */
    data = (uint16_t)HAL_ADC_GetValue(&g_adc_handle);        /* �������һ��ADC1�������ת����� */

    return data;
}



/**********************************************************
* @funcName ��Oversampling
* @brief    ��ADC�������㷨����߷ֱ��ʣ���12�ֱ��ʱ�Ϊ16�ֱ���
* @param    ��void
* @retval   ��uint16_t  ---  (ADC��16�ֱ�������)
* @details  ��
* @fn       ��
*            ����ת��������������
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
* @funcName ��ADC1_Chip_Temperature1
* @brief    ����ADCֵת��ΪоƬ�¶�ֵ(�ǹ�����)
* @param    ��void
* @retval   ��double
* @details  ��
* @fn       ��
*            ���ڼ��оƬ�¶�
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
        stm_t[i]=(float)data*(3.3/4096);     /* ��ѹֵ */
        stm_t[i]=(1.43-stm_t[i])/0.0043+25;  /* ת��Ϊ�¶�ֵ */
        sum += stm_t[i];
    }

    stm_t[5] = sum/5;

    return stm_t[5];
}



/**********************************************************
* @funcName ��ADC1_Chip_Temperature2
* @brief    ����ADCֵת��ΪоƬ�¶�ֵ(������)
* @param    ��void
* @retval   ��double
* @details  ��
* @fn       ��
*            ���ڼ��оƬ�¶�
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
        stm_t[i]=(float)data*(3.3/65536);     /* ��ѹֵ */
        stm_t[i]=(1.43-stm_t[i])/0.0043+25;  /* ת��Ϊ�¶�ֵ */
        sum += stm_t[i];
    }

    stm_t[5] = sum/5;

    return stm_t[5];
}

