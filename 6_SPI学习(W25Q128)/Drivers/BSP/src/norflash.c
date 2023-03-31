#include "./BSP/inc/norflash.h"
#include "./BSP/inc/spi.h"

/*********************************************
*W25Q128存储芯片
*容量大小             ：16Mbyte  或  128Mbit
*共有块区             ：256块
*每块区共有扇区       ：16扇区
*每扇区共有页         ：16页
*每页字节             ：256字节
*地址可写最大十六进制数：0x9fffff
*        9f:块区   f：扇区    f：页    ff：字节
**********************************************/

uint16_t g_norflash_type = NM25Q128;     /* norFlash(W25QXXX)型号，默认是NM25Q128 */

/**********************************************************
*@funcName ：norFlash_init
*@brief    ：norFlash（W25QXXX）初始化
*@param    ：void(无)
*@retval   ：void(无)
*@details  ：
************************************************************/
void norFlash_Init(void)
{
    uint8_t temp;
    GPIO_InitTypeDef gpio_init_struct;

    NORFLASH_CS_GPIO_CLK_ENABLE();      /* NORFLASH CS脚 时钟使能 */
    
    gpio_init_struct.Pin = NORFLASH_CS_GPIO_PIN;         /* CS */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;         /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                 /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;       /* 高速 */
    HAL_GPIO_Init(NORFLASH_CS_GPIO_PORT, &gpio_init_struct); /* CS引脚模式设置(复用输出) */

    NORFLASH_CS(1);                     /* 取消片选 */
    
    SPI2_Init();                        /* 初始化SPI2 */
    SPI2_Set_Speed(SPI_SPEED_2);        /* SPI2 切换到高速状态 18Mhz */
    
    g_norflash_type = norFlash_Read_id();   /* 读取FLASH ID. */
    
    if (g_norflash_type == W25Q256)     /* SPI FLASH为W25Q256, 必须使能4字节地址模式 */
    {
        temp = norFlash_Read_SR(3);     /* 读取状态寄存器3，判断地址模式 */

        if ((temp & 0X01) == 0)         /* 如果不是4字节地址模式,则进入4字节地址模式 */
        {
            norFlash_Write_Enable();    /* 写使能 */
            temp |= 1 << 1;             /* ADP=1, 上电4位地址模式 */
            norFlash_Write_SR(3, temp); /* 写SR3 */
            
            NORFLASH_CS(0);
            SPI2_RW_Byte(FLASH_Enable4ByteAddr);    /* 使能4字节地址指令 */
            NORFLASH_CS(1);
        }
    }
    
    printf("ID:%x\r\n", g_norflash_type);
}

/**********************************************************
*@funcName ：norFlash_Read_id
*@brief    ：获取norFlash（W25QXXX）的ID，用于识别型号
*@param    ：void(无)
*@retval   ：norFlash（W25QXXX）的ID
*@details  ：
************************************************************/
uint16_t norFlash_Read_id(void)
{
    uint16_t deviceid;

    NORFLASH_CS(0);
    SPI2_RW_Byte(FLASH_ManufactDeviceID);   /* 发送读 ID 命令 */
    SPI2_RW_Byte(0);    /* 写入一个字节 */
    SPI2_RW_Byte(0);
    SPI2_RW_Byte(0);
    deviceid = SPI2_RW_Byte(0xFF) << 8;     /* 读取高8位字节 */
    deviceid |= SPI2_RW_Byte(0xFF);         /* 读取低8位字节 */
    NORFLASH_CS(1);

    return deviceid;
}


/**********************************************************
*@funcName ：norFlash_Read_SR
*@brief    ：读取25QXX的状态寄存器，25QXX一共有3个状态寄存器
*@param    ：regon---状态寄存器号，范围:1~3
*@retval   ：状态寄存器值
*@details  ：
*       状态寄存器1：
*              BIT7  6   5   4   3   2   1   0
*              SPR   RV  TB BP2 BP1 BP0 WEL BUSY
*              SPR:默认0,状态寄存器保护位,配合WP使用
*              TB,BP2,BP1,BP0:FLASH区域写保护设置
*              WEL:写使能锁定
*              BUSY:忙标记位(1,忙;0,空闲)
*              默认:0x00
*
*       状态寄存器2：
*              BIT7  6   5   4   3   2   1   0
*              SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
*
*       状态寄存器3：
*              BIT7      6    5    4   3   2   1   0
*              HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
*
*       更多信息，请自己查阅相关硬件资料
************************************************************/
uint8_t norFlash_Read_SR(uint8_t regon)
{
    uint8_t byte = 0, command = 0;

    switch (regon)
    {
        case 1:
            command = FLASH_ReadStatusReg1;  /* 读状态寄存器1指令 */
            break;

        case 2:
            command = FLASH_ReadStatusReg2;  /* 读状态寄存器2指令 */
            break;

        case 3:
            command = FLASH_ReadStatusReg3;  /* 读状态寄存器3指令 */
            break;

        default:
            command = FLASH_ReadStatusReg1;
            break;
    }
    
    NORFLASH_CS(0);
    SPI2_RW_Byte(command);      /* 发送读寄存器命令 */
    byte = SPI2_RW_Byte(0Xff);  /* 读取一个字节 */
    NORFLASH_CS(1);
    
    return byte;
}

/**********************************************************
*@funcName ：norFlash_Write_Enable
*@brief    ：25QXX写使能,将S1寄存器的WEL置位
*@param    ：void(无)
*@retval   ：void(无)
*@details  ：
************************************************************/
void norFlash_Write_Enable(void)
{
    NORFLASH_CS(0);
    SPI2_RW_Byte(FLASH_WriteEnable);   /* 发送写使能 */
    NORFLASH_CS(1);
}


/**********************************************************
*@funcName ：norFlash_Write_SR
*@brief    ：写25QXX状态寄存器
*@param    ：regon---状态寄存器号，范围:1~3
*            sr---要写入状态寄存器的值    
*@retval   ：void(无)
*@details  ：
************************************************************/
void norFlash_Write_SR(uint8_t regon, uint8_t sr)
{
    uint8_t command = 0;

    switch (regon)
    {
        case 1:
            command = FLASH_WriteStatusReg1;  /* 写状态寄存器1指令 */
            break;

        case 2:
            command = FLASH_WriteStatusReg2;  /* 写状态寄存器2指令 */
            break;

        case 3:
            command = FLASH_WriteStatusReg3;  /* 写状态寄存器3指令 */
            break;

        default:
            command = FLASH_WriteStatusReg1;
            break;
    }

    NORFLASH_CS(0);
    SPI2_RW_Byte(command);  /* 发送读寄存器命令 */
    SPI2_RW_Byte(sr);       /* 写入一个字节 */
    NORFLASH_CS(1);
}


/**********************************************************
*@funcName ：norFlash_Wait_Busy
*@brief    ：等待W25QXX空闲
*@param    ：void(无) 
*@retval   ：void(无)
*@details  ：
************************************************************/
static void norFlash_Wait_Busy(void)
{
    while ((norFlash_Read_SR(1) & 0x01) == 0x01);   /* 等待BUSY位清空 */
}


/**********************************************************
*@funcName ：norFlash_Send_Address
*@brief    ：向25QXX发送地址,根据芯片型号的不同, 发送24ibt / 32bit地址
*@param    ：address---要发送的地址 
*@retval   ：void(无)
*@details  ：
************************************************************/
static void norFlash_Send_Address(uint32_t address)
{
    if (g_norflash_type == W25Q256) /*  只有W25Q256支持4字节地址模式 */
    {
        SPI2_RW_Byte((uint8_t)((address)>>24)); /* 发送 bit31 ~ bit24 地址 */
    } 
    SPI2_RW_Byte((uint8_t)((address)>>16));     /* 发送 bit23 ~ bit16 地址 */
    SPI2_RW_Byte((uint8_t)((address)>>8));      /* 发送 bit15 ~ bit8  地址 */
    SPI2_RW_Byte((uint8_t)address);             /* 发送 bit7  ~ bit0  地址 */
}


/**********************************************************
*@funcName ：norFlash_Read
*@brief    ：在指定地址开始读取指定长度的数据
*@param    ：pbuf---数据存储区
*            addr---开始读取的地址(最大32bit)
*            datalen---要读取的字节数(最大65535)
*@retval   ：void(无)
*@details  ：
************************************************************/
void norFlash_Read(uint8_t *pbuf, uint32_t addr, uint16_t datalen)
{
    uint16_t i;

    NORFLASH_CS(0);
    SPI2_RW_Byte(FLASH_ReadData);       /* 发送读取命令 */
    norFlash_Send_Address(addr);        /* 发送地址 */
    
    for(i=0;i<datalen;i++)
    {
        pbuf[i] = SPI2_RW_Byte(0XFF);   /* 循环读取 */
    }
    
    NORFLASH_CS(1);
}


/**********************************************************
*@funcName ：norFlash_Write_Page
*@brief    ：在指定地址开始写入最大256字节的数据(页写，不可跨页)
*@param    ：pbuf---数据存储区
*            addr---开始写入的地址(最大32bit)
*            datalen---要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
*@retval   ：void(无)
*@details  ：
************************************************************/
static void norFlash_Write_Page(uint8_t *pbuf, uint32_t addr, uint16_t datalen)
{
    uint16_t i;

    norFlash_Write_Enable();            /* 写使能 */

    NORFLASH_CS(0);
    SPI2_RW_Byte(FLASH_PageProgram);    /* 发送写页命令 */
    norFlash_Send_Address(addr);        /* 发送地址 */

    for(i=0;i<datalen;i++)
    {
        SPI2_RW_Byte(pbuf[i]);          /* 循环写入 */
    }
    
    NORFLASH_CS(1);
    norFlash_Wait_Busy();               /* 等待写入结束 */
}


/**********************************************************
*@funcName ：norFlash_Write_noCheck
*@brief    ：在指定地址开始写入指定长度的数据(无检验，不带擦除,可跨页)
*@param    ：pbuf---数据存储区
*            addr---开始写入的地址(最大32bit)
*            datalen---要写入的字节数(最大65535)
*@retval   ：void(无)
*@details  ：
*       注意：
*             1.必须确保所写的地址范围内的数据全部为0XFF,否则
*               在非0XFF处写入的数据将失败!
*             2.具有自动换页功能。
*             3.在指定地址开始写入指定长度的数据,但是要确保地址不越界!
************************************************************/
static void norFlash_Write_noCheck(uint8_t *pbuf, uint32_t addr, uint16_t datalen)
{
    uint16_t pageremain;
    pageremain = 256 - addr % 256;  /* 单页剩余的字节数 */
    
    if (datalen <= pageremain)      /* 不大于256个字节 */
    {
        pageremain = datalen;
    }
    
    while (1)
    {
        /* 当写入字节比页内剩余地址还少的时候, 一次性写完
         * 当写入直接比页内剩余地址还多的时候, 先写完整个页内剩余地址, 然后根据剩余长度进行不同处理
         */
        norFlash_Write_Page(pbuf, addr, pageremain);

        if (datalen == pageremain)      /* 写入结束了 */
        {
            break;
        }
        else                            /* datalen > pageremain */
        {
            pbuf += pageremain;         /* pbuf指针地址偏移,前面已经写了pageremain字节 */
            addr += pageremain;         /* 写地址偏移,前面已经写了pageremain字节 */
            datalen -= pageremain;      /* 写入总长度减去已经写入了的字节数 */

            if (datalen > 256)          /* 剩余数据还大于一页,可以一次写一页 */
            {
                pageremain = 256;       /* 一次可以写入256个字节 */
            }
            else                        /* 剩余数据小于一页,可以一次写完 */
            {
                pageremain = datalen;   /* 不够256个字节了 */
            }
        }
    }
}

 
/**********************************************************
*@funcName ：norFlash_Write
*@brief    ：在指定地址开始写入指定长度的数据，
*            该函数带擦除操作!(无检验，带擦写,可跨页)
*@param    ：pbuf---数据存储区  
*            addr---开始写入的地址(最大32bit)
*            datalen---要写入的字节数(最大65535)
*@retval   ：void(无)
*@details  ：
*             256个字节为一个Page(页), 
*            4Kbytes为一个Sector(扇区), 
*            16个Sector为1个Block(块)
*              擦除的最小单位为Sector.
************************************************************/
uint8_t g_norflash_buf[4096];   /* 扇区缓存 */
void norFlash_Write(uint8_t *pbuf, uint32_t addr, uint16_t datalen)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    uint8_t *norflash_buf;
    
    
    norflash_buf = g_norflash_buf;
    secpos = addr / 4096;       /* 扇区地址 */
    secoff = addr % 4096;       /* 在扇区内的偏移 */
    secremain = 4096 - secoff;  /* 扇区剩余空间大小 */
    
    if (datalen <= secremain)
    {
        secremain = datalen;    /* 不大于4096个字节 */
    }
    
     while (1)
    {
        norFlash_Read(norflash_buf, secpos * 4096, 4096);   /* 读出整个扇区的内容 */
        
        for (i = 0; i < secremain; i++)                     /* 校验数据 */
        {
            if (norflash_buf[secoff + i] != 0XFF)
            {
                break;                                      /* 需要擦除, 直接退出for循环 */
            }
        }
        
        if (i < secremain)                      /* 需要擦除 */
        {
            norFlash_Erase_Sector(secpos);      /* 擦除这个扇区 */
            
            for (i = 0; i < secremain; i++)     /* 复制 */
            {
                norflash_buf[i + secoff] = pbuf[i];
            }
            
            norFlash_Write_noCheck(norflash_buf, secpos * 4096, 4096);  /* 写入整个扇区 */
        }
        else        /* 写已经擦除了的,直接写入扇区剩余区间. */
        {
            norFlash_Write_noCheck(pbuf, addr, secremain);  /* 直接写扇区 */
        }
        
        if (datalen == secremain)
        {
            break;  /* 写入结束了 */
        }
        else        /* 写入未结束 */
        {
            secpos++;               /* 扇区地址增1 */
            secoff = 0;             /* 偏移位置为0 */

            pbuf += secremain;      /* 指针偏移 */
            addr += secremain;      /* 写地址偏移 */
            datalen -= secremain;   /* 字节数递减 */

            if (datalen > 4096)
            {
                secremain = 4096;   /* 下一个扇区还是写不完 */
            }
            else
            {
                secremain = datalen;/* 下一个扇区可以写完了 */
            }
        }
    }
}

/**********************************************************
*@funcName ：norFlash_Erase_Sector
*@brief    ：擦除一个扇区
*@param    ：saddr---扇区地址 根据实际容量设置
*@retval   ：void(无)
*@details  ：
*       注意：这里是扇区地址,不是字节地址!(看一下norFlash_Write函数就
*             能知道，这个传进来的地址是经过处理过的)
*
*            擦除一个扇区的最少时间:150ms
************************************************************/
void norFlash_Erase_Sector(uint32_t saddr)
{
    saddr *= 4096;
    norFlash_Write_Enable();        /* 写使能 */
    norFlash_Wait_Busy();           /* 等待空闲 */

    NORFLASH_CS(0);
    SPI2_RW_Byte(FLASH_SectorErase);/* 发送扇区删除命令 */
    norFlash_Send_Address(saddr);   /* 发送地址 */
    NORFLASH_CS(1);
    norFlash_Wait_Busy();           /* 等待扇区擦除完成 */
}



/**********************************************************
*@funcName ：norFlash_Erase_Block
*@brief    ：擦除一个块区
*@param    ：addr---要删除的地址，会删除地址所在的整个块
*@retval   ：void(无)
*@details  ：
*       注意：这里传入的地址和norFlash_Erase_Sector函数传入
*             的地址是不同的，norFlash_Erase_Sector函数传入
*             的地址是处理过的地址，这里传入的地址是不经过处理的
************************************************************/
void norFlash_Erase_Block(uint32_t addr)
{
    norFlash_Write_Enable();        /* 写使能 */
    norFlash_Wait_Busy();           /* 等待空闲 */

    NORFLASH_CS(0);
    SPI2_RW_Byte(FLASH_BlockErase); /* 发送块删除命令 */
    norFlash_Send_Address(addr);   /* 发送地址 */
    NORFLASH_CS(1);
    
    norFlash_Wait_Busy();           /* 等待扇区擦除完成 */
}


/**********************************************************
*@funcName ：norFlash_Erase_Chip
*@brief    ：擦除整个芯片
*@param    ：void(无)
*@retval   ：void(无)
*@details  ：
*          擦除整个芯片所需时间非常长
************************************************************/
void norFlash_Erase_Chip(void)
{
    norFlash_Write_Enable();        /* 写使能 */
    norFlash_Wait_Busy();           /* 等待空闲 */
    NORFLASH_CS(0);
    SPI2_RW_Byte(FLASH_ChipErase);  /* 发送读寄存器命令 */ 
    NORFLASH_CS(1);
    norFlash_Wait_Busy();           /* 等待芯片擦除结束 */
}
