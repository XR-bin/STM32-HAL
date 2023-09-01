#ifndef __ADC_H
#define __ADC_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h"



    /********************   �궨��   ********************/
    /* ADC3���� */
    #define ADC3_CHY_GPIO_PORT                  GPIOA
    #define ADC3_CHY_GPIO_PIN                   GPIO_PIN_1 
    #define ADC3_CHY_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOA_CLK_ENABLE()  /* PA��ʱ��ʹ�� */
    /* ADC_x */
    #define ADC_ADCX                            ADC3 
    #define ADC3_CHY                            ADC_CHANNEL_6                 /* ͨ��Y,  0 <= Y <= 17 */ 
    #define ADC3_CHY_CLK_ENABLE()               __HAL_RCC_ADC3_CLK_ENABLE()   /* ADC1 ʱ��ʹ�� */



    /****************    �����ⲿ����   *****************/
    void ADC3_In6_Init(void);       /* ADC3��ʼ�� */
    uint16_t ADC3_In6_Data(void);   /* ��ȡADC3ת����ֵ */
    uint16_t Oversampling(void);    /* ������ */

#endif
