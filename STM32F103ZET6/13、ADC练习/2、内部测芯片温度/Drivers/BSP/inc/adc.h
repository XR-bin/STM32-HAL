#ifndef __ADC_H
#define __ADC_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h"



    /****************    �����ⲿ����   *****************/
    void ADC1_In16_Init(void);            /* ADC1��ʼ�� */
    uint16_t ADC1_In16_Data(void);        /* ��ȡADC1ת��ֵ */
    uint16_t Oversampling(void);          /* ������ */
    double ADC1_Chip_Temperature1(void);  /* ��ADCֵת��ΪоƬ�¶�ֵ(�ǹ�����) */
    double ADC1_Chip_Temperature2(void);  /* ��ADCֵת��ΪоƬ�¶�ֵ(������) */

#endif
