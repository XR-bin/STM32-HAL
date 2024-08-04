#include "adc.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\adc.c
*              .\Drivers\BSP\inc\adc.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-07-27
* @brief     ��ADC���ô���
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/***************************************
* F407��ADCʱ��Ƶ�ʹٷ�����: ������36Mhz
* F103��ADCʱ��Ƶ�ʹٷ�����: ������14Mhz
****************************************/



ADC_HandleTypeDef g_adc3_handle;         /* ADC��� */

/**********************************************************
* @funcName ��ADC3_In6_Init
* @brief    ��ADC3�ĵ�6��ͨ����ʼ������
* @param    ��void
* @retval   ��void
* @details  ��
*             PF8----------ADC3_IN6   ģ��ģʽ
* @fn       ��
************************************************************/
void ADC3_In6_Init(void)
{
    RCC_PeriphCLKInitTypeDef adc_clk_init = {0};                /* ADCxһ�����ýṹ�� */
    ADC_ChannelConfTypeDef adc_ch_conf;                         /* ADCxͨ�����ýṹ�� */
    ADC_MultiModeTypeDef multimode;                             /* ��ģʽ�ṹ�� */

    /* ʱ��ʹ��  ADC3 */
    ADC3_CHY_CLK_ENABLE();                                     /* ADCʱ��ʹ�� */

    adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC;     /* ADC����ʱ�� */
    adc_clk_init.AdcClockSelection    = RCC_ADCPCLK2_DIV6;     /* ��Ƶ����6ʱ��Ϊ72M/6=12MHz */
    HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);                  /* ����ADCʱ�� */

    g_adc3_handle.Instance                   = ADC_ADCX;               /* ѡ���ĸ�ADC */
    g_adc3_handle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;    /* ���ݶ��룺�Ҷ���(ADC_CR2�Ĵ����ĵ�11λ) */
    g_adc3_handle.Init.ScanConvMode          = ADC_SCAN_DISABLE;       /* ��ɨ��(ADC_CR1�Ĵ����ĵ�8λ) */
    g_adc3_handle.Init.ContinuousConvMode    = DISABLE;                /* ����ת��ģʽ(ADC_CR2�Ĵ����ĵ�1λ) */
    g_adc3_handle.Init.NbrOfConversion       = 1;                      /* �м���ת��(ADC_SQR1�Ĵ����ĵ�20-23λ) */
    g_adc3_handle.Init.DiscontinuousConvMode = DISABLE;                /* ��ֹ����ͨ������ģʽ */
    g_adc3_handle.Init.NbrOfDiscConversion   = 0;                      /* ���ü��ģʽ�Ĺ���ͨ����������ֹ����ͨ������ģʽ�󣬴˲������� */
    g_adc3_handle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;     /* ������ʽ���������(ADC_CR2�Ĵ����ĵ�17-19λ) */
    HAL_ADC_Init(&g_adc3_handle);                                      /* ��ʼ�� */

    multimode.Mode = ADC_MODE_INDEPENDENT;                             /* ����ģʽ(ADC_CR1�Ĵ����ĵ�16-19λ) */
    HAL_ADCEx_MultiModeConfigChannel(&g_adc3_handle, &multimode);      /* ����ģʽ */

    adc_ch_conf.Channel      = ADC3_CHY;                               /* ͨ�� */
    adc_ch_conf.Rank         = 1;                                      /* ���� */
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;             /* ����ʱ�� */
    HAL_ADC_ConfigChannel(&g_adc3_handle, &adc_ch_conf);               /* ͨ������ */

    /* �ж����� */
    __HAL_ADC_ENABLE_IT(&g_adc3_handle, ADC_IT_EOC);                   /* ת�������ж� */
    HAL_NVIC_SetPriority(ADC3_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(ADC3_IRQn);

    HAL_ADCEx_Calibration_Start(&g_adc3_handle);                       /* У׼ADC */
    HAL_ADC_Start_IT(&g_adc3_handle);                                  /* ����ADC */
}



/**********************************************************
* @funcName ��HAL_ADC_MspInit
* @brief    ��ADC�ײ��ʼ������(�ײ��ʼ���ص����Ӻ���)
* @param    ��ADC_HandleTypeDef *hadc (ADC�������ָ��)
* @retval   ��void
* @details  ��
* @fn       ��
*        �����������HAL_ADC_Init()�����б����ã���ADC�ײ��
*    ʼ���ص����Ӻ�����
************************************************************/
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    GPIO_InitTypeDef gpio_init_struct;                          /* GPIO���ýṹ�� */

    if(hadc->Instance == ADC_ADCX)
    {
        /* ʱ��ʹ��  GPIOF */
        ADC3_CHY_GPIO_CLK_ENABLE();                             /* IO��ʱ��ʹ�� */

        /* GPIOx��ʼ������ */
        /* GPIOx�˿����� */
        gpio_init_struct.Pin   = ADC3_CHY_GPIO_PIN;                /* ADCͨ����Ӧ��IO���� */
        gpio_init_struct.Mode  = GPIO_MODE_ANALOG;                 /* ģ���������� */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;             /* IO���ٶ�Ϊ50MHz */
        HAL_GPIO_Init(ADC3_CHY_GPIO_PORT, &gpio_init_struct);      /* �����趨������ʼ�� */
    }
}



/**********************************************************
* @funcName ��ADC3_IRQHandler
* @brief    ��ADC3�жϷ�����
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void ADC3_IRQHandler(void)
{
    HAL_ADC_IRQHandler(&g_adc3_handle);
}



/**********************************************************
* @funcName ��HAL_ADC_ConvCpltCallback
* @brief    ��ADCת�������жϻص�����
* @param    ��ADC_HandleTypeDef* hadc (ADC�������ָ��)
* @retval   ��void
* @details  ��
* @fn       ��
*            ������ת�������жϺ����øûص�����
************************************************************/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    uint16_t data;

    if(hadc->Instance == ADC_ADCX)
    {
        /* ��ȡת��������� */
        data = HAL_ADC_GetValue(hadc);
        printf("data:%d\r\n",data);
        __HAL_ADC_CLEAR_FLAG(&g_adc3_handle, ADC_FLAG_EOC);
    }
}




