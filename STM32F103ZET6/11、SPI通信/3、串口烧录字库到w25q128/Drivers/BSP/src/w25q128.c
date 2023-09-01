#include "w25q128.h"
#include "spi.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\w25q128.c
*              .\Drivers\BSP\inc\w25q128.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-07-01
* @brief     ��w25q128��д��������
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/***
*W25Q128�洢оƬ
*������С             ��16Mbyte == 128Mbit
*���п���             ��256��
*ÿ������������       ��16����
*ÿ��������ҳ         ��16ҳ
*ÿҳ�ֽ�             ��256�ֽ�
*��ַ��д���ʮ����������0x9fffff
*        9f:����   f������    f��ҳ    ff���ֽ�
*/



/**********************************************************
* @funcName ��W25Q128_Init
* @brief    ��W25Q128��ʼ��
* @param    ��void
* @retval   ��void
* @details  ��
*             SPI_CS     PB12
* @fn       ��
************************************************************/
void W25Q128_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;               /* GPIO���ýṹ�� */

    __HAL_RCC_GPIOB_CLK_ENABLE();                    /* ʱ��ʹ��   GPIOB */

    /* GPIOx��ʼ������ */
    /* GPIOx�˿����� */
    /* PB12 */
    gpio_init_struct.Pin   = GPIO_PIN_12;            /* PB12 �˿����� */
    gpio_init_struct.Mode  = GPIO_MODE_OUTPUT_PP;    /* ������� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;   /* IO���ٶ�Ϊ50MHz */
    HAL_GPIO_Init(GPIOB, &gpio_init_struct);         /* �����趨������ʼ��PB12 */

    /* �˿�������ݼĴ���(Ƭѡ����) */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);  /* �˿�����Ĵ��������(��������) */

    SPI2_Init();                                          /* SPI��ʼ�� */
}

 
/**********************************************************
* @funcName ��W25Q128_Read_SR
* @brief    ����ȡ25QXX��״̬�Ĵ�����25QXXһ����3��״̬�Ĵ���
* @param    ��uint8_t serial      ״̬�Ĵ������кţ���Χ:1~3
*   @arg    ��1 --- ״̬�Ĵ���1   2 --- ״̬�Ĵ���2   3 --- ״̬�Ĵ���3
* @retval   ��uint8_t (״̬�Ĵ���ֵ)
* @details  ��
* @fn       ��
************************************************************/
uint8_t W25Q128_Read_SR(uint8_t serial)
{
    uint8_t byte = 0, command = 0;

    switch (serial)
    {
        /* ��״̬�Ĵ���1ָ�� */
        case 1: command = FLASH_ReadStatusReg1; break;
        /* ��״̬�Ĵ���2ָ�� */
        case 2: command = FLASH_ReadStatusReg2; break;
        /* ��״̬�Ĵ���3ָ�� */
        case 3: command = FLASH_ReadStatusReg3; break;
        /* serial�޷�ƥ��ʱ */
        default: command = FLASH_ReadStatusReg1; break;
    }

    SPI_CS_L;                   /* ʹ��Ƭѡ */
    SPI2_RS_Byte(command);      /* ���Ͷ��Ĵ������� */
    byte = SPI2_RS_Byte(0Xff);  /* ��ȡһ���ֽ� */
    SPI_CS_H;                   /* ʧ��Ƭѡ */

    return byte;
}



/**********************************************************
* @funcName ��W25Q128_Write_SR
* @brief    ��д25QXX��״̬�Ĵ�����25QXXһ����3��״̬�Ĵ���
* @param    ��uint8_t serial (״̬�Ĵ������кţ���Χ:1~3)
*   @arg    ��1 --- ״̬�Ĵ���1   2 --- ״̬�Ĵ���2   3 --- ״̬�Ĵ���3
* @param    ��uint8_t data (Ҫд��״̬�Ĵ���������)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void W25Q128_Write_SR(uint8_t serial, uint8_t data)
{
    uint8_t command = 0;

    switch (serial)
    {
        /* д״̬�Ĵ���1ָ�� */
        case 1: command = FLASH_WriteStatusReg1; break;
        /* д״̬�Ĵ���2ָ�� */
        case 2: command = FLASH_WriteStatusReg2; break;
        /* д״̬�Ĵ���3ָ�� */
        case 3: command = FLASH_WriteStatusReg3; break;
        /* serial�޷�ƥ��ʱ */
        default: command = FLASH_WriteStatusReg1; break;
    }

    SPI_CS_L;               /* ʹ��Ƭѡ */
    SPI2_RS_Byte(command);  /* ���Ͷ��Ĵ������� */
    SPI2_RS_Byte(data);     /* д��һ���ֽ� */
    SPI_CS_H;               /* ʧ��Ƭѡ */
}



/**********************************************************
* @funcName ��W25Q128_Wait_Busy
* @brief    ���ȴ�����
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
static void W25Q128_Wait_Busy(void)
{
    while ((W25Q128_Read_SR(1) & 0x01) == 0x01);   /* �ȴ�BUSYλ��� */
}



/**********************************************************
* @funcName ��W25Q128_Write_Enable
* @brief    ����W25Q128����дʹ��
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void W25Q128_Write_Enable(void)
{
    SPI_CS_L;
    SPI2_RS_Byte(FLASH_WriteEnable);
    SPI_CS_H;
}



/**********************************************************
* @funcName ��W25Q128_Send_Address
* @brief    ����W25QXX���͵�ַ
* @param    ��uint32_t address (w25Q128�ڲ��Ĵ洢��ַ)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
static void W25Q128_Send_Address(uint32_t address)
{
    SPI2_RS_Byte((uint8_t)((address)>>16));     /* ���� bit23 ~ bit16 ��ַ */
    SPI2_RS_Byte((uint8_t)((address)>>8));      /* ���� bit15 ~ bit8  ��ַ */
    SPI2_RS_Byte((uint8_t)address);             /* ���� bit7  ~ bit0  ��ַ */
}



/**********************************************************
* @funcName ��W25Q128_Read_Bytes
* @brief    ������оƬ��W25Q128��ĳ����ַ��n���ֽ�����
* @param    ��uint32_t address  (w25Q128�ڲ��Ĵ洢��ַ)
* @param    ��uint32_t datalen  (Ҫ��ȡ�������ֽ���)
* @param    ��uint8_t *arr      (������ݵĻ�������)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void W25Q128_Read_Bytes(uint32_t address, uint32_t datalen, uint8_t *arr)
{
    SPI_CS_L;                       /* Ƭѡʹ�� */
    SPI2_RS_Byte(FLASH_ReadData);   /* ��������ָ�� */
    W25Q128_Send_Address(address);  /* ���͵�ַ */

    /* ������ */
    while(datalen)
    {
        *arr = SPI2_RS_Byte(0xff);
        arr++;
        datalen--;
    }

    SPI_CS_H;      /* Ƭѡʧ�� */
}



/**********************************************************
* @funcName ��W25Q128_Page_Write
* @brief    ������оƬ��W25Q128��ĳ����ַ��дn���ֽ�����(���ɿ�ҳ)
* @param    ��uint32_t address  (w25Q128�ڲ��Ĵ洢��ַ)
* @param    ��uint32_t datalen  (Ҫ��ȡ�������ֽ���)
* @param    ��uint8_t *arr      (���Ҫд������ݵ�����)
* @retval   ��void
* @details  ��
*            ע�⣺
*                ��ʼд��ĵ�ַ���32bit
*                Ҫд����ֽ������256,������Ӧ�ó�����ҳ��ʣ���ֽ���!!!
* @fn       ��
************************************************************/
void W25Q128_Page_Write(uint32_t address, uint32_t datalen, uint8_t *arr)
{
    W25Q128_Write_Enable();    /* дʹ�� */

    SPI_CS_L;                        /* Ƭѡʹ�� */
    SPI2_RS_Byte(FLASH_PageProgram); /* дҳָ�� */
    W25Q128_Send_Address(address);   /* ���͵�ַ */

    while(datalen)
    {
        SPI2_RS_Byte(*arr);
        arr++;
        datalen--;
    }

    SPI_CS_H;      /* Ƭѡʧ�� */
    W25Q128_Wait_Busy();   /* �ȴ�д����� */
}



/**********************************************************
* @funcName ��W25Q128_Write_NoCheck
* @brief    ���޼���дSPI FLASH
* @param    ��uint32_t address  (w25Q128�ڲ��Ĵ洢��ַ)
* @param    ��uint16_t datalen  (Ҫ��ȡ�������ֽ���)
* @param    ��uint8_t *arr      (���Ҫд������ݵ�����)
* @retval   ��void
* @details  ��
* @fn       ��
*           ע�⣺����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,
*                 �����ڷ�0XFF��д������ݽ�ʧ��!
*
*            �ú��������Զ���ҳ����
*
*            ��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
************************************************************/
static void W25Q128_Write_NoCheck(uint32_t address, uint16_t datalen, uint8_t *arr)
{
    uint16_t pageremain;
    pageremain = 256 - address % 256;  /* ��ҳʣ����ֽ��� */

    if (datalen <= pageremain)         /* ������256���ֽ� */
    {
        pageremain = datalen;
    }

    while (1)
    {
        /* ��д���ֽڱ�ҳ��ʣ���ַ���ٵ�ʱ��, һ����д��
         * ��д��ֱ�ӱ�ҳ��ʣ���ַ�����ʱ��, ��д������ҳ��ʣ���ַ, Ȼ�����ʣ�೤�Ƚ��в�ͬ����
         */
        W25Q128_Page_Write(address, pageremain, arr);

        if (datalen == pageremain)   /* д������� */
        {
            break;
        }
        else                            /* datalen > pageremain */
        {
            arr += pageremain;          /* arrָ���ַƫ��,ǰ���Ѿ�д��pageremain�ֽ� */
            address += pageremain;      /* д��ַƫ��,ǰ���Ѿ�д��pageremain�ֽ� */
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



uint8_t g_norflash_buf[4096];   /* �������棬�Ȱ�W25Q128��ĳ������������������ڣ��ٲ����� */
/**********************************************************
* @funcName ��W25Q128_Write
* @brief    ����ָ����ַ��ʼд��ָ�����ȵ����� , �ú�������������!
* @param    ��uint32_t address  (w25Q128�ڲ��Ĵ洢��ַ�����32bit)
* @param    ��uint16_t datalen  (Ҫ��ȡ�������ֽ���)
* @param    ��uint8_t *arr      (���Ҫд������ݵ����飬���65535)
* @retval   ��void
* @details  ��
*             SPI FLASH һ����: 256���ֽ�Ϊһ��Page, 
*                               4KbytesΪһ��Sector, 
*                               16������Ϊ1��Block
*              ��������С��λΪSector.
* @fn       ��
************************************************************/
void W25Q128_Write(uint32_t address, uint16_t datalen, uint8_t *arr)
{
    uint32_t secpos;       /* ������ַ */
    uint16_t secoff;       /* �����ڵ�ַ */
    uint16_t secremain;    /* ��ǰ����ʣ���С */
    uint16_t i;
    uint8_t *norflash_buf;

    norflash_buf = g_norflash_buf;
    secpos = address / 4096;       /* ������ַ */
    secoff = address % 4096;       /* �������ڵ�ƫ�� */
    secremain = 4096 - secoff;     /* ����ʣ��ռ��С */

    //printf("ad:%X,nb:%X\r\n", address, datalen); /* ������ */

    if (datalen <= secremain)
    {
        secremain = datalen;    /* ������4096���ֽ� */
    }

    while (1)
    {
        W25Q128_Read_Bytes(secpos * 4096, 4096, norflash_buf);  /* ������������������ */

        for (i = 0; i < secremain; i++)   /* У������ */
        {
            if (norflash_buf[secoff + i] != 0XFF)
            {
                break;      /* ��Ҫ����, ֱ���˳�forѭ�� */
            }
        }

        if (i < secremain)   /* ��Ҫ���� */
        {
            W25Q128_Sector_Erase(secpos);  /* ����������� */

            for (i = 0; i < secremain; i++)   /* ���� */
            {
                norflash_buf[i + secoff] = arr[i];
            }

            W25Q128_Write_NoCheck(secpos * 4096, 4096, norflash_buf);  /* д���������� */
        }
        else        /* д�Ѿ������˵�,ֱ��д������ʣ������. */
        {
            W25Q128_Write_NoCheck(address, secremain, arr);  /* ֱ��д���� */
        }

        if (datalen == secremain)
        {
            break;  /* д������� */
        }
        else        /* д��δ���� */
        {
            secpos++;               /* ������ַ��1 */
            secoff = 0;             /* ƫ��λ��Ϊ0 */

            arr += secremain;      /* ָ��ƫ�� */
            address += secremain;  /* д��ַƫ�� */
            datalen -= secremain;  /* �ֽ����ݼ� */

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
* @funcName ��W25Q128_Sector_Erase
* @brief    ��W25Q128��������
* @param    ��uint32_t address  (w25Q128�ڲ��Ĵ洢��ַ�����32bit)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void W25Q128_Sector_Erase(uint32_t address)
{
    W25Q128_Write_Enable();          /* дʹ�� */
    W25Q128_Wait_Busy();             /* �ȴ����� */
    /* ���� */
    SPI_CS_L;                        /* Ƭѡʹ�� */
    SPI2_RS_Byte(FLASH_SectorErase); /* ��������ָ�� */
    W25Q128_Send_Address(address);   /* ���͵�ַ */
    SPI_CS_H;                        /* Ƭѡʧ�� */
    W25Q128_Wait_Busy();             /* �ȴ�������� */
}



/**********************************************************
* @funcName ��W25Q128_Block_Erase
* @brief    ��W25Q128��������
* @param    ��uint32_t address  (w25Q128�ڲ��Ĵ洢��ַ�����32bit)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void W25Q128_Block_Erase(uint32_t address)
{
    W25Q128_Write_Enable();          /* дʹ�� */
    W25Q128_Wait_Busy();             /* �ȴ����� */
    /* ���� */
    SPI_CS_L;                        /* Ƭѡʹ�� */
    SPI2_RS_Byte(FLASH_BlockErase);  /* ��������ָ�� */
    W25Q128_Send_Address(address);   /* ���͵�ַ */
    SPI_CS_H;                        /* Ƭѡʧ�� */
    W25Q128_Wait_Busy();             /* �ȴ�������� */
}



/**********************************************************
* @funcName ��W25Q128_Chip_Erase
* @brief    ��WW25Q128оƬ����
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void W25Q128_Chip_Erase(void)
{
    W25Q128_Write_Enable();          /* дʹ�� */
    W25Q128_Wait_Busy();             /* �ȴ����� */
    /* ���� */
    SPI_CS_L;                        /* Ƭѡʹ�� */
    SPI2_RS_Byte(FLASH_ChipErase);   /* оƬ���� */
    SPI_CS_H;                        /* Ƭѡʧ�� */
    W25Q128_Wait_Busy();             /* �ȴ�������� */
}



/**********************************************************
* @funcName ��W25Q128_Blocks_Erase
* @brief    ��W25Q128������������
* @param    ��uint32_t address   (w25Q128�ڲ��Ĵ洢��ַ�����32bit)
* @param    ��uint8_t blockNum   (Ҫ������ĸ���)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void W25Q128_Blocks_Erase(uint32_t address,uint8_t blockNum)
{
    while(blockNum)
    {
        W25Q128_Block_Erase(address);
        address += 65536;
        blockNum--;
    }
}












