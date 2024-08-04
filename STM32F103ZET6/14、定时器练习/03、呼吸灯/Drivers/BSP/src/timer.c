#include "timer.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\timer.c
*              .\Drivers\BSP\inc\timer.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-07-27
* @brief     ��������ʱ���ж���ϰ����
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



TIM_HandleTypeDef g_timx_handle;  /* ��ʱ����� */

/**********************************************************
* @funcName ��TIM6_Init
* @brief    ���Զ�ʱ��6���г�ʼ������
* @param    ��uint16_t arr (����ֵ)
* @param    ��uint16_t psc (Ԥ��Ƶֵ)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void TIM6_Init(uint16_t arr, uint16_t psc)
{
    /* TIMxʱ��ʹ�� */
    __HAL_RCC_TIM6_CLK_ENABLE();

    /* ��ʱ������ */
    g_timx_handle.Instance         = TIM6;                      /* ͨ�ö�ʱ��X */
    g_timx_handle.Init.Prescaler   = psc;                       /* ��Ƶ(TIMx_PSC�Ĵ���) */
    g_timx_handle.Init.Period      = arr;                       /* ����ֵ(TIMx_ARR�Ĵ���) */
    g_timx_handle.Init.CounterMode = TIM_COUNTERMODE_UP;        /* ���ϼ���(TIMx_CR1�Ĵ����ĵ�4λ)(������ʱ��ֻ����) */
    HAL_TIM_Base_Init(&g_timx_handle);                          /* ��ʼ��TIM6 */

    /* �ж����� */
    __HAL_TIM_ENABLE_IT(&g_timx_handle, TIM_IT_UPDATE);         /* ����TIMx�����ж� */
    HAL_NVIC_SetPriority(TIM6_IRQn, 1, 1);                      /* ��ռ1�������ȼ�1����2 */
    HAL_NVIC_EnableIRQ(TIM6_IRQn);                              /* ����ITM6�ж� */

    HAL_TIM_Base_Start_IT(&g_timx_handle);                      /* ʹ�ܶ�ʱ��x��������ж� */
}



/**********************************************************
* @funcName ��TIM6_IRQHandler
* @brief    ����ʱ��6���жϷ�����
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void TIM6_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_handle); /* ��ʱ���жϹ��������� */
}



/**********************************************************
* @funcName ��HAL_TIM_PeriodElapsedCallback
* @brief    ��TIME��ʱ�������жϻص�����
* @param    ��UART_HandleTypeDef *huart (UART�������ָ��)
* @retval   ��void
* @details  ��
* @fn       ��
*            ��������ʱ�������жϺ����øûص�����
************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static uint8_t temp = 0;

    if (htim->Instance == TIM6)
    {
        /* �����¼� */
        if(temp == 0)
        {
            if(TIM3->CCR2 == 0) temp = 1;
            else TIM3->CCR2 -=20;
        }
        if(temp == 1)
        {
            if(TIM3->CCR2 == 1000) temp = 0;
            else TIM3->CCR2 +=20;
        }
        __HAL_TIM_CLEAR_FLAG(&g_timx_handle, TIM_FLAG_UPDATE);
    }
}


