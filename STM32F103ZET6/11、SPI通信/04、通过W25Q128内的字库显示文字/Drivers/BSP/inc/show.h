#ifndef __SHOW_H
#define __SHOW_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h"



    /****************    �����ⲿ����   *****************/
    void LCD_Show_HZ16(uint16_t x, uint16_t y,uint8_t *str, uint16_t color, uint16_t backdrop);  /* ��ʾһ��16*16�ַ�/���� */
    void LCD_Show_HZ32(uint16_t x, uint16_t y,uint8_t *str, uint16_t color, uint16_t backdrop);  /* ��ʾһ��32*32�ַ�/���� */

#endif
