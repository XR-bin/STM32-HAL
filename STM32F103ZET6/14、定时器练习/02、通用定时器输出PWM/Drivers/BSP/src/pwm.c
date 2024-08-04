#include "pwm.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\pwm.c
*              .\Drivers\BSP\inc\pwm.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-07-27
* @brief     ��pwm��ϰ����
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



TIM_HandleTypeDef g_timx_pwm_chy_handle;     /* ��ʱ��x��� */

/**********************************************************
* @funcName ��TIM3_PWM_Init
* @brief    ����ͨ�ö�ʱ��3���г�ʼ�����ò����PWM
* @param    ��uint16_t arr (����ֵ)
* @param    ��uint16_t psc (Ԥ��Ƶֵ)
* @param    ��uint16_t ccr (�Ƚ�ֵ/ռ�ձ�)
* @retval   ��void
* @details  ��
*            PWM�����  PB5   TIM3_CH2  ���ͨ��2
* @fn       ��
*            ��Ч��ƽΪ�ߵ�ƽ
*            ��ʱ�����ϼ���
*            �������Ч��ƽ�������Ч��ƽ
************************************************************/
void TIM3_PWM_Init(uint16_t arr, uint16_t psc, uint16_t ccr)
{
    TIM_OC_InitTypeDef timx_oc_pwm_chy  = {0};                          /* ��ʱ��PWM������� */

    /* ʱ��ʹ��  GPIOB  TIM3 */
    __HAL_RCC_TIM3_CLK_ENABLE();

    /* GPIOx���� */
    g_timx_pwm_chy_handle.Instance           = TIM3;                      /* ��ʱ��x */
    g_timx_pwm_chy_handle.Init.Period        = arr;                       /* ����ֵ(TIMx_ARR�Ĵ���) */
    g_timx_pwm_chy_handle.Init.Prescaler     = psc;                       /* ��Ƶ(TIMx_PSC�Ĵ���) */
    g_timx_pwm_chy_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;    /* TDTS = Tck_tim(TIMx_CR1�Ĵ����ĵ�9-8λ)(������ʱ��û��) */
    g_timx_pwm_chy_handle.Init.CounterMode   = TIM_COUNTERMODE_UP;        /* ���ϼ���(TIMx_CR1�Ĵ����ĵ�4λ)(������ʱ��ֻ����) */
    HAL_TIM_PWM_Init(&g_timx_pwm_chy_handle);                             /* ��ʼ�� */

    /* PWM ͨ����ʼ������ */
    /* ͨ��2 ������� */
    timx_oc_pwm_chy.OCMode     = TIM_OCMODE_PWM1;                          /* ����ΪPWMģʽ1(TIMx_CCMR1��OC1Mλ) */
    timx_oc_pwm_chy.Pulse      = ccr;                                      /* ���ñȽϼĴ���(TIMx->CCR2) */
    timx_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                       /* ������Ч��ƽΪ�͵�ƽ(TIMx_CCER��CC1Pλ) */
    HAL_TIM_PWM_ConfigChannel(&g_timx_pwm_chy_handle, &timx_oc_pwm_chy, TIM_CHANNEL_2); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_timx_pwm_chy_handle, TIM_CHANNEL_2);              /* ������ӦPWMͨ�� */
}



/**********************************************************
* @funcName ��HAL_TIM_PWM_MspInit
* @brief    ��PWM�ײ��ʼ������(�ײ��ʼ���ص����Ӻ���)
* @param    ��TIM_HandleTypeDef *htim (��ʱ�����)
* @retval   ��void
* @details  ��
* @fn       ��
*        �����������HAL_TIM_PWM_Init()�����б����ã���PWM�ײ��
*    ʼ���ص����Ӻ�����
************************************************************/
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef gpio_init_struct;                          /* GPIO���ýṹ�� */

    if (htim->Instance == TIM3)
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();                      /* ����ͨ��y��CPIOʱ�� */
        __HAL_RCC_AFIO_CLK_ENABLE();                       /* AFIOʱ��ʹ�� */
        __HAL_AFIO_REMAP_TIM3_PARTIAL();                   /* ����ӳ��,ʹPB5���PWMͨ��2 */

        gpio_init_struct.Pin   = GPIO_PIN_5;               /* ͨ��y��CPIO�� */
        gpio_init_struct.Mode  = GPIO_MODE_AF_PP;          /* ����������� */
        gpio_init_struct.Pull  = GPIO_PULLUP;              /* ���� */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;     /* ���� */
        HAL_GPIO_Init(GPIOB, &gpio_init_struct);           /* ��ʼ�� */
    }
}

