#include "touch.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\touch.c
*              .\Drivers\BSP\inc\touch.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-07-03
* @brief     �����败�����������úͲ�������
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ��Touch_IO_Init
* @brief    ���Ե��败����оƬ��Ӧ��GPIO�ڽ��г�ʼ������
* @param    ��void
* @retval   ��void
* @details  ��
*            CS     PF11  �������
*            SCK    PB1   �������
*            PEN    PF10  ͨ������
*            MISO   PB2   ͨ������
*            MOSI   PF9   �������
* @fn       ��
************************************************************/
void Touch_IO_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;    /* GPIO���ýṹ�� */

    /* ʱ��ʹ�� */
    T_PEN_GPIO_CLK_ENABLE();    /* T_PEN��ʱ��ʹ�� */
    T_CS_GPIO_CLK_ENABLE();     /* T_CS��ʱ��ʹ�� */
    T_MISO_GPIO_CLK_ENABLE();   /* T_MISO��ʱ��ʹ�� */
    T_MOSI_GPIO_CLK_ENABLE();   /* T_MOSI��ʱ��ʹ�� */
    T_SCK_GPIO_CLK_ENABLE();    /* T_SCK��ʱ��ʹ�� */

    /* GPIOx��ʼ������ */
    /* GPIOx�˿����� */
    /* T_PEN */
    gpio_init_struct.Pin   = T_PEN_GPIO_PIN;                 /* PF10 �˿����� */
    gpio_init_struct.Mode  = GPIO_MODE_INPUT;                /* ���� */
    gpio_init_struct.Pull  = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;           /* ���� */
    HAL_GPIO_Init(T_PEN_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��T_PEN���� */
    /* T_MISO */
    gpio_init_struct.Pin = T_MISO_GPIO_PIN;                  /* PB2 */
    HAL_GPIO_Init(T_MISO_GPIO_PORT, &gpio_init_struct);      /* ��ʼ��T_MISO���� */
    /* T_MOSI */
    gpio_init_struct.Pin = T_MOSI_GPIO_PIN;                  /* PF9 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;             /* ������� */
    HAL_GPIO_Init(T_MOSI_GPIO_PORT, &gpio_init_struct);      /* ��ʼ��T_MOSI���� */
    /* SCK */
    gpio_init_struct.Pin = T_SCK_GPIO_PIN;                   /* PB1 */
    HAL_GPIO_Init(T_SCK_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��T_CLK���� */
    /* CS */
    gpio_init_struct.Pin = T_CS_GPIO_PIN;                    /* F11 */
    HAL_GPIO_Init(T_CS_GPIO_PORT, &gpio_init_struct);        /* ��ʼ��T_CS���� */

    /* ��ʼ��״̬���� */
    T_CS_H;
    T_SCK_L;
    T_MOSI_L;
}



/**********************************************************
* @funcName ��Touch_Data
* @brief    ���Ե��败����оƬ��Ӧ��GPIO�ڽ��г�ʼ������
* @param    ��uint8_t command (ָ��)
* @retval   ��uint16_t
* @details  ��
*            Ƭѡ��        T_CS_H       T_CS_L  
*            ʱ����        T_SCK_H      T_SCK_L  
*            ���������    T_MOSI_H     T_MOSI_L
*            ����������    T_MISO
*            �ʽӴ���      T_PEN
* @fn       ��
************************************************************/
uint16_t Touch_Data(uint8_t command)
{
    uint8_t i;
    uint16_t data = 0;

    T_MOSI_L;            /* ���������� */
    T_CS_L;              /* Ƭѡʹ�� */

    for(i=0;i<8;i++)
    {
        T_SCK_L;             /* ʱ�������� */

        /* �ж�ָ��λ��ƽ */
        if(command & 0x80)T_MOSI_H;
        else T_MOSI_L;

        command <<= 1;  /* ָ���������� */

        T_SCK_H;          /* ʱ�������� */
        delay_us(1);      /* ��ʱ�����ڴӻ���ȡ���� */
    }
    T_SCK_L;            /* ʱ�������� */
    T_MOSI_L;           /* ���������� */
    delay_us(6);        /* ��ʱ6us�����ڵȴ�ADC������ת���� */

    /* �ճ�һ�������� */
    T_SCK_L;
    delay_us(1);
    T_SCK_H;
    delay_us(1);

    for(i=0;i<12;i++)
    {
        data <<= 1;              /* ָ���������� */
        T_SCK_L;                 /* ʱ�������� */
        delay_us(1);             /* ��ʱ���������ݽ��� */
        T_SCK_H;                 /* ʱ�������� */
        if(T_MISO) data |= 0x1;  /* ��ȡ����λ��ƽ */
        delay_us(1);             /* ��ʱ���������ݽ��� */
    }

    T_CS_H;    /* Ƭѡ���� */
    T_SCK_L;   /* ʱ�������� */

    return data;
}





