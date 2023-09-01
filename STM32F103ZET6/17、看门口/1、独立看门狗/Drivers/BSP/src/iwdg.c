#include "iwdg.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\iwdg.c
*              .\Drivers\BSP\inc\iwdg.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-08-05
* @brief     ���������Ź���ϰ����
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



IWDG_HandleTypeDef g_iwdg_handle;  /* �������Ź���� */

/**********************************************************
* @funcName ��IWDG_Init
* @brief    ���Զ������Ź���������
* @param    ��uint8_t prer (Ԥ��Ƶ����Ƶ��:0~7(ֻ�е�3λ��Ч!))
*   @arg    ����Ƶ���� = 4 * 2^prer. �����ֵֻ����256!
* @param    ��uint16_t rlr (�Զ���װ��ֵ,0~0XFFF)
* @retval   ��void
* @details  ��
* @fn       ��
*           ʱ�����(���):Tout=((4 * 2^prer) * rlr) / 40 (ms).
************************************************************/
void IWDG_Init(uint8_t prer, uint16_t rlr)
{
    g_iwdg_handle.Instance       = IWDG; /* �������Ź� */
    g_iwdg_handle.Init.Prescaler = prer; /* ���÷�Ƶϵ�� (IWDG_PR�Ĵ���) */
    g_iwdg_handle.Init.Reload    = rlr;  /* ����IWDGװ��ֵ(IWDG->RLR�Ĵ���) */
    HAL_IWDG_Init(&g_iwdg_handle);       /* ��ʼ��IWDG������ */
}




/**********************************************************
* @funcName ��IWDG_Feed
* @brief    ��ι�������Ź�
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void IWDG_Feed(void)
{
    HAL_IWDG_Refresh(&g_iwdg_handle);  /* ��װ�ؼ����� */
}






