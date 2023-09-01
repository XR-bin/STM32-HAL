#include "capture.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\capture.c
*              .\Drivers\BSP\inc\capture.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-07-27
* @brief     ��������ϰ����
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



TIM_HandleTypeDef g_timx_cap_chy_handle;      /* ��ʱ��x��� */

/**********************************************************
* @funcName ��TIM4_Capture_Init
* @brief    ����ͨ�ö�ʱ��4���г�ʼ�����ò����������ź�
* @param    ��uint16_t arr (����ֵ)
* @param    ��uint16_t psc (Ԥ��Ƶֵ)
* @retval   ��void
* @details  ��
*            ���������  PB6   TIM4_CH1  ����ͨ��1  ӳ��ͨ��IC1��IC2
* @fn       ��
************************************************************/
void TIM4_Capture_Init(uint16_t arr, uint16_t psc)
{
    TIM_IC_InitTypeDef timx_ic_cap_chy = {0};                             /* �������ýṹ�� */
    TIM_SlaveConfigTypeDef sSlaveConfig = {0};

    /*TIMx����*/
    __HAL_RCC_TIM4_CLK_ENABLE();      /* ����TIM4ʱ�� */

    /* TIMx��ʼ������ */
    g_timx_cap_chy_handle.Instance           = TIM4;                      /* ��ʱ��4 */
    g_timx_cap_chy_handle.Init.Period        = arr;                       /* ����ֵ(TIMx_ARR�Ĵ���) */
    g_timx_cap_chy_handle.Init.Prescaler     = psc;                       /* ��Ƶ(TIMx_PSC�Ĵ���) */
    g_timx_cap_chy_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;    /* TDTS = Tck_tim(TIMx_CR1�Ĵ����ĵ�9-8λ)(������ʱ��û��) */
    g_timx_cap_chy_handle.Init.CounterMode   = TIM_COUNTERMODE_UP;        /* ���ϼ���(TIMx_CR1�Ĵ����ĵ�4λ)(������ʱ��ֻ����) */
    HAL_TIM_IC_Init(&g_timx_cap_chy_handle);                              /* ��ʼ�� */

    /* ��ģʽ���ƼĴ��� */
    sSlaveConfig.SlaveMode    = TIM_SLAVEMODE_RESET;   /* ����ģʽ */
    sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;         /* �˲���Ķ�ʱ������ 1 (TI1FP1) */
    HAL_TIM_SlaveConfigSynchro_IT(&g_timx_cap_chy_handle, &sSlaveConfig);

    /*********ICC1ͨ��**********/
    /* ����ͨ������ */
    /* ͨ��1ӳ�䵽T1 */
    timx_ic_cap_chy.ICPolarity  = TIM_ICPOLARITY_RISING;                /* �����ز���(TIMx->CCER�Ĵ����ĵ�1λ) */
    timx_ic_cap_chy.ICSelection = TIM_ICSELECTION_DIRECTTI;             /* ӳ�䵽TI1��(TIMx->CCMR1�Ĵ���������ģʽ�ĵ�0-1λ) */
    timx_ic_cap_chy.ICPrescaler = TIM_ICPSC_DIV1;                       /* ���������Ƶ������Ƶ(TIMx->CCMR1�Ĵ���������ģʽ�ĵ�2-3λ) */
    timx_ic_cap_chy.ICFilter = 0;                                       /* ���������˲��������˲�(TIMx->CCMR1�Ĵ���������ģʽ�ĵ�4-7λ) */
    HAL_TIM_IC_ConfigChannel(&g_timx_cap_chy_handle, &timx_ic_cap_chy, TIM_CHANNEL_1);  /* ����ͨ��1���ó�ʼ�� */
    TIM_CCxChannelCmd(TIM4, TIM_CHANNEL_1, ENABLE);      /* ʹ�ܲ��� */


    /*********ICC2ͨ��**********/
    /* ͨ��1ӳ�䵽T2 */
    /* ��Ϊ����һ��������һЩ��������ı� */
    timx_ic_cap_chy.ICPolarity  = TIM_ICPOLARITY_FALLING;               /* �½��ز���(TIMx->CCER�Ĵ����ĵ�1λ) */
    timx_ic_cap_chy.ICSelection = TIM_ICSELECTION_INDIRECTTI;           /* ӳ�䵽ͨ��1��(TI1)��(����������ֲ�������)(TIMx->CCMR1�Ĵ���������ģʽ�ĵ�0-1λ) */
    HAL_TIM_IC_ConfigChannel(&g_timx_cap_chy_handle, &timx_ic_cap_chy, TIM_CHANNEL_2);  
    TIM_CCxChannelCmd(TIM4, TIM_CHANNEL_2, ENABLE);        /* ʹ�ܲ��� */

    /* �ж����� */
    __HAL_TIM_ENABLE_IT(&g_timx_cap_chy_handle, TIM_IT_CC1);         /* ����CC1IE�����ж� */
    __HAL_TIM_ENABLE_IT(&g_timx_cap_chy_handle, TIM_IT_CC2);         /* ����CC2IE�����ж� */

    HAL_NVIC_SetPriority(TIM4_IRQn, 1, 1);                           /* �����ж����ȼ�����ռ���ȼ�1�������ȼ�3 */
    HAL_NVIC_EnableIRQ(TIM4_IRQn);

    /* TIMxʹ�� */
    HAL_TIM_IC_Start_IT(&g_timx_cap_chy_handle, TIM_CHANNEL_1);     /* ��ʼ����TIM5 */
}



/**********************************************************
* @funcName ��HAL_TIM_IC_MspInit
* @brief    ��TIM�ײ��ʼ������(�ײ��ʼ���ص����Ӻ���)
* @param    ��TIM_HandleTypeDef *htim (��ʱ�����)
* @retval   ��void
* @details  ��
* @fn       ��
*        �����������HAL_TIM_IC_Init()�����б����ã���TIM�ײ��
*    ʼ���ص����Ӻ�����
************************************************************/
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef gpio_init_struct;                          /* GPIO���ýṹ�� */

    if (htim->Instance == TIM4)
    {
        /* ʱ��ʹ��   GPIOB AFIO */
        __HAL_RCC_GPIOB_CLK_ENABLE();                       /* GPIOBʱ��ʹ�� */

        /* GPIOx���ò��� */
        gpio_init_struct.Pin = GPIO_PIN_6;                  /* ���벶���GPIO�� */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;            /* ����������� */
        gpio_init_struct.Pull = GPIO_PULLDOWN;              /* ���� */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;      /* ���� */
        HAL_GPIO_Init(GPIOB, &gpio_init_struct);
    }
}



/**********************************************************
* @funcName ��TIM4_IRQHandler
* @brief    ����ʱ��4�жϺ���
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void TIM4_IRQHandler(void)
{
    static uint16_t data_2;
    uint16_t data_1;
    uint16_t p_val;

    if(__HAL_TIM_GET_FLAG(&g_timx_cap_chy_handle, TIM_FLAG_CC2) ==  SET)
    {
        /* �����ͨ��2�Ļ��־λ */
        __HAL_TIM_CLEAR_FLAG(&g_timx_cap_chy_handle,TIM_FLAG_CC2);
        /* �����¼� */
        /* CCR2 */
        data_2 = HAL_TIM_ReadCapturedValue(&g_timx_cap_chy_handle, TIM_CHANNEL_2);
    }

    if(__HAL_TIM_GET_FLAG(&g_timx_cap_chy_handle, TIM_FLAG_CC1OF) ==  SET)
    {
        /* ���ͨ��1�ظ������־λ */
        __HAL_TIM_CLEAR_FLAG(&g_timx_cap_chy_handle,TIM_FLAG_CC1OF);
        /* ���ͨ��1�Ĳ����־λ */
        __HAL_TIM_CLEAR_FLAG(&g_timx_cap_chy_handle,TIM_FLAG_CC1);
        /* �����¼� */
        /* ��¼ֵCCR1 */
        data_1 = HAL_TIM_ReadCapturedValue(&g_timx_cap_chy_handle, TIM_CHANNEL_1);
        /* �������ں�ռ�ձ� */
        p_val = (float)data_2/data_1*100;

        printf("���ڣ�%d   �ߵ�ƽ��%d   ռ�ձȣ�%d%%\r\n",data_1,data_2,p_val);
    }
}

