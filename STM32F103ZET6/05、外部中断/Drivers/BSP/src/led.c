#include "led.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\led.c
*              .\Drivers\BSP\inc\led.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-06-16
* @brief     ��LED���������ô���
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ��LED_Init
* @brief    ����LED��Ӧ��GPIO�ڽ��г�ʼ������
* @param    ��void
* @retval   ��void
* @details  ��
*            LED0     PB5
*            LED1     PE5
*            �ߵ�ƽ�𣬵͵�ƽ��-----���ģʽ
* @fn       ��
************************************************************/
void LED_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;                      /* GPIO���ýṹ�� */

    /* LED0��LED1ʱ��ʹ�� */
    LED0_GPIO_CLK_ENABLE();
    LED1_GPIO_CLK_ENABLE();

    /* LED0�������� */
    gpio_init_struct.Pin   = LED0_GPIO_PIN;                 /* LED0���� */
    gpio_init_struct.Mode  = GPIO_MODE_OUTPUT_PP;           /* ������� */
    gpio_init_struct.Pull  = GPIO_PULLUP;                   /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* ���� */
    HAL_GPIO_Init(LED0_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��LED0���� */

    /* LED1�������� */
    gpio_init_struct.Pin = LED1_GPIO_PIN;                   /* LED1���� */
    HAL_GPIO_Init(LED1_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��LED1���� */

    LED0(1);                                                /* �ر� LED0 */
    LED1(1);                                                /* �ر� LED1 */
}
