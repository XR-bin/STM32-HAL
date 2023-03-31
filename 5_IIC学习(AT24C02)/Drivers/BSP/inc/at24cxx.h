#ifndef _AT24CXX_H
#define _AT24CXX_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "./SYSTEM/inc/sys.h"
    #include "./SYSTEM/inc/delay.h"
    #include "./SYSTEM/inc/usart.h"
    
    
    
    /********************   �궨��   ********************/
    #define AT24CXX_NO_ERR 0      /* Ӧ����Ч   ���ݴ���ɹ� */
    #define AT24CXX_ERR1   1      /* Ӧ��ʧЧ   ������ַ���ݴ���ʧ�� */
    #define AT24CXX_ERR2   2      /* Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ�� */
    #define AT24CXX_ERR3   3      /* Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ�� */
    #define AT24CXX_ERR4   4      /* Ӧ��ʧЧ   ���ֿ�ҳ���� */
    #define AT24CXX_WRITE  0xa0   /* ������ַ   д���� */
    #define AT24CXX_READ   0xa1   /* ������ַ   ������ */

    #define AT24C01		127        /* 8���ֽ�Ϊһҳ */
    #define AT24C02		255        /* 8���ֽ�Ϊһҳ */
    #define AT24C04		511        /* 16���ֽ�Ϊһҳ */
    #define AT24C08		1023       /* 16���ֽ�Ϊһҳ */
    #define AT24C16		2047       /* 16���ֽ�Ϊһҳ */
    #define AT24C32		4095       /* 32���ֽ�Ϊһҳ */
    #define AT24C64	    8191       /* 32���ֽ�Ϊһҳ */
    #define AT24C128	16383      /* 64���ֽ�Ϊһҳ */
    #define AT24C256	32767      /* 64���ֽ�Ϊһҳ */
    
    /* ������ʹ�õ���24c02�����Զ���AT24CXXΪAT24C02 */
    #define    AT24CXX          AT24C02
    #define    AT24CXX_PAGE     8
    
    
    
    /****************    �����ⲿ����   ****************/
    void AT24CXX_Init(void);
    uint8_t AT24CXX_Read_Byte(uint16_t address, uint8_t *data);
    uint8_t AT24CXX_Write_Byte(uint16_t address, uint8_t data);
    uint8_t AT24CXX_Check(void);
    uint8_t AT24CXX_Read_Bytes(uint16_t address, uint8_t num, uint8_t *pbuf);
    uint8_t AT24CXX_Write_Bytes1(uint16_t address, uint8_t num, uint8_t *pbuf);
    uint8_t AT24CXX_Write_Bytes(uint16_t address, uint8_t num, uint8_t *pbuf);
    void AT24CXX_Read(uint16_t address, uint16_t datalen, uint8_t *pbuf);
    void AT24CXX_Write(uint16_t address, uint16_t datalen, uint8_t *pbuf);

#endif
