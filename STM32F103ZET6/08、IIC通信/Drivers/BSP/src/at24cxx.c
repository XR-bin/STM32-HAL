#include "at24cxx.h"
#include "iic.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\at24cxx.c
*              .\Drivers\BSP\inc\at24cxx.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-07-01
* @brief     ��at24cxx��д��������
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ��AT24CXX_Init
* @brief    ����at24cxx����Ҫ��GPIO�ڽ��г�ʼ������
* @param    ��void
* @retval   ��void
* @details  ��
*            ʱ����  IIC_SCL  PB6   �������
*            ������  IIC_SDA  PB7   ��©���
*                 �ߵ�ƽ���ڿ���״̬
* @fn       ��
************************************************************/
void AT24CXX_Init(void)
{
    IIC_Init();
}



/**********************************************************
* @funcName ��AT24CXX_Write_Byte
* @brief    ��дһ���ֽڵ����ݵ�AT24CXX��ĳ���ڲ���ַ��
* @param    ��uint16_t address (�洢��ַ)
* @param    ��uint8_t  data    (����)
* @retval   ��uint8_t  ---  (�������)
* @details  ��
* @fn       ��
*           AT24CXX_NO_ERR       0      Ӧ����Ч   ���ݴ���ɹ�
*           AT24CXX_ERR1         1      Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*           AT24CXX_ERR2         2      Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*           AT24CXX_ERR3         3      Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*
*           AT24CXX_WRITE        0xa0   ������ַ   д����
*           AT24CXX_READ         0xa1   ������ַ   ������
************************************************************/
uint8_t AT24CXX_Write_Byte(uint16_t address,uint8_t data)
{
    uint8_t ack;

    IIC_Start();                      /* ����������ʼ�ź� */

    /* �ж�AT24CXX�ͺ� */
    if(AT24CXX > AT24C16)             /* �����AT24C16���ϵ��ͺ�(������AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE); /* ��������������ַ(д) */
        ack = IIC_Receive_Ack();      /* ���մӻ�Ӧ���ź� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }

        IIC_Send_Byte(address>>8);    /* ���͸߰�λ�洢��ַ(�ֵ�ַ) */
        ack = IIC_Receive_Ack();      /* ���մӻ�Ӧ���ź� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR2;
        }
    }
    else                              /* �����AT24C16���µ��ͺ�(����AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE+((address/256)<<1));   /* ��������������ַ(д) */
        ack = IIC_Receive_Ack();                           /* ���մӻ�Ӧ���ź� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }
    }

    IIC_Send_Byte(address%256);   /* ���͵Ͱ�λ�洢��ַ(�ֵ�ַ) */
    ack = IIC_Receive_Ack();      /* ���մӻ�Ӧ���ź� */

    /* �ж�Ӧ�� */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    IIC_Send_Byte(data);          /* ������������ */
    ack = IIC_Receive_Ack();      /* ���մӻ�Ӧ���ź� */

    /* �ж�Ӧ�� */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    IIC_Stop();          /* ��������ֹͣ�ź� */
    delay_ms(6);         /* ��ʱ6ms */

    return AT24CXX_NO_ERR;
}



/**********************************************************
* @funcName ��AT24CXX_Read_Byte
* @brief    ����AT24CXX�����ȡ1���ֽڵ�����
* @param    ��uint16_t address (�洢��ַ)
* @param    ��uint8_t  *data   (����)
* @retval   ��uint8_t  ---  (�������)
* @details  ��
* @fn       ��
*           AT24CXX_NO_ERR       0      Ӧ����Ч   ���ݴ���ɹ�
*           AT24CXX_ERR1         1      Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*           AT24CXX_ERR2         2      Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*           AT24CXX_ERR3         3      Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*
*           AT24CXX_WRITE        0xa0   ������ַ   д����
*           AT24CXX_READ         0xa1   ������ַ   ������
************************************************************/
uint8_t AT24CXX_Read_Byte(uint16_t address,uint8_t *data)
{
    uint8_t ack;

    IIC_Start();                    /* ����������ʼ�ź� */

    /*�ж�AT24CXX�ͺ�*/
    if(AT24CXX > AT24C16)             /* �����AT24C16���ϵ��ͺ�(������AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE); /* ��������������ַ(д) */
        ack = IIC_Receive_Ack();      /* ���մӻ�Ӧ���ź� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }

        IIC_Send_Byte(address>>8);      /* ���͸߰�λ�洢��ַ(�ֵ�ַ) */
        ack = IIC_Receive_Ack();        /* ���մӻ�Ӧ���ź� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR2;
        }
    }
    else                              /* �����AT24C16���µ��ͺ�(����AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE+((address/256)<<1));      /* ��������������ַ(д) */
        ack = IIC_Receive_Ack();                              /* ���մӻ�Ӧ���ź� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }
    }

    IIC_Send_Byte(address%256);            /* ���͵Ͱ�λ�洢��ַ(�ֵ�ַ) */
    ack = IIC_Receive_Ack();               /* ���մӻ�Ӧ���ź� */

    /* �ж�Ӧ�� */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    IIC_Start();                    /* ����������ʼ�ź� */
    IIC_Send_Byte(AT24CXX_READ);    /* ��������������ַ(��) */
    ack = IIC_Receive_Ack();        /* ��������Ӧ�� */

    /* �ж�Ӧ�� */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    *data = IIC_Receive_Byte();       /* ������ʼ�������� */
    IIC_Send_Ack(1);                  /* �������Ͳ�Ӧ���ź� */
    IIC_Stop();                       /* ��������ֹͣ�ź� */

    return ack;
}



/**********************************************************
* @funcName ��AT24CXX_Write_Bytes1
* @brief    ��д����ֽڵ����ݵ�AT24CXX����ܿ�ҳ��
* @param    ��uint16_t address  (�洢��ַ)
* @param    ��uint8_t  byte_num (�����鳤��)
* @param    ��uint8_t  *arr     (������)
* @retval   ��uint8_t  ---  (�������)
* @details  ��
* @fn       ��
*           AT24CXX_NO_ERR       0      Ӧ����Ч   ���ݴ���ɹ�
*           AT24CXX_ERR1         1      Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*           AT24CXX_ERR2         2      Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*           AT24CXX_ERR3         3      Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*
*           AT24CXX_WRITE        0xa0   ������ַ   д����
*           AT24CXX_READ         0xa1   ������ַ   ������
************************************************************/
uint8_t AT24CXX_Write_Bytes1(uint16_t address, uint8_t byte_num, uint8_t *arr)
{
    uint8_t i;
    uint8_t ack;

    IIC_Start();                   /* ����������ʼ�ź� */

    /*�ж�AT24CXX�ͺ�*/
    if(AT24CXX > AT24C16)          /* �����AT24C16���ϵ��ͺ�(������AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE);  /* ��������������ַ(д) */
        ack = IIC_Receive_Ack();       /* ���մӻ�Ӧ���ź� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }

        IIC_Send_Byte(address>>8);        /* ���͸߰�λ�洢��ַ(�ֵ�ַ) */
        ack = IIC_Receive_Ack();          /* ���մӻ�Ӧ���ź� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR2;
        }
    }
    else                            /* �����AT24C16���µ��ͺ�(����AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE+((address/256)<<1));      /* ��������������ַ(д) */
        ack = IIC_Receive_Ack();                              /* ���մӻ�Ӧ���ź� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }
    }

    IIC_Send_Byte(address%256);     /* ���͵Ͱ�λ�洢��ַ(�ֵ�ַ) */
    ack = IIC_Receive_Ack();        /* ���մӻ�Ӧ���ź� */

    /* �ж�Ӧ�� */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    for(i=0;i<byte_num;i++)
    {
        IIC_Send_Byte(*arr);          /* ������������ */
        ack = IIC_Receive_Ack();      /* ��������Ӧ�� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            delay_ms(6);
            return AT24CXX_ERR3;
        }
        arr++;
    }

    IIC_Stop();        /* ��������ֹͣ�ź� */
    delay_ms(6);       /* ��ʱ6ms */

    return AT24CXX_NO_ERR;
}



/**********************************************************
* @funcName ��AT24CXX_Write_Bytes
* @brief    ��д����ֽڵ����ݵ�AT24CXX��ɿ�ҳ��
* @param    ��uint16_t address  (�洢��ַ)
* @param    ��uint8_t  byte_num (�����鳤��)
* @param    ��uint8_t  *arr     (������)
* @retval   ��uint8_t  ---  (�������)
* @details  ��
* @fn       ��
************************************************************/
uint8_t AT24CXX_Write_Bytes(uint16_t address, uint8_t byte_num, uint8_t *arr)
{
    uint8_t sck = 0;
    uint8_t remain;

    while(1)
    {
        /* ���㵱ǰҪ�洢��ҳ��ʣ�����ֽڿ���д */
        remain =AT24CXX_PAGE - address%AT24CXX_PAGE;
        if(remain >= byte_num)
        {
            sck = AT24CXX_Write_Bytes1(address, byte_num, arr);
            break;
        }
        else
        {
            sck = AT24CXX_Write_Bytes1(address, remain, arr);
            byte_num -= remain;   /* ��ȥд������� */
            address += remain;    /* ����洢�µ�ַ */
            arr += remain;        /* ����������µ�ַ */
        } 
    }
    return sck;
}



/**********************************************************
* @funcName ��AT24CXX_Read_Bytes
* @brief    ����AT24CXX�����ȡ����ֽڵ�����
* @param    ��uint16_t address  (�洢��ַ)
* @param    ��uint8_t  byte_num (�����鳤��)
* @param    ��uint8_t  *arr     (������)
* @retval   ��uint8_t  ---  (�������)
* @details  ��
* @fn       ��
*           AT24CXX_NO_ERR       0      Ӧ����Ч   ���ݴ���ɹ�
*           AT24CXX_ERR1         1      Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*           AT24CXX_ERR2         2      Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*           AT24CXX_ERR3         3      Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*
*           AT24CXX_WRITE        0xa0   ������ַ   д����
*           AT24CXX_READ         0xa1   ������ַ   ������
************************************************************/
uint8_t AT24CXX_Read_Bytes(uint16_t address, uint8_t byte_num, uint8_t *arr)
{
    uint8_t ack;

    IIC_Start();                       /* ����������ʼ�ź� */

    /*�ж�AT24CXX�ͺ�*/
    if(AT24CXX > AT24C16)              /* �����AT24C16���ϵ��ͺ�(������AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE);      /* ��������������ַ(д) */
        ack = IIC_Receive_Ack();           /* ���մӻ�Ӧ���ź� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }

        IIC_Send_Byte(address>>8);        /* ���͸߰�λ�洢��ַ(�ֵ�ַ) */
        ack = IIC_Receive_Ack();          /* ���մӻ�Ӧ���ź� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR2;
        }
    }
    else                            /* �����AT24C16���µ��ͺ�(����AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE+((address/256)<<1));      /* ��������������ַ(д) */
        ack = IIC_Receive_Ack();                              /* ���մӻ�Ӧ���ź� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }
    }

    IIC_Send_Byte(address%256);        /* ���͵Ͱ�λ�洢��ַ(�ֵ�ַ) */
    ack = IIC_Receive_Ack();           /* ���մӻ�Ӧ���ź� */

    /* �ж�Ӧ�� */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    IIC_Start();                    /* ����������ʼ�ź� */
    IIC_Send_Byte(AT24CXX_READ);    /* ��������������ַ(��) */
    ack = IIC_Receive_Ack();        /* ��������Ӧ�� */

    /* �ж�Ӧ�� */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    while(1)
    {
        *arr = IIC_Receive_Byte();         /* ������ʼ�������� */
        byte_num--;
        if(byte_num == 0)
        {
            IIC_Send_Ack(1);               /* ����Ӧ�� */
            break;
        }
        IIC_Send_Ack(0);
        arr++;
    }

    IIC_Stop();              /* ��������ֹͣ�ź� */

    return ack;
}



/**********************************************************
* @funcName ��AT24CXX_Write_Addr
* @brief    ��д����ֽڵ����ݵ�AT24CXX��(ֱ��һ��һ����ַд�����ÿ��ǿ粻��ҳ)
* @param    ��uint16_t address  (�洢��ַ)
* @param    ��uint8_t  byte_num (�����鳤��)
* @param    ��uint8_t  *arr     (������)
* @retval   ��uint8_t  ---  (�������)
* @details  ��
* @fn       ��
************************************************************/
uint8_t AT24CXX_Write_Addr(uint16_t address, uint8_t byte_num, uint8_t *arr)
{
    uint8_t i;
    uint8_t ack;

    for(i=0;i<byte_num;i++)
    {
        ack = AT24CXX_Write_Byte(address, *arr);
        address++;
        arr++;
    }

    return ack;
}



/**********************************************************
* @funcName ��AT24CXX_Read_Addr
* @brief    ����AT24CXX�����ȡ����ֽڵ�����(ֱ��һ��һ����ַд�����ÿ��ǿ粻��ҳ)
* @param    ��uint16_t address  (�洢��ַ)
* @param    ��uint8_t  byte_num (�����鳤��)
* @param    ��uint8_t  *arr     (������)
* @retval   ��uint8_t  ---  (�������)
* @details  ��
* @fn       ��
************************************************************/
uint8_t AT24CXX_Read_Addr(uint16_t address, uint8_t byte_num, uint8_t *arr)
{
    uint8_t i;
    uint8_t ack;

    for(i=0;i<byte_num;i++)
    {
        ack = AT24CXX_Read_Byte(address, arr);
        address++;
        arr++;
    }

    return ack;
}




/**********************************************************
* @funcName ��AT24CXXaddr_Write_Byte
* @brief    ��(���AT24CXX����ʱ)дһ���ֽڵ����ݵ�ĳ��AT24CXX��ĳ���ڲ���ַ��
* @param    ��uint16_t AT24CXXmodel (�����ͺ�)
* @param    ��uint8_t  AT24CXXaddr  (������ַ(д))
* @param    ��uint16_t address      (�洢��ַ)
* @param    ��uint8_t  data         (����)
* @retval   ��uint8_t  ---  (�������)
* @details  ��
* @fn       ��
*           AT24CXX_NO_ERR       0      Ӧ����Ч   ���ݴ���ɹ�
*           AT24CXX_ERR1         1      Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*           AT24CXX_ERR2         2      Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*           AT24CXX_ERR3         3      Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*
*           AT24CXX_WRITE        0xa0   ������ַ   д����
*           AT24CXX_READ         0xa1   ������ַ   ������
************************************************************/
uint8_t AT24CXXaddr_Write_Byte(uint16_t AT24CXXmodel, uint8_t AT24CXXaddr,uint16_t address, uint8_t data)
{
    uint8_t ack;

    IIC_Start();                    /* ����������ʼ�ź� */

    /* �ж�AT24CXX�ͺ� */
    if(AT24CXXmodel > AT24C16)      /* �����AT24C16���ϵ��ͺ�(������AT24C16) */
    {
        IIC_Send_Byte(AT24CXXaddr);   /* ��������������ַ(д) */
        ack = IIC_Receive_Ack();      /* ���մӻ�Ӧ���ź� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }

        IIC_Send_Byte(address>>8);        /* ���͸߰�λ�洢��ַ(�ֵ�ַ) */
        ack = IIC_Receive_Ack();          /* ���մӻ�Ӧ���ź� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR2;
        }
    }
    else                             /* �����AT24C16���µ��ͺ�(����AT24C16) */
    {
        IIC_Send_Byte(AT24CXXaddr+((address/256)<<1));    /* ��������������ַ(д) */
        ack = IIC_Receive_Ack();                          /* ���մӻ�Ӧ���ź� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }
    }

    IIC_Send_Byte(address%256);       /* ���͵Ͱ�λ�洢��ַ(�ֵ�ַ) */
    ack = IIC_Receive_Ack();          /* ���մӻ�Ӧ���ź� */

    /* �ж�Ӧ�� */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    IIC_Send_Byte(data);          /* ������������ */
    ack = IIC_Receive_Ack();      /* ���մӻ�Ӧ���ź� */

    /* �ж�Ӧ�� */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    IIC_Stop();      /* ��������ֹͣ�ź� */
    delay_ms(6);     /* ��ʱ6ms */

    return AT24CXX_NO_ERR;
}



/**********************************************************
* @funcName ��AT24CXXaddr_Read_Byte
* @brief    ��(���AT24CXX����ʱ)��ĳ��AT24CXX��ĳ���洢��ַ���ȡ1���ֽڵ�����
* @param    ��uint16_t AT24CXXmodel (�����ͺ�)
* @param    ��uint8_t  AT24CXXaddr  (������ַ(д))
* @param    ��uint16_t address      (�洢��ַ)
* @param    ��uint8_t  data         (����)
* @retval   ��uint8_t  ---  (�������)
* @details  ��
* @fn       ��
*           AT24CXX_NO_ERR       0      Ӧ����Ч   ���ݴ���ɹ�
*           AT24CXX_ERR1         1      Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*           AT24CXX_ERR2         2      Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*           AT24CXX_ERR3         3      Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*
*           AT24CXX_WRITE        0xa0   ������ַ   д����
*           AT24CXX_READ         0xa1   ������ַ   ������
************************************************************/
uint8_t AT24CXXaddr_Read_Byte(uint16_t AT24CXXmodel, uint8_t AT24CXXaddr, uint16_t address, uint8_t *data)
{
    uint8_t ack;

    IIC_Start();                        /* ����������ʼ�ź� */

    /* �ж�AT24CXX�ͺ� */
    if(AT24CXXmodel > AT24C16)          /* �����AT24C16���ϵ��ͺ�(������AT24C16) */
    {
        IIC_Send_Byte(AT24CXXaddr);     /* ��������������ַ(д) */
        ack = IIC_Receive_Ack();        /* ���մӻ�Ӧ���ź� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }

        IIC_Send_Byte(address>>8);      /* ���͸߰�λ�洢��ַ(�ֵ�ַ) */
        ack = IIC_Receive_Ack();        /* ���մӻ�Ӧ���ź� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR2;
        }
    }
    else                                /* �����AT24C16���µ��ͺ�(����AT24C16) */
    {
        IIC_Send_Byte(AT24CXXaddr+((address/256)<<1));      /* ��������������ַ(д) */
        ack = IIC_Receive_Ack();                            /* ���մӻ�Ӧ���ź� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }
    }

    IIC_Send_Byte(address%256);      /* ���͵Ͱ�λ�洢��ַ(�ֵ�ַ) */
    ack = IIC_Receive_Ack();         /* ���մӻ�Ӧ���ź� */

    /* �ж�Ӧ�� */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    IIC_Start();                         /* ����������ʼ�ź� */
    IIC_Send_Byte(AT24CXXaddr|0x1);      /* ��������������ַ(��) */
    ack = IIC_Receive_Ack();             /* ��������Ӧ�� */

    /* �ж�Ӧ�� */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    *data = IIC_Receive_Byte();         /* ������ʼ�������� */
    IIC_Send_Ack(1);                    /* �������Ͳ�Ӧ���ź� */
    IIC_Stop();                         /* ��������ֹͣ�ź� */

    return ack;
}



/**********************************************************
* @funcName ��AT24CXXaddr_Write_Bytes1
* @brief    ��(���AT24CXX)��ĳ��AT24CXX��ĳ���洢��ַ��ʼд����ֽڵ����ݵ������ܿ�ҳ��
* @param    ��uint16_t AT24CXXmodel (�����ͺ�)
* @param    ��uint8_t  AT24CXXaddr  (������ַ(д))
* @param    ��uint16_t address      (�洢��ַ)
* @param    ��uint8_t  byte_num     (�����鳤��)
* @param    ��uint8_t  *arr         (������)
* @retval   ��uint8_t  ---  (�������)
* @details  ��
* @fn       ��
*           AT24CXX_NO_ERR       0      Ӧ����Ч   ���ݴ���ɹ�
*           AT24CXX_ERR1         1      Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*           AT24CXX_ERR2         2      Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*           AT24CXX_ERR3         3      Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*
*           AT24CXX_WRITE        0xa0   ������ַ   д����
*           AT24CXX_READ         0xa1   ������ַ   ������
************************************************************/
uint8_t AT24CXXaddr_Write_Bytes1(uint16_t AT24CXXmodel, uint8_t AT24CXXaddr, uint16_t address, uint8_t byte_num, uint8_t *arr)
{
    uint8_t i;
    uint8_t ack;

    IIC_Start();                       /* ����������ʼ�ź� */

    /* �ж�AT24CXX�ͺ� */
    if(AT24CXXmodel > AT24C16)         /* �����AT24C16���ϵ��ͺ�(������AT24C16) */
    {
        IIC_Send_Byte(AT24CXXaddr);        /* ��������������ַ(д) */
        ack = IIC_Receive_Ack();           /* ���մӻ�Ӧ���ź� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }

        IIC_Send_Byte(address>>8);        /* ���͸߰�λ�洢��ַ(�ֵ�ַ) */
        ack = IIC_Receive_Ack();          /* ���մӻ�Ӧ���ź� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR2;
        }
    }
    else                              /* �����AT24C16���µ��ͺ�(����AT24C16) */
    {
        IIC_Send_Byte(AT24CXXaddr+((address/256)<<1));     /* ��������������ַ(д) */
        ack = IIC_Receive_Ack();                           /* ���մӻ�Ӧ���ź� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }
    }

    IIC_Send_Byte(address%256);      /* ���͵Ͱ�λ�洢��ַ(�ֵ�ַ) */
    ack = IIC_Receive_Ack();         /* ���մӻ�Ӧ���ź� */

    /* �ж�Ӧ�� */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    for(i=0;i<byte_num;i++)
    {
        IIC_Send_Byte(*arr);       /* ������������ */
        ack = IIC_Receive_Ack();   /* ��������Ӧ�� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            delay_ms(6);
            return AT24CXX_ERR3;
        }
        arr++;
    }

    IIC_Stop();       /* ��������ֹͣ�ź� */
    delay_ms(6);      /* ��ʱ6ms */

    return AT24CXX_NO_ERR;
}



/**********************************************************
* @funcName ��AT24CXXaddr_Write_Bytes
* @brief    ��(���AT24CXX)��ĳ��AT24CXX��ĳ���洢��ַ��ʼд����ֽڵ����ݵ����ɿ�ҳ��
* @param    ��uint16_t AT24CXXmodel (�����ͺ�)
* @param    ��uint8_t  AT24CXXaddr  (������ַ(д))
* @param    ��uint16_t address      (�洢��ַ)
* @param    ��uint8_t  byte_num     (�����鳤��)
* @param    ��uint8_t  *arr         (������)
* @retval   ��uint8_t  ---  (�������)
* @details  ��
* @fn       ��
************************************************************/
uint8_t AT24CXXaddr_Write_Bytes(uint16_t AT24CXXmodel, uint8_t AT24CXXaddr, uint8_t AT24CXXpage, uint16_t address, uint8_t byte_num, uint8_t *arr)
{
    uint8_t sck = 0;
    uint8_t remain;

    while(1)
    {
        /* ���㵱ǰҪ�洢��ҳ��ʣ�����ֽڿ���д */
        remain =AT24CXXpage - address%AT24CXXpage;
        if(remain >= byte_num)
        {
            sck = AT24CXXaddr_Write_Bytes1(AT24CXXmodel,AT24CXXaddr,address,byte_num,arr);
            break;
        }
        else
        {
            sck = AT24CXXaddr_Write_Bytes1(AT24CXXmodel,AT24CXXaddr,address,remain,arr);
            byte_num -= remain;   /* ��ȥд������� */
            address += remain;    /* ����洢�µ�ַ */
            arr += remain;        /* ����������µ�ַ */
        } 
    }
    return sck;
}



/**********************************************************
* @funcName ��AT24CXXaddr_Read_Bytes
* @brief    ��(���AT24CXX)��ĳ��AT24CXX��ĳ����ַ��ʼ��ȡ����ֽڵ�����
* @param    ��uint16_t AT24CXXmodel (�����ͺ�)
* @param    ��uint8_t  AT24CXXaddr  (������ַ(д))
* @param    ��uint16_t address      (�洢��ַ)
* @param    ��uint8_t  byte_num     (�����鳤��)
* @param    ��uint8_t  *arr         (������)
* @retval   ��uint8_t  ---  (�������)
* @details  ��
* @fn       ��
*           AT24CXX_NO_ERR       0      Ӧ����Ч   ���ݴ���ɹ�
*           AT24CXX_ERR1         1      Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*           AT24CXX_ERR2         2      Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*           AT24CXX_ERR3         3      Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*
*           AT24CXX_WRITE        0xa0   ������ַ   д����
*           AT24CXX_READ         0xa1   ������ַ   ������
************************************************************/
uint8_t AT24CXXaddr_Read_Bytes(uint16_t AT24CXXmodel, uint8_t AT24CXXaddr, uint16_t address, uint8_t byte_num, uint8_t *arr)
{
    uint8_t ack;

    IIC_Start();                  /* ����������ʼ�ź� */

    /* �ж�AT24CXX�ͺ� */
    if(AT24CXXmodel > AT24C16)    /* �����AT24C16���ϵ��ͺ�(������AT24C16) */
    {
        IIC_Send_Byte(AT24CXXaddr); /* ��������������ַ(д) */
        ack = IIC_Receive_Ack();    /* ���մӻ�Ӧ���ź� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }

        IIC_Send_Byte(address>>8);   /* ���͸߰�λ�洢��ַ(�ֵ�ַ) */
        ack = IIC_Receive_Ack();     /* ���մӻ�Ӧ���ź� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR2;
        }
    }
    else                           /* �����AT24C16���µ��ͺ�(����AT24C16) */
    {
        IIC_Send_Byte(AT24CXXaddr+((address/256)<<1));     /* ��������������ַ(д) */
        ack = IIC_Receive_Ack();                           /* ���մӻ�Ӧ���ź� */

        /* �ж�Ӧ�� */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }
    }

    IIC_Send_Byte(address%256);        /* ���͵Ͱ�λ�洢��ַ(�ֵ�ַ) */
    ack = IIC_Receive_Ack();           /* ���մӻ�Ӧ���ź� */

    /* �ж�Ӧ�� */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    IIC_Start();                    /* ����������ʼ�ź� */
    IIC_Send_Byte(AT24CXXaddr|1);   /* ��������������ַ(��) */
    ack = IIC_Receive_Ack();        /* ��������Ӧ�� */

    /* �ж�Ӧ�� */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    while(1)
    {
        *arr = IIC_Receive_Byte();        /* ������ʼ�������� */
        byte_num--;

        if(byte_num == 0)
        {
            IIC_Send_Ack(1);        /* ����Ӧ�� */
            break;
        }

        IIC_Send_Ack(0);
        arr++;
    }

    IIC_Stop();   /* ��������ֹͣ�ź� */

    return ack;
}



/**********************************************************
* @funcName ��AT24CXXaddr_Write_Addr
* @brief    ��(���AT24CXX)д����ֽڵ����ݵ�AT24CXX��(ֱ��һ��һ����ַд�����ÿ��ǿ粻��ҳ)
* @param    ��uint16_t AT24CXXmodel (�����ͺ�)
* @param    ��uint8_t  AT24CXXaddr  (������ַ(д))
* @param    ��uint16_t address      (�洢��ַ)
* @param    ��uint8_t  byte_num     (�����鳤��)
* @param    ��uint8_t  *arr         (������)
* @retval   ��uint8_t  ---  (�������)
* @details  ��
* @fn       ��
************************************************************/
uint8_t AT24CXXaddr_Write_Addr(uint16_t AT24CXXmodel, uint8_t AT24CXXaddr, uint16_t address, uint8_t byte_num, uint8_t *arr)
{
    uint8_t i;
    uint8_t ack;
    for(i=0;i<byte_num;i++)
    {
        ack = AT24CXXaddr_Write_Byte(AT24CXXmodel,AT24CXXaddr,address,*arr);
        address++;
        arr++;
    }
    return ack;
}



/**********************************************************
* @funcName ��AT24CXXaddr_Read_Addr
* @brief    ����AT24CXX�����ȡ����ֽڵ�����(ֱ��һ��һ����ַд�����ÿ��ǿ粻��ҳ)
* @param    ��uint16_t AT24CXXmodel (�����ͺ�)
* @param    ��uint8_t  AT24CXXaddr  (������ַ(д))
* @param    ��uint16_t address      (�洢��ַ)
* @param    ��uint8_t  byte_num     (�����鳤��)
* @param    ��uint8_t  *arr         (������)
* @retval   ��uint8_t  ---  (�������)
* @details  ��
* @fn       ��
************************************************************/
uint8_t AT24CXXaddr_Read_Addr(uint16_t AT24CXXmodel, uint8_t AT24CXXaddr, uint16_t address, uint8_t byte_num, uint8_t *arr)
{
    uint8_t i;
    uint8_t ack;

    for(i=0;i<byte_num;i++)
    {
        ack = AT24CXXaddr_Read_Byte(AT24CXXmodel,AT24CXXaddr,address,arr);
        address++;
        arr++;
    }
    return ack;
}







