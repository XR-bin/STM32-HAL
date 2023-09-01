#include "wwdg.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\wwdg.c
*              .\Drivers\BSP\inc\wwdg.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-08-05
* @brief     �����ڿ��Ź���ϰ����
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



WWDG_HandleTypeDef g_wwdg_handle;  /* WWDG��� */

/**********************************************************
* @funcName ��WWDG_Init
* @brief    ����ʼ�����ڿ��Ź�
* @param    ��uint8_t tr (������ֵ)
* @param    ��uint8_t wr (����ֵ)
* @param    ��uint8_t fprer (��Ƶϵ����WDGTB��,��Χ:0~7,��ʾ2^WDGTB��Ƶ)
* @retval   ��void
* @details  ��
* @fn       ��
*           Fwwdg=PCLK1/(4096*2^fprer). һ��PCLK1=36Mhz
*           ���ʱ��=(4096*2^fprer)*(tr-0X3F)/PCLK3
*           ����fprer=4,tr=7f,PCLK1=36Mhz
*           �����ʱ��=4096*16*64/36Mhz=116.50ms
************************************************************/
void WWDG_Init(uint8_t tr, uint8_t wr, uint8_t fprer)
{
    __HAL_RCC_WWDG_CLK_ENABLE();                    /* ʹ�ܴ��ڿ��Ź�ʱ�� */

    g_wwdg_handle.Instance       = WWDG;            /* ���ڿ��Ź� */
    g_wwdg_handle.Init.Prescaler = fprer;           /* ���÷�Ƶϵ�� */
    g_wwdg_handle.Init.Window    = wr;              /* ���ô���ֵ */
    g_wwdg_handle.Init.Counter   = tr;              /* ���ü�����ֵ */
    g_wwdg_handle.Init.EWIMode   = WWDG_EWI_ENABLE; /* ʹ�ܴ��ڿ��Ź���ǰ�����ж� */

    HAL_NVIC_SetPriority(WWDG_IRQn, 1, 1);          /* ��ռ���ȼ�1�������ȼ�Ϊ1 */
    HAL_NVIC_EnableIRQ(WWDG_IRQn);                  /* ʹ�ܴ��ڿ��Ź��ж� */

    HAL_WWDG_Init(&g_wwdg_handle);                  /* ��ʼ��WWDG */
}



/**********************************************************
* @funcName ��WWDG_IRQHandler
* @brief    �����ڿ��Ź��жϷ������
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void WWDG_IRQHandler(void)
{
    HAL_WWDG_IRQHandler(&g_wwdg_handle);  /* ����WWDG�����жϴ����� */
}



/**********************************************************
* @funcName ��HAL_WWDG_EarlyWakeupCallback
* @brief    �����ڿ��Ź��жϻص�����
* @param    ��WWDG_HandleTypeDef *hwwdg (���ڿ��Ź����)
* @retval   ��void
* @details  ��
* @fn       ��
*            ���������ڿ��Ź��жϺ����øûص�����
************************************************************/
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg)
{
    HAL_WWDG_Refresh(&g_wwdg_handle); /* ���´��ڿ��Ź�ֵ */
    GPIOE->ODR ^= (1<<5);             /* LED1�̵���˸ */
}





