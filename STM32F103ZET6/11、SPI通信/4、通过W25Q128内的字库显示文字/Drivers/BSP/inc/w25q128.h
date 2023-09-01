#ifndef __W25Q128_H
#define __W25Q128_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h"



    /********************   �궨��   ********************/
    /* Ƭѡ */
    #define SPI_CS_H  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)     /* Ƭѡʧ�� */
    #define SPI_CS_L  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)   /* Ƭѡʹ�� */

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
    #define FLASH_ManufactDeviceID      0x90 
    #define FLASH_JedecDeviceID         0x9F 
    #define FLASH_Enable4ByteAddr       0xB7
    #define FLASH_Exit4ByteAddr         0xE9
    #define FLASH_SetReadParam          0xC0 
    #define FLASH_EnterQPIMode          0x38
    #define FLASH_ExitQPIMode           0xFF



    /****************    �����ⲿ����   *****************/
    void W25Q128_Init(void);                             /* W25Q128��ʼ�� */
    uint8_t W25Q128_Read_SR(uint8_t serial);             /* ��ȡW25Q128״̬�Ĵ���ֵ */
    void W25Q128_Write_SR(uint8_t serial, uint8_t data); /* �޸�W25Q128״̬�Ĵ���ֵ */
    static void W25Q128_Wait_Busy(void);                 /* �ȴ�W25Q128���� */
    void W25Q128_Write_Enable(void);                     /* W25Q128дʹ�� */
    static void W25Q128_Send_Address(uint32_t address);  /* ��W25QXX���͵�ַ */

    void W25Q128_Page_Write(uint32_t address, uint32_t datalen, uint8_t *arr);           /* д���ݵ�W25Q128(ҳ���ݣ����ɿ�ҳ) */
    static void W25Q128_Write_NoCheck(uint32_t address, uint16_t datalen, uint8_t *arr); /* д���ݵ�W25Q128(�޼���д����) */

    /* W25Q128��д��ʽ���� */
    void W25Q128_Read_Bytes(uint32_t address, uint32_t datalen, uint8_t *arr); /* ��W25Q128��ȡ���� */
    void W25Q128_Write(uint32_t address, uint16_t datalen, uint8_t *arr);      /* д���ݵ�W25Q128(��������) */

    void W25Q128_Sector_Erase(uint32_t address);                    /* �������� */
    void W25Q128_Block_Erase(uint32_t address);                     /* �������� */
    void W25Q128_Chip_Erase(void);                                  /* оƬ���� */
    void W25Q128_Blocks_Erase(uint32_t address,uint8_t blockNum);   /* ��������� */

#endif



