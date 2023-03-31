#include "./BSP/inc/at24cxx.h"
#include "./BSP/inc/iic.h"


/**********************************************************
*@funcName ��AT24C04_Init
*@brief    ����at24cxx����Ҫ��GPIO�ڽ��г�ʼ������
*@param    ��void(��)
*@retval   ��void(��)
*@details  ��
*          ʱ����  IIC_SCL  PB8   �������
*          ������  IIC_SDA  PB9   ��©���
************************************************************/
void AT24CXX_Init(void)
{
  IIC_Init();
}



/**********************************************************
*@funcName ��AT24CXX_Read_Byte
*@brief    ����AT24CXX�����ȡ1���ֽڵ�����
*@param    ��address---��ʼ��ַ   *data---���ڽ������ݵı����ĵ�ַ
*@retval   ����ȡ�����Ƿ�ɹ�
*@details  ��
*     0      AT24CXX_NO_ERR    //Ӧ����Ч   ���ݴ���ɹ�
*     1      AT24CXX_ERR1      //Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*     2      AT24CXX_ERR2      //Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*     3      AT24CXX_ERR3      //Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*     0xa0   AT24CXX_WRITE     //������ַ   д����
*     0xa1   AT24CXX_READ      //������ַ   ������
*
*  ���ݲ�ͬ��24CXX�ͺ�, ���͸�λ��ַ
*  1, 24C16���ϵ��ͺ�, ��2���ֽڷ��͵�ַ
*  2, 24C16�����µ��ͺ�, ��1�����ֽڵ�ַ + ռ��������ַ��bit1~bit3λ ���ڱ�ʾ��λ��ַ, ���11λ��ַ
*    ����24C01/02, ��������ַ��ʽ(8bit)Ϊ: 1  0  1  0  A2  A1  A0  R/W
*    ����24C04,    ��������ַ��ʽ(8bit)Ϊ: 1  0  1  0  A2  A1  a8  R/W
*    ����24C08,    ��������ַ��ʽ(8bit)Ϊ: 1  0  1  0  A2  a9  a8  R/W
*    ����24C16,    ��������ַ��ʽ(8bit)Ϊ: 1  0  1  0  a10 a9  a8  R/W
*    R/W      : ��/д����λ 0,��ʾд; 1,��ʾ��;
*    A0/A1/A2 : ��Ӧ������1,2,3����(ֻ��24C01/02/04/8����Щ��)
*    a8/a9/a10: ��Ӧ�洢���еĸ�λ��ַ, 11bit��ַ�����Ա�ʾ2048��λ��,����Ѱַ24C16�����ڵ��ͺ�
************************************************************/
uint8_t AT24CXX_Read_Byte(uint16_t address, uint8_t *data)
{
//    uint8_t ack;
  
    IIC_Start(); /* ����������ʼ�ź� */
    
    /* �ж�AT24CXX�ͺ� */
    if(AT24CXX > AT24C16)              /* �����AT24C16���ϵ��ͺ�(������AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE);  /* ��������������ַ(д) */
        IIC_Wait_Ack();                /* ���մӻ�Ӧ���ź� */
        
//        ack = IIC_Wait_Ack();          /* ���մӻ�Ӧ���ź� */
//        if(ack != 0)                   /* �ж�Ӧ�� */
//        {
//            IIC_Stop();
//            return AT24CXX_ERR1;
//        }
    
        IIC_Send_Byte(address>>8);     /* ���͸߰�λ�洢��ַ(�ֵ�ַ) */
        IIC_Wait_Ack();                /* ���մӻ�Ӧ���ź� */
        
//        ack = IIC_Wait_Ack();          /* ���մӻ�Ӧ���ź� */
//        if(ack != 0)                   /* �ж�Ӧ�� */
//        {
//            IIC_Stop();
//            return AT24CXX_ERR2;
//        }
    }
    else                                                  /* �����AT24C16���µ��ͺ�(����AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE+((address>>8)<<1));   /* ��������������ַ(д) */
        IIC_Wait_Ack();                                   /* ���մӻ�Ӧ���ź� */
        
//        ack = IIC_Wait_Ack();                             /* ���մӻ�Ӧ���ź� */
//        /* �ж�Ӧ�� */
//        if(ack != 0)
//        {
//            IIC_Stop();
//            return AT24CXX_ERR1;
//        }
    }
    IIC_Send_Byte(address&0xFF);  /* ���͵Ͱ�λ�洢��ַ(�ֵ�ַ) */
    IIC_Wait_Ack();               /* ���մӻ�Ӧ���ź� */
    
//    ack = IIC_Wait_Ack();         /* ���մӻ�Ӧ���ź� */
//    if(ack != 0)                  /* �ж�Ӧ�� */
//    {
//        IIC_Stop();
//        return AT24CXX_ERR2;
//    }  

    IIC_Start();                  /* ����������ʼ�ź� */
    IIC_Send_Byte(AT24CXX_READ);  /* ��������������ַ(��) */
    IIC_Wait_Ack();               /* ���մӻ�Ӧ���ź� */
    
//    ack = IIC_Wait_Ack();         /* ��������Ӧ�� */
//    if(ack != 0)                  /* �ж�Ӧ�� */
//    {
//        IIC_Stop();
//        return AT24CXX_ERR2;
//    }
 
    *data = IIC_Receive_Byte(1);  /* ������ʼ��������,���Ҳ�Ӧ�� */
    IIC_Stop();                   /* ��������ֹͣ�ź� */

    return AT24CXX_NO_ERR;
}


/**********************************************************
*@funcName ��AT24CXX_Write_Byte
*@brief    ��дһ���ֽڵ����ݵ�AT24CXX��ĳ���ڲ���ַ��
*@param    ��address---��ʼ��ַ   data---Ҫ�洢������
*@retval   ��д������Ƿ�ɹ�
*@details  ��
*     0      AT24CXX_NO_ERR    //Ӧ����Ч   ���ݴ���ɹ�
*     1      AT24CXX_ERR1      //Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*     2      AT24CXX_ERR2      //Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*     3      AT24CXX_ERR3      //Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*     0xa0   AT24CXX_WRITE     //������ַ   д����
*     0xa1   AT24CXX_READ      //������ַ   ������
**********************************************************/
uint8_t AT24CXX_Write_Byte(uint16_t address, uint8_t data)
{
//    uint8_t ack;

    IIC_Start();                         /* ����������ʼ�ź� */
  
    /*�ж�AT24CXX�ͺ�*/
    if(AT24CXX > AT24C16)                /* �����AT24C16���ϵ��ͺ�(������AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE);      /* ��������������ַ(д) */
        IIC_Wait_Ack();                    /* ���մӻ�Ӧ���ź� */
        
//        ack = IIC_Wait_Ack();              /* ���մӻ�Ӧ���ź� */
//        if(ack != 0)                       /* �ж�Ӧ�� */
//        {
//            IIC_Stop();
//            return AT24CXX_ERR1;
//        }
   
        IIC_Send_Byte(address>>8);          /* ���͸߰�λ�洢��ַ(�ֵ�ַ) */
        IIC_Wait_Ack();                     /* ���մӻ�Ӧ���ź� */
        
//        ack = IIC_Wait_Ack();               /* ���մӻ�Ӧ���ź� */
//        if(ack != 0)                        /* �ж�Ӧ�� */
//        {
//            IIC_Stop();
//            return AT24CXX_ERR2;
//        }
    }
    else                                                     /* �����AT24C16���µ��ͺ�(����AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE+((address>>8)<<1));      /* ��������������ַ(д) */
        IIC_Wait_Ack();                                      /* ���մӻ�Ӧ���ź� */
        
//        ack = IIC_Wait_Ack();                                /* ���մӻ�Ӧ���ź� */
//        if(ack != 0)                                         /* �ж�Ӧ�� */
//        {
//            IIC_Stop();
//            return AT24CXX_ERR1;
//        }
    }
  
    IIC_Send_Byte(address&0xFF);        /* ���͵Ͱ�λ�洢��ַ(�ֵ�ַ) */
    IIC_Wait_Ack();                     /* ���մӻ�Ӧ���ź� */
    
//    ack = IIC_Wait_Ack();               /* ���մӻ�Ӧ���ź� */
//    if(ack != 0)                        /* �ж�Ӧ�� */
//    {
//        IIC_Stop();
//        return AT24CXX_ERR2;
//    }
 
    IIC_Send_Byte(data);                /* ������������ */
    IIC_Wait_Ack();                     /* ���մӻ�Ӧ���ź� */
    
//    ack = IIC_Wait_Ack();               /* ���մӻ�Ӧ���ź� */
//    if(ack != 0)                        /* �ж�Ӧ�� */
//    {
//        IIC_Stop();
//        return AT24CXX_ERR2;
//    }
  
    IIC_Stop();                         /* ��������ֹͣ�ź� */
    delay_ms(10);                       /* ע��: EEPROM д��Ƚ���,����ȵ�10ms����д��һ���ֽ� */

    return AT24CXX_NO_ERR;
}


/**********************************************************
*@funcName ��AT24CXX_Check
*@brief    �����AT24CXXӲ���Ƿ�����
*@param    ��void(��)
*@retval   ��0---����  1---������
*@details  ��
*     0      AT24CXX_NO_ERR    //Ӧ����Ч   ���ݴ���ɹ�
*     1      AT24CXX_ERR1      //Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*     2      AT24CXX_ERR2      //Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*     3      AT24CXX_ERR3      //Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
**********************************************************/
uint8_t AT24CXX_Check(void)
{
    uint8_t temp = 0;
    uint8_t data = 0x66;

    AT24CXX_Read_Byte(AT24CXX, &temp);        /* ����ÿ�ο�����дAT24CXX */

    if (temp == 0X55)                      /* ��ȡ�������� */
    {
        return 0;
    }
    else                                   /* �ų���һ�γ�ʼ������� */
    {
        AT24CXX_Write_Byte(AT24CXX, data);    /* ��д������ */
        AT24CXX_Read_Byte(AT24CXX, &temp);    /* �ٶ�ȡ���� */

        if (temp == 0X55)return 0;
    }

    return 1;
}


/**********************************************************
*@funcName ��AT24CXX_Read_Bytes
*@brief    ����AT24CXX�����ȡ����ֽڵ�����
*@param    ��address---д���ַ   num---д���ֽڸ���  pbuf---Ҫд�������
*@retval   ����������ֽ��Ƿ�ɹ�
*@details  ��
*     0      AT24CXX_NO_ERR    //Ӧ����Ч   ���ݴ���ɹ�
*     1      AT24CXX_ERR1      //Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*     2      AT24CXX_ERR2      //Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*     3      AT24CXX_ERR3      //Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*     4      AT24CXX_ERR4      //Ӧ��ʧЧ   ���ֿ�ҳ����
*     0xa0   AT24CXX_WRITE     //������ַ   д����
*     0xa1   AT24CXX_READ      //������ַ   ������
**********************************************************/
uint8_t AT24CXX_Read_Bytes(uint16_t address, uint8_t num, uint8_t *pbuf)
{
//    uint8_t ack;

    IIC_Start();                         /* ����������ʼ�ź� */
  
    /*�ж�AT24CXX�ͺ�*/ 
    if(AT24CXX > AT24C16)                /* �����AT24C16���ϵ��ͺ�(������AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE);      /* ��������������ַ(д) */
        IIC_Wait_Ack();                    /* ���մӻ�Ӧ���ź� */
        
//        ack = IIC_Wait_Ack();              /* ���մӻ�Ӧ���ź� */
//        if(ack != 0)                       /* �ж�Ӧ�� */
//        {
//            IIC_Stop();
//            return AT24CXX_ERR1;
//        }
  
        IIC_Send_Byte(address>>8);          /* ���͸߰�λ�洢��ַ(�ֵ�ַ) */
        IIC_Wait_Ack();                     /* ���մӻ�Ӧ���ź� */
        
//        ack = IIC_Wait_Ack();               /* ���մӻ�Ӧ���ź� */
//        if(ack != 0)                        /* �ж�Ӧ�� */
//        {
//            IIC_Stop();
//            return AT24CXX_ERR2;
//        }
    }
    else                                                  /* �����AT24C16���µ��ͺ�(����AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE+((address>>8)<<1));   /* ��������������ַ(д) */
        IIC_Wait_Ack();                                   /* ���մӻ�Ӧ���ź� */
        
//        ack = IIC_Wait_Ack();                             /* ���մӻ�Ӧ���ź� */
//        if(ack != 0)                                      /* �ж�Ӧ�� */
//        {
//            IIC_Stop();
//            return AT24CXX_ERR1;
//        }
    }

    IIC_Send_Byte(address&0xFF);               /* ���͵Ͱ�λ�洢��ַ(�ֵ�ַ) */
    IIC_Wait_Ack();                            /* ���մӻ�Ӧ���ź� */
    
//    ack = IIC_Wait_Ack();                      /* ���մӻ�Ӧ���ź� */
//    if(ack != 0)                               /* �ж�Ӧ�� */
//    {
//        IIC_Stop();
//        return AT24CXX_ERR2;
//    }  
 
    IIC_Start();                               /* ����������ʼ�ź� */
    IIC_Send_Byte(AT24CXX_READ);               /* ��������������ַ(��) */
    IIC_Wait_Ack();                            /* ���մӻ�Ӧ���ź� */
    
//    ack = IIC_Wait_Ack();                      /* ��������Ӧ�� */
//    if(ack != 0)                               /* �ж�Ӧ�� */
//    {
//        IIC_Stop();
//        return AT24CXX_ERR2;
//    }
  
    while(1)
    {
        num--;
        if(num == 0)
        {
            *pbuf = IIC_Receive_Byte(1);       /* ������ʼ��������,����Ӧ�� */          
            break;
        }
        IIC_Receive_Byte(0);                   /* ������ʼ��������,��Ӧ�� */
        pbuf++;
    }
  
    IIC_Stop();                               /* ��������ֹͣ�ź� */

    return AT24CXX_NO_ERR;
}


/**********************************************************
*@funcName ��AT24CXX_Write_Bytes1
*@brief    ��д����ֽڵ����ݵ�AT24CXX����ܿ�ҳ��
*@param    ��address---д���ַ   num---д���ֽڸ���  pbuf---Ҫд�������
*@retval   ��д�����ֽ��Ƿ�ɹ�
*@details  ��
*     0      AT24CXX_NO_ERR    //Ӧ����Ч   ���ݴ���ɹ�
*     1      AT24CXX_ERR1      //Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*     2      AT24CXX_ERR2      //Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*     3      AT24CXX_ERR3      //Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*     4      AT24CXX_ERR4      //Ӧ��ʧЧ   ���ֿ�ҳ����
*     0xa0   AT24CXX_WRITE     //������ַ   д����
*     0xa1   AT24CXX_READ      //������ַ   ������
**********************************************************/
uint8_t AT24CXX_Write_Bytes1(uint16_t address, uint8_t num, uint8_t *pbuf)
{
    uint8_t i;
//    uint8_t ack;
  
    IIC_Start();                          /* ����������ʼ�ź� */
    
    /*�ж�AT24CXX�ͺ�*/
    if(AT24CXX > AT24C16)                 /* �����AT24C16���ϵ��ͺ�(������AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE);     /* ��������������ַ(д) */
        IIC_Wait_Ack();                   /* ���մӻ�Ӧ���ź� */
        
//        ack = IIC_Wait_Ack();             /* ���մӻ�Ӧ���ź� */
//        if(ack != 0)                      /* �ж�Ӧ�� */
//        {
//            IIC_Stop();
//            return AT24CXX_ERR1;
//        }
   
        IIC_Send_Byte(address>>8);        /* ���͸߰�λ�洢��ַ(�ֵ�ַ) */
        IIC_Wait_Ack();                   /* ���մӻ�Ӧ���ź� */
        
//        ack = IIC_Wait_Ack();             /* ���մӻ�Ӧ���ź� */
//        if(ack != 0)                      /* �ж�Ӧ�� */
//        {
//            IIC_Stop();
//            return AT24CXX_ERR2;
//        }
    }
    else                                                 /* �����AT24C16���µ��ͺ�(����AT24C16) */
    {

        IIC_Send_Byte(AT24CXX_WRITE+((address>>8)<<1));  /* ��������������ַ(д) */
        IIC_Wait_Ack();                                  /* ���մӻ�Ӧ���ź� */
        
//        ack = IIC_Wait_Ack();                            /* ���մӻ�Ӧ���ź� */
//        if(ack != 0)                                     /* �ж�Ӧ�� */
//        {
//            IIC_Stop();
//            return AT24CXX_ERR1;
//        }
    }
  
    IIC_Send_Byte(address&0xFF);  /* ���͵Ͱ�λ�洢��ַ(�ֵ�ַ) */
    IIC_Wait_Ack();               /* ���մӻ�Ӧ���ź� */
    
//    ack = IIC_Wait_Ack();         /* ���մӻ�Ӧ���ź� */
//    if(ack != 0)                  /* �ж�Ӧ�� */
//    {
//        IIC_Stop();
//        return AT24CXX_ERR2;
//    }
    
    for(i=0;i<num;i++)
    {
        IIC_Send_Byte(*pbuf);       /* ������������ */
        IIC_Wait_Ack();             /* ���մӻ�Ӧ���ź� */
        
//        ack = IIC_Wait_Ack();      /* ��������Ӧ�� */
//        if(ack != 0)               /* �ж�Ӧ�� */
//        {
//            IIC_Stop();
//            delay_ms(10);
//            return AT24CXX_ERR3;
//        }
        pbuf++;
    }
    
    IIC_Stop();           /* ��������ֹͣ�ź� */
    delay_ms(10);         /* ע��: EEPROM д��Ƚ���,����ȵ�10ms����д��һ���ֽ� */

    return AT24CXX_NO_ERR;
}



/**********************************************************
*@funcName ��AT24CXX_Write_Bytes
*@brief    ��д����ֽڵ����ݵ�AT24CXX��ɿ�ҳ��
*@param    ��address---д���ַ   num---д���ֽڸ���  pbuf---Ҫд�������
*@retval   ��д�����ֽ��Ƿ�ɹ�
*@details  ��
*     0      AT24CXX_NO_ERR    //Ӧ����Ч   ���ݴ���ɹ�
*     1      AT24CXX_ERR1      //Ӧ��ʧЧ   ������ַ���ݴ���ʧ��
*     2      AT24CXX_ERR2      //Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ��
*     3      AT24CXX_ERR3      //Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ��
*     4      AT24CXX_ERR4      //Ӧ��ʧЧ   ���ֿ�ҳ����
*     0xa0   AT24CXX_WRITE     //������ַ   д����
*     0xa1   AT24CXX_READ      //������ַ   ������
**********************************************************/
uint8_t AT24CXX_Write_Bytes(uint16_t address, uint8_t num, uint8_t *pbuf)
{
  uint8_t sck = 0, remain;
  
  while(1)
  {
    remain =AT24CXX_PAGE - address%AT24CXX_PAGE;        /* ���㵱ǰҪ�洢��ҳ��ʣ�����ֽڿ���д */
    if(remain >= num)
    {
      sck = AT24CXX_Write_Bytes1(address,num,pbuf);     /* д������ */
      break;
    }
    else
    {
      sck = AT24CXX_Write_Bytes1(address,remain,pbuf);  /* д������ */
      num -= remain;                                   /* ��ȥд������� */
      address += remain;                               /* ����洢�µ�ַ */
      pbuf += remain;                                   /* ����������µ�ַ */
    } 
  }
  return sck;
}





/**********************************************************
*@funcName ��AT24CXX_Read
*@brief    ����AT24CXX�����ȡ����ֽڵ�����(ֱ��һ��һ����ַд�����ÿ��ǿ粻��ҳ)
*@param    ��address---д���ַ   datalen---д���ֽڸ���  pbuf---Ҫд�������
*@retval   ����������ֽ��Ƿ�ɹ�
*@details  ��
**********************************************************/
void AT24CXX_Read(uint16_t address, uint16_t datalen, uint8_t *pbuf)
{
    while (datalen--)
    {
        AT24CXX_Read_Byte(address, pbuf);
        address++; pbuf++;
    }
}


/**********************************************************
*@funcName ��AT24CXX_Write
*@brief    ��д����ֽڵ����ݵ�AT24CXX��(ֱ��һ��һ����ַд�����ÿ��ǿ粻��ҳ)
*@param    ��address---д���ַ   datalen---д���ֽڸ���  pbuf---Ҫд�������
*@retval   ����������ֽ��Ƿ�ɹ�
*@details  ��
**********************************************************/
void AT24CXX_Write(uint16_t address, uint16_t datalen, uint8_t *pbuf)
{
    while (datalen--)
    {
        AT24CXX_Write_Byte(address, *pbuf);
        address++; pbuf++;
    }
}



