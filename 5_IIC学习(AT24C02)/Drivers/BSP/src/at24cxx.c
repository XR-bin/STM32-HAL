#include "./BSP/inc/at24cxx.h"
#include "./BSP/inc/iic.h"


/**********************************************************
*@funcName ：AT24C04_Init
*@brief    ：对at24cxx所需要的GPIO口进行初始化设置
*@param    ：void(无)
*@retval   ：void(无)
*@details  ：
*          时钟线  IIC_SCL  PB8   推挽输出
*          数据线  IIC_SDA  PB9   开漏输出
************************************************************/
void AT24CXX_Init(void)
{
  IIC_Init();
}



/**********************************************************
*@funcName ：AT24CXX_Read_Byte
*@brief    ：从AT24CXX的里读取1个字节的数据
*@param    ：address---开始地址   *data---用于接收数据的变量的地址
*@retval   ：读取操作是否成功
*@details  ：
*     0      AT24CXX_NO_ERR    //应答有效   数据传输成功
*     1      AT24CXX_ERR1      //应答失效   器件地址数据传输失败
*     2      AT24CXX_ERR2      //应答失效   字节地址数据传输失败
*     3      AT24CXX_ERR3      //应答失效   要存储的数据传输失败
*     0xa0   AT24CXX_WRITE     //器件地址   写操作
*     0xa1   AT24CXX_READ      //器件地址   读操作
*
*  根据不同的24CXX型号, 发送高位地址
*  1, 24C16以上的型号, 分2个字节发送地址
*  2, 24C16及以下的型号, 分1个低字节地址 + 占用器件地址的bit1~bit3位 用于表示高位地址, 最多11位地址
*    对于24C01/02, 其器件地址格式(8bit)为: 1  0  1  0  A2  A1  A0  R/W
*    对于24C04,    其器件地址格式(8bit)为: 1  0  1  0  A2  A1  a8  R/W
*    对于24C08,    其器件地址格式(8bit)为: 1  0  1  0  A2  a9  a8  R/W
*    对于24C16,    其器件地址格式(8bit)为: 1  0  1  0  a10 a9  a8  R/W
*    R/W      : 读/写控制位 0,表示写; 1,表示读;
*    A0/A1/A2 : 对应器件的1,2,3引脚(只有24C01/02/04/8有这些脚)
*    a8/a9/a10: 对应存储整列的高位地址, 11bit地址最多可以表示2048个位置,可以寻址24C16及以内的型号
************************************************************/
uint8_t AT24CXX_Read_Byte(uint16_t address, uint8_t *data)
{
//    uint8_t ack;
  
    IIC_Start(); /* 主机发送起始信号 */
    
    /* 判断AT24CXX型号 */
    if(AT24CXX > AT24C16)              /* 如果是AT24C16以上的型号(不包括AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE);  /* 主机发送器件地址(写) */
        IIC_Wait_Ack();                /* 接收从机应答信号 */
        
//        ack = IIC_Wait_Ack();          /* 接收从机应答信号 */
//        if(ack != 0)                   /* 判断应答 */
//        {
//            IIC_Stop();
//            return AT24CXX_ERR1;
//        }
    
        IIC_Send_Byte(address>>8);     /* 发送高八位存储地址(字地址) */
        IIC_Wait_Ack();                /* 接收从机应答信号 */
        
//        ack = IIC_Wait_Ack();          /* 接收从机应答信号 */
//        if(ack != 0)                   /* 判断应答 */
//        {
//            IIC_Stop();
//            return AT24CXX_ERR2;
//        }
    }
    else                                                  /* 如果是AT24C16以下的型号(包括AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE+((address>>8)<<1));   /* 主机发送器件地址(写) */
        IIC_Wait_Ack();                                   /* 接收从机应答信号 */
        
//        ack = IIC_Wait_Ack();                             /* 接收从机应答信号 */
//        /* 判断应答 */
//        if(ack != 0)
//        {
//            IIC_Stop();
//            return AT24CXX_ERR1;
//        }
    }
    IIC_Send_Byte(address&0xFF);  /* 发送低八位存储地址(字地址) */
    IIC_Wait_Ack();               /* 接收从机应答信号 */
    
//    ack = IIC_Wait_Ack();         /* 接收从机应答信号 */
//    if(ack != 0)                  /* 判断应答 */
//    {
//        IIC_Stop();
//        return AT24CXX_ERR2;
//    }  

    IIC_Start();                  /* 主机发送起始信号 */
    IIC_Send_Byte(AT24CXX_READ);  /* 主机发送器件地址(读) */
    IIC_Wait_Ack();               /* 接收从机应答信号 */
    
//    ack = IIC_Wait_Ack();         /* 主机接收应答 */
//    if(ack != 0)                  /* 判断应答 */
//    {
//        IIC_Stop();
//        return AT24CXX_ERR2;
//    }
 
    *data = IIC_Receive_Byte(1);  /* 主机开始接收数据,并且不应答 */
    IIC_Stop();                   /* 主机发送停止信号 */

    return AT24CXX_NO_ERR;
}


/**********************************************************
*@funcName ：AT24CXX_Write_Byte
*@brief    ：写一个字节的数据到AT24CXX的某个内部地址里
*@param    ：address---开始地址   data---要存储的数据
*@retval   ：写入操作是否成功
*@details  ：
*     0      AT24CXX_NO_ERR    //应答有效   数据传输成功
*     1      AT24CXX_ERR1      //应答失效   器件地址数据传输失败
*     2      AT24CXX_ERR2      //应答失效   字节地址数据传输失败
*     3      AT24CXX_ERR3      //应答失效   要存储的数据传输失败
*     0xa0   AT24CXX_WRITE     //器件地址   写操作
*     0xa1   AT24CXX_READ      //器件地址   读操作
**********************************************************/
uint8_t AT24CXX_Write_Byte(uint16_t address, uint8_t data)
{
//    uint8_t ack;

    IIC_Start();                         /* 主机发送起始信号 */
  
    /*判断AT24CXX型号*/
    if(AT24CXX > AT24C16)                /* 如果是AT24C16以上的型号(不包括AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE);      /* 主机发送器件地址(写) */
        IIC_Wait_Ack();                    /* 接收从机应答信号 */
        
//        ack = IIC_Wait_Ack();              /* 接收从机应答信号 */
//        if(ack != 0)                       /* 判断应答 */
//        {
//            IIC_Stop();
//            return AT24CXX_ERR1;
//        }
   
        IIC_Send_Byte(address>>8);          /* 发送高八位存储地址(字地址) */
        IIC_Wait_Ack();                     /* 接收从机应答信号 */
        
//        ack = IIC_Wait_Ack();               /* 接收从机应答信号 */
//        if(ack != 0)                        /* 判断应答 */
//        {
//            IIC_Stop();
//            return AT24CXX_ERR2;
//        }
    }
    else                                                     /* 如果是AT24C16以下的型号(包括AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE+((address>>8)<<1));      /* 主机发送器件地址(写) */
        IIC_Wait_Ack();                                      /* 接收从机应答信号 */
        
//        ack = IIC_Wait_Ack();                                /* 接收从机应答信号 */
//        if(ack != 0)                                         /* 判断应答 */
//        {
//            IIC_Stop();
//            return AT24CXX_ERR1;
//        }
    }
  
    IIC_Send_Byte(address&0xFF);        /* 发送低八位存储地址(字地址) */
    IIC_Wait_Ack();                     /* 接收从机应答信号 */
    
//    ack = IIC_Wait_Ack();               /* 接收从机应答信号 */
//    if(ack != 0)                        /* 判断应答 */
//    {
//        IIC_Stop();
//        return AT24CXX_ERR2;
//    }
 
    IIC_Send_Byte(data);                /* 主机发送数据 */
    IIC_Wait_Ack();                     /* 接收从机应答信号 */
    
//    ack = IIC_Wait_Ack();               /* 接收从机应答信号 */
//    if(ack != 0)                        /* 判断应答 */
//    {
//        IIC_Stop();
//        return AT24CXX_ERR2;
//    }
  
    IIC_Stop();                         /* 主机发送停止信号 */
    delay_ms(10);                       /* 注意: EEPROM 写入比较慢,必须等到10ms后再写下一个字节 */

    return AT24CXX_NO_ERR;
}


/**********************************************************
*@funcName ：AT24CXX_Check
*@brief    ：检测AT24CXX硬件是否正常
*@param    ：void(无)
*@retval   ：0---正常  1---不正常
*@details  ：
*     0      AT24CXX_NO_ERR    //应答有效   数据传输成功
*     1      AT24CXX_ERR1      //应答失效   器件地址数据传输失败
*     2      AT24CXX_ERR2      //应答失效   字节地址数据传输失败
*     3      AT24CXX_ERR3      //应答失效   要存储的数据传输失败
**********************************************************/
uint8_t AT24CXX_Check(void)
{
    uint8_t temp = 0;
    uint8_t data = 0x66;

    AT24CXX_Read_Byte(AT24CXX, &temp);        /* 避免每次开机都写AT24CXX */

    if (temp == 0X55)                      /* 读取数据正常 */
    {
        return 0;
    }
    else                                   /* 排除第一次初始化的情况 */
    {
        AT24CXX_Write_Byte(AT24CXX, data);    /* 先写入数据 */
        AT24CXX_Read_Byte(AT24CXX, &temp);    /* 再读取数据 */

        if (temp == 0X55)return 0;
    }

    return 1;
}


/**********************************************************
*@funcName ：AT24CXX_Read_Bytes
*@brief    ：从AT24CXX的里读取多个字节的数据
*@param    ：address---写入地址   num---写入字节个数  pbuf---要写入的数据
*@retval   ：读出多个字节是否成功
*@details  ：
*     0      AT24CXX_NO_ERR    //应答有效   数据传输成功
*     1      AT24CXX_ERR1      //应答失效   器件地址数据传输失败
*     2      AT24CXX_ERR2      //应答失效   字节地址数据传输失败
*     3      AT24CXX_ERR3      //应答失效   要存储的数据传输失败
*     4      AT24CXX_ERR4      //应答失效   出现跨页错误
*     0xa0   AT24CXX_WRITE     //器件地址   写操作
*     0xa1   AT24CXX_READ      //器件地址   读操作
**********************************************************/
uint8_t AT24CXX_Read_Bytes(uint16_t address, uint8_t num, uint8_t *pbuf)
{
//    uint8_t ack;

    IIC_Start();                         /* 主机发送起始信号 */
  
    /*判断AT24CXX型号*/ 
    if(AT24CXX > AT24C16)                /* 如果是AT24C16以上的型号(不包括AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE);      /* 主机发送器件地址(写) */
        IIC_Wait_Ack();                    /* 接收从机应答信号 */
        
//        ack = IIC_Wait_Ack();              /* 接收从机应答信号 */
//        if(ack != 0)                       /* 判断应答 */
//        {
//            IIC_Stop();
//            return AT24CXX_ERR1;
//        }
  
        IIC_Send_Byte(address>>8);          /* 发送高八位存储地址(字地址) */
        IIC_Wait_Ack();                     /* 接收从机应答信号 */
        
//        ack = IIC_Wait_Ack();               /* 接收从机应答信号 */
//        if(ack != 0)                        /* 判断应答 */
//        {
//            IIC_Stop();
//            return AT24CXX_ERR2;
//        }
    }
    else                                                  /* 如果是AT24C16以下的型号(包括AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE+((address>>8)<<1));   /* 主机发送器件地址(写) */
        IIC_Wait_Ack();                                   /* 接收从机应答信号 */
        
//        ack = IIC_Wait_Ack();                             /* 接收从机应答信号 */
//        if(ack != 0)                                      /* 判断应答 */
//        {
//            IIC_Stop();
//            return AT24CXX_ERR1;
//        }
    }

    IIC_Send_Byte(address&0xFF);               /* 发送低八位存储地址(字地址) */
    IIC_Wait_Ack();                            /* 接收从机应答信号 */
    
//    ack = IIC_Wait_Ack();                      /* 接收从机应答信号 */
//    if(ack != 0)                               /* 判断应答 */
//    {
//        IIC_Stop();
//        return AT24CXX_ERR2;
//    }  
 
    IIC_Start();                               /* 主机发送起始信号 */
    IIC_Send_Byte(AT24CXX_READ);               /* 主机发送器件地址(读) */
    IIC_Wait_Ack();                            /* 接收从机应答信号 */
    
//    ack = IIC_Wait_Ack();                      /* 主机接收应答 */
//    if(ack != 0)                               /* 判断应答 */
//    {
//        IIC_Stop();
//        return AT24CXX_ERR2;
//    }
  
    while(1)
    {
        num--;
        if(num == 0)
        {
            *pbuf = IIC_Receive_Byte(1);       /* 主机开始接收数据,并不应答 */          
            break;
        }
        IIC_Receive_Byte(0);                   /* 主机开始接收数据,并应答 */
        pbuf++;
    }
  
    IIC_Stop();                               /* 主机发送停止信号 */

    return AT24CXX_NO_ERR;
}


/**********************************************************
*@funcName ：AT24CXX_Write_Bytes1
*@brief    ：写多个字节的数据到AT24CXX里（不能跨页）
*@param    ：address---写入地址   num---写入字节个数  pbuf---要写入的数据
*@retval   ：写入多个字节是否成功
*@details  ：
*     0      AT24CXX_NO_ERR    //应答有效   数据传输成功
*     1      AT24CXX_ERR1      //应答失效   器件地址数据传输失败
*     2      AT24CXX_ERR2      //应答失效   字节地址数据传输失败
*     3      AT24CXX_ERR3      //应答失效   要存储的数据传输失败
*     4      AT24CXX_ERR4      //应答失效   出现跨页错误
*     0xa0   AT24CXX_WRITE     //器件地址   写操作
*     0xa1   AT24CXX_READ      //器件地址   读操作
**********************************************************/
uint8_t AT24CXX_Write_Bytes1(uint16_t address, uint8_t num, uint8_t *pbuf)
{
    uint8_t i;
//    uint8_t ack;
  
    IIC_Start();                          /* 主机发送起始信号 */
    
    /*判断AT24CXX型号*/
    if(AT24CXX > AT24C16)                 /* 如果是AT24C16以上的型号(不包括AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE);     /* 主机发送器件地址(写) */
        IIC_Wait_Ack();                   /* 接收从机应答信号 */
        
//        ack = IIC_Wait_Ack();             /* 接收从机应答信号 */
//        if(ack != 0)                      /* 判断应答 */
//        {
//            IIC_Stop();
//            return AT24CXX_ERR1;
//        }
   
        IIC_Send_Byte(address>>8);        /* 发送高八位存储地址(字地址) */
        IIC_Wait_Ack();                   /* 接收从机应答信号 */
        
//        ack = IIC_Wait_Ack();             /* 接收从机应答信号 */
//        if(ack != 0)                      /* 判断应答 */
//        {
//            IIC_Stop();
//            return AT24CXX_ERR2;
//        }
    }
    else                                                 /* 如果是AT24C16以下的型号(包括AT24C16) */
    {

        IIC_Send_Byte(AT24CXX_WRITE+((address>>8)<<1));  /* 主机发送器件地址(写) */
        IIC_Wait_Ack();                                  /* 接收从机应答信号 */
        
//        ack = IIC_Wait_Ack();                            /* 接收从机应答信号 */
//        if(ack != 0)                                     /* 判断应答 */
//        {
//            IIC_Stop();
//            return AT24CXX_ERR1;
//        }
    }
  
    IIC_Send_Byte(address&0xFF);  /* 发送低八位存储地址(字地址) */
    IIC_Wait_Ack();               /* 接收从机应答信号 */
    
//    ack = IIC_Wait_Ack();         /* 接收从机应答信号 */
//    if(ack != 0)                  /* 判断应答 */
//    {
//        IIC_Stop();
//        return AT24CXX_ERR2;
//    }
    
    for(i=0;i<num;i++)
    {
        IIC_Send_Byte(*pbuf);       /* 主机发送数据 */
        IIC_Wait_Ack();             /* 接收从机应答信号 */
        
//        ack = IIC_Wait_Ack();      /* 主机接收应答 */
//        if(ack != 0)               /* 判断应答 */
//        {
//            IIC_Stop();
//            delay_ms(10);
//            return AT24CXX_ERR3;
//        }
        pbuf++;
    }
    
    IIC_Stop();           /* 主机发送停止信号 */
    delay_ms(10);         /* 注意: EEPROM 写入比较慢,必须等到10ms后再写下一个字节 */

    return AT24CXX_NO_ERR;
}



/**********************************************************
*@funcName ：AT24CXX_Write_Bytes
*@brief    ：写多个字节的数据到AT24CXX里（可跨页）
*@param    ：address---写入地址   num---写入字节个数  pbuf---要写入的数据
*@retval   ：写入多个字节是否成功
*@details  ：
*     0      AT24CXX_NO_ERR    //应答有效   数据传输成功
*     1      AT24CXX_ERR1      //应答失效   器件地址数据传输失败
*     2      AT24CXX_ERR2      //应答失效   字节地址数据传输失败
*     3      AT24CXX_ERR3      //应答失效   要存储的数据传输失败
*     4      AT24CXX_ERR4      //应答失效   出现跨页错误
*     0xa0   AT24CXX_WRITE     //器件地址   写操作
*     0xa1   AT24CXX_READ      //器件地址   读操作
**********************************************************/
uint8_t AT24CXX_Write_Bytes(uint16_t address, uint8_t num, uint8_t *pbuf)
{
  uint8_t sck = 0, remain;
  
  while(1)
  {
    remain =AT24CXX_PAGE - address%AT24CXX_PAGE;        /* 计算当前要存储的页还剩多少字节可以写 */
    if(remain >= num)
    {
      sck = AT24CXX_Write_Bytes1(address,num,pbuf);     /* 写入数据 */
      break;
    }
    else
    {
      sck = AT24CXX_Write_Bytes1(address,remain,pbuf);  /* 写入数据 */
      num -= remain;                                   /* 减去写入的数量 */
      address += remain;                               /* 计算存储新地址 */
      pbuf += remain;                                   /* 计算数组的新地址 */
    } 
  }
  return sck;
}





/**********************************************************
*@funcName ：AT24CXX_Read
*@brief    ：从AT24CXX的里读取多个字节的数据(直接一个一个地址写，不用考虑跨不跨页)
*@param    ：address---写入地址   datalen---写入字节个数  pbuf---要写入的数据
*@retval   ：读出多个字节是否成功
*@details  ：
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
*@funcName ：AT24CXX_Write
*@brief    ：写多个字节的数据到AT24CXX里(直接一个一个地址写，不用考虑跨不跨页)
*@param    ：address---写入地址   datalen---写入字节个数  pbuf---要写入的数据
*@retval   ：读出多个字节是否成功
*@details  ：
**********************************************************/
void AT24CXX_Write(uint16_t address, uint16_t datalen, uint8_t *pbuf)
{
    while (datalen--)
    {
        AT24CXX_Write_Byte(address, *pbuf);
        address++; pbuf++;
    }
}



