#include "iic.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\iic.c
*              .\Drivers\BSP\inc\iic.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-07-01
* @brief     ��iic���������Լ�iicʱ�����
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ��IIC_Init
* @brief    ����ģ��IIC����Ҫ��GPIO�ڽ��г�ʼ������
* @param    ��void
* @retval   ��void
* @details  ��
*            ʱ����  IIC_SCL  PB6   �������
*            ������  IIC_SDA  PB7   ��©���
*                 �ߵ�ƽ���ڿ���״̬
* @fn       ��
************************************************************/
void IIC_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;      /* GPIO���ýṹ�� */

    IIC_SCL_GPIO_CLK_ENABLE();  /* SCL����ʱ��ʹ�� */
    IIC_SDA_GPIO_CLK_ENABLE();  /* SDA����ʱ��ʹ�� */

    gpio_init_struct.Pin   = IIC_SCL_GPIO_PIN;            /* SCL���� */
    gpio_init_struct.Mode  = GPIO_MODE_OUTPUT_PP;         /* ������� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;        /* ���� */
    HAL_GPIO_Init(IIC_SCL_GPIO_PORT, &gpio_init_struct);  /* SCL */

    gpio_init_struct.Pin  = IIC_SDA_GPIO_PIN;             /* SDA���� */
    gpio_init_struct.Pull = GPIO_PULLUP;                  /* ���� */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;          /* ��©��� */
    HAL_GPIO_Init(IIC_SDA_GPIO_PORT, &gpio_init_struct);  /* SDA */

    /* �˿�������ݼĴ���      �ߵ�ƽ���� */
    SCL_H;
    SDA_OUT_H;
}



/**********************************************************
* @funcName ��IIC_Start
* @brief    ��ģ��IIC����ʼ�ź�
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
*            SCL_H         ʱ��������
*            SCL_L         ʱ��������
*            SDA_OUT_H     ���ģʽ������������
*            SDA_OUT_L     ���ģʽ������������
************************************************************/
void IIC_Start(void)
{
    SCL_L;         /* ʱ�������� */
    SDA_OUT_H;     /* ���������� */
    SCL_H;         /* ʱ�������� */
    delay_us(5);   /* ��ʱ5us */
    SDA_OUT_L;     /* ���������� */
    delay_us(5);   /* ��ʱ5us */
    SCL_L;         /* ʱ��������0 */
}



/**********************************************************
* @funcName ��IIC_Stop
* @brief    ��ģ��IIC��ֹͣ�ź�
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
*            SCL_H         ʱ��������
*            SCL_L         ʱ��������
*            SDA_OUT_H     ���ģʽ������������
*            SDA_OUT_L     ���ģʽ������������
************************************************************/
void IIC_Stop(void)
{
    SCL_L;        /* ʱ�������� */
    SDA_OUT_L;    /* ���������� */
    SCL_H;        /* ʱ�������� */
    delay_us(5);  /* ��ʱ5us */
    SDA_OUT_H;    /* ���������� */
    delay_us(5);  /* ��ʱ5us */
}



/**********************************************************
* @funcName ��IIC_Send_Ack
* @brief    ��ģ��IIC����������Ӧ���ź�
* @param    ��uint8_t ack
*   @arg    ��0 --- Ӧ��   1 --- ��Ӧ��
* @retval   ��void
* @details  ��
* @fn       ��
*            SCL_H         ʱ��������
*            SCL_L         ʱ��������
*            SDA_OUT_H     ���ģʽ������������
*            SDA_OUT_L     ���ģʽ������������
************************************************************/
void IIC_Send_Ack(uint8_t ack)
{
    SCL_L;                /* ʱ�������� */
    delay_us(5);          /* ��ʱ5us */
    /*�ж��Ƿ�Ҫ����Ӧ���ź�*/
    if(ack == 1)          /* ���Ͳ�Ӧ���ź� */
    {
        SDA_OUT_H;        /* ���������� */
        delay_us(1);      /* ��ʱ1us */
        SCL_H;            /* ʱ�������� */
    }
    else if(ack == 0)     /* ����Ӧ���ź� */
    {
        SDA_OUT_L;        /* ���������� */
        delay_us(1);      /* ��ʱ1us */
        SCL_H;            /* ʱ�������� */
    }

    delay_us(5);          /* ��ʱ5us */
    SCL_L;                /* ʱ�������� */
}



/**********************************************************
* @funcName ��IIC_Receive_Ack
* @brief    ��ģ��IIC����������Ӧ���ź�
* @param    ��void
* @retval   ��uint8_t  ---  (0-Ӧ��   1-��Ӧ��)
* @details  ��
* @fn       ��
*            SCL_H         ʱ��������
*            SCL_L         ʱ��������
*            SDA_OUT_H     ���ģʽ������������
*            SDA_OUT_L     ���ģʽ������������
*            SDA_IN        ����ģʽ
*            SDA_INT       ���յ���ֵ
************************************************************/
uint8_t IIC_Receive_Ack(void)
{
    SCL_L;         /* ʱ�������� */
    SDA_IN;        /* ��������ģʽ */
    delay_us(5);   /* ��ʱ5us */
    SCL_H;         /* ʱ�������� */

    /* �жϴӻ����͵�Ӧ���ź� */
    /* �ӻ���Ӧ�� */
    if(SDA_INT)
    {
        SCL_L;      /* ʱ�������� */
        return 1;
    }
    /* �ӻ�Ӧ�� */
    /* ʱ�������� */
    SCL_L;
    return 0;
}



/**********************************************************
* @funcName ��IIC_Send_Byte
* @brief    ��ģ��IIC����������һ���ֽڵ�����
* @param    ��uint8_t data
* @retval   ��void
* @details  ��
* @fn       ��
*            SCL_H         ʱ��������
*            SCL_L         ʱ��������
*            SDA_OUT_H     ���ģʽ������������
*            SDA_OUT_L     ���ģʽ������������
************************************************************/
void IIC_Send_Byte(uint8_t data)
{
    uint8_t i;

    for(i=0;i<8;i++)
    {
        SCL_L;             /* ʱ�������� */
        delay_us(5);       /* ��ʱ5us */

        /*�жϷ�����0����1*/
        if(data & 0x80)    /* ����1 */
        {
            SDA_OUT_H;     /* ���������� */
        }
        else               /* ����0 */
        {
            SDA_OUT_L;     /* ���������� */
        }

        delay_us(1);       /* ��ʱ1us */
        SCL_H;             /* ʱ�������� */
        delay_us(5);       /* ��ʱ5us */

        data <<= 1;        /* data����һλ */
    }

    SCL_L;                 /* ʱ�������� */
}



/**********************************************************
* @funcName ��IIC_Receive_Byte
* @brief    ��ģ��IIC����������һ���ֽڵ�����
* @param    ��void
* @retval   ��uint8_t
* @details  ��
* @fn       ��
*            SCL_H         ʱ��������
*            SCL_L         ʱ��������
*            SDA_OUT_H     ���ģʽ������������
*            SDA_OUT_L     ���ģʽ������������
*            SDA_IN        ����ģʽ
*            SDA_INT       ���յ���ֵ
************************************************************/
uint8_t IIC_Receive_Byte(void)
{
    uint8_t i;
    uint8_t data = 0;

    SCL_L;               /* ����ʱ���� */
    SDA_IN;              /* �������ģʽ */

    for(i=0;i<8;i++)
    {
        SCL_L;           /* ʱ�������� */

        data <<= 1;      /* data����һλ */
        delay_us(5);     /* ��ʱ5us */
        SCL_H;           /* ʱ�������� */

        /* �жϽ��յ���1����0 */ 
        /* ���յ���1 */
        if(SDA_INT)
        {
            data |= 0x1;
        }
        /* ���յ���0 �������� */
    }

    SCL_L;                /* ʱ�������� */

    return data;
}












