#include "./BSP/inc/norflash.h"
#include "./BSP/inc/spi.h"

/*********************************************
*W25Q128�洢оƬ
*������С             ��16Mbyte  ��  128Mbit
*���п���             ��256��
*ÿ������������       ��16����
*ÿ��������ҳ         ��16ҳ
*ÿҳ�ֽ�             ��256�ֽ�
*��ַ��д���ʮ����������0x9fffff
*        9f:����   f������    f��ҳ    ff���ֽ�
**********************************************/

uint16_t g_norflash_type = NM25Q128;     /* norFlash(W25QXXX)�ͺţ�Ĭ����NM25Q128 */

/**********************************************************
*@funcName ��norFlash_init
*@brief    ��norFlash��W25QXXX����ʼ��
*@param    ��void(��)
*@retval   ��void(��)
*@details  ��
************************************************************/
void norFlash_Init(void)
{
    uint8_t temp;
    GPIO_InitTypeDef gpio_init_struct;

    NORFLASH_CS_GPIO_CLK_ENABLE();      /* NORFLASH CS�� ʱ��ʹ�� */
    
    gpio_init_struct.Pin = NORFLASH_CS_GPIO_PIN;         /* CS */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;         /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                 /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;       /* ���� */
    HAL_GPIO_Init(NORFLASH_CS_GPIO_PORT, &gpio_init_struct); /* CS����ģʽ����(�������) */

    NORFLASH_CS(1);                     /* ȡ��Ƭѡ */
    
    SPI2_Init();                        /* ��ʼ��SPI2 */
    SPI2_Set_Speed(SPI_SPEED_2);        /* SPI2 �л�������״̬ 18Mhz */
    
    g_norflash_type = norFlash_Read_id();   /* ��ȡFLASH ID. */
    
    if (g_norflash_type == W25Q256)     /* SPI FLASHΪW25Q256, ����ʹ��4�ֽڵ�ַģʽ */
    {
        temp = norFlash_Read_SR(3);     /* ��ȡ״̬�Ĵ���3���жϵ�ַģʽ */

        if ((temp & 0X01) == 0)         /* �������4�ֽڵ�ַģʽ,�����4�ֽڵ�ַģʽ */
        {
            norFlash_Write_Enable();    /* дʹ�� */
            temp |= 1 << 1;             /* ADP=1, �ϵ�4λ��ַģʽ */
            norFlash_Write_SR(3, temp); /* дSR3 */
            
            NORFLASH_CS(0);
            SPI2_RW_Byte(FLASH_Enable4ByteAddr);    /* ʹ��4�ֽڵ�ַָ�� */
            NORFLASH_CS(1);
        }
    }
    
    printf("ID:%x\r\n", g_norflash_type);
}

/**********************************************************
*@funcName ��norFlash_Read_id
*@brief    ����ȡnorFlash��W25QXXX����ID������ʶ���ͺ�
*@param    ��void(��)
*@retval   ��norFlash��W25QXXX����ID
*@details  ��
************************************************************/
uint16_t norFlash_Read_id(void)
{
    uint16_t deviceid;

    NORFLASH_CS(0);
    SPI2_RW_Byte(FLASH_ManufactDeviceID);   /* ���Ͷ� ID ���� */
    SPI2_RW_Byte(0);    /* д��һ���ֽ� */
    SPI2_RW_Byte(0);
    SPI2_RW_Byte(0);
    deviceid = SPI2_RW_Byte(0xFF) << 8;     /* ��ȡ��8λ�ֽ� */
    deviceid |= SPI2_RW_Byte(0xFF);         /* ��ȡ��8λ�ֽ� */
    NORFLASH_CS(1);

    return deviceid;
}


/**********************************************************
*@funcName ��norFlash_Read_SR
*@brief    ����ȡ25QXX��״̬�Ĵ�����25QXXһ����3��״̬�Ĵ���
*@param    ��regon---״̬�Ĵ����ţ���Χ:1~3
*@retval   ��״̬�Ĵ���ֵ
*@details  ��
*       ״̬�Ĵ���1��
*              BIT7  6   5   4   3   2   1   0
*              SPR   RV  TB BP2 BP1 BP0 WEL BUSY
*              SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
*              TB,BP2,BP1,BP0:FLASH����д��������
*              WEL:дʹ������
*              BUSY:æ���λ(1,æ;0,����)
*              Ĭ��:0x00
*
*       ״̬�Ĵ���2��
*              BIT7  6   5   4   3   2   1   0
*              SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
*
*       ״̬�Ĵ���3��
*              BIT7      6    5    4   3   2   1   0
*              HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
*
*       ������Ϣ�����Լ��������Ӳ������
************************************************************/
uint8_t norFlash_Read_SR(uint8_t regon)
{
    uint8_t byte = 0, command = 0;

    switch (regon)
    {
        case 1:
            command = FLASH_ReadStatusReg1;  /* ��״̬�Ĵ���1ָ�� */
            break;

        case 2:
            command = FLASH_ReadStatusReg2;  /* ��״̬�Ĵ���2ָ�� */
            break;

        case 3:
            command = FLASH_ReadStatusReg3;  /* ��״̬�Ĵ���3ָ�� */
            break;

        default:
            command = FLASH_ReadStatusReg1;
            break;
    }
    
    NORFLASH_CS(0);
    SPI2_RW_Byte(command);      /* ���Ͷ��Ĵ������� */
    byte = SPI2_RW_Byte(0Xff);  /* ��ȡһ���ֽ� */
    NORFLASH_CS(1);
    
    return byte;
}

/**********************************************************
*@funcName ��norFlash_Write_Enable
*@brief    ��25QXXдʹ��,��S1�Ĵ�����WEL��λ
*@param    ��void(��)
*@retval   ��void(��)
*@details  ��
************************************************************/
void norFlash_Write_Enable(void)
{
    NORFLASH_CS(0);
    SPI2_RW_Byte(FLASH_WriteEnable);   /* ����дʹ�� */
    NORFLASH_CS(1);
}


/**********************************************************
*@funcName ��norFlash_Write_SR
*@brief    ��д25QXX״̬�Ĵ���
*@param    ��regon---״̬�Ĵ����ţ���Χ:1~3
*            sr---Ҫд��״̬�Ĵ�����ֵ    
*@retval   ��void(��)
*@details  ��
************************************************************/
void norFlash_Write_SR(uint8_t regon, uint8_t sr)
{
    uint8_t command = 0;

    switch (regon)
    {
        case 1:
            command = FLASH_WriteStatusReg1;  /* д״̬�Ĵ���1ָ�� */
            break;

        case 2:
            command = FLASH_WriteStatusReg2;  /* д״̬�Ĵ���2ָ�� */
            break;

        case 3:
            command = FLASH_WriteStatusReg3;  /* д״̬�Ĵ���3ָ�� */
            break;

        default:
            command = FLASH_WriteStatusReg1;
            break;
    }

    NORFLASH_CS(0);
    SPI2_RW_Byte(command);  /* ���Ͷ��Ĵ������� */
    SPI2_RW_Byte(sr);       /* д��һ���ֽ� */
    NORFLASH_CS(1);
}


/**********************************************************
*@funcName ��norFlash_Wait_Busy
*@brief    ���ȴ�W25QXX����
*@param    ��void(��) 
*@retval   ��void(��)
*@details  ��
************************************************************/
static void norFlash_Wait_Busy(void)
{
    while ((norFlash_Read_SR(1) & 0x01) == 0x01);   /* �ȴ�BUSYλ��� */
}


/**********************************************************
*@funcName ��norFlash_Send_Address
*@brief    ����25QXX���͵�ַ,����оƬ�ͺŵĲ�ͬ, ����24ibt / 32bit��ַ
*@param    ��address---Ҫ���͵ĵ�ַ 
*@retval   ��void(��)
*@details  ��
************************************************************/
static void norFlash_Send_Address(uint32_t address)
{
    if (g_norflash_type == W25Q256) /*  ֻ��W25Q256֧��4�ֽڵ�ַģʽ */
    {
        SPI2_RW_Byte((uint8_t)((address)>>24)); /* ���� bit31 ~ bit24 ��ַ */
    } 
    SPI2_RW_Byte((uint8_t)((address)>>16));     /* ���� bit23 ~ bit16 ��ַ */
    SPI2_RW_Byte((uint8_t)((address)>>8));      /* ���� bit15 ~ bit8  ��ַ */
    SPI2_RW_Byte((uint8_t)address);             /* ���� bit7  ~ bit0  ��ַ */
}


/**********************************************************
*@funcName ��norFlash_Read
*@brief    ����ָ����ַ��ʼ��ȡָ�����ȵ�����
*@param    ��pbuf---���ݴ洢��
*            addr---��ʼ��ȡ�ĵ�ַ(���32bit)
*            datalen---Ҫ��ȡ���ֽ���(���65535)
*@retval   ��void(��)
*@details  ��
************************************************************/
void norFlash_Read(uint8_t *pbuf, uint32_t addr, uint16_t datalen)
{
    uint16_t i;

    NORFLASH_CS(0);
    SPI2_RW_Byte(FLASH_ReadData);       /* ���Ͷ�ȡ���� */
    norFlash_Send_Address(addr);        /* ���͵�ַ */
    
    for(i=0;i<datalen;i++)
    {
        pbuf[i] = SPI2_RW_Byte(0XFF);   /* ѭ����ȡ */
    }
    
    NORFLASH_CS(1);
}


/**********************************************************
*@funcName ��norFlash_Write_Page
*@brief    ����ָ����ַ��ʼд�����256�ֽڵ�����(ҳд�����ɿ�ҳ)
*@param    ��pbuf---���ݴ洢��
*            addr---��ʼд��ĵ�ַ(���32bit)
*            datalen---Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!
*@retval   ��void(��)
*@details  ��
************************************************************/
static void norFlash_Write_Page(uint8_t *pbuf, uint32_t addr, uint16_t datalen)
{
    uint16_t i;

    norFlash_Write_Enable();            /* дʹ�� */

    NORFLASH_CS(0);
    SPI2_RW_Byte(FLASH_PageProgram);    /* ����дҳ���� */
    norFlash_Send_Address(addr);        /* ���͵�ַ */

    for(i=0;i<datalen;i++)
    {
        SPI2_RW_Byte(pbuf[i]);          /* ѭ��д�� */
    }
    
    NORFLASH_CS(1);
    norFlash_Wait_Busy();               /* �ȴ�д����� */
}


/**********************************************************
*@funcName ��norFlash_Write_noCheck
*@brief    ����ָ����ַ��ʼд��ָ�����ȵ�����(�޼��飬��������,�ɿ�ҳ)
*@param    ��pbuf---���ݴ洢��
*            addr---��ʼд��ĵ�ַ(���32bit)
*            datalen---Ҫд����ֽ���(���65535)
*@retval   ��void(��)
*@details  ��
*       ע�⣺
*             1.����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,����
*               �ڷ�0XFF��д������ݽ�ʧ��!
*             2.�����Զ���ҳ���ܡ�
*             3.��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
************************************************************/
static void norFlash_Write_noCheck(uint8_t *pbuf, uint32_t addr, uint16_t datalen)
{
    uint16_t pageremain;
    pageremain = 256 - addr % 256;  /* ��ҳʣ����ֽ��� */
    
    if (datalen <= pageremain)      /* ������256���ֽ� */
    {
        pageremain = datalen;
    }
    
    while (1)
    {
        /* ��д���ֽڱ�ҳ��ʣ���ַ���ٵ�ʱ��, һ����д��
         * ��д��ֱ�ӱ�ҳ��ʣ���ַ�����ʱ��, ��д������ҳ��ʣ���ַ, Ȼ�����ʣ�೤�Ƚ��в�ͬ����
         */
        norFlash_Write_Page(pbuf, addr, pageremain);

        if (datalen == pageremain)      /* д������� */
        {
            break;
        }
        else                            /* datalen > pageremain */
        {
            pbuf += pageremain;         /* pbufָ���ַƫ��,ǰ���Ѿ�д��pageremain�ֽ� */
            addr += pageremain;         /* д��ַƫ��,ǰ���Ѿ�д��pageremain�ֽ� */
            datalen -= pageremain;      /* д���ܳ��ȼ�ȥ�Ѿ�д���˵��ֽ��� */

            if (datalen > 256)          /* ʣ�����ݻ�����һҳ,����һ��дһҳ */
            {
                pageremain = 256;       /* һ�ο���д��256���ֽ� */
            }
            else                        /* ʣ������С��һҳ,����һ��д�� */
            {
                pageremain = datalen;   /* ����256���ֽ��� */
            }
        }
    }
}

 
/**********************************************************
*@funcName ��norFlash_Write
*@brief    ����ָ����ַ��ʼд��ָ�����ȵ����ݣ�
*            �ú�������������!(�޼��飬����д,�ɿ�ҳ)
*@param    ��pbuf---���ݴ洢��  
*            addr---��ʼд��ĵ�ַ(���32bit)
*            datalen---Ҫд����ֽ���(���65535)
*@retval   ��void(��)
*@details  ��
*             256���ֽ�Ϊһ��Page(ҳ), 
*            4KbytesΪһ��Sector(����), 
*            16��SectorΪ1��Block(��)
*              ��������С��λΪSector.
************************************************************/
uint8_t g_norflash_buf[4096];   /* �������� */
void norFlash_Write(uint8_t *pbuf, uint32_t addr, uint16_t datalen)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    uint8_t *norflash_buf;
    
    
    norflash_buf = g_norflash_buf;
    secpos = addr / 4096;       /* ������ַ */
    secoff = addr % 4096;       /* �������ڵ�ƫ�� */
    secremain = 4096 - secoff;  /* ����ʣ��ռ��С */
    
    if (datalen <= secremain)
    {
        secremain = datalen;    /* ������4096���ֽ� */
    }
    
     while (1)
    {
        norFlash_Read(norflash_buf, secpos * 4096, 4096);   /* ������������������ */
        
        for (i = 0; i < secremain; i++)                     /* У������ */
        {
            if (norflash_buf[secoff + i] != 0XFF)
            {
                break;                                      /* ��Ҫ����, ֱ���˳�forѭ�� */
            }
        }
        
        if (i < secremain)                      /* ��Ҫ���� */
        {
            norFlash_Erase_Sector(secpos);      /* ����������� */
            
            for (i = 0; i < secremain; i++)     /* ���� */
            {
                norflash_buf[i + secoff] = pbuf[i];
            }
            
            norFlash_Write_noCheck(norflash_buf, secpos * 4096, 4096);  /* д���������� */
        }
        else        /* д�Ѿ������˵�,ֱ��д������ʣ������. */
        {
            norFlash_Write_noCheck(pbuf, addr, secremain);  /* ֱ��д���� */
        }
        
        if (datalen == secremain)
        {
            break;  /* д������� */
        }
        else        /* д��δ���� */
        {
            secpos++;               /* ������ַ��1 */
            secoff = 0;             /* ƫ��λ��Ϊ0 */

            pbuf += secremain;      /* ָ��ƫ�� */
            addr += secremain;      /* д��ַƫ�� */
            datalen -= secremain;   /* �ֽ����ݼ� */

            if (datalen > 4096)
            {
                secremain = 4096;   /* ��һ����������д���� */
            }
            else
            {
                secremain = datalen;/* ��һ����������д���� */
            }
        }
    }
}

/**********************************************************
*@funcName ��norFlash_Erase_Sector
*@brief    ������һ������
*@param    ��saddr---������ַ ����ʵ����������
*@retval   ��void(��)
*@details  ��
*       ע�⣺������������ַ,�����ֽڵ�ַ!(��һ��norFlash_Write������
*             ��֪��������������ĵ�ַ�Ǿ����������)
*
*            ����һ������������ʱ��:150ms
************************************************************/
void norFlash_Erase_Sector(uint32_t saddr)
{
    saddr *= 4096;
    norFlash_Write_Enable();        /* дʹ�� */
    norFlash_Wait_Busy();           /* �ȴ����� */

    NORFLASH_CS(0);
    SPI2_RW_Byte(FLASH_SectorErase);/* ��������ɾ������ */
    norFlash_Send_Address(saddr);   /* ���͵�ַ */
    NORFLASH_CS(1);
    norFlash_Wait_Busy();           /* �ȴ������������ */
}



/**********************************************************
*@funcName ��norFlash_Erase_Block
*@brief    ������һ������
*@param    ��addr---Ҫɾ���ĵ�ַ����ɾ����ַ���ڵ�������
*@retval   ��void(��)
*@details  ��
*       ע�⣺���ﴫ��ĵ�ַ��norFlash_Erase_Sector��������
*             �ĵ�ַ�ǲ�ͬ�ģ�norFlash_Erase_Sector��������
*             �ĵ�ַ�Ǵ�����ĵ�ַ�����ﴫ��ĵ�ַ�ǲ����������
************************************************************/
void norFlash_Erase_Block(uint32_t addr)
{
    norFlash_Write_Enable();        /* дʹ�� */
    norFlash_Wait_Busy();           /* �ȴ����� */

    NORFLASH_CS(0);
    SPI2_RW_Byte(FLASH_BlockErase); /* ���Ϳ�ɾ������ */
    norFlash_Send_Address(addr);   /* ���͵�ַ */
    NORFLASH_CS(1);
    
    norFlash_Wait_Busy();           /* �ȴ������������ */
}


/**********************************************************
*@funcName ��norFlash_Erase_Chip
*@brief    ����������оƬ
*@param    ��void(��)
*@retval   ��void(��)
*@details  ��
*          ��������оƬ����ʱ��ǳ���
************************************************************/
void norFlash_Erase_Chip(void)
{
    norFlash_Write_Enable();        /* дʹ�� */
    norFlash_Wait_Busy();           /* �ȴ����� */
    NORFLASH_CS(0);
    SPI2_RW_Byte(FLASH_ChipErase);  /* ���Ͷ��Ĵ������� */ 
    NORFLASH_CS(1);
    norFlash_Wait_Busy();           /* �ȴ�оƬ�������� */
}
