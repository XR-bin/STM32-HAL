#ifndef __LCD_H
#define __LCD_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h"
    #include "delay.h"



    /****************     �ṹ�嶨��     ****************/
    /* LCD��Ҫ������ */
    typedef struct  
    {
        uint16_t width;      /* LCD ��� */
        uint16_t height;     /* LCD �߶� */
        uint16_t id;         /* LCD ID */
        uint8_t  dir;        /* ���������������ƣ�0��������1������ */
        uint16_t wramcmd;    /* ��ɫָ�� */
        uint16_t setxcmd;    /* ����x����ָ�� */
        uint16_t setycmd;    /* ����y����ָ�� */
    }_lcd_dev; 



    /********************   �궨��   ********************/
    /* ���� */
    #define LCD_BL_H    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET)        /* �������� */
    #define LCD_BL_L    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET)      /* �رձ��� */

    /*** ��Ļ�Ļ�����Ϣ ***/
    /* ��Ļ�ͺ� */
    #define LED_ID        9341   /* 9341�ͺ���Ļ */

    /* ��Ļ��С */
    #define LCD_WIDTH     240    /* �� */
    #define LCD_HEIGHT    320    /* �� */

    /* ��ʾ���� */
    #define LCD_Crosswise  0xA8  /* ���� */
    #define LCD_Lengthways 0x08  /* ���� */
  
    /* ɨ�跽���� */
    #define L2R_U2D  0 /* ������,���ϵ��� */
    #define L2R_D2U  1 /* ������,���µ��� */
    #define R2L_U2D  2 /* ���ҵ���,���ϵ��� */
    #define R2L_D2U  3 /* ���ҵ���,���µ��� */

    #define U2D_L2R  4 /* ���ϵ���,������ */
    #define U2D_R2L  5 /* ���ϵ���,���ҵ��� */
    #define D2U_L2R  6 /* ���µ���,������ */
    #define D2U_R2L  7 /* ���µ���,���ҵ��� */
    
    #define DFT_SCAN_DIR  L2R_U2D  /* Ĭ�ϵ�ɨ�跽�� */

    /*** ��ɫ ***/
    /* ���û�����ɫ */
    #define WHITE           0xFFFF      /* ��ɫ */
    #define BLACK           0x0000      /* ��ɫ */
    #define RED             0xF800      /* ��ɫ */
    #define GREEN           0x07E0      /* ��ɫ */
    #define BLUE            0x001F      /* ��ɫ */ 
    #define MAGENTA         0XF81F      /* Ʒ��ɫ/�Ϻ�ɫ = BLUE + RED */
    #define YELLOW          0XFFE0      /* ��ɫ = GREEN + RED */
    #define CYAN            0X07FF      /* ��ɫ = GREEN + BLUE */  

    /* �ǳ�����ɫ */
    #define BROWN           0XBC40      /* ��ɫ */
    #define BRRED           0XFC07      /* �غ�ɫ */
    #define GRAY            0X8430      /* ��ɫ */ 
    #define DARKBLUE        0X01CF      /* ����ɫ */
    #define LIGHTBLUE       0X7D7C      /* ǳ��ɫ */ 
    #define GRAYBLUE        0X5458      /* ����ɫ */ 
    #define LIGHTGREEN      0X841F      /* ǳ��ɫ */  
    #define LGRAY           0XC618      /* ǳ��ɫ(PANNEL),���屳��ɫ */ 
    #define LGRAYBLUE       0XA651      /* ǳ����ɫ(�м����ɫ) */ 
    #define LBBLUE          0X2B12      /* ǳ����ɫ(ѡ����Ŀ�ķ�ɫ) */ 



    /****************    �����ⲿ����   *****************/
    extern _lcd_dev lcddev;



    /****************    �����ⲿ����   *****************/
    /* ����Ļ�ͺ�Ӱ�� */
    void LCD_ILI9341_Init(void);                          /* ��Ļ��ʼ�� */
    void LCD_ILI9341_SetCursor(uint16_t x, uint16_t y);   /* ���ù��λ�� */
    void LCD_ILI9341_ScanDir(uint8_t dir);                /* ����LCD(ILI9341)���Զ�ɨ�跽�� */
    void LCD_ILI9341_DisplayDir(uint8_t dir);             /* ����LCD������ */
    void LCD_ILI9341_ClearXY(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);  /* LCD��Ļ��ĳ����������ĳ����ɫ */

    /* ������Ļ�ͺ�Ӱ�� */
    void LCD_WriteCMD(uint16_t cmd);                /* ��LCD�������� */
    void LCD_WriteData(uint16_t data);              /* ��LCD�������� */
    uint16_t LCD_ReadData(void);                    /* ����LCD���������� */
    uint32_t LCD_ReadPoint(uint16_t x, uint16_t y); /* ��ȡĳ�����ص����ɫ */

    /* ��ͼ���� */
    void LCD_Clear(uint16_t color);                 /* LCD���� */

#endif





