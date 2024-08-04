#include "key.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\key.c
*              .\Drivers\BSP\inc\key.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-06-16
* @brief     �������������ô���
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ��KEY_Init
* @brief    ����KEY��Ӧ��GPIO�ڽ��г�ʼ������
* @param    ��void
* @retval   ��void
* @details  ��
*            KEY0     PE4
*            KEY1     PE3
*            KEY_UP   PA0
*                                 -------------���ģʽ
* @fn       ��
************************************************************/
void KEY_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;                          /* GPIO���ýṹ�� */

    /* KEY0��KEY1��KEY_UPʱ��ʹ�� */
    KEY0_GPIO_CLK_ENABLE();
    KEY1_GPIO_CLK_ENABLE();
    KEYUP_GPIO_CLK_ENABLE();

    /* KEY0�������� */
    gpio_init_struct.Pin   = KEY0_GPIO_PIN;                     /* KEY0���� */
    gpio_init_struct.Mode  = GPIO_MODE_INPUT;                   /* ���� */
    gpio_init_struct.Pull  = GPIO_PULLUP;                       /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* ���� */
    HAL_GPIO_Init(KEY0_GPIO_PORT, &gpio_init_struct);           /* KEY0����ģʽ����,�������� */

    /* KEY1�������� */
    gpio_init_struct.Pin = KEY1_GPIO_PIN;                       /* KEY1���� */
    HAL_GPIO_Init(KEY1_GPIO_PORT, &gpio_init_struct);           /* KEY1����ģʽ����,�������� */

    /* KEY_UP�������� */
    gpio_init_struct.Pin = KEYUP_GPIO_PIN;                      /* KEY_UP���� */
    gpio_init_struct.Pull = GPIO_PULLDOWN;                      /* ���� */
    HAL_GPIO_Init(KEYUP_GPIO_PORT, &gpio_init_struct);          /* KEY_UP����ģʽ����,�������� */
}



/**********************************************************
* @funcName ��KEY_Scan
* @brief    ������ɨ�躯��
* @param    ��void
* @retval   ��uint8_t
* @details  ��
*            KEY0��KEY1 �͵�ƽ���£��ߵ�ƽ̧��
*            KEY_UP     �ߵ�ƽ���£��͵�ƽ̧��
* @fn       ��
************************************************************/
uint8_t KEY_Scan(void)
{
    uint8_t temp = 0;
    static uint8_t key_flay = 1;

    if(key_flay && ( KEY_UP==1 || KEY0==0 || KEY1==0 ))
    {
        delay_ms(10);
        if( KEY_UP==1 || KEY0==0 || KEY1==0 )
        {
            key_flay = 0;
            if(KEY0==0)        temp = 1;
            else if(KEY1==0)   temp = 2;
            else if(KEY_UP==1) temp = 3;
        }
    }

    if(KEY_UP==0 && KEY0==1 && KEY1==1)
    {
        key_flay = 1;
    }

    return temp;
}








