#ifndef __NORFLASH_H
#define __NORFLASH_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h"
    #include "usart.h"



    /********************   �궨��   ********************/
    /* Ƭѡ */
    #define SPI_CS_H  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)     /* Ƭѡʧ�� */
    #define SPI_CS_L  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)   /* Ƭѡʹ�� */

    /* FLASHоƬ�б� */
    #define W25Q80      0XEF13          /* W25Q80   оƬID */
    #define W25Q16      0XEF14          /* W25Q16   оƬID */
    #define W25Q32      0XEF15          /* W25Q32   оƬID */
    #define W25Q64      0XEF16          /* W25Q64   оƬID */
    #define W25Q128     0XEF17          /* W25Q128  оƬID */
    #define W25Q256     0XEF18          /* W25Q256  оƬID */
    #define BY25Q64     0X6816          /* BY25Q64  оƬID */
    #define BY25Q128    0X6817          /* BY25Q128 оƬID */
    #define NM25Q64     0X5216          /* NM25Q64  оƬID */
    #define NM25Q128    0X5217          /* NM25Q128 оƬID */

    /* ָ��� */
    #define FLASH_WriteEnable           0x06        /* дʹ�� */
    #define FLASH_WriteDisable          0x04        /* дʧ�� */
    #define FLASH_ReadStatusReg1        0x05        /* ��״̬�Ĵ���1 */
    #define FLASH_ReadStatusReg2        0x35        /* ��״̬�Ĵ���2 */
    #define FLASH_ReadStatusReg3        0x15        /* ��״̬�Ĵ���3 */
    #define FLASH_WriteStatusReg1       0x01        /* д״̬�Ĵ���1 */
    #define FLASH_WriteStatusReg2       0x31        /* д״̬�Ĵ���2 */
    #define FLASH_WriteStatusReg3       0x11        /* д״̬�Ĵ���3 */
    #define FLASH_ReadData              0x03        /* ������ָ�� */
    #define FLASH_FastReadData          0x0B 
    #define FLASH_FastReadDual          0x3B 
    #define FLASH_FastReadQuad          0xEB  
    #define FLASH_PageProgram           0x02        /* дҳָ�� */
    #define FLASH_PageProgramQuad       0x32 
    #define FLASH_BlockErase            0xD8 
    #define FLASH_SectorErase           0x20        /* �������� */
    #define FLASH_ChipErase             0xC7        /* оƬ���� */
    #define FLASH_PowerDown             0xB9 
    #define FLASH_ReleasePowerDown      0xAB 
    #define FLASH_DeviceID              0xAB 
    #define FLASH_ManufactDeviceID      0x90        /* ��ȡоƬID */
    #define FLASH_JedecDeviceID         0x9F 
    #define FLASH_Enable4ByteAddr       0xB7
    #define FLASH_Exit4ByteAddr         0xE9
    #define FLASH_SetReadParam          0xC0 
    #define FLASH_EnterQPIMode          0x38
    #define FLASH_ExitQPIMode           0xFF



    /****************    �����ⲿ����   *****************/
    void NorFlash_Init(void);                 /* NorFlashоƬ������ų�ʼ�� */
    uint16_t NorFlash_Read_ID(void);          /* ��ȡоƬID */

#endif



