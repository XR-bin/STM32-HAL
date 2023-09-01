#include "beep.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\beep.c
*              .\Drivers\BSP\inc\beep.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-06-16
* @brief     ��BEEP�������������ô���
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ��BEEP_Init
* @brief    ����BEEP��Ӧ��GPIO�ڽ��г�ʼ������
* @param    ��void
* @retval   ��void
* @details  ��
*            BEEP     PB8
*            �ߵ�ƽ�죬�͵�ƽ����-----���ģʽ
* @fn       ��
************************************************************/
void BEEP_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;                      /* GPIO���ýṹ�� */

    /* BEEPʱ��ʹ�� */
    BEEP_GPIO_CLK_ENABLE();

    /* BEEP�������� */
    gpio_init_struct.Pin   = BEEP_GPIO_PIN;                 /* BEEP���� */
    gpio_init_struct.Mode  = GPIO_MODE_OUTPUT_PP;           /* ������� */
    gpio_init_struct.Pull  = GPIO_PULLUP;                   /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* ���� */
    HAL_GPIO_Init(BEEP_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��BEEP���� */

    BEEP(0);                                                /* �ر� BEEP */
}



