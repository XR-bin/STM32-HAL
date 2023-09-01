#ifndef __RTC_H
#define __RTC_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h"
    #include "delay.h"
    #include "usart.h"



    /****************     �ṹ�嶨��     ****************/
    /* RTC�ṹ�� */
    typedef struct
    {
        uint16_t year;  /* �� */
        uint8_t month;  /* �� */
        uint8_t day;    /* �� */
        uint8_t week;   /* ���� */
        uint8_t hour;   /* ʱ */
        uint8_t min;    /* �� */
        uint8_t sec;    /* �� */
    } RTC_DATES;



    /****************    �����ⲿ����   *****************/
    extern RTC_DATES rtc;



    /****************    �����ⲿ����   *****************/
    uint16_t RTC_Read_Bkr(uint8_t bkrx);                          /* RTC��ȡ������SRAM */
    void RTC_Write_Bkr(uint8_t bkrx, uint16_t data);              /* RTCд�������SRAM */
    static long RTC_DateSec(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);   /* ��������ʱ����ת���������� */
    static uint8_t Leap_Year(uint16_t year);                      /* �ж����껹��ƽ�� */

    uint8_t RTC_Init(void);                                       /* ��RTC���г�ʼ������ */
    uint8_t RTC_Set_Time(RTC_DATES rtc);                          /* ����RTCʱ�� */
    void RTC_Get_Time(void);                                      /* ��ȡRTCʱ�� */
    uint8_t RTC_Week(uint16_t year, uint8_t month, uint8_t day);  /* ͨ�������ջ�ȡ�������� */
    uint8_t RTC_Set_Alarm(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);    /* ����RTC���� */

#endif
