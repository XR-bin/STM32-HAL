#include "exti.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\exti.c
*              .\Drivers\BSP\inc\exti.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-06-16
* @brief     ������жϴ���
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ��EXTI5_Swier_Init
* @brief    ������ⲿ�жϳ�ʼ��
* @param    ��void
* @retval   ��void
* @details  ��
*            ����˿�
* @fn       ��
************************************************************/
void EXTI5_Swier_Init(void)
{
    /* �ⲿ�ж����� */
    /* ����ж��¼��Ĵ��� */
    EXTI->SWIER &= ~(1<<5);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);               /* ��ռ0�������ȼ�0 */
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);                       /* ʹ������ж�5 */

    /* �ж����μĴ��� */
    EXTI->IMR |= (1<<5);
}



/**********************************************************
* @funcName ��EXTI9_5_IRQHandler
* @brief    ���ⲿ�жϷ�����
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void EXTI9_5_IRQHandler(void)
{
    if(EXTI->PR & (1<<5))
    {
        GPIOB->ODR ^= (1<<5);            /* LED0��ת */
        EXTI->PR |= (1<<5);              /* д1����жϱ�־ */
    }
}







