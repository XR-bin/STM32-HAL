#ifndef __OLED_H
#define __OLED_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h"
    #include "delay.h"



    /********************   �궨��   ********************/
    /* ���ݺ������ѡ��궨�� */
    #define  OLED_CMD     0      /* ���� */
    #define  OLED_DATA    1      /* ���� */

    /* OLED����С */
    #define  OLED_LINE       128    /* 128������ */
    #define  OLED_ROW        8      /* 8*8������(1ҳ8�У�����8ҳ) */



    /****************    �����ⲿ����   *****************/
    uint8_t OLED_Write_Command(uint8_t cmd);                  /* ��OLE������ָ�� */
    uint8_t OLED_Write_Data(uint8_t data);                    /* ��OLE���������� */
    void OLED_WR_Byte(uint8_t Byte, uint8_t DC);              /* ��OLE������ָ��/���� */
    void OLED_Clear(void);                                    /* ��OLE������ */
    void OLED_XY(uint8_t x,uint8_t y);                        /* ������ʾ�������� */
    void OLED_Open(void);                                     /* ����OLED�� */
    void OLED_Close(void);                                    /* �ر�OLED�� */
    void OLED_Init(void);                                     /* OLED����ʼ�� */
    void OLED_ShowChar16(uint16_t x,uint8_t y, uint8_t ch);   /* ��ʾһ��16*16�ַ� */
    void OLED_ShowString16(uint8_t x,uint8_t y,uint8_t *str); /* ��ʾһ��16*16�ַ��� */

#endif



