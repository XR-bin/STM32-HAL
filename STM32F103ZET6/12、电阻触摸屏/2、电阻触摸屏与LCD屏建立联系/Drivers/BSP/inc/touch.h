#ifndef __TOUCH_H
#define __TOUCH_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h"
    #include "delay.h"



    /****************     �ṹ�嶨��     ****************/
    /* ���ڵ��败����x��y�����ȡ */
    typedef struct touch
    {
        uint16_t x;     /* ����x���� */
        uint16_t y;     /* ����y���� */
    }TOUCH_XY;

    /* ������������ */
    typedef struct
    {
        void (*init)(void);      /* ��ʼ�������������� */
        void (*adjust)(void);       /* ������У׼ */
        TOUCH_XY (*scan)(uint8_t);   /* ɨ�败����.0,��Ļɨ��;1,��������; */

        uint8_t comX;    /* ��ȡx��������ָ�� */
        uint8_t comY;    /* ��ȡy��������ָ�� */

        /* 5��У׼������У׼����(����������ҪУ׼) */
        float xfac;                 /* LCD�ʹ�����x����ı���ϵ�� */
        float yfac;                 /* LCD�ʹ�����y����ı���ϵ�� */
        short xc;                   /* LCD�ʹ�����x�����ƫ���� */
        short yc;                   /* LCD�ʹ�����x�����ƫ���� */

        /* �����Ĳ���,��������������������ȫ�ߵ�ʱ��Ҫ�õ�.
         * b0:0, ����(�ʺ�����ΪX����,����ΪY�����TP)
         *    1, ����(�ʺ�����ΪY����,����ΪX�����TP)
         * b1~6: ����.
         * b7:0, ������
         *    1, ������
         */
        uint8_t touchtype;
    } _m_tp_dev;



    /********************   �궨��   ********************/
    /* ���败��������IC T_PEN/T_CS/T_MISO/T_MOSI/T_SCK ���� ���� */
    /* T_PEN���� */
    #define T_PEN_GPIO_PORT                 GPIOF
    #define T_PEN_GPIO_PIN                  GPIO_PIN_10
    #define T_PEN_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOF_CLK_ENABLE()    /* ����IO��ʱ��ʹ�� */
    /* T_CS���� */
    #define T_CS_GPIO_PORT                  GPIOF
    #define T_CS_GPIO_PIN                   GPIO_PIN_11
    #define T_CS_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOF_CLK_ENABLE()    /* ����IO��ʱ��ʹ�� */
    /* T_MISO���� */
    #define T_MISO_GPIO_PORT                GPIOB
    #define T_MISO_GPIO_PIN                 GPIO_PIN_2
    #define T_MISO_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()    /* ����IO��ʱ��ʹ�� */
    /* T_MOSI���� */
    #define T_MOSI_GPIO_PORT                GPIOF
    #define T_MOSI_GPIO_PIN                 GPIO_PIN_9
    #define T_MOSI_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOF_CLK_ENABLE()    /* ����IO��ʱ��ʹ�� */
    /* T_SCK���� */
    #define T_SCK_GPIO_PORT                 GPIOB
    #define T_SCK_GPIO_PIN                  GPIO_PIN_1
    #define T_SCK_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()    /* ����IO��ʱ��ʹ�� */

    /* Ƭѡ�� */
    #define T_CS_H    HAL_GPIO_WritePin(T_CS_GPIO_PORT, T_CS_GPIO_PIN, GPIO_PIN_SET)   /* Ƭѡʧ�� */
    #define T_CS_L    HAL_GPIO_WritePin(T_CS_GPIO_PORT, T_CS_GPIO_PIN, GPIO_PIN_RESET) /* Ƭѡʹ�� */
    /* ʱ���� */
    #define T_SCK_H   HAL_GPIO_WritePin(T_SCK_GPIO_PORT, T_SCK_GPIO_PIN, GPIO_PIN_SET)
    #define T_SCK_L   HAL_GPIO_WritePin(T_SCK_GPIO_PORT, T_SCK_GPIO_PIN, GPIO_PIN_RESET)
    /* ����� */
    #define T_MOSI_H  HAL_GPIO_WritePin(T_MOSI_GPIO_PORT, T_MOSI_GPIO_PIN, GPIO_PIN_SET)
    #define T_MOSI_L  HAL_GPIO_WritePin(T_MOSI_GPIO_PORT, T_MOSI_GPIO_PIN, GPIO_PIN_RESET)
    /* ������ */
    #define T_MISO    HAL_GPIO_ReadPin(T_MISO_GPIO_PORT, T_MISO_GPIO_PIN)
    /* �ʽӴ��� */
    #define T_PEN     HAL_GPIO_ReadPin(T_PEN_GPIO_PORT, T_PEN_GPIO_PIN)

    /* ָ�� */
    #define TOUCH_X   0xD0    //��ȡx��������
    #define TOUCH_Y   0x90    //��ȡy��������
    
    /* ���败������оƬ ���ݲɼ� �˲��ò��� */
    #define TP_READ_TIMES   5       /* ��ȡ���� */
    #define TP_LOST_VAL     1       /* ����ֵ(Ҫ�����������Сֵ����) */
    
    /* �������ζ�ȡX,Y�������������������ֵ */
    #define TP_ERR_RANGE    50         /* ��Χ */

    /* У׼�������Χ */
    #define CORRECT_RANGE_MAX    1.03    /* ��Χ */
    #define CORRECT_RANGE_MIN    0.97    /* ��Χ */



    /****************    �����ⲿ����   *****************/
    extern _m_tp_dev tp_dev;        /* ������������touch.c���涨�� */



    /****************    �����ⲿ����   *****************/
    void Touch_IO_Init(void);                /* ���败�������ų�ʼ�� */
    uint16_t Touch_Data(uint8_t command);    /* ����败�����������ݲ����յ��败�������������� */
    TOUCH_XY Touch_Smoothing(void);          /* ���败���������˲����� */
    void LCD_Calibration_Cross(uint16_t x, uint16_t y, uint16_t color); /* ����У׼��ʮ����ʾ���� */
    void LCD_Touch_Calibration(void);        /* LCD�봥����������ϵ����ĻУ׼ */
    uint8_t Touch_Read_XY(TOUCH_XY* coord);  /* ��ȡ���败���������������� */
    TOUCH_XY Touch_Scan(uint8_t mode);       /* ��ȡ���败������������(�����������Ļ����) */
    void Touch_Init(void);                   /* ��������ʼ�� */

#endif







