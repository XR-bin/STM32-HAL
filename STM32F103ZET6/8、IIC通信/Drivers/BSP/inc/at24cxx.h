#ifndef __AT24CXX_H
#define __AT24CXX_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h"
    #include "delay.h"



    /********************   �궨��   ********************/
    #define AT24CXX_NO_ERR 0      /* Ӧ����Ч   ���ݴ���ɹ� */
    #define AT24CXX_ERR1   1      /* Ӧ��ʧЧ   ������ַ���ݴ���ʧ�� */
    #define AT24CXX_ERR2   2      /* Ӧ��ʧЧ   �ֽڵ�ַ���ݴ���ʧ�� */
    #define AT24CXX_ERR3   3      /* Ӧ��ʧЧ   Ҫ�洢�����ݴ���ʧ�� */
    #define AT24CXX_ERR4   4      /* Ӧ��ʧЧ   ���ֿ�ҳ���� */
    #define AT24CXX_WRITE  0xa0   /* ������ַ   д���� */
    #define AT24CXX_READ   0xa1   /* ������ַ   ������ */

    /* �����ͺ� */
    #define AT24C01     127        /* 8���ֽ�Ϊһҳ */
    #define AT24C02     255        /* 8���ֽ�Ϊһҳ */
    #define AT24C04     511        /* 16���ֽ�Ϊһҳ */
    #define AT24C08     1023       /* 16���ֽ�Ϊһҳ */
    #define AT24C16     2047       /* 16���ֽ�Ϊһҳ */
    #define AT24C32     4095       /* 32���ֽ�Ϊһҳ */
    #define AT24C64     8191       /* 32���ֽ�Ϊһҳ */
    #define AT24C128    16383      /* 64���ֽ�Ϊһҳ */
    #define AT24C256    32767      /* 64���ֽ�Ϊһҳ */

    /* ������ʹ�õ���24c02�����Զ���AT24CXXΪAT24C02 */
    #define    AT24CXX          AT24C02
    #define    AT24CXX_PAGE     8



    /****************    �����ⲿ����   *****************/
    void AT24CXX_Init(void);/* ��ʼ�� */

    /* ��AT24CXX */
    uint8_t AT24CXX_Write_Byte(uint16_t address,uint8_t data);                       /* �����ֽ�д */
    uint8_t AT24CXX_Read_Byte(uint16_t address,uint8_t *data);                       /* �����ֽڶ� */
    uint8_t AT24CXX_Write_Bytes1(uint16_t address,uint8_t byte_num,uint8_t *arr);    /* ����ֽ�д���ɿ�ҳ */
    uint8_t AT24CXX_Write_Bytes(uint16_t address,uint8_t byte_num,uint8_t *arr);     /* ����ֽ�д�ɿ�ҳ(�ֶ���ҳ) */
    uint8_t AT24CXX_Read_Bytes(uint16_t address,uint8_t byte_num,uint8_t *arr);      /* ����ֽڶ��ɿ�ҳ(Ӳ���Զ���ҳ) */
    uint8_t AT24CXX_Write_Addr(uint16_t address,uint8_t byte_num,uint8_t *arr);      /* ����ֽ�д�ɿ�ҳ(һ����ַһ����ַд�����迼�ǿ�ҳ) */
    uint8_t AT24CXX_Read_Addr(uint16_t address,uint8_t byte_num,uint8_t *arr);       /* ����ֽڶ��ɿ�ҳ(һ����ַһ����ַд�����迼�ǿ�ҳ) */

    /* ��AT24CXX */
    uint8_t AT24CXXaddr_Write_Byte(uint16_t AT24CXXmodel,uint8_t AT24CXXaddr,uint16_t address,uint8_t data);      /* �����ֽ�д */
    uint8_t AT24CXXaddr_Read_Byte(uint16_t AT24CXXmodel,uint8_t AT24CXXaddr,uint16_t address,uint8_t *data);      /* �����ֽڶ� */
    uint8_t AT24CXXaddr_Write_Bytes1(uint16_t AT24CXXmodel,uint8_t AT24CXXaddr,uint16_t address,uint8_t byte_num,uint8_t *arr);     /* ����ֽ�д���ɿ�ҳ */
    uint8_t AT24CXXaddr_Write_Bytes(uint16_t AT24CXXmodel,uint8_t AT24CXXaddr,uint8_t AT24CXXpage,uint16_t address,uint8_t byte_num,uint8_t *arr);    /* ����ֽ�д�ɿ�ҳ(�ֶ���ҳ) */
    uint8_t AT24CXXaddr_Read_Bytes(uint16_t AT24CXXmodel,uint8_t AT24CXXaddr,uint16_t address,uint8_t byte_num,uint8_t *arr);     /* ����ֽڶ��ɿ�ҳ(Ӳ���Զ���ҳ) */
    uint8_t AT24CXXaddr_Write_Addr(uint16_t AT24CXXmodel,uint8_t AT24CXXaddr,uint16_t address,uint8_t byte_num,uint8_t *arr);     /* ����ֽ�д�ɿ�ҳ(һ����ַһ����ַд�����迼�ǿ�ҳ) */
    uint8_t AT24CXXaddr_Read_Addr(uint16_t AT24CXXmodel,uint8_t AT24CXXaddr,uint16_t address,uint8_t byte_num,uint8_t *arr);      /* ����ֽڶ��ɿ�ҳ(һ����ַһ����ַд�����迼�ǿ�ҳ) */

#endif






