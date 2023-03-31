#include "./BSP/inc/iic.h"

/**********************************************************
*@funcName ��IIC_Init
*@brief    ����ģ��IIC����Ҫ��GPIO�ڽ��г�ʼ������
*@param    ��void(��)
*@retval   ��void(��)
*@details  ��
*          ʱ����  IIC_SCL  PB6   �������
*          ������  IIC_SDA  PB7   ��©���
*                 �ߵ�ƽ���ڿ���״̬
*
*    ע��SDA����ģʽ����,��©���,����, �����Ͳ�����
*       ����IO������, ��©�����ʱ��(=1), Ҳ���Զ�ȡ
*       �ⲿ�źŵĸߵ͵�ƽ
************************************************************/
void IIC_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    IIC_SCL_GPIO_CLK_ENABLE();  /* SCL����ʱ��ʹ�� */
    IIC_SDA_GPIO_CLK_ENABLE();  /* SDA����ʱ��ʹ�� */
    
    //SCL��������
    gpio_init_struct.Pin = IIC_SCL_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;        /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;      /* ���� */
    HAL_GPIO_Init(IIC_SCL_GPIO_PORT, &gpio_init_struct);/* SCL�������� */
    //SDA��������
    gpio_init_struct.Pin = IIC_SDA_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;        /* ��©��� */
    HAL_GPIO_Init(IIC_SDA_GPIO_PORT, &gpio_init_struct);/* SDA�������� */
    
    IIC_Stop();                                         /* ��IIC���ڿ��� */
}


/**********************************************************
*@funcName ��iic_delay
*@brief    ��IICר����ʱ���������ڿ���IIC��д�ٶ�
*@param    ��void(��)
*@retval   ��void(��)
*@details  ��
************************************************************/
static void iic_delay(void)
{
    delay_us(2);    /* 2us����ʱ, ��д�ٶ���250Khz���� */
}

/**********************************************************
*@funcName ��IIC_Start
*@brief    ��ģ��IIC����ʼ�ź�
*@param    ��void(��)
*@retval   ��void(��)
*@details  ��
************************************************************/
void IIC_Start(void)
{
    /* ׼�� */
    IIC_SDA(1);
    IIC_SCL(1);
    iic_delay();
    
    /* SDA��SCL�ߵ�ƽʱ����ƽ�ɸ߱�� */
    IIC_SDA(0); 
    iic_delay();
    
    /* ǯסI2C���ߣ�׼�����ͻ�������� */
    IIC_SCL(0);     
    iic_delay();
}

/**********************************************************
*@funcName ��IIC_Stop
*@brief    ��ģ��IIC��ֹͣ�ź�
*@param    ��void(��)
*@retval   ��void(��)
*@details  ��
************************************************************/
void IIC_Stop(void)
{
    /* ׼�� */
    IIC_SDA(0);  
    iic_delay();
    IIC_SCL(1);
    iic_delay();
    
    /* SDA��SCL�ߵ�ƽʱ����ƽ�ɵͱ�� */
    IIC_SDA(1);  
    iic_delay();
}

/**********************************************************
*@funcName ��IIC_Wait_Ack
*@brief    ��ģ��IIC����������Ӧ���ź�
*@param    ��void(��)
*@retval   ��0---����Ӧ��ɹ�   1---����Ӧ��ʧ��
*@details  ��
************************************************************/
uint8_t IIC_Wait_Ack(void)
{
    uint8_t waittime = 0;
    uint8_t rack = 0;

    /* ׼�� */
    IIC_SDA(1);     /* �����ͷ�SDA��(��ʱ�ⲿ������������SDA��) */
    iic_delay();
    IIC_SCL(1);     /* SCL=1, ��ʱ�ӻ����Է���ACK */
    iic_delay();

    /* ��ʼ�ȴ�Ӧ�� */
    while (IIC_READ_SDA)    /* �ȴ�Ӧ�� */
    {
        waittime++;
        /* ��ʱ���� */
        if (waittime > 250)
        {
            IIC_Stop();
            rack = 1;
            break;
        }
    }

    IIC_SCL(0);     /* SCL=0, ����ACK��� */
    iic_delay();
    return rack;
}


/**********************************************************
*@funcName ��IIC_Send_Ack
*@brief    ��ģ��IIC����������Ӧ���ź�
*@param    ��ack---0Ӧ�� 1��Ӧ��
*@retval   ��void(��)
*@details  ��
************************************************************/
void IIC_Send_Ack(uint8_t ack)
{
    /*�ж��Ƿ�Ҫ����Ӧ���ź�*/
    //���Ͳ�Ӧ���ź�
    if(ack)
    {
        IIC_SDA(1);
        iic_delay();
        IIC_SCL(1);
    }
    else 
    {
        IIC_SDA(0);
        iic_delay();
        IIC_SCL(1);
    }
    
    iic_delay();
    IIC_SCL(0);
    iic_delay();
}

/**********************************************************
*@funcName ��IIC_Send_Byte
*@brief    ��ģ��IIC����������һ���ֽڵ�����
*@param    ��data---һ���ֽ�����
*@retval   ��void(��)
*@details  ��
*           �ȷ���λ���󷢵�λ
************************************************************/
void IIC_Send_Byte(uint8_t data)
{
    uint8_t t;
    
    for (t = 0; t < 8; t++)
    {
        IIC_SDA((data & 0x80) >> 7);    /* ��λ�ȷ��� */
        iic_delay();
        IIC_SCL(1);
        iic_delay();
        IIC_SCL(0);
        data <<= 1;     /* ����1λ,������һ�η��� */
    }
    IIC_SDA(1);         /* �������, �����ͷ�SDA�� */
}

/**********************************************************
*@funcName ��IIC_Receive_Byte
*@brief    ��ģ��IIC����������һ���ֽڵ�����
*@param    ��ack---0Ӧ�� 1��Ӧ��
*@retval   �����յ�������
*@details  ��
*           ���յ���λ�����յ���λ
************************************************************/
uint8_t IIC_Receive_Byte(uint8_t ack)
{
    uint8_t i, receive = 0;

    for (i = 0; i < 8; i++ )    /* ����1���ֽ����� */
    {
        receive <<= 1;  /* ��λ�����,�������յ�������λҪ���� */
        IIC_SCL(1);
        iic_delay();

        if (IIC_READ_SDA)
        {
            receive++;
        }
        
        IIC_SCL(0);
        iic_delay();
    }

    IIC_Send_Ack(ack);

    return receive;
}

