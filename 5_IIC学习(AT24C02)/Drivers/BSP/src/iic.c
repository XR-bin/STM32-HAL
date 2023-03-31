#include "./BSP/inc/iic.h"

/**********************************************************
*@funcName ：IIC_Init
*@brief    ：对模拟IIC所需要的GPIO口进行初始化设置
*@param    ：void(无)
*@retval   ：void(无)
*@details  ：
*          时钟线  IIC_SCL  PB6   推挽输出
*          数据线  IIC_SDA  PB7   开漏输出
*                 高电平处于空闲状态
*
*    注：SDA引脚模式设置,开漏输出,上拉, 这样就不用再
*       设置IO方向了, 开漏输出的时候(=1), 也可以读取
*       外部信号的高低电平
************************************************************/
void IIC_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    IIC_SCL_GPIO_CLK_ENABLE();  /* SCL引脚时钟使能 */
    IIC_SDA_GPIO_CLK_ENABLE();  /* SDA引脚时钟使能 */
    
    //SCL引脚配置
    gpio_init_struct.Pin = IIC_SCL_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;        /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;      /* 高速 */
    HAL_GPIO_Init(IIC_SCL_GPIO_PORT, &gpio_init_struct);/* SCL引脚配置 */
    //SDA引脚配置
    gpio_init_struct.Pin = IIC_SDA_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;        /* 开漏输出 */
    HAL_GPIO_Init(IIC_SDA_GPIO_PORT, &gpio_init_struct);/* SDA引脚配置 */
    
    IIC_Stop();                                         /* 让IIC处于空闲 */
}


/**********************************************************
*@funcName ：iic_delay
*@brief    ：IIC专用延时函数，用于控制IIC读写速度
*@param    ：void(无)
*@retval   ：void(无)
*@details  ：
************************************************************/
static void iic_delay(void)
{
    delay_us(2);    /* 2us的延时, 读写速度在250Khz以内 */
}

/**********************************************************
*@funcName ：IIC_Start
*@brief    ：模拟IIC的起始信号
*@param    ：void(无)
*@retval   ：void(无)
*@details  ：
************************************************************/
void IIC_Start(void)
{
    /* 准备 */
    IIC_SDA(1);
    IIC_SCL(1);
    iic_delay();
    
    /* SDA在SCL高电平时，电平由高变低 */
    IIC_SDA(0); 
    iic_delay();
    
    /* 钳住I2C总线，准备发送或接收数据 */
    IIC_SCL(0);     
    iic_delay();
}

/**********************************************************
*@funcName ：IIC_Stop
*@brief    ：模拟IIC的停止信号
*@param    ：void(无)
*@retval   ：void(无)
*@details  ：
************************************************************/
void IIC_Stop(void)
{
    /* 准备 */
    IIC_SDA(0);  
    iic_delay();
    IIC_SCL(1);
    iic_delay();
    
    /* SDA在SCL高电平时，电平由低变高 */
    IIC_SDA(1);  
    iic_delay();
}

/**********************************************************
*@funcName ：IIC_Wait_Ack
*@brief    ：模拟IIC的主机接收应答信号
*@param    ：void(无)
*@retval   ：0---接收应答成功   1---接收应答失败
*@details  ：
************************************************************/
uint8_t IIC_Wait_Ack(void)
{
    uint8_t waittime = 0;
    uint8_t rack = 0;

    /* 准备 */
    IIC_SDA(1);     /* 主机释放SDA线(此时外部器件可以拉低SDA线) */
    iic_delay();
    IIC_SCL(1);     /* SCL=1, 此时从机可以返回ACK */
    iic_delay();

    /* 开始等待应答 */
    while (IIC_READ_SDA)    /* 等待应答 */
    {
        waittime++;
        /* 超时处理 */
        if (waittime > 250)
        {
            IIC_Stop();
            rack = 1;
            break;
        }
    }

    IIC_SCL(0);     /* SCL=0, 结束ACK检查 */
    iic_delay();
    return rack;
}


/**********************************************************
*@funcName ：IIC_Send_Ack
*@brief    ：模拟IIC的主机发送应答信号
*@param    ：ack---0应答 1不应答
*@retval   ：void(无)
*@details  ：
************************************************************/
void IIC_Send_Ack(uint8_t ack)
{
    /*判断是否要发送应答信号*/
    //发送不应答信号
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
*@funcName ：IIC_Send_Byte
*@brief    ：模拟IIC的主机发送一个字节的数据
*@param    ：data---一个字节数据
*@retval   ：void(无)
*@details  ：
*           先发高位，后发低位
************************************************************/
void IIC_Send_Byte(uint8_t data)
{
    uint8_t t;
    
    for (t = 0; t < 8; t++)
    {
        IIC_SDA((data & 0x80) >> 7);    /* 高位先发送 */
        iic_delay();
        IIC_SCL(1);
        iic_delay();
        IIC_SCL(0);
        data <<= 1;     /* 左移1位,用于下一次发送 */
    }
    IIC_SDA(1);         /* 发送完成, 主机释放SDA线 */
}

/**********************************************************
*@funcName ：IIC_Receive_Byte
*@brief    ：模拟IIC的主机接收一个字节的数据
*@param    ：ack---0应答 1不应答
*@retval   ：接收到的数据
*@details  ：
*           先收到高位，后收到低位
************************************************************/
uint8_t IIC_Receive_Byte(uint8_t ack)
{
    uint8_t i, receive = 0;

    for (i = 0; i < 8; i++ )    /* 接收1个字节数据 */
    {
        receive <<= 1;  /* 高位先输出,所以先收到的数据位要左移 */
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

